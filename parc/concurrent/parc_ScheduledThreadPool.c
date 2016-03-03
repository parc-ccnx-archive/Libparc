/*
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * Copyright 2015 Palo Alto Research Center, Inc. (PARC), a Xerox company.  All Rights Reserved.
 * The content of this file, whole or in part, is subject to licensing terms.
 * If distributing this software, include this License Header Notice in each
 * file and provide the accompanying LICENSE file.
 */
/**
 * @author <#gscott#>, Computing Science Laboratory, PARC
 * @copyright 2015 Palo Alto Research Center, Inc. (PARC), A Xerox Company.  All Rights Reserved.
 */
#include <config.h>
#include <stdio.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_SortedList.h>
#include <parc/algol/parc_LinkedList.h>
#include <parc/algol/parc_Memory.h>

#include <parc/concurrent/parc_ScheduledThreadPool.h>
#include <parc/concurrent/parc_Thread.h>

struct PARCScheduledThreadPool {
    bool continueExistingPeriodicTasksAfterShutdown;
    bool executeExistingDelayedTasksAfterShutdown;
    bool removeOnCancel;
    PARCSortedList *workQueue;
    PARCLinkedList *threads;
    int poolSize;
};

static void *
_superVisorThread(PARCThread *thread, PARCScheduledThreadPool *pool)
{
    if (parcObject_Lock(pool)) {
        while (parcThread_IsCancelled(thread) == false) {
            /*
             supervisor:
                repeat:
                    Pick the top of the queue.
                    If the delay is <= 0, then wakeup the workers.
                    If the delay is > 0, then wait on the pool for that amount of time.
                    Otherwise, wait on the pool for a new task to be enqueued.
             
             worker:
                repeat:
                    Pick the top of the queue.
                    If the delay is <= 0, then take the task and execute it.
                    Otherwise, wait on the workQueue for the supervisor
             
             */
            
            { char *string = parcThread_ToString(thread); printf("%s wait\n", string); parcMemory_Deallocate(&string); fflush(stdout); }
            
            if (parcSortedList_Size(pool->workQueue) > 0) {
                PARCScheduledTask *task = parcSortedList_GetFirst(pool->workQueue);
                if (parcScheduledTask_GetDelay(task) <= 0) {
                    parcObject_Notify(pool);
                    
                    task = parcSortedList_RemoveFirst(pool->workQueue);
                    parcObject_Unlock(pool);
                    parcScheduledTask_Get(task, PARCTimeout_Never);
                    parcScheduledTask_Release(&task);
                    parcObject_Lock(pool);
                }
            }
            parcObject_WaitFor(pool, parcTimeout_MilliSeconds(500));
        }
        
        { char *string = parcThread_ToString(thread); printf("%s unlock\n", string); parcMemory_Deallocate(&string); fflush(stdout);}
        parcObject_Unlock(pool);
    } else {
        { char *string = parcThread_ToString(thread); printf("%s failed to lock\n", string); parcMemory_Deallocate(&string); fflush(stdout);}
    }
    
    { char *string = parcThread_ToString(thread); printf("%s done\n", string); parcMemory_Deallocate(&string); fflush(stdout);}
    
    return NULL;
}

static void *
_workerThread(PARCThread *thread, PARCScheduledThreadPool *pool)
{
//    { char *string = parcThread_ToString(thread); printf("%s lock\n", string); parcMemory_Deallocate(&string); fflush(stdout); }
    
    if (parcObject_Lock(pool)) {

        while (parcThread_IsCancelled(thread) == false) {
            { char *string = parcThread_ToString(thread); printf("%s wait\n", string); parcMemory_Deallocate(&string); fflush(stdout); }

            if (parcSortedList_Size(pool->workQueue) > 0) {
                PARCScheduledTask *task = parcSortedList_GetAtIndex(pool->workQueue, 0);
                if (parcScheduledTask_GetDelay(task) <= 0) {
                    task = parcSortedList_RemoveFirst(pool->workQueue);
                    parcObject_Unlock(pool);
                    parcScheduledTask_Get(task, PARCTimeout_Never);
                    parcScheduledTask_Release(&task);
                    parcObject_Lock(pool);
                }
            }
            parcObject_WaitFor(pool, parcTimeout_MilliSeconds(500));
        }

        { char *string = parcThread_ToString(thread); printf("%s unlock\n", string); parcMemory_Deallocate(&string); fflush(stdout);}
        parcObject_Unlock(pool);
    } else {
        { char *string = parcThread_ToString(thread); printf("%s failed to lock\n", string); parcMemory_Deallocate(&string); fflush(stdout);}
    }

    { char *string = parcThread_ToString(thread); printf("%s done\n", string); parcMemory_Deallocate(&string); fflush(stdout);}

    return NULL;
}

