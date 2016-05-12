/*
 * Copyright (c) 2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEROX OR PARC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * ################################################################################
 * #
 * # PATENT NOTICE
 * #
 * # This software is distributed under the BSD 2-clause License (see LICENSE
 * # file).  This BSD License does not make any patent claims and as such, does
 * # not act as a patent grant.  The purpose of this section is for each contributor
 * # to define their intentions with respect to intellectual property.
 * #
 * # Each contributor to this source code is encouraged to state their patent
 * # claims and licensing mechanisms for any contributions made. At the end of
 * # this section contributors may each make their own statements.  Contributor's
 * # claims and grants only apply to the pieces (source code, programs, text,
 * # media, etc) that they have contributed directly to this software.
 * #
 * # There is no guarantee that this section is complete, up to date or accurate. It
 * # is up to the contributors to maintain their portion of this section and up to
 * # the user of the software to verify any claims herein.
 * #
 * # Do not remove this header notification.  The contents of this section must be
 * # present in all distributions of the software.  You may only modify your own
 * # intellectual property statements.  Please provide contact information.
 * 
 * - Palo Alto Research Center, Inc
 * This software distribution does not grant any rights to patents owned by Palo
 * Alto Research Center, Inc (PARC). Rights to these patents are available via
 * various mechanisms. As of January 2016 PARC has committed to FRAND licensing any
 * intellectual property used by its contributions to this software. You may
 * contact PARC at cipo@parc.com for more information or visit http://www.ccnx.org
 */
/**
 * @author Glenn Scott, Computing Science Laboratory, PARC
 * @copyright (c) 2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>
#include <stdio.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_Execution.h>

#include <parc/concurrent/parc_FutureTask.h>

struct PARCFutureTask {
    void *(*function)(PARCFutureTask *task, void *parameter);
    void *parameter;
    void *result;
    bool isRunning;
    bool isDone;
    bool isCancelled;
};

static void
_parcFutureTask_Initialise(PARCFutureTask *futureTask)
{
    futureTask->result = NULL;
    futureTask->isDone = false;
    futureTask->isCancelled = false;
    futureTask->isRunning = false;
}

static bool
_parcFutureTask_Destructor(PARCFutureTask **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCFutureTask pointer.");
    PARCFutureTask *task = *instancePtr;

    if (parcObject_IsInstanceOf(task->parameter, &PARCObject_Descriptor)) {
        parcObject_Release(&task->parameter);
    }

    return true;
}

parcObject_ImplementAcquire(parcFutureTask, PARCFutureTask);

parcObject_ImplementRelease(parcFutureTask, PARCFutureTask);

parcObject_Override(PARCFutureTask, PARCObject,
                    .isLockable = true,
                    .destructor = (PARCObjectDestructor *) _parcFutureTask_Destructor,
                    .copy = (PARCObjectCopy *) parcFutureTask_Copy,
                    .toString = (PARCObjectToString *) parcFutureTask_ToString,
                    .equals = (PARCObjectEquals *) parcFutureTask_Equals,
                    .compare = (PARCObjectCompare *) parcFutureTask_Compare,
                    .hashCode = (PARCObjectHashCode *) parcFutureTask_HashCode,
                    .display = (PARCObjectDisplay *) parcFutureTask_Display);

void
parcFutureTask_AssertValid(const PARCFutureTask *task)
{
    assertTrue(parcFutureTask_IsValid(task),
               "PARCFutureTask is not valid.");
}

PARCFutureTask *
parcFutureTask_Create(void *(*function)(PARCFutureTask *task, void *parameter), void *parameter)
{
    PARCFutureTask *result = parcObject_CreateInstance(PARCFutureTask);

    if (parcObject_IsInstanceOf(parameter, &PARCObject_Descriptor)) {
        parameter = parcObject_Acquire(parameter);
    }

    if (result != NULL) {
        result->function = function;
        result->parameter = parameter;
        _parcFutureTask_Initialise(result);
    }

    return result;
}

int
parcFutureTask_Compare(const PARCFutureTask *instance, const PARCFutureTask *other)
{
    int result = 0;

    return result;
}

PARCFutureTask *
parcFutureTask_Copy(const PARCFutureTask *original)
{
    PARCFutureTask *result = parcFutureTask_Create(original->function, original->parameter);

    return result;
}

void
parcFutureTask_Display(const PARCFutureTask *instance, int indentation)
{
    parcDisplayIndented_PrintLine(indentation, "PARCFutureTask@%p {", instance);
    /* Call Display() functions for the fields here. */
    parcDisplayIndented_PrintLine(indentation, "}");
}

