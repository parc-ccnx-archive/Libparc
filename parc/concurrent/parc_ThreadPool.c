/*
 * Copyright (c) 2016, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Patent rights are not granted under this agreement. Patent rights are
 *       available under FRAND terms.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEROX or PARC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * @author Glenn Scott, Computing Science Laboratory, PARC
 * @copyright 2016 Palo Alto Research Center, Inc. (PARC), A Xerox Company.  All Rights Reserved.
 */
#include <config.h>
#include <stdio.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_Memory.h>

#include <parc/algol/parc_SortedList.h>
#include <parc/algol/parc_LinkedList.h>

#include <parc/concurrent/parc_AtomicUint64.h>
#include <parc/concurrent/parc_ThreadPool.h>
#include <parc/concurrent/parc_Thread.h>

struct PARCThreadPool {
    bool continueExistingPeriodicTasksAfterShutdown;
    bool executeExistingDelayedTasksAfterShutdown;
    bool removeOnCancel;
    PARCLinkedList *workQueue;
    PARCLinkedList *threads;
    int poolSize;
    int maximumPoolSize;
    long taskCount;
    bool isShutdown;
    bool isTerminated;
    bool isTerminating;

    PARCAtomicUint64 *completedTaskCount;
};

static void *
_workerThread(PARCThread *thread, PARCThreadPool *pool)
{
    while (parcThread_IsCancelled(thread) == false) {
        if (parcLinkedList_Lock(pool->workQueue)) {
            PARCFutureTask *task = parcLinkedList_RemoveFirst(pool->workQueue);
            if (task != NULL) {
                parcLinkedList_Unlock(pool->workQueue);
                parcFutureTask_Run(task);
                parcFutureTask_Release(&task);
                parcAtomicUint64_Increment(pool->completedTaskCount);
                parcLinkedList_Lock(pool->workQueue);

                parcLinkedList_Notify(pool->workQueue);
            } else {
                parcLinkedList_Wait(pool->workQueue);                
            }
        }
        parcLinkedList_Unlock(pool->workQueue);
    }
   
    return NULL;
}

static void
_parcThreadPool_CancelAll(const PARCThreadPool *pool)
{
    PARCIterator *iterator = parcLinkedList_CreateIterator(pool->threads);
    
    while (parcIterator_HasNext(iterator)) {
        PARCThread *thread = parcIterator_Next(iterator);
        parcThread_Cancel(thread);
    }
    parcIterator_Release(&iterator);
}

static void
_parcThreadPool_JoinAll(const PARCThreadPool *pool)
{
    PARCIterator *iterator = parcLinkedList_CreateIterator(pool->threads);
    
    while (parcIterator_HasNext(iterator)) {
        PARCThread *thread = parcIterator_Next(iterator);
        parcThread_Join(thread);
    }
    parcIterator_Release(&iterator);
}

static bool
_parcThreadPool_Destructor(PARCThreadPool **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCThreadPool pointer.");
    PARCThreadPool *pool = *instancePtr;
    
    if (pool->isShutdown == false) {
        _parcThreadPool_CancelAll(pool);
        _parcThreadPool_JoinAll(pool);
    }

    parcAtomicUint64_Release(&pool->completedTaskCount);
    parcLinkedList_Release(&pool->threads);
    
    if (parcObject_Lock(pool->workQueue)) {
        parcLinkedList_Release(&pool->workQueue);
    }
    
    return true;
}

parcObject_ImplementAcquire(parcThreadPool, PARCThreadPool);

parcObject_ImplementRelease(parcThreadPool, PARCThreadPool);

parcObject_Override(PARCThreadPool, PARCObject,
                    .isLockable = true,
                    .destructor = (PARCObjectDestructor *) _parcThreadPool_Destructor,
                    .copy = (PARCObjectCopy *) parcThreadPool_Copy,
                    .toString = (PARCObjectToString *) parcThreadPool_ToString,
                    .equals = (PARCObjectEquals *) parcThreadPool_Equals,
                    .compare = (PARCObjectCompare *) parcThreadPool_Compare,
                    .hashCode = (PARCObjectHashCode *) parcThreadPool_HashCode,
//                    .display = (PARCObjectDisplay *) parcScheduledThreadPool_Display
);

void
parcThreadPool_AssertValid(const PARCThreadPool *instance)
{
    assertTrue(parcThreadPool_IsValid(instance),
               "PARCThreadPool is not valid.");
}


PARCThreadPool *
parcThreadPool_Create(int poolSize)
{
    PARCThreadPool *result = parcObject_CreateInstance(PARCThreadPool);
    
    if (result != NULL) {
        result->poolSize = poolSize;
        result->maximumPoolSize = poolSize;
        result->taskCount = 0;
        result->isShutdown = false;
        result->isTerminated = false;
        result->isTerminating = false;
        result->workQueue = parcLinkedList_Create();
        result->threads = parcLinkedList_Create();
        
        result->completedTaskCount = parcAtomicUint64_Create(0);
        
        result->continueExistingPeriodicTasksAfterShutdown = false;
        result->executeExistingDelayedTasksAfterShutdown = false;
        result->removeOnCancel = true;
        
        if (parcObject_Lock(result)) {
            for (int i = 0; i < poolSize; i++) {
                PARCThread *thread = parcThread_Create((void *(*)(PARCThread *, PARCObject *)) _workerThread, (PARCObject *) result);
                parcLinkedList_Append(result->threads, thread);
                parcThread_Start(thread);
                parcThread_Release(&thread);
            }
            parcObject_Unlock(result);
        }
    }
    
    return result;
}