static bool
_parcScheduledThreadPool_Destructor(PARCScheduledThreadPool **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCScheduledThreadPool pointer.");
    PARCScheduledThreadPool *pool = *instancePtr;
    parcLinkedList_Release(&pool->threads);
    
    if (parcObject_Lock(pool->workQueue)) {
        parcSortedList_Release(&pool->workQueue);
    }
    
    return true;
}

parcObject_ImplementAcquire(parcScheduledThreadPool, PARCScheduledThreadPool);

parcObject_ImplementRelease(parcScheduledThreadPool, PARCScheduledThreadPool);

parcObject_Override(PARCScheduledThreadPool, PARCObject,
                    .destructor = (PARCObjectDestructor *) _parcScheduledThreadPool_Destructor,
                    .copy = (PARCObjectCopy *) parcScheduledThreadPool_Copy,
                    .toString = (PARCObjectToString *) parcScheduledThreadPool_ToString,
                    .equals = (PARCObjectEquals *) parcScheduledThreadPool_Equals,
                    .compare = (PARCObjectCompare *) parcScheduledThreadPool_Compare,
                    .hashCode = (PARCObjectHashCode *) parcScheduledThreadPool_HashCode,
//                    .display = (PARCObjectDisplay *) parcScheduledThreadPool_Display
                    );

void
parcScheduledThreadPool_AssertValid(const PARCScheduledThreadPool *instance)
{
    assertTrue(parcScheduledThreadPool_IsValid(instance),
               "PARCScheduledThreadPool is not valid.");
}

