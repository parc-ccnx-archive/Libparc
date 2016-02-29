/*
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * Copyright 2015 Palo Alto Research Center, Inc. (PARC), a Xerox company.  All Rights Reserved.
 * The content of this file, whole or in part, is subject to licensing terms.
 * If distributing this software, include this License Header Notice in each
 * file and provide the accompanying LICENSE file.
 */
/**
 * @author Glenn Scott, Computing Science Laboratory, PARC
 * @copyright 2016 Palo Alto Research Center, Inc. (PARC), A Xerox Company.  All Rights Reserved.
 */
#include <config.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_Memory.h>

#include <parc/concurrent/parc_ScheduledTask.h>
#include <parc/concurrent/parc_FutureTask.h>

struct PARCScheduledTask {
    PARCFutureTask *task;
    PARCTimeout delay;
};

static bool
_parcScheduledTask_Destructor(PARCScheduledTask **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCScheduledTask pointer.");
    PARCScheduledTask *task = *instancePtr;
    
    parcFutureTask_Release(&task->task);
    return true;
}

parcObject_ImplementAcquire(parcScheduledTask, PARCScheduledTask);

parcObject_ImplementRelease(parcScheduledTask, PARCScheduledTask);

parcObject_Override(PARCScheduledTask, PARCObject,
                    .destructor = (PARCObjectDestructor *) _parcScheduledTask_Destructor,
                    .copy = (PARCObjectCopy *) parcScheduledTask_Copy,
                    .toString = (PARCObjectToString *) parcScheduledTask_ToString,
                    .equals = (PARCObjectEquals *) parcScheduledTask_Equals,
                    .compare = (PARCObjectCompare *) parcScheduledTask_Compare,
                    .hashCode = (PARCObjectHashCode *) parcScheduledTask_HashCode,
                    .display = (PARCObjectDisplay *) parcScheduledTask_Display);

void
parcScheduledTask_AssertValid(const PARCScheduledTask *instance)
{
    assertTrue(parcScheduledTask_IsValid(instance),
               "PARCScheduledTask is not valid.");
}


PARCScheduledTask *
parcScheduledTask_Create(PARCFutureTask *task, PARCTimeout delay)
{
    PARCScheduledTask *result = parcObject_CreateInstance(PARCScheduledTask);
    
    if (result != NULL) {
        result->task = parcFutureTask_Acquire(task);
        result->delay = delay;
    }

    return result;
}

int
parcScheduledTask_Compare(const PARCScheduledTask *instance, const PARCScheduledTask *other)
{
    int result = 0;
    
    return result;
}

PARCScheduledTask *
parcScheduledTask_Copy(const PARCScheduledTask *original)
{
    PARCScheduledTask *result = parcScheduledTask_Create(original->task, original->delay);
    
    return result;
}

void
parcScheduledTask_Display(const PARCScheduledTask *instance, int indentation)
{
    parcDisplayIndented_PrintLine(indentation, "PARCScheduledTask@%p {", instance);
    /* Call Display() functions for the fields here. */
    parcDisplayIndented_PrintLine(indentation, "}");
}

bool
parcScheduledTask_Equals(const PARCScheduledTask *x, const PARCScheduledTask *y)
{
    bool result = false;
    
    if (x == y) {
        result = true;
    } else if (x == NULL || y == NULL) {
        result = false;
    } else {
        if (parcFutureTask_Equals(x->task, y->task)) {
            if (parcTimeout_Equals(x->delay, y->delay)) {
                result = true;
            }
        }
    }
    
    return result;
}

PARCHashCode
parcScheduledTask_HashCode(const PARCScheduledTask *instance)
{
    PARCHashCode result = 0;
    
    return result;
}

bool
parcScheduledTask_IsValid(const PARCScheduledTask *instance)
{
    bool result = false;
    
    if (instance != NULL) {
        result = true;
    }
    
    return result;
}

PARCJSON *
parcScheduledTask_ToJSON(const PARCScheduledTask *instance)
{
    PARCJSON *result = parcJSON_Create();
    
    if (result != NULL) {
        
    }
    
    return result;
}

char *
parcScheduledTask_ToString(const PARCScheduledTask *instance)
{
    char *result = parcMemory_Format("PARCScheduledTask@%p\n", instance);

    return result;
}

PARCTimeout
parcScheduledTask_GetDelay(const PARCScheduledTask *task)
{
    return task->delay;
}

bool
parcScheduledTask_Cancel(PARCScheduledTask *task, bool mayInterruptIfRunning)
{
    return parcFutureTask_Cancel(task->task, mayInterruptIfRunning);
}

PARCFutureTaskResult
parcScheduledTask_Get(const PARCScheduledTask *task, PARCTimeout timeout)
{
    return parcFutureTask_Get(task->task, timeout);
}

bool
parcScheduledTask_IsCancelled(const PARCScheduledTask *task)
{
    return parcFutureTask_IsCancelled(task->task);
}

bool
parcScheduledTask_IsDone(const PARCScheduledTask *task)
{
    return parcFutureTask_IsDone(task->task);
}