int
parcThreadPool_Compare(const PARCThreadPool *instance, const PARCThreadPool *other)
{
    int result = 0;
    
    return result;
}

PARCThreadPool *
parcThreadPool_Copy(const PARCThreadPool *original)
{
    PARCThreadPool *result = parcThreadPool_Create(original->poolSize);
    
    return result;
}

void
parcThreadPool_Display(const PARCThreadPool *instance, int indentation)
{
    parcDisplayIndented_PrintLine(indentation, "PARCThreadPool@%p {", instance);
    /* Call Display() functions for the fields here. */
    parcDisplayIndented_PrintLine(indentation, "}");
}

bool
parcThreadPool_Equals(const PARCThreadPool *x, const PARCThreadPool *y)
{
    bool result = false;
    
    if (x == y) {
        result = true;
    } else if (x == NULL || y == NULL) {
        result = false;
    } else {
        /* perform instance specific equality tests here. */
        if (x->poolSize == y->poolSize) {
            result = true;
        }
    }
    
    return result;
}

PARCHashCode
parcThreadPool_HashCode(const PARCThreadPool *instance)
{
    PARCHashCode result = 0;
    
    return result;
}

bool
parcThreadPool_IsValid(const PARCThreadPool *instance)
{
    bool result = false;
    
    if (instance != NULL) {
        result = true;
    }
    
    return result;
}

PARCJSON *
parcThreadPool_ToJSON(const PARCThreadPool *instance)
{
    PARCJSON *result = parcJSON_Create();
    
    if (result != NULL) {
        
    }
    
    return result;
}

char *
parcThreadPool_ToString(const PARCThreadPool *instance)
{
    char *result = parcMemory_Format("PARCThreadPool@%p\n", instance);

    return result;
}

/**
 * Sets the policy governing whether core threads may time out and terminate if no tasks arrive within the keep-alive time, being replaced if needed when new tasks arrive.
 */
void
parcThreadPool_SetAllowCoreThreadTimeOut(PARCThreadPool *pool, bool value)
{

}

/**
 * Returns true if this pool allows core threads to time out and terminate if no tasks arrive within the keepAlive time, being replaced if needed when new tasks arrive.
 */
bool
parcThreadPool_GetAllowsCoreThreadTimeOut(const PARCThreadPool *pool)
{
    
    return false;
}

/**
 * Blocks until all tasks have completed execution after a shutdown request, or the timeout occurs, or the current thread is interrupted, whichever happens first.
 */
bool
parcThreadPool_AwaitTermination(PARCThreadPool *pool, PARCTimeout *timeout)
{
    bool result = false;
    
    if (pool->isTerminating) {
        if (parcLinkedList_Lock(pool->workQueue)) {
            while (parcLinkedList_Size(pool->workQueue) > 0) {
                parcLinkedList_Wait(pool->workQueue);
            }
            parcLinkedList_Unlock(pool->workQueue);
            result = true;
        }
    }
    
    return result;
}

/**
 * Method invoked prior to executing the given Runnable in the given thread.
 */
//protected void	beforeExecute(Thread t, Runnable r)

/**
 * Executes the given task sometime in the future.
 */
bool
parcThreadPool_Execute(PARCThreadPool *pool, PARCFutureTask *task)
{
    bool result = false;
    
    if (parcThreadPool_Lock(pool)) {
        if (pool->isTerminated == false && pool->isTerminating == false) {
            parcThreadPool_Unlock(pool);
            if (parcLinkedList_Lock(pool->workQueue)) {
                parcLinkedList_Append(pool->workQueue, task);
                parcLinkedList_Notify(pool->workQueue);
                parcLinkedList_Unlock(pool->workQueue);
                result = true;
            }
        } else {
            parcThreadPool_Unlock(pool);
        }
    }
    
    return result;
}

/**
 * Returns the approximate number of threads that are actively executing tasks.
 */
int
parcThreadPool_GetActiveCount(const PARCThreadPool *pool)
{
    return 0;
}

/**
 * Returns the approximate total number of tasks that have completed execution.
 */
uint64_t
parcThreadPool_GetCompletedTaskCount(const PARCThreadPool *pool)
{
    return parcAtomicUint64_GetValue(pool->completedTaskCount);
}

/**
 * Returns the core number of threads.
 */
int
parcThreadPool_GetCorePoolSize(const PARCThreadPool *pool)
{
    return pool->poolSize;
}

