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

#include <parc/concurrent/parc_AtomicUint16.h>

struct PARCAtomicUint16 {
    uint16_t value;
#ifdef PARCLibrary_DISABLE_ATOMICS
    pthread_mutex_t mutex;
#endif
};

static void
_parcAtomicUint16_Finalize(PARCAtomicUint16 **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCAtomicUint16 pointer.");

    parcAtomicUint16_OptionalAssertValid((*instancePtr));

    /* cleanup the instance fields here */
}

parcObject_ImplementAcquire(parcAtomicUint16, PARCAtomicUint16);

parcObject_ImplementRelease(parcAtomicUint16, PARCAtomicUint16);

parcObject_ExtendPARCObject(PARCAtomicUint16, _parcAtomicUint16_Finalize, parcAtomicUint16_Copy, NULL, parcAtomicUint16_Equals, parcAtomicUint16_Compare, parcAtomicUint16_HashCode, NULL);


void
parcAtomicUint16_AssertValid(const PARCAtomicUint16 *instance)
{
    assertTrue(parcAtomicUint16_IsValid(instance),
               "PARCAtomicUint16 is not valid.");
}

PARCAtomicUint16 *
parcAtomicUint16_Create(uint16_t value)
{
    PARCAtomicUint16 *result = parcObject_CreateAndClearInstance(PARCAtomicUint16);

#ifdef PARCLibrary_DISABLE_ATOMICS
    pthread_mutex_init(&result->mutex, NULL);
    result->value = value;
#else
    *result = value;
#endif

    return result;
}

int
parcAtomicUint16_Compare(const PARCAtomicUint16 *instance, const PARCAtomicUint16 *other)
{
    int16_t comparison = parcAtomicUint16_GetValue(instance) - parcAtomicUint16_GetValue(other);

    int result = 0;
    if (comparison < 0) {
        result = -1;
    } else if (comparison > 0) {
        result = 1;
    }

    return result;
}

PARCAtomicUint16 *
parcAtomicUint16_Copy(const PARCAtomicUint16 *original)
{
    PARCAtomicUint16 *result = parcAtomicUint16_Create(parcAtomicUint16_GetValue(original));

    return result;
}

bool
parcAtomicUint16_Equals(const PARCAtomicUint16 *x, const PARCAtomicUint16 *y)
{
    bool result = false;

    result = parcAtomicUint16_GetValue(x) == parcAtomicUint16_GetValue(y);

    return result;
}

PARCHashCode
parcAtomicUint16_HashCode(const PARCAtomicUint16 *instance)
{
    uint32_t result = (uint32_t) parcAtomicUint16_GetValue(instance);

    return result;
}

bool
parcAtomicUint16_IsValid(const PARCAtomicUint16 *instance)
{
    bool result = false;

    if (instance != NULL) {
        result = true;
    }

    return result;
}

uint16_t
parcAtomicUint16_GetValue(const PARCAtomicUint16 *instance)
{
#ifdef PARCLibrary_DISABLE_ATOMICS
    return instance->value;
#else
    return *instance;
#endif
}

uint16_t
parcAtomicUint16_AddImpl(PARCAtomicUint16 *value, uint16_t addend)
{
#ifdef PARCLibrary_DISABLE_ATOMICS
    pthread_mutex_lock(&value->mutex);
    value->value += addend;
    uint16_t result = value->value;
    pthread_mutex_unlock(&value->mutex);
    return result;
#else
    return __sync_add_and_fetch(value, addend);
#endif
}

uint16_t
parcAtomicUint16_SubtractImpl(PARCAtomicUint16 *value, uint16_t subtrahend)
{
#ifdef PARCLibrary_DISABLE_ATOMICS
    pthread_mutex_lock(&value->mutex);
    value->value -= subtrahend;
    uint16_t result = value->value;
    pthread_mutex_unlock(&value->mutex);
    return result;
#else
    return __sync_sub_and_fetch(value, subtrahend);
#endif
}

bool
parcAtomicUint16_CompareAndSwapImpl(PARCAtomicUint16 *value, uint16_t predicate, uint16_t newValue)
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
