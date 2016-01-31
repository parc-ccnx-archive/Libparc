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

#include <parc/concurrent/parc_AtomicUint32.h>

struct PARCAtomicUint32 {
    uint32_t value;
#ifdef PARCLibrary_DISABLE_ATOMICS
    pthread_mutex_t mutex;
#endif
};

static void
_parcAtomicUint32_Finalize(PARCAtomicUint32 **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCAtomicUint32 pointer.");

    parcAtomicUint32_OptionalAssertValid((*instancePtr));

    /* cleanup the instance fields here */
}

parcObject_ImplementAcquire(parcAtomicUint32, PARCAtomicUint32);

parcObject_ImplementRelease(parcAtomicUint32, PARCAtomicUint32);

parcObject_ExtendPARCObject(PARCAtomicUint32, _parcAtomicUint32_Finalize, parcAtomicUint32_Copy, NULL, parcAtomicUint32_Equals, parcAtomicUint32_Compare, parcAtomicUint32_HashCode, NULL);


void
parcAtomicUint32_AssertValid(const PARCAtomicUint32 *instance)
{
    assertTrue(parcAtomicUint32_IsValid(instance),
               "PARCAtomicUint32 is not valid.");
}

PARCAtomicUint32 *
parcAtomicUint32_Create(uint32_t value)
{
    PARCAtomicUint32 *result = parcObject_CreateAndClearInstance(PARCAtomicUint32);

#ifdef PARCLibrary_DISABLE_ATOMICS
    pthread_mutex_init(&result->mutex, NULL);
    result->value = value;
#else
    *result = value;
#endif

    return result;
}

int
parcAtomicUint32_Compare(const PARCAtomicUint32 *instance, const PARCAtomicUint32 *other)
{
    int32_t comparison = parcAtomicUint32_GetValue(instance) - parcAtomicUint32_GetValue(other);

    int result = 0;
    if (comparison < 0) {
        result = -1;
    } else if (comparison > 0) {
        result = 1;
    }

    return result;
}

PARCAtomicUint32 *
parcAtomicUint32_Copy(const PARCAtomicUint32 *original)
{
    PARCAtomicUint32 *result = parcAtomicUint32_Create(parcAtomicUint32_GetValue(original));

    return result;
}

bool
parcAtomicUint32_Equals(const PARCAtomicUint32 *x, const PARCAtomicUint32 *y)
{
    bool result = false;

    result = parcAtomicUint32_GetValue(x) == parcAtomicUint32_GetValue(y);

    return result;
}

uint32_t
parcAtomicUint32_HashCode(const PARCAtomicUint32 *instance)
{
    uint32_t result = (uint32_t) parcAtomicUint32_GetValue(instance);

    return result;
}

bool
parcAtomicUint32_IsValid(const PARCAtomicUint32 *instance)
{
    bool result = false;

    if (instance != NULL) {
        result = true;
    }

    return result;
}

uint32_t
parcAtomicUint32_GetValue(const PARCAtomicUint32 *instance)
{
#ifdef PARCLibrary_DISABLE_ATOMICS
    return instance->value;
#else
    return *instance;
#endif
}

uint32_t
parcAtomicUint32_AddImpl(PARCAtomicUint32 *value, uint32_t addend)
{
#ifdef PARCLibrary_DISABLE_ATOMICS
    pthread_mutex_lock(&value->mutex);
    value->value += addend;
    uint32_t result = value->value;
    pthread_mutex_unlock(&value->mutex);
    return result;
#else
    return __sync_add_and_fetch(value, addend);
#endif
}

uint32_t
parcAtomicUint32_SubtractImpl(PARCAtomicUint32 *value, uint32_t subtrahend)
{
#ifdef PARCLibrary_DISABLE_ATOMICS
    pthread_mutex_lock(&value->mutex);
    value->value -= subtrahend;
    uint32_t result = value->value;
    pthread_mutex_unlock(&value->mutex);
    return result;
#else
    return __sync_sub_and_fetch(value, subtrahend);
#endif
}

bool
parcAtomicUint32_CompareAndSwapImpl(PARCAtomicUint32 *value, uint32_t predicate, uint32_t newValue)
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
