/*
 * Copyright (c) 2015-2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @copyright (c) 2015-2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>
#include <sys/time.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_Time.h>

#include <parc/concurrent/parc_ScheduledTask.h>
#include <parc/concurrent/parc_FutureTask.h>

struct PARCScheduledTask {
    PARCFutureTask *task;
    uint64_t executionTime;
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
                    .isLockable = true,
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
parcScheduledTask_Create(PARCFutureTask *task, uint64_t executionTime)
{
    PARCScheduledTask *result = parcObject_CreateInstance(PARCScheduledTask);

    if (result != NULL) {
        result->task = parcFutureTask_Acquire(task);
        result->executionTime = executionTime;
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
    PARCScheduledTask *result = parcScheduledTask_Create(original->task, original->executionTime);

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
            if (x->executionTime == y->executionTime) {
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

uint64_t
parcScheduledTask_GetExecutionTime(const PARCScheduledTask *task)
{
    return task->executionTime;
}

bool
parcScheduledTask_Cancel(PARCScheduledTask *task, bool mayInterruptIfRunning)
{
    return parcFutureTask_Cancel(task->task, mayInterruptIfRunning);
}

PARCFutureTaskResult
parcScheduledTask_Get(const PARCScheduledTask *task, const PARCTimeout *timeout)
{
    return parcFutureTask_Get(task->task, timeout);
}

PARCFutureTask *
parcScheduledTask_GetTask(const PARCScheduledTask *task)
{
    return task->task;
}

void *
parcScheduledTask_Run(const PARCScheduledTask *task)
{
    return parcFutureTask_Run(task->task);
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
