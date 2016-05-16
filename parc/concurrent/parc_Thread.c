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
#include <pthread.h>
#include <stdio.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_Memory.h>

#include <parc/concurrent/parc_Thread.h>

struct PARCThread {
    void *(*run)(PARCThread *, PARCObject *param);
    PARCObject *argument;
    bool isCancelled;
    bool isRunning;
    pthread_t thread;
};

static bool
_parcThread_Destructor(PARCThread **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCThread pointer.");
    PARCThread *thread = *instancePtr;
   
    if (thread->argument != NULL) {
        parcObject_Release(&thread->argument);
    }

    thread->isCancelled = true;
    parcThread_Join(thread);

    return true;
}

parcObject_ImplementAcquire(parcThread, PARCThread);

parcObject_ImplementRelease(parcThread, PARCThread);

parcObject_Override(PARCThread, PARCObject,
                    .isLockable = true,
                    .destructor = (PARCObjectDestructor *) _parcThread_Destructor,
                    .copy = (PARCObjectCopy *) parcThread_Copy,
                    .toString = (PARCObjectToString *) parcThread_ToString,
                    .equals = (PARCObjectEquals *) parcThread_Equals,
                    .compare = (PARCObjectCompare *) parcThread_Compare,
                    .hashCode = (PARCObjectHashCode *) parcThread_HashCode,
                    .display = (PARCObjectDisplay *) parcThread_Display
                    );

void
parcThread_AssertValid(const PARCThread *instance)
{
    assertTrue(parcThread_IsValid(instance),
               "PARCThread is not valid.");
}

PARCThread *
parcThread_Create(void *(*runFunction)(PARCThread *, PARCObject *), PARCObject *restrict parameter)
{
    assertNotNull(parameter, "Parameter cannot be NULL.");

    PARCThread *result = parcObject_CreateAndClearInstance(PARCThread);

    if (result) {
        result->run = runFunction;
        result->argument = parcObject_Acquire(parameter);
        result->isCancelled = false;
        result->isRunning = false;
    }

    return result;
}

int
parcThread_Compare(const PARCThread *instance, const PARCThread *other)
{
    int result = 0;
    return result;
}

PARCThread *
parcThread_Copy(const PARCThread *original)
{
    PARCThread *result = parcThread_Create(original->run, original->argument);
    result->isCancelled = original->isCancelled;
    result->isRunning = original->isRunning;

    return result;
}

void
parcThread_Display(const PARCThread *instance, int indentation)
{
    parcDisplayIndented_PrintLine(indentation, "PARCThread@%p {", instance);
    /* Call Display() functions for the fields here. */
    parcDisplayIndented_PrintLine(indentation, "}");
}

bool
parcThread_Equals(const PARCThread *x, const PARCThread *y)
{
    bool result = false;

    if (x == y) {
        result = true;
    } else if (x == NULL || y == NULL) {
        result = false;
    } else {
        result = pthread_equal(x->thread, y->thread);
    }

    return result;
}

PARCHashCode
parcThread_HashCode(const PARCThread *instance)
{
    PARCHashCode result = 0;

    return result;
}

bool
parcThread_IsValid(const PARCThread *thread)
{
    bool result = false;

    if (thread != NULL) {
        result = true;
    }

    return result;
}

PARCJSON *
parcThread_ToJSON(const PARCThread *thread)
{
    PARCJSON *result = parcJSON_Create();

    return result;
}

char *
parcThread_ToString(const PARCThread *thread)
{
    char *result = parcMemory_Format("PARCThread@%p{.id=%p, .isCancelled=%s}", thread, thread->thread, thread->isCancelled ? "true" : "false");

    return result;
}

static void *
_parcThread_Run(PARCThread *thread)
{
    thread->isRunning = true;
    thread->run(thread, thread->argument);
    thread->isRunning = false;

    // The thread is done, release the reference to the argument acquired when this PARCThread was created.
    // This prevents the reference from lingering leading to memory leaks if the thread is not properly joined.
    if (thread->argument != NULL) {
        parcObject_Release(&thread->argument);
    }
    //  Release the thread reference that was acquired *before* this thread was started.
    parcThread_Release(&thread);

    return NULL;
}

void
parcThread_Start(PARCThread *thread)
{
    PARCThread *parameter = parcThread_Acquire(thread);
    pthread_create(&thread->thread, NULL, (void *(*)(void *)) _parcThread_Run, parameter);
}

PARCObject *
parcThread_GetParameter(const PARCThread *thread)
{
    return thread->argument;
}

bool
parcThread_Cancel(PARCThread *thread)
{
    if (parcThread_Lock(thread)) {
        thread->isCancelled = true;
        parcThread_Notify(thread);
        parcThread_Unlock(thread);
    }
    return true;
}

int
parcThread_GetId(const PARCThread *thread)
{
    return (int) thread->thread;
}

bool
parcThread_IsRunning(const PARCThread *thread)
{
    return thread->isRunning;
}

bool
parcThread_IsCancelled(const PARCThread *thread)
{
    return thread->isCancelled;
}

void
parcThread_Join(PARCThread *thread)
{
    pthread_join(thread->thread, NULL);
}
