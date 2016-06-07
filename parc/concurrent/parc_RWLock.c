/*
 * Copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @author <#Greg Rutz <g.rutz@cablelabs.com>#>, Cable Television Laboratories, Inc. (CableLabs)
 * @copyright (c) 2016, Cable Television Laboratories, Inc.  All rights reserved.
 */
#include <config.h>

#include <pthread.h>
#include <errno.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>

#include <parc/concurrent/parc_RWLock.h>

struct PARCRWLock {
    pthread_rwlock_t lock;
};

static void
_parcRWLock_Finalize(PARCRWLock **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCRWLock pointer.");

    parcRWLock_OptionalAssertValid(*instancePtr);

    /* cleanup the instance fields here */
    pthread_rwlock_destroy(&(*instancePtr)->lock);
}

parcObject_ImplementAcquire(parcRWLock, PARCRWLock);

parcObject_ImplementRelease(parcRWLock, PARCRWLock);

parcObject_ExtendPARCObject(PARCRWLock, _parcRWLock_Finalize, NULL, NULL, NULL, NULL, NULL, NULL);

void
parcRWLock_AssertValid(const PARCRWLock *instance)
{
    assertTrue(parcRWLock_IsValid(instance),
               "PARCRWLock is not valid.");
}

PARCRWLock *
parcRWLock_Create(void)
{
    PARCRWLock *result = parcObject_CreateInstance(PARCRWLock);

    pthread_rwlock_init(&result->lock, NULL);

    return result;
}

bool
parcRWLock_IsValid(const PARCRWLock *instance)
{
    bool result = false;

    if (instance != NULL) {
        result = true;
    }

    return result;
}

bool
parcRWLock_ReadLock(PARCRWLock *lock)
{
    bool result = false;

    parcRWLock_OptionalAssertValid(lock);

    int error = pthread_rwlock_rdlock(&lock->lock);

    if (error == 0) {
        result = true;
        errno = 0;
    } else {
        errno = error;
    }

    return result;
}

bool
parcRWLock_WriteLock(PARCRWLock *lock)
{
    bool result = false;

    parcRWLock_OptionalAssertValid(lock);

    int error = pthread_rwlock_wrlock(&lock->lock);

    if (error == 0) {
        result = true;
        errno = 0;
    } else {
        errno = error;
    }

    return result;
}

bool
parcRWLock_TryReadLock(PARCRWLock *lock)
{
    bool result = false;

    parcRWLock_OptionalAssertValid(lock);

    int error = pthread_rwlock_tryrdlock(&lock->lock);

    if (error == 0) {
        result = true;
        errno = 0;
    } else {
        errno = error;
    }

    return result;
}

bool
parcRWLock_TryWriteLock(PARCRWLock *lock)
{
    bool result = false;

    parcRWLock_OptionalAssertValid(lock);

    int error = pthread_rwlock_trywrlock(&lock->lock);

    if (error == 0) {
        result = true;
        errno = 0;
    } else {
        errno = error;
    }

    return result;
}

bool
parcRWLock_Unlock(PARCRWLock *lock)
{
    bool result = false;

    parcRWLock_OptionalAssertValid(lock);

    int error = pthread_rwlock_unlock(&lock->lock);

    if (error == 0) {
        result = true;
        errno = 0;
    } else {
        errno = error;
    }

    return result;
}
