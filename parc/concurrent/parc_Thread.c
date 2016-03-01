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
#include <pthread.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_Memory.h>

#include <parc/concurrent/parc_Thread.h>

struct PARCThread {
    void *(*run)(PARCThread *);
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
    
    parcThread_Cancel(thread);
    parcThread_Join(thread);
    
    return true;
}

parcObject_ImplementAcquire(parcThread, PARCThread);

parcObject_ImplementRelease(parcThread, PARCThread);

parcObject_Override(PARCThread, PARCObject,
                    .destructor = (PARCObjectDestructor *) _parcThread_Destructor,
                    .copy = (PARCObjectCopy *) parcThread_Copy,
                    .toString = (PARCObjectToString *) parcThread_ToString,
                    .equals = (PARCObjectEquals *) parcThread_Equals,
                    .compare = (PARCObjectCompare *) parcThread_Compare,
                    .hashCode = (PARCObjectHashCode *) parcThread_HashCode,
                    .display = (PARCObjectDisplay *) parcThread_Display);

void
parcThread_AssertValid(const PARCThread *instance)
{
    assertTrue(parcThread_IsValid(instance),
               "PARCThread is not valid.");
}

PARCThread *
parcThread_Create(void *(*runFunction)(PARCThread *), PARCObject *restrict parameter)
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
    PARCThread *result = NULL;
    
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
        if (x->thread == y->thread) {
            result = true;
        }
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
    char *result = parcMemory_Format("PARCThread@%p{.id=%d, .isCancelled=%s}", thread, thread->thread, thread->isCancelled ? "true" : "false");

    return result;
}

void
parcThread_Start(PARCThread *thread)
{
    thread->isRunning = true;
    pthread_create(&thread->thread, NULL, (void *(*)(void *)) thread->run, thread);
}

PARCObject *
parcThread_GetParameter(const PARCThread *thread)
{
    return thread->argument;
}

bool
parcThread_Cancel(PARCThread *thread)
{
    int returnValue = pthread_cancel(thread->thread);
    
    if (thread->argument != NULL) {
        parcObject_Release(&thread->argument);
    }
    thread->isCancelled = true;
    return returnValue == 0;
}

int
parcThread_GetId(const PARCThread *thread)
{
    return thread->thread;
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
parcThread_Join(const PARCThread *thread)
{
    pthread_join(thread->thread, NULL);
}
