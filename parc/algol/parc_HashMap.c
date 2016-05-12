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
 * @author <#Glenn Scott <Glenn.Scott@parc.com>#>, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>
#include <LongBow/runtime.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_Memory.h>

#include "parc_HashMap.h"
#include "parc_LinkedList.h"

#include <math.h>

static const uint32_t DEFAULT_CAPACITY = 43;

typedef struct PARCHashMapEntry {
    PARCObject *key;
    PARCObject *value;
} _PARCHashMapEntry;


static bool
_parcHashMapEntry_IsValid(_PARCHashMapEntry *hashEntry)
{
    bool result = false;

    if (hashEntry) {
        if (parcObject_IsValid(hashEntry->key)) {
            if (parcObject_IsValid(hashEntry->value)) {
                result = true;
            }
        }
    }

    return result;
}

static void
_parcHashMapEntry_Finalize(_PARCHashMapEntry **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCHashMap pointer.");
    _PARCHashMapEntry *hashMapEntry = *instancePtr;

    _parcHashMapEntry_IsValid(hashMapEntry);

    parcObject_Release(&hashMapEntry->key);
    parcObject_Release(&hashMapEntry->value);
}

static bool
_parcHashMapEntry_Equals(const _PARCHashMapEntry *a, const _PARCHashMapEntry *b)
{
    return (parcObject_Equals(a->key, b->key) && parcObject_Equals(a->value, b->value));
}


static PARCHashCode
_parcHashMapEntry_HashCode(const _PARCHashMapEntry *entry)
{
    return parcObject_HashCode(entry->key);
}


struct PARCHashMap {
    PARCLinkedList **buckets;
    size_t minCapacity;
    size_t capacity;
    size_t size;
    double maxLoadFactor;
    double minLoadFactor;
};

static _PARCHashMapEntry *
_parcHashMap_GetEntry(const PARCHashMap *hashMap, const PARCObject *key)
{
    PARCHashCode keyHash = parcObject_HashCode(key);

    int bucket = keyHash % hashMap->capacity;

    _PARCHashMapEntry *result = NULL;

    if (hashMap->buckets[bucket] != NULL) {
        PARCIterator *iterator = parcLinkedList_CreateIterator(hashMap->buckets[bucket]);

        while (parcIterator_HasNext(iterator)) {
            _PARCHashMapEntry *entry = parcIterator_Next(iterator);
            if (parcObject_Equals(key, entry->key)) {
                result = entry;
                break;
            }
        }
        parcIterator_Release(&iterator);
    }

    return result;
}

//static parcObject_ImplementAcquire(parcHashMapEntry, _PARCHashMapEntry);

static parcObject_ImplementRelease(_parcHashMapEntry, _PARCHashMapEntry);

parcObject_ExtendPARCObject(_PARCHashMapEntry, _parcHashMapEntry_Finalize, NULL, NULL, _parcHashMapEntry_Equals, NULL, _parcHashMapEntry_HashCode, NULL);

static _PARCHashMapEntry *
_parcHashMapEntry_Create(const PARCObject *key, const PARCObject *value)
{
    parcObject_OptionalAssertValid(key);
    parcObject_OptionalAssertValid(value);

    _PARCHashMapEntry *result = parcObject_CreateInstance(_PARCHashMapEntry);

    result->key = parcObject_Copy(key);
    result->value = parcObject_Acquire(value);

    return result;
}

static void
_parcHashMap_Finalize(PARCHashMap **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCHashMap pointer.");
    PARCHashMap *hashMap = *instancePtr;

    for (unsigned int i = 0; i < hashMap->capacity; i++) {
        if (hashMap->buckets[i] != NULL) {
            parcLinkedList_Release(&hashMap->buckets[i]);
        }
    }

    parcMemory_Deallocate(&hashMap->buckets);

    /* cleanup the instance fields here */
}

parcObject_ImplementAcquire(parcHashMap, PARCHashMap);

parcObject_ImplementRelease(parcHashMap, PARCHashMap);

parcObject_ExtendPARCObject(PARCHashMap, _parcHashMap_Finalize, parcHashMap_Copy, parcHashMap_ToString, parcHashMap_Equals, NULL, parcHashMap_HashCode, parcHashMap_ToJSON);

