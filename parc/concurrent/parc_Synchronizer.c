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
 * @author Glenn Scott <Glenn.Scott@parc.com>, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>

#include <parc/concurrent/parc_Synchronizer.h>

#ifdef PARCLibrary_DISABLE_ATOMICS
#  include <pthread.h>
#else
//#  include <pthread.h>
#endif

typedef enum {
    _PARCSynchronizer_Unlocked = 0,
    _PARCSynchronizer_Locked = 1
} _PARCSynchronizer;

struct PARCSynchronizer {
#ifdef PARCLibrary_DISABLE_ATOMICS
    pthread_mutex_t mutex;
#else
    int mutex;
#endif
};

static void
_parcSynchronizer_Finalize(PARCSynchronizer **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCSynchronizer pointer.");

    parcSynchronizer_OptionalAssertValid((*instancePtr));
}

parcObject_ImplementAcquire(parcSynchronizer, PARCSynchronizer);

parcObject_ImplementRelease(parcSynchronizer, PARCSynchronizer);

parcObject_ExtendPARCObject(PARCSynchronizer, _parcSynchronizer_Finalize, NULL, NULL, NULL, NULL, NULL, NULL);

void
parcSynchronizer_AssertValid(const PARCSynchronizer *instance)
{
    assertTrue(parcSynchronizer_IsValid(instance),
               "PARCSynchronizer is not valid.");
}

PARCSynchronizer *
parcSynchronizer_Create(void)
{
    PARCSynchronizer *result = parcObject_CreateInstance(PARCSynchronizer);

#ifdef PARCLibrary_DISABLE_ATOMICS
    pthread_mutex_init(&result->mutex, NULL);
#else
    result->mutex = _PARCSynchronizer_Unlocked;
#endif

    return result;
}

void
parcSynchronizer_Display(const PARCSynchronizer *instance, int indentation)
{
    parcDisplayIndented_PrintLine(indentation, "PARCSynchronizer@%p {", instance);
    /* Call Display() functions for the fields here. */
    parcDisplayIndented_PrintLine(indentation, "}");
}

bool
parcSynchronizer_IsValid(const PARCSynchronizer *instance)
{
    bool result = false;

    if (instance != NULL) {
        result = true;
    }

    return result;
}

bool
parcSynchronizer_TryLock(PARCSynchronizer *instance)
{
#ifdef PARCLibrary_DISABLE_ATOMICS
    bool result = pthread_mutex_trylock(&instance->mutex) == 0;
#else
    bool result = __sync_bool_compare_and_swap(&instance->mutex, _PARCSynchronizer_Unlocked, _PARCSynchronizer_Locked);
#endif
    return result;
}

void
parcSynchronizer_Lock(PARCSynchronizer *instance)
{
#ifdef PARCLibrary_DISABLE_ATOMICS
    pthread_mutex_lock(&instance->mutex);
#else
    while (!__sync_bool_compare_and_swap(&instance->mutex, _PARCSynchronizer_Unlocked, _PARCSynchronizer_Locked)) {
        ;
    }
#endif
}

void
parcSynchronizer_Unlock(PARCSynchronizer *instance)
{
#ifdef PARCLibrary_DISABLE_ATOMICS
    pthread_mutex_unlock(&instance->mutex);
#else
    while (!__sync_bool_compare_and_swap(&instance->mutex, _PARCSynchronizer_Locked, _PARCSynchronizer_Unlocked)) {
        ;
    }
#endif
}

bool
parcSynchronizer_IsLocked(const PARCSynchronizer *barrier)
{
#ifdef PARCLibrary_DISABLE_ATOMICS
    PARCSynchronizer *instance = (PARCSynchronizer *) barrier;

    bool result = pthread_mutex_trylock(&instance->mutex) != 0;
    pthread_mutex_unlock(&instance->mutex);
    return result;
#else
    return barrier->mutex == _PARCSynchronizer_Locked;
#endif
}