/**
 * Returns the thread keep-alive time, which is the amount of time that threads in excess of the core pool size may remain idle before being terminated.
 */
PARCTimeout *
parcThreadPool_GetKeepAliveTime(const PARCThreadPool *pool)
{
    return 0;
}

/**
 * Returns the largest number of threads that have ever simultaneously been in the pool.
 */
int
parcThreadPool_GetLargestPoolSize(const PARCThreadPool *pool)
{
    return 0;
}

/**
 * Returns the maximum allowed number of threads.
 */
int
parcThreadPool_GetMaximumPoolSize(const PARCThreadPool *pool)
{
    return pool->maximumPoolSize;
}

/**
 * Returns the current number of threads in the pool.
 */
int
parcThreadPool_GetPoolSize(const PARCThreadPool *pool)
{
    return pool->poolSize;
}

/**
 * Returns the task queue used by this executor.
 */
PARCLinkedList
*parcThreadPool_GetQueue(const PARCThreadPool *pool)
{
    return pool->workQueue;
}

///**
// * Returns the current handler for unexecutable tasks.
// */
//RejectedExecutionHandler parcThreadPool_GetRejectedExecutionHandler(PARCThreadPool *pool);

/**
 * Returns the approximate total number of tasks that have ever been scheduled for execution.
 */
long
parcThreadPool_GetTaskCount(const PARCThreadPool *pool)
{
    return pool->taskCount;
}

/**
 * Returns the thread factory used to create new threads.
 */
//ThreadFactory parcThreadPool_GetThreadFactory(PARCThreadPool *pool, );

/**
 * Returns true if this executor has been shut down.
 */
bool
parcThreadPool_IsShutdown(const PARCThreadPool *pool)
{
    return pool->isShutdown;
}

/**
 * Returns true if all tasks have completed following shut down.
 */
bool
parcThreadPool_IsTerminated(const PARCThreadPool *pool)
{
    return pool->isTerminated;
}

/**
 * Returns true if this executor is in the process of terminating after shutdown() or shutdownNow() but has not completely terminated.
 */
bool
parcThreadPool_IsTerminating(const PARCThreadPool *pool)
{
    return pool->isTerminating;
}

/**
 * Starts all core threads, causing them to idly wait for work.
 */
int
parcThreadPool_PrestartAllCoreThreads(PARCThreadPool *pool)
{
    return 0;
}

/**
 * Starts a core thread, causing it to idly wait for work.
 */
bool
parcThreadPool_PrestartCoreThread(PARCThreadPool *pool)
{
    return 0;
}

/**
 * Tries to remove from the work queue all Future tasks that have been cancelled.
 */
void
parcThreadPool_Purge(PARCThreadPool *pool)
{

}

/**
 * Removes this task from the executor's internal queue if it is present, thus causing it not to be run if it has not already started.
 */
bool
parcThreadPool_Remove(PARCThreadPool *pool, PARCFutureTask *task)
{
    return false;
}

/**
 * Sets the core number of threads.
 */
void
parcThreadPool_SetCorePoolSize(PARCThreadPool *pool, int corePoolSize)
{

}

/**
 * Sets the time limit for which threads may remain idle before being terminated.
 */
void
parcThreadPool_SetKeepAliveTime(PARCThreadPool *pool, PARCTimeout *timeout)
{

}

/**
 * Sets the maximum allowed number of threads.
 */
void
parcThreadPool_SetMaximumPoolSize(PARCThreadPool *pool, int maximumPoolSize)
{

}

///**
// * Sets a new handler for unexecutable tasks.
// */
//void parcThreadPool_SetRejectedExecutionHandler(PARCThreadPool *pool, RejectedExecutionHandler handler);

///**
// * Sets the thread factory used to create new threads.
// */
//void parcThreadPool_SetThreadFactory(PARCThreadPool *pool, ThreadFactory threadFactory);



/**
 * Initiates an orderly shutdown in which previously submitted tasks are executed, but no new tasks will be accepted.
 */
void
parcThreadPool_Shutdown(PARCThreadPool *pool)
{
    pool->isTerminating = true;
}

/**
 * Attempts to stop all actively executing tasks, halts the processing of waiting tasks, and returns a list of the tasks that were awaiting execution.
 */
PARCLinkedList *
parcThreadPool_ShutdownNow(PARCThreadPool *pool)
{
    parcThreadPool_Shutdown(pool);
    
    // Cause all of the worker threads to exit.
    _parcThreadPool_CancelAll(pool);
    
    // Wake them all up so they detect that they are cancelled.
    if (parcThreadPool_Lock(pool)) {
        pool->isTerminating = true;
        parcThreadPool_NotifyAll(pool);
        parcThreadPool_Unlock(pool);
    }
    
    if (parcLinkedList_Lock(pool->workQueue)) {
        parcLinkedList_NotifyAll(pool->workQueue);
        parcLinkedList_Unlock(pool->workQueue);
    }
    // Join with all of them, thereby cleaning up all of them.
    _parcThreadPool_JoinAll(pool);
    
    pool->isTerminated = true;
    return NULL;
}