void
parcHashMap_AssertValid(const PARCHashMap *instance)
{
    assertTrue(parcHashMap_IsValid(instance),
               "PARCHashMap is not valid.");
}

PARCHashMap *
parcHashMap_CreateCapacity(unsigned int capacity)
{
    PARCHashMap *result = parcObject_CreateInstance(PARCHashMap);

    if (result != NULL) {
        if (capacity == 0) {
            capacity = DEFAULT_CAPACITY;
        }

        result->minCapacity = capacity;
        result->capacity = capacity;
        result->size = 0;
        result->maxLoadFactor = 0.75;
        result->minLoadFactor = result->maxLoadFactor / 3.0;
        result->buckets = parcMemory_AllocateAndClear(capacity * sizeof(PARCLinkedList*));
    }

    return result;
}

PARCHashMap *
parcHashMap_Create(void)
{
    PARCHashMap *result = parcHashMap_CreateCapacity(DEFAULT_CAPACITY);

    return result;
}

PARCHashMap *
parcHashMap_Copy(const PARCHashMap *original)
{
    parcHashMap_OptionalAssertValid(original);

    PARCHashMap *result = parcObject_CreateInstance(PARCHashMap);

    result->capacity = original->capacity;
    result->minCapacity = original->minCapacity;
    result->maxLoadFactor = original->maxLoadFactor;
    result->minLoadFactor = original->minLoadFactor;
    result->size = original->size;
    result->buckets = parcMemory_Allocate(result->capacity * sizeof(PARCLinkedList*));

    for (unsigned int i = 0; i < result->capacity; i++) {
        result->buckets[i] = NULL;
        if (original->buckets[i] != NULL) {
            result->buckets[i] = parcLinkedList_Copy(original->buckets[i]);
        }
    }

    return result;
}

void
parcHashMap_Display(const PARCHashMap *hashMap, int indentation)
{
    parcDisplayIndented_PrintLine(indentation, "PARCHashMap@%p {", hashMap);

    PARCIterator *iterator = parcHashMap_CreateKeyIterator((PARCHashMap *) hashMap);

    while (parcIterator_HasNext(iterator)) {
        PARCObject *keyObject = parcIterator_Next(iterator);
        const PARCObject *valueObject = parcHashMap_Get(hashMap, keyObject);
        char *key = parcObject_ToString(keyObject);
        char *value = parcObject_ToString(valueObject);
        parcDisplayIndented_PrintLine(indentation + 1, "%s -> %s", key, value);
        parcMemory_Deallocate(&key);
        parcMemory_Deallocate(&value);
    }
    parcIterator_Release(&iterator);

    parcDisplayIndented_PrintLine(indentation, "}");
}

bool
parcHashMap_Equals(const PARCHashMap *x, const PARCHashMap *y)
{
    bool result = false;

    if (x == y) {
        result = true;
    } else if (x == NULL || y == NULL) {
        result = false;
    } else {
        parcHashMap_OptionalAssertValid(x);
        parcHashMap_OptionalAssertValid(y);

        if (x->capacity == y->capacity) {
            if (x->size == y->size) {
                result = true;
                for (unsigned int i = 0; (i < x->capacity) && result; i++) {
                    if ((x->buckets[i] == NULL) || (y->buckets[i] == NULL)) {
                        result =  (x->buckets[i] == y->buckets[i]);
                    } else {
                        // For each item in an X bucket, it must be in the Y bucket.
                        result = parcLinkedList_SetEquals(x->buckets[i], y->buckets[i]);
                    }
                }
            }
        }
    }

    return result;
}

PARCHashCode
parcHashMap_HashCode(const PARCHashMap *hashMap)
{
    parcHashMap_OptionalAssertValid(hashMap);

    PARCHashCode result = 0;

    for (unsigned int i = 0; i < hashMap->capacity; i++) {
        if (hashMap->buckets[i] != NULL) {
            result += parcLinkedList_HashCode(hashMap->buckets[i]);
        }
    }

    return result;
}

bool
parcHashMap_IsValid(const PARCHashMap *map)
{
    bool result = false;

    if (map != NULL) {
        if (parcObject_IsValid(map)) {
            result = true;

            for (unsigned int i = 0; i < map->capacity; i++) {
                if (map->buckets[i] != NULL) {
                    if (parcLinkedList_IsValid(map->buckets[i]) == false) {
                        result = false;
                        break;
                    }
                }
            }
        }
    }

    return result;
}

