/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @author Nacho "Ignacio" Solis, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

#include <config.h>

#include <LongBow/runtime.h>

#include <string.h>

#include <parc/algol/parc_Dictionary.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_TreeRedBlack.h>

struct parc_dictionary {
    PARCDictionary_CompareKey keyCompareFunction;
    PARCDictionary_KeyHashFunc keyHashFunction;
    PARCDictionary_FreeKey keyFreeFunction;
    PARCDictionary_FreeValue valueFreeFunction;
    PARCDictionary_ValueEquals valueEqualsFunction;
    PARCTreeRedBlack *tree;
};


PARCDictionary *
parcDictionary_Create(PARCDictionary_CompareKey keyCompareFunction,
                      PARCDictionary_KeyHashFunc keyHashFunction,
                      PARCDictionary_FreeKey keyFreeFunction,
                      PARCDictionary_ValueEquals valueEqualsFunction,
                      PARCDictionary_FreeValue valueFreeFunction)
{
    assertNotNull(keyCompareFunction, "KeyCompareFunction can't be null");
    assertNotNull(keyHashFunction, "KeyHashFunction can't be null");
    PARCDictionary *dictionary = parcMemory_Allocate(sizeof(PARCDictionary));
    assertNotNull(dictionary, "parcMemory_Allocate(%zu) returned NULL", sizeof(PARCDictionary));
    dictionary->keyCompareFunction = keyCompareFunction;
    dictionary->keyHashFunction = keyHashFunction;
    dictionary->keyFreeFunction = keyFreeFunction;
    dictionary->valueFreeFunction = valueFreeFunction;
    dictionary->valueEqualsFunction = valueEqualsFunction;
    dictionary->tree = parcTreeRedBlack_Create(keyCompareFunction,
                                               keyFreeFunction,
                                               NULL,
                                               valueEqualsFunction,
                                               valueFreeFunction,
                                               NULL);
    return dictionary;
}


void
parcDictionary_Destroy(PARCDictionary **dictionaryPointer)
{
    assertNotNull(dictionaryPointer, "Pointer to dictionary pointer can't be NULL");
    assertNotNull(*dictionaryPointer, "Pointer to dictionary can't be NULL");
    parcTreeRedBlack_Destroy(&((*dictionaryPointer)->tree));
    parcMemory_Deallocate((void **) dictionaryPointer);
    *dictionaryPointer = NULL;
}

void
parcDictionary_SetValue(PARCDictionary *dictionary, void *key, void *value)
{
    assertNotNull(dictionary, "dictionary pointer can't be NULL");
    assertNotNull(key, "Key pointer can't be NULL");
    parcTreeRedBlack_Insert(dictionary->tree, key, value);
}

void *
parcDictionary_GetValue(PARCDictionary *dictionary, const void *key)
{
    assertNotNull(dictionary, "dictionary pointer can't be NULL");
    assertNotNull(key, "Key pointer can't be NULL");
    return parcTreeRedBlack_Get(dictionary->tree, key);
}

void *
parcDictionary_RemoveValue(PARCDictionary *dictionary, const void *key)
{
    assertNotNull(dictionary, "dictionary pointer can't be NULL");
    assertNotNull(key, "Key pointer can't be NULL");
    return parcTreeRedBlack_Remove(dictionary->tree, key);
}

void
parcDictionary_RemoveAndDestroyValue(PARCDictionary *dictionary, const void *key)
{
    assertNotNull(dictionary, "dictionary pointer can't be NULL");
    assertNotNull(key, "Key pointer can't be NULL");
    parcTreeRedBlack_RemoveAndDestroy(dictionary->tree, key);
}

PARCArrayList *
parcDictionary_Keys(const PARCDictionary *dictionary)
{
    assertNotNull(dictionary, "dictionary pointer can't be NULL");
    return parcTreeRedBlack_Keys(dictionary->tree);
}

PARCArrayList *
parcDictionary_Values(const PARCDictionary *dictionary)
{
    assertNotNull(dictionary, "dictionary pointer can't be NULL");
    return parcTreeRedBlack_Values(dictionary->tree);
}

size_t
parcDictionary_Size(const PARCDictionary *dictionary)
{
    assertNotNull(dictionary, "dictionary pointer can't be NULL");
    return parcTreeRedBlack_Size(dictionary->tree);
}

int
parcDictionary_Equals(const PARCDictionary *dictionary1, const PARCDictionary *dictionary2)
{
    assertNotNull(dictionary1, "dictionary pointer can't be NULL");
    assertNotNull(dictionary2, "dictionary pointer can't be NULL");
    return parcTreeRedBlack_Equals(dictionary1->tree, dictionary2->tree);
}