PARCScheduledThreadPool *
parcScheduledThreadPool_Create(int poolSize)
{
    PARCScheduledThreadPool *result = parcObject_CreateInstance(PARCScheduledThreadPool);
    
    if (result != NULL) {
        result->poolSize = poolSize;
        result->workQueue = parcSortedList_Create();
        result->threads = parcLinkedList_Create();
        
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
parcScheduledThreadPool_Compare(const PARCScheduledThreadPool *instance, const PARCScheduledThreadPool *other)
{
    int result = 0;
    
    return result;
}

PARCScheduledThreadPool *
parcScheduledThreadPool_Copy(const PARCScheduledThreadPool *original)
{
    PARCScheduledThreadPool *result = parcScheduledThreadPool_Create(original->poolSize);
    
    return result;
}

void
parcScheduledThreadPool_Display(const PARCScheduledThreadPool *instance, int indentation)
{
    parcDisplayIndented_PrintLine(indentation, "PARCScheduledThreadPool@%p {", instance);
    /* Call Display() functions for the fields here. */
    parcDisplayIndented_PrintLine(indentation, "}");
}

bool
parcScheduledThreadPool_Equals(const PARCScheduledThreadPool *x, const PARCScheduledThreadPool *y)
{
    bool result = false;
    
    if (x == y) {
        result = true;
    } else if (x == NULL || y == NULL) {
        result = false;
    } else {
        if (x->poolSize == y->poolSize) {
            result = true;
        }
    }
    
    return result;
}

PARCHashCode
parcScheduledThreadPool_HashCode(const PARCScheduledThreadPool *instance)
{
    PARCHashCode result = 0;
    
    return result;
}

bool
parcScheduledThreadPool_IsValid(const PARCScheduledThreadPool *instance)
{
    bool result = false;
    
    if (instance != NULL) {
        result = true;
    }
    
    return result;
}

PARCJSON *
parcScheduledThreadPool_ToJSON(const PARCScheduledThreadPool *instance)
{
    PARCJSON *result = parcJSON_Create();
    
    if (result != NULL) {
        
    }
    
    return result;
}

char *
parcScheduledThreadPool_ToString(const PARCScheduledThreadPool *instance)
{
    char *result = parcMemory_Format("PARCScheduledThreadPool@%p\n", instance);

    return result;
}

void
parcScheduledThreadPool_Execute(PARCScheduledThreadPool *pool, PARCFutureTask *command)
{
    
}

bool
parcScheduledThreadPool_GetContinueExistingPeriodicTasksAfterShutdownPolicy(PARCScheduledThreadPool *pool)
{
    return pool->continueExistingPeriodicTasksAfterShutdown;
}

bool
parcScheduledThreadPool_GetExecuteExistingDelayedTasksAfterShutdownPolicy(PARCScheduledThreadPool *pool)
{
    return pool->executeExistingDelayedTasksAfterShutdown;
}

PARCSortedList *
parcScheduledThreadPool_GetQueue(PARCScheduledThreadPool *pool)
{
    return pool->workQueue;
}

bool
parcScheduledThreadPool_GetRemoveOnCancelPolicy(PARCScheduledThreadPool *pool)
{
    return pool->removeOnCancel;
}

PARCFutureTask *
parcScheduledThreadPool_Schedule(PARCScheduledThreadPool *pool, PARCFutureTask *task, PARCTimeout delay)
{
    return task;

}

PARCFutureTask *
parcScheduledThreadPool_ScheduleAtFixedRate(PARCScheduledThreadPool *pool, PARCFutureTask *task, PARCTimeout initialDelay, PARCTimeout period)
{
    return task;
}

PARCFutureTask *
parcScheduledThreadPool_ScheduleWithFixedDelay(PARCScheduledThreadPool *pool, PARCFutureTask *task, PARCTimeout initialDelay, PARCTimeout delay)
{
    return task;
}

void
parcScheduledThreadPool_SetContinueExistingPeriodicTasksAfterShutdownPolicy(PARCScheduledThreadPool *pool, bool value)
{
    
}

void
parcScheduledThreadPool_SetExecuteExistingDelayedTasksAfterShutdownPolicy(PARCScheduledThreadPool *pool, bool value)
{
    
}

void
parcScheduledThreadPool_SetRemoveOnCancelPolicy(PARCScheduledThreadPool *pool, bool value)
{
    
}

void
parcScheduledThreadPool_Shutdown(PARCScheduledThreadPool *pool)
{
    parcScheduledThreadPool_ShutdownNow(pool);
}

static void
_parcScheduledThreadPool_CancelAll(const PARCScheduledThreadPool *pool)
{
    PARCIterator *iterator = parcLinkedList_CreateIterator(pool->threads);
    
    while (parcIterator_HasNext(iterator)) {
        PARCThread *thread = parcIterator_Next(iterator);
        parcThread_Cancel(thread);
    }
    parcIterator_Release(&iterator);
}

static void
_parcScheduledThreadPool_JoinAll(const PARCScheduledThreadPool *pool)
{
    PARCIterator *iterator = parcLinkedList_CreateIterator(pool->threads);
    
    while (parcIterator_HasNext(iterator)) {
        PARCThread *thread = parcIterator_Next(iterator);
        parcThread_Join(thread);
    }
    parcIterator_Release(&iterator);
}

PARCList *
parcScheduledThreadPool_ShutdownNow(PARCScheduledThreadPool *pool)
{
    // Cause all of the worker threads to exit.
    _parcScheduledThreadPool_CancelAll(pool);
    
    // Wake them all up so they detect that they are cancelled.
    if (parcObject_Lock(pool)) {
        parcObject_NotifyAll(pool);
        parcObject_Unlock(pool);
    }
    if (parcObject_Lock(pool->workQueue)) {
        parcObject_NotifyAll(pool->workQueue);
        parcObject_Unlock(pool->workQueue);
    }
    // Join with all of them, thereby cleaning up all of them.
    _parcScheduledThreadPool_JoinAll(pool);
    
    
    return NULL;
}

PARCScheduledTask *
parcScheduledThreadPool_Submit(PARCScheduledThreadPool *pool, PARCFutureTask *task)
{
    PARCScheduledTask *scheduledTask = parcScheduledTask_Create(task, 0);
    
    parcSortedList_Add(pool->workQueue, scheduledTask);

    return scheduledTask;
}