PARCJSON *
parcHashMap_ToJSON(const PARCHashMap *hashMap)
{
    parcHashMap_OptionalAssertValid(hashMap);

    PARCJSON *result = parcJSON_Create();

    PARCIterator *iterator = parcHashMap_CreateKeyIterator((PARCHashMap *) hashMap);

    while (parcIterator_HasNext(iterator)) {
        PARCObject *keyObject = parcIterator_Next(iterator);
        const PARCObject *valueObject = parcHashMap_Get(hashMap, keyObject);
        char *key = parcObject_ToString(keyObject);
        PARCJSON *value = parcObject_ToJSON(valueObject);

        parcJSON_AddObject(result, key, value);

        parcMemory_Deallocate(&key);
        parcJSON_Release(&value);
    }

    parcIterator_Release(&iterator);


    return result;
}

PARCBufferComposer *
parcHashMap_BuildString(const PARCHashMap *hashMap, PARCBufferComposer *composer)
{
    PARCIterator *iterator = parcHashMap_CreateKeyIterator((PARCHashMap *) hashMap);

    while (parcIterator_HasNext(iterator)) {
        PARCObject *keyObject = parcIterator_Next(iterator);
        const PARCObject *valueObject = parcHashMap_Get(hashMap, keyObject);
        char *key = parcObject_ToString(keyObject);
        char *value = parcObject_ToString(valueObject);
        parcBufferComposer_Format(composer, "%s -> %s\n", key, value);
        parcMemory_Deallocate(&key);
        parcMemory_Deallocate(&value);
    }

    parcIterator_Release(&iterator);

    return composer;
}

char *
parcHashMap_ToString(const PARCHashMap *hashMap)
{
    parcHashMap_OptionalAssertValid(hashMap);
    char *result = NULL;

    PARCBufferComposer *composer = parcBufferComposer_Create();
    if (composer != NULL) {
        parcHashMap_BuildString(hashMap, composer);
        PARCBuffer *tempBuffer = parcBufferComposer_ProduceBuffer(composer);
        result = parcBuffer_ToString(tempBuffer);
        parcBuffer_Release(&tempBuffer);
        parcBufferComposer_Release(&composer);
    }

    return result;
}

bool
parcHashMap_Contains(PARCHashMap *hashMap, const PARCObject *key)
{
    PARCObject *result = NULL;

    _PARCHashMapEntry *entry = _parcHashMap_GetEntry(hashMap, key);
    if (entry != NULL) {
        result = entry->value;
    }

    return result;
}

static void
_parcHashMap_Resize(PARCHashMap *hashMap, size_t newCapacity)
{
    if (newCapacity < hashMap->minCapacity) {
        return;
    }

    PARCLinkedList **newBuckets = parcMemory_AllocateAndClear(newCapacity * sizeof(PARCLinkedList*));

    for (unsigned int i = 0; i < hashMap->capacity; i++) {
        if (hashMap->buckets[i] != NULL) {
            if (!parcLinkedList_IsEmpty(hashMap->buckets[i])) {
                PARCIterator *elementIt = parcLinkedList_CreateIterator(hashMap->buckets[i]);
                while (parcIterator_HasNext(elementIt)) {
                    _PARCHashMapEntry *entry = parcIterator_Next(elementIt);
                    PARCHashCode keyHash = parcObject_HashCode(entry->key);
                    int newBucket = keyHash % newCapacity;
                    if (newBuckets[newBucket] == NULL) {
                        newBuckets[newBucket] = parcLinkedList_Create();
                    }
                    parcLinkedList_Append(newBuckets[newBucket], entry);
                }
                parcIterator_Release(&elementIt);
            }
            parcLinkedList_Release(&hashMap->buckets[i]);
        }
    }
    PARCLinkedList **cleanupBuckets = hashMap->buckets;
    hashMap->buckets = newBuckets;
    hashMap->capacity = newCapacity;

    parcMemory_Deallocate(&cleanupBuckets);
}

