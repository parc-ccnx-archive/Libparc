/*
 * Copyright (c) 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @author <#Glenn Scott <Glenn.Scott@parc.com>#>, Palo Alto Research Center (Xerox PARC)
 * @copyright 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>

#include <pthread.h>
#include <errno.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>

#include <parc/concurrent/parc_Lock.h>

struct PARCLock {
    pthread_mutex_t lock;
    pthread_mutexattr_t lockAttributes;
    pthread_cond_t notification;
    bool locked;

    bool notified;
};

static void
_parcLock_Finalize(PARCLock **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCLock pointer.");

    parcLock_OptionalAssertValid(*instancePtr);

    /* cleanup the instance fields here */
}

parcObject_ImplementAcquire(parcLock, PARCLock);

parcObject_ImplementRelease(parcLock, PARCLock);

parcObject_ExtendPARCObject(PARCLock, _parcLock_Finalize, NULL, parcLock_ToString, NULL, NULL, NULL, NULL);

void
parcLock_AssertValid(const PARCLock *instance)
{
    assertTrue(parcLock_IsValid(instance),
               "PARCLock is not valid.");
}

PARCLock *
parcLock_Create(void)
{
    PARCLock *result = parcObject_CreateInstance(PARCLock);

    pthread_mutexattr_init(&result->lockAttributes);
    pthread_mutexattr_settype(&result->lockAttributes, PTHREAD_MUTEX_ERRORCHECK);

    pthread_mutex_init(&result->lock, &result->lockAttributes);

    result->locked = false;
    pthread_cond_init(&result->notification, NULL);
    result->notified = false;
    return result;
}

void
parcLock_Display(const PARCLock *lock, int indentation)
{
    parcDisplayIndented_PrintLine(indentation, "PARCLock@%p {", lock);
    parcDisplayIndented_PrintLine(indentation + 1, ".locked=%s", lock->locked ? "true" : "false");

    parcDisplayIndented_PrintLine(indentation, "}");
}

bool
parcLock_IsValid(const PARCLock *instance)
{
    bool result = false;

    if (instance != NULL) {
        result = true;
    }

    return result;
}

PARCBufferComposer *
parcLock_BuildString(const PARCLock *lock, PARCBufferComposer *composer)
{
    parcBufferComposer_Format(composer, "lock{.state=%s }", lock->locked ? "true" : "false");

    return composer;
}

char *
parcLock_ToString(const PARCLock *lock)
{
    char *result = NULL;

    PARCBufferComposer *composer = parcBufferComposer_Create();
    if (composer != NULL) {
        parcLock_BuildString(lock, composer);
        result = parcBufferComposer_ToString(composer);
        parcBufferComposer_Release(&composer);
    }

    return result;
}

bool
parcLock_Unlock(PARCLock *lock)
{
    bool result = false;

    parcLock_OptionalAssertValid(lock);

    if (lock->locked) {
        result = (pthread_mutex_unlock(&lock->lock) == 0);
    }

    return result;
}

bool
parcLock_Lock(PARCLock *lock)
{
    bool result = false;

    parcLock_OptionalAssertValid(lock);

    int error = pthread_mutex_lock(&lock->lock);

    if (error == 0) {
        lock->locked = true;
        result = true;
        errno = 0;
    } else {
        errno = error;
    }

    return result;
}

bool
parcLock_TryLock(PARCLock *lock)
{
    bool result = false;

    parcLock_OptionalAssertValid(lock);

    result = (pthread_mutex_trylock(&lock->lock) == 0);
    if (result) {
        lock->locked = true;
    }

    return result;
}

bool
parcLock_IsLocked(const PARCLock *lock)
{
    parcLock_OptionalAssertValid(lock);

    return lock->locked;
}

void
parcLock_Wait(PARCLock *lock)
{
    parcLock_OptionalAssertValid(lock);

    trapUnexpectedStateIf(lock->locked == false,
                          "You must Lock the object before calling parcLock_Wait");

    lock->notified = false;
    while (lock->notified == false) {
        pthread_cond_wait(&lock->notification, &lock->lock);
    }
}

void
parcLock_Notify(PARCLock *lock)
{
    parcLock_OptionalAssertValid(lock);

    trapUnexpectedStateIf(lock->locked == false,
                          "You must Lock the object before calling parcLock_Notify");

    lock->notified = true;
    pthread_cond_signal(&lock->notification);
}

