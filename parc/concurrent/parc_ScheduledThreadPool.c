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

PARCObject *
parcSortedlist_Pop(PARCSortedList *list)
{
    PARCObject *result = parcSortedList_GetAtIndex(list, 0);
    parcSortedList_Remove(list, result);
    return result;
}

static void *
_workingThread(PARCThread *thread, PARCObject *pool)
{
    { char *string = parcThread_ToString(thread); printf("%s lock\n", string); parcMemory_Deallocate(&string); fflush(stdout); }
    
    if (parcObject_Lock(thread)) {

        while (parcThread_IsCancelled(thread) == false) {
            { char *string = parcThread_ToString(thread); printf("%s wait\n", string); parcMemory_Deallocate(&string); fflush(stdout); }
            parcObject_Wait(thread);
        }

        { char *string = parcThread_ToString(thread); printf("%s unlock\n", string); parcMemory_Deallocate(&string); fflush(stdout);}
        parcObject_Unlock(thread);
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
                PARCThread *thread = parcThread_Create(_workingThread, (PARCObject *) result);
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
    
    
}

PARCList *
parcScheduledThreadPool_ShutdownNow(PARCScheduledThreadPool *pool)
{
    if (parcObject_Lock(pool)) {
        PARCIterator *iterator = parcLinkedList_CreateIterator(pool->threads);
        
        while (parcIterator_HasNext(iterator)) {
            PARCThread *thread = parcIterator_Next(iterator);
            parcThread_Cancel(thread);
            { char *string = parcThread_ToString(thread); printf("parcScheduledThreadPool_ShutdownNow %s\n", string); parcMemory_Deallocate(&string); fflush(stdout); }
        }
        parcIterator_Release(&iterator);
        
        printf("parcScheduledThreadPool_ShutdownNow lock\n");
        printf("parcScheduledThreadPool_ShutdownNow notify\n");
        parcObject_Notify(pool);
        printf("parcScheduledThreadPool_ShutdownNow unlock\n");
        parcObject_Unlock(pool);
    }
    
    return NULL;
}

PARCScheduledTask *
parcScheduledThreadPool_Submit(PARCScheduledThreadPool *pool, PARCFutureTask *task)
{
    PARCScheduledTask *scheduledTask = parcScheduledTask_Create(task, 0);
    
    parcSortedList_Add(pool->workQueue, scheduledTask);

    return scheduledTask;
}