bool
parcHashMap_Remove(PARCHashMap *hashMap, const PARCObject *key)
{
    PARCHashCode keyHash = parcObject_HashCode(key);

    int bucket = keyHash % hashMap->capacity;

    bool result = false;

    if (hashMap->buckets[bucket] != NULL) {
        PARCIterator *iterator = parcLinkedList_CreateIterator(hashMap->buckets[bucket]);

        while (parcIterator_HasNext(iterator)) {
            _PARCHashMapEntry *entry = parcIterator_Next(iterator);
            if (parcObject_Equals(key, entry->key)) {
                parcIterator_Remove(iterator);
                hashMap->size--;
                result = true;
                break;
            }
        }
        parcIterator_Release(&iterator);
    }

    // When expanded by 2 the load factor goes from .75 (3/4) to .375 (3/8), if
    // we compress by 2 when the load factor is .25 (1/4) the load
    // factor becomes .5 (1/2).
    double loadFactor = (double)hashMap->size/(double)hashMap->capacity;
    if (loadFactor <= (hashMap->minLoadFactor)) {
        _parcHashMap_Resize(hashMap, hashMap->capacity / 2);
    }

    return result;
}

#include <stdio.h>

PARCHashMap *
parcHashMap_Put(PARCHashMap *hashMap, const PARCObject *key, const PARCObject *value)
{

    // When expanded by 2 the load factor goes from .75 (3/4) to .375 (3/8), if
    // we compress by 2 when the load factor is .25 (1/4) the load
    // factor becomes .5 (1/2).
    double loadFactor = (double)hashMap->size/(double)hashMap->capacity;
    if (loadFactor >= hashMap->maxLoadFactor) {
        _parcHashMap_Resize(hashMap, hashMap->capacity * 2);
    }

    _PARCHashMapEntry *entry = _parcHashMap_GetEntry(hashMap, key);

    if (entry != NULL) {
        if (entry->value != value) {
            parcObject_Release(&entry->value);
            entry->value = parcObject_Acquire(value);
        }
    } else {
        entry = _parcHashMapEntry_Create(key, value);

        PARCHashCode keyHash = parcObject_HashCode(key);
        int bucket = keyHash % hashMap->capacity;

        if (hashMap->buckets[bucket] == NULL) {
            hashMap->buckets[bucket] = parcLinkedList_Create();
        }
        parcLinkedList_Append(hashMap->buckets[bucket], entry);
        hashMap->size++;
        _parcHashMapEntry_Release(&entry);
    }

    return hashMap;
}

const PARCObject *
parcHashMap_Get(const PARCHashMap *hashMap, const PARCObject *key)
{
    PARCObject *result = NULL;

    _PARCHashMapEntry *entry = _parcHashMap_GetEntry(hashMap, key);
    if (entry != NULL) {
        result = entry->value;
    }

    return result;
}

size_t
parcHashMap_Size(const PARCHashMap *hashMap)
{
    parcHashMap_OptionalAssertValid(hashMap);
    return hashMap->size;
}

double
parcHashMap_GetClusteringNumber(const PARCHashMap *hashMap)
{
    // This function compute the standard deviation of the chain-lengths
    // from a value of 1.0 (as opposed to the mean) and weights the
    // result by in inverse of the current load factor. The deviation
    // from 1.0 is used because the hashmap's max load factor is < 1.0 and
    // thus the ideal average chain-length is 1.0
    //
    // A result of 0.0 equates to an ideal distribution, a result of ~1.0 should
    // represent a fairly normal or random distribution, and a result > 1.5 or so
    // implies some amount of undesirable clumping may be happening.

    size_t totalLength = 0;
    double variance = 0;

    // Compute the variance vs 1.0
    for (size_t i = 0; i < hashMap->capacity; ++i) {
        if (hashMap->buckets[i] != NULL) {
            size_t bucketSize = parcLinkedList_Size(hashMap->buckets[i]);
            totalLength += bucketSize;
            variance += (bucketSize - 1) * (bucketSize - 1); //Variance relative to 1
        }
    }
    variance /= ((double)totalLength);

    // Compute the standard deviation
    double standardDeviation = sqrt(variance);

    // Weight the standard deviation by the inverse of the current load factor
    return standardDeviation * ((double)hashMap->capacity/(double)totalLength);
}

typedef struct {
    PARCHashMap *map;
    int bucket;
    PARCIterator *listIterator;
    _PARCHashMapEntry *current;
} _PARCHashMapIterator;

