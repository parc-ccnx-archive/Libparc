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
 * @author Glenn Scott <Glenn.Scott@parc.com>, Palo Alto Research Center (Xerox PARC)
 * @copyright 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>

#include <parc/concurrent/parc_AtomicUint64.h>

struct PARCAtomicUint64 {
    uint64_t value;
#ifdef PARCLibrary_DISABLE_ATOMICS
    pthread_mutex_t mutex;
#endif
};

static void
_parcAtomicUint64_Finalize(PARCAtomicUint64 **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCAtomicUint64 pointer.");

    parcAtomicUint64_OptionalAssertValid((*instancePtr));

    /* cleanup the instance fields here */
}

parcObject_ImplementAcquire(parcAtomicUint64, PARCAtomicUint64);

parcObject_ImplementRelease(parcAtomicUint64, PARCAtomicUint64);

parcObject_ExtendPARCObject(PARCAtomicUint64, _parcAtomicUint64_Finalize, parcAtomicUint64_Copy, NULL, parcAtomicUint64_Equals, parcAtomicUint64_Compare, parcAtomicUint64_HashCode, NULL);


void
parcAtomicUint64_AssertValid(const PARCAtomicUint64 *instance)
{
    assertTrue(parcAtomicUint64_IsValid(instance),
               "PARCAtomicUint64 is not valid.");
}

PARCAtomicUint64 *
parcAtomicUint64_Create(uint64_t value)
{
    PARCAtomicUint64 *result = parcObject_CreateAndClearInstance(PARCAtomicUint64);

#ifdef PARCLibrary_DISABLE_ATOMICS
    pthread_mutex_init(&result->mutex, NULL);
    result->value = value;
#else
    *result = value;
#endif

    return result;
}

int
parcAtomicUint64_Compare(const PARCAtomicUint64 *instance, const PARCAtomicUint64 *other)
{
    int64_t comparison = parcAtomicUint64_GetValue(instance) - parcAtomicUint64_GetValue(other);

    int result = 0;
    if (comparison < 0) {
        result = -1;
    } else if (comparison > 0) {
        result = 1;
    }

    return result;
}

PARCAtomicUint64 *
parcAtomicUint64_Copy(const PARCAtomicUint64 *original)
{
    PARCAtomicUint64 *result = parcAtomicUint64_Create(parcAtomicUint64_GetValue(original));

    return result;
}

bool
parcAtomicUint64_Equals(const PARCAtomicUint64 *x, const PARCAtomicUint64 *y)
{
    bool result = false;

    result = parcAtomicUint64_GetValue(x) == parcAtomicUint64_GetValue(y);

    return result;
}

PARCHashCode
parcAtomicUint64_HashCode(const PARCAtomicUint64 *instance)
{
    PARCHashCode result = (PARCHashCode) parcAtomicUint64_GetValue(instance);

    return result;
}

bool
parcAtomicUint64_IsValid(const PARCAtomicUint64 *instance)
{
    bool result = false;

    if (instance != NULL) {
        result = true;
    }

    return result;
}

uint64_t
parcAtomicUint64_GetValue(const PARCAtomicUint64 *instance)
{
#ifdef PARCLibrary_DISABLE_ATOMICS
    return instance->value;
#else
    return *instance;
#endif
}

uint64_t
parcAtomicUint64_AddImpl(PARCAtomicUint64 *value, uint64_t addend)
{
#ifdef PARCLibrary_DISABLE_ATOMICS
    pthread_mutex_lock(&value->mutex);
    value->value += addend;
    uint64_t result = value->value;
    pthread_mutex_unlock(&value->mutex);
    return result;
#else
    return __sync_add_and_fetch(value, addend);
#endif
}

uint64_t
parcAtomicUint64_SubtractImpl(PARCAtomicUint64 *value, uint64_t subtrahend)
{
#ifdef PARCLibrary_DISABLE_ATOMICS
    pthread_mutex_lock(&value->mutex);
    value->value -= subtrahend;
    uint64_t result = value->value;
    pthread_mutex_unlock(&value->mutex);
    return result;
#else
    return __sync_sub_and_fetch(value, subtrahend);
#endif
}

bool
parcAtomicUint64_CompareAndSwapImpl(PARCAtomicUint64 *value, uint64_t predicate, uint64_t newValue)
{
    bool result = false;
#ifdef PARCLibrary_DISABLE_ATOMICS
    pthread_mutex_lock(&value->mutex);
    if (value->value == predicate) {
        value->value = newValue;
        result = true;
    }
    pthread_mutex_unlock(&value->mutex);
    return result;
#else
    result = __sync_bool_compare_and_swap(value, predicate, newValue);
#endif
    return result;
}