bool
parcFutureTask_Equals(const PARCFutureTask *x, const PARCFutureTask *y)
{
    bool result = false;

    if (x == y) {
        result = true;
    } else if (x == NULL || y == NULL) {
        result = false;
    } else {
        if (x->function == y->function) {
            if (x->parameter == y->parameter) {
                result = true;
            }
        }
    }

    return result;
}

PARCHashCode
parcFutureTask_HashCode(const PARCFutureTask *instance)
{
    PARCHashCode result = 0;

    return result;
}

bool
parcFutureTask_IsValid(const PARCFutureTask *instance)
{
    bool result = false;

    if (instance != NULL) {
        result = true;
    }

    return result;
}

PARCJSON *
parcFutureTask_ToJSON(const PARCFutureTask *instance)
{
    PARCJSON *result = parcJSON_Create();

    if (result != NULL) {
    }

    return result;
}

char *
parcFutureTask_ToString(const PARCFutureTask *instance)
{
    char *result = parcMemory_Format("PARCFutureTask@%p\n", instance);

    return result;
}

bool
parcFutureTask_Cancel(PARCFutureTask *task, bool mayInterruptIfRunning)
{
    bool result = false;

    if (parcObject_Lock(task)) {
        if (task->isRunning) {
            if (mayInterruptIfRunning) {
                printf("Interrupting a running task is not implemented yet.\n");
            }
            result = false;
        } else {
            task->isCancelled = true;
            task->isDone = true;
            parcObject_Notify(task);
            result = true;
        }

        parcObject_Unlock(task);
    }

    return result;
}

PARCFutureTaskResult
parcFutureTask_Get(const PARCFutureTask *futureTask, const PARCTimeout *timeout)
{
    PARCFutureTaskResult result;

    result.execution = PARCExecution_Timeout;
    result.value = 0;

    if (parcTimeout_IsImmediate(timeout)) {
        if (futureTask->isDone) {
            result.execution = PARCExecution_OK;
            result.value = futureTask->result;
        }
    } else {
        result.execution = PARCExecution_Interrupted;
        result.value = 0;

        parcObject_Lock(futureTask);
        while (!futureTask->isDone) {
            if (parcTimeout_IsNever(timeout)) {
                parcObject_Wait(futureTask);
                result.execution = PARCExecution_OK;
                result.value = futureTask->result;
                break;
            } else {
                if (parcObject_WaitFor(futureTask, parcTimeout_InNanoSeconds(timeout))) {
                    result.execution = PARCExecution_OK;
                    result.value = futureTask->result;
                    break;
                }
            }
        }
        parcObject_Unlock(futureTask);
    }

    return result;
}

bool
parcFutureTask_IsCancelled(const PARCFutureTask *task)
{
    return task->isCancelled;
}

bool
parcFutureTask_IsDone(const PARCFutureTask *task)
{
    return task->isDone;
}

static void *
_parcFutureTask_Execute(PARCFutureTask *task)
{
    task->isRunning = true;
    void *result = task->function(task, task->parameter);
    task->isRunning = false;

    return result;
}

void *
parcFutureTask_Run(PARCFutureTask *task)
{
    if (parcFutureTask_Lock(task)) {
        if (!task->isCancelled) {
            task->result = _parcFutureTask_Execute(task);
            task->isDone = true;
            parcFutureTask_Notify(task);
        }
        parcFutureTask_Unlock(task);
    } else {
        trapCannotObtainLock("Cannot lock PARCFutureTask");
    }
    return task->result;
}

bool
parcFutureTask_RunAndReset(PARCFutureTask *task)
{
    bool result = false;

    if (parcObject_Lock(task)) {
        if (!task->isCancelled) {
            _parcFutureTask_Execute(task);
            parcFutureTask_Reset(task);
            result = true;
        }
        parcFutureTask_Unlock(task);
    } else {
        trapCannotObtainLock("Cannot lock PARCFutureTask");
    }

    return result;
}

void
parcFutureTask_Reset(PARCFutureTask *task)
{
    _parcFutureTask_Initialise(task);
}