static _PARCHashMapIterator *
_parcHashMap_Init(PARCHashMap *map __attribute__((unused)))
{
    _PARCHashMapIterator *state = parcMemory_AllocateAndClear(sizeof(_PARCHashMapIterator));

    if (state != NULL) {
        state->map = map;
        state->bucket = 0;
        state->listIterator = NULL;
        for (size_t i = 0; i < map->capacity; ++i) {
            if (map->buckets[i] != NULL) {
                state->bucket = i;
                state->listIterator = parcLinkedList_CreateIterator(map->buckets[i]);
                break;
            }
        }

        trapOutOfMemoryIf(state->listIterator == NULL, "Cannot create parcLinkedList_CreateIterator");
    }

    return state;
}

static bool
_parcHashMap_Fini(PARCHashMap *map __attribute__((unused)), _PARCHashMapIterator *state __attribute__((unused)))
{
    if (state->listIterator != NULL) {
        parcIterator_Release(&state->listIterator);
    }
    parcMemory_Deallocate(&state);
    return true;
}

static _PARCHashMapIterator *
_parcHashMap_Next(PARCHashMap *map __attribute__((unused)), _PARCHashMapIterator *state)
{
    _PARCHashMapEntry *result = parcIterator_Next(state->listIterator);
    state->current = result;
    return state;
}

static void
_parcHashMap_Remove(PARCHashMap *map, _PARCHashMapIterator **statePtr __attribute__((unused)))
{
    _PARCHashMapIterator *state = *statePtr;

    if (state->listIterator != NULL) {
        parcIterator_Remove(state->listIterator);
        map->size--;
    }
}

static bool
_parcHashMap_HasNext(PARCHashMap *map __attribute__((unused)), _PARCHashMapIterator *state)
{
    bool result = false;
    if (state->listIterator != NULL) {
        if (parcIterator_HasNext(state->listIterator)) {
            result = true;
        } else {
            while ((result == false) && (++state->bucket < map->capacity)) {
                if (map->buckets[state->bucket] != NULL) {
                    parcIterator_Release(&state->listIterator);
                    state->listIterator = parcLinkedList_CreateIterator(map->buckets[state->bucket]);
                    trapOutOfMemoryIf(state->listIterator == NULL, "Cannot create parcLinkedList_CreateIterator");
                    result = parcIterator_HasNext(state->listIterator);
                }
            }
        }
    }
    return result;
}

static PARCObject *
_parcHashMapValue_Element(PARCHashMap *map __attribute__((unused)), const _PARCHashMapIterator *state)
{
    return state->current->value;
}

static PARCObject *
_parcHashMapKey_Element(PARCHashMap *map __attribute__((unused)), const _PARCHashMapIterator *state)
{
    return state->current->key;
}

PARCIterator *
parcHashMap_CreateValueIterator(PARCHashMap *hashMap)
{
    PARCIterator *iterator = parcIterator_Create(hashMap,
                                                 (void *(*)(PARCObject *))_parcHashMap_Init,
                                                 (bool (*)(PARCObject *, void *))_parcHashMap_HasNext,
                                                 (void *(*)(PARCObject *, void *))_parcHashMap_Next,
                                                 (void (*)(PARCObject *, void **))_parcHashMap_Remove,
                                                 (void *(*)(PARCObject *, void *))_parcHashMapValue_Element,
                                                 (void (*)(PARCObject *, void *))_parcHashMap_Fini,
                                                 NULL);

    return iterator;
}


PARCIterator *
parcHashMap_CreateKeyIterator(PARCHashMap *hashMap)
{
    PARCIterator *iterator = parcIterator_Create(hashMap,
                                                 (void *(*)(PARCObject *))_parcHashMap_Init,
                                                 (bool (*)(PARCObject *, void *))_parcHashMap_HasNext,
                                                 (void *(*)(PARCObject *, void *))_parcHashMap_Next,
                                                 (void (*)(PARCObject *, void **))_parcHashMap_Remove,
                                                 (void *(*)(PARCObject *, void *))_parcHashMapKey_Element,
                                                 (void (*)(PARCObject *, void *))_parcHashMap_Fini,
                                                 NULL);

    return iterator;
}
