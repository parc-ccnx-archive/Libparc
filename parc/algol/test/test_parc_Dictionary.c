/*
 * Copyright (c) 2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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

#include <config.h>
#include <stdio.h>

#include <parc/algol/parc_SafeMemory.h>
#include <LongBow/unit-test.h>

#include "../parc_Dictionary.c"

static int *
_keyNewInt(int key)
{
    int *newKey = parcMemory_Allocate(sizeof(int));
    assertNotNull(newKey, "parcMemory_Allocate(%zu) returned NULL",
                  sizeof(int));
    *newKey = key;
    return newKey;
}

static int *
_valueNewInt(int value)
{
    int *newValue = parcMemory_Allocate(sizeof(int));
    assertNotNull(newValue, "parcMemory_Allocate(%zu) returned NULL", sizeof(int));
    *newValue = value;
    return newValue;
}

static bool
_valueEquals(const void *value1, const void *value2)
{
    return *(int *) value1 == *(int *) value2;
}

static int
_intKeyComp(const void *key1, const void *key2)
{
    if (*(int *) key1 < *(int *) key2) {
        return -1;
    }
    if (*(int *) key1 == *(int *) key2) {
        return 0;
    }
    return 1;
}

static uint32_t
_intKeyHash(const void *key1)
{
    return *(int *) key1;
}


static void
_keyFree(void **value)
{
    parcMemory_Deallocate((void **) value);
    *value = NULL;
}

static void
_valueFree(void **key)
{
    parcMemory_Deallocate((void **) key);
    *key = NULL;
}




LONGBOW_TEST_RUNNER(PARC_Dictionary)
{
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Local);
}

LONGBOW_TEST_RUNNER_SETUP(PARC_Dictionary)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_RUNNER_TEARDOWN(PARC_Dictionary)
{
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(-1);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestRunner_GetName(testRunner), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, PARC_Dictionary_Create);
    LONGBOW_RUN_TEST_CASE(Global, PARC_Dictionary_SetValue_Destroy);
    LONGBOW_RUN_TEST_CASE(Global, PARC_Dictionary_Size_Empty);
    LONGBOW_RUN_TEST_CASE(Global, PARC_Dictionary_Size);
    LONGBOW_RUN_TEST_CASE(Global, PARC_Dictionary_Size_AfterDelete);
    LONGBOW_RUN_TEST_CASE(Global, PARC_Dictionary_Size_AfterOverwrite);
    LONGBOW_RUN_TEST_CASE(Global, PARC_Dictionary_Get_EmptyTree);
    LONGBOW_RUN_TEST_CASE(Global, PARC_Dictionary_Get_NonExistent);
    LONGBOW_RUN_TEST_CASE(Global, PARC_Dictionary_Get_First);
    LONGBOW_RUN_TEST_CASE(Global, PARC_Dictionary_Get);
    LONGBOW_RUN_TEST_CASE(Global, PARC_Dictionary_Get_Last);
    LONGBOW_RUN_TEST_CASE(Global, PARC_Dictionary_Remove_First);
    LONGBOW_RUN_TEST_CASE(Global, PARC_Dictionary_Remove);
    LONGBOW_RUN_TEST_CASE(Global, PARC_Dictionary_Remove_Last);
    LONGBOW_RUN_TEST_CASE(Global, PARC_Dictionary_RemoveAndDestroy_First);
    LONGBOW_RUN_TEST_CASE(Global, PARC_Dictionary_RemoveAndDestroy);
    LONGBOW_RUN_TEST_CASE(Global, PARC_Dictionary_RemoveAndDestroy_Last);
    LONGBOW_RUN_TEST_CASE(Global, PARC_Dictionary_Keys);
    LONGBOW_RUN_TEST_CASE(Global, PARC_Dictionary_Values);
    LONGBOW_RUN_TEST_CASE(Global, PARC_Dictionary_Equals_Empty);
    LONGBOW_RUN_TEST_CASE(Global, PARC_Dictionary_Equals);
    LONGBOW_RUN_TEST_CASE(Global, PARC_Dictionary_Equals_Not_Values);
    LONGBOW_RUN_TEST_CASE(Global, PARC_Dictionary_Equals_Not_Keys);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDERR_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, PARC_Dictionary_Create)
{
    PARCDictionary *dictionary = parcDictionary_Create(_intKeyComp, _intKeyHash, NULL, NULL, NULL);

    parcDictionary_Destroy(&dictionary);

    dictionary = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);

    parcDictionary_Destroy(&dictionary);
}

LONGBOW_TEST_CASE(Global, PARC_Dictionary_SetValue_Destroy)
{
    PARCDictionary *dictionary;

    dictionary = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);

    parcDictionary_SetValue(dictionary, (void *) _keyNewInt(1), (void *) _valueNewInt(11));
    parcDictionary_SetValue(dictionary, (void *) _keyNewInt(2), (void *) _valueNewInt(12));
    parcDictionary_SetValue(dictionary, (void *) _keyNewInt(3), (void *) _valueNewInt(13));

    parcDictionary_Destroy(&dictionary);
}

LONGBOW_TEST_CASE(Global, PARC_Dictionary_Size_Empty)
{
    PARCDictionary *dictionary;

    dictionary = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);

    assertTrue(0 == parcDictionary_Size(dictionary), "Wrong size of dictionary - empty, start");

    parcDictionary_Destroy(&dictionary);
}

LONGBOW_TEST_CASE(Global, PARC_Dictionary_Size)
{
    PARCDictionary *dictionary;

    dictionary = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);

    parcDictionary_SetValue(dictionary, (void *) _keyNewInt(4), (void *) _valueNewInt(1004));
    parcDictionary_SetValue(dictionary, (void *) _keyNewInt(3), (void *) _valueNewInt(1003));
    parcDictionary_SetValue(dictionary, (void *) _keyNewInt(2), (void *) _valueNewInt(1002));
    parcDictionary_SetValue(dictionary, (void *) _keyNewInt(8), (void *) _valueNewInt(1008));

    assertTrue(4 == parcDictionary_Size(dictionary), "Wrong size of dictionary after add 3");

    parcDictionary_Destroy(&dictionary);
}

LONGBOW_TEST_CASE(Global, PARC_Dictionary_Size_AfterDelete)
{
    PARCDictionary *dictionary;

    dictionary = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);

    parcDictionary_SetValue(dictionary, (void *) _keyNewInt(4), (void *) _valueNewInt(1004));
    parcDictionary_SetValue(dictionary, (void *) _keyNewInt(3), (void *) _valueNewInt(1003));
    parcDictionary_SetValue(dictionary, (void *) _keyNewInt(2), (void *) _valueNewInt(1002));
    parcDictionary_SetValue(dictionary, (void *) _keyNewInt(8), (void *) _valueNewInt(1008));

    int searchKey = 2;

    parcDictionary_RemoveAndDestroyValue(dictionary, &searchKey);

    size_t size = parcDictionary_Size(dictionary);

    assertTrue(3 == size, "Wrong size of dictionary after 1 delete (%zu instead of 3)", size);

    parcDictionary_Destroy(&dictionary);
}

LONGBOW_TEST_CASE(Global, PARC_Dictionary_Size_AfterOverwrite)
{
    PARCDictionary *dictionary;

    dictionary = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);

    parcDictionary_SetValue(dictionary, (void *) _keyNewInt(4), (void *) _valueNewInt(1004));
    parcDictionary_SetValue(dictionary, (void *) _keyNewInt(3), (void *) _valueNewInt(1003));
    parcDictionary_SetValue(dictionary, (void *) _keyNewInt(2), (void *) _valueNewInt(1002));
    parcDictionary_SetValue(dictionary, (void *) _keyNewInt(8), (void *) _valueNewInt(1008));

    parcDictionary_SetValue(dictionary, (void *) _keyNewInt(3), (void *) _valueNewInt(1010));

    size_t size = parcDictionary_Size(dictionary);

    assertTrue(4 == size, "Wrong size of dictionary after 1 delete (%zu instead of 4)", size);

    parcDictionary_Destroy(&dictionary);
}

LONGBOW_TEST_CASE(Global, PARC_Dictionary_Get_EmptyTree)
{
    PARCDictionary *dictionary;

    int key = 100;

    dictionary = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);

    void *value = parcDictionary_GetValue(dictionary, &key);

    assertTrue(NULL == value, "Object did not exist, must return NULL");

    parcDictionary_Destroy(&dictionary);
}

LONGBOW_TEST_CASE(Global, PARC_Dictionary_Get_NonExistent)
{
    PARCDictionary *dictionary;
    int key = 100;

    dictionary = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);

    for (int i = 1; i < 10; i++) {
        // Add some elements to the dictionary
        parcDictionary_SetValue(dictionary, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
    }

    void *value = parcDictionary_GetValue(dictionary, &key);

    assertTrue(NULL == value, "Object did not exist, must return NULL");

    parcDictionary_Destroy(&dictionary);
}

LONGBOW_TEST_CASE(Global, PARC_Dictionary_Get_First)
{
    PARCDictionary *dictionary;
    int key = 1;

    dictionary = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);

    for (int i = 1; i < 10; i++) {
        // Add some elements to the dictionary
        parcDictionary_SetValue(dictionary, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
    }

    int *value = parcDictionary_GetValue(dictionary, &key);

    assertNotNull(value, "NULL value returned");
    assertTrue(*value == (1 << 8), "Wrong object returned or not found");

    parcDictionary_Destroy(&dictionary);
}

LONGBOW_TEST_CASE(Global, PARC_Dictionary_Get)
{
    PARCDictionary *dictionary;
    int key = 5;

    dictionary = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);

    for (int i = 1; i < 10; i++) {
        // Add some elements to the dictionary
        parcDictionary_SetValue(dictionary, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
    }

    int *value = parcDictionary_GetValue(dictionary, &key);

    assertNotNull(value, "NULL value returned");
    assertTrue(*value == (5 << 8), "Wrong object returned or not found");

    parcDictionary_Destroy(&dictionary);
}

LONGBOW_TEST_CASE(Global, PARC_Dictionary_Get_Last)
{
    PARCDictionary *dictionary;
    int key = 9;

    dictionary = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);

    for (int i = 1; i < 10; i++) {
        // Add some elements to the dictionary
        parcDictionary_SetValue(dictionary, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
    }

    int *value = parcDictionary_GetValue(dictionary, &key);

    assertNotNull(value, "NULL value returned");
    assertTrue(*value == (9 << 8), "Wrong object returned or not found");

    parcDictionary_Destroy(&dictionary);
}

LONGBOW_TEST_CASE(Global, PARC_Dictionary_Remove_First)
{
    PARCDictionary *dictionary1;
    PARCDictionary *dictionary2;

    dictionary1 = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);
    dictionary2 = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);

    for (int i = 30; i < 40; i++) {
        // Add some elements to the dictionary
        parcDictionary_SetValue(dictionary1, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
        parcDictionary_SetValue(dictionary2, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
    }
    parcDictionary_SetValue(dictionary1, (void *) _keyNewInt(1), (void *) _valueNewInt(1 << 8));
    for (int i = 2; i < 10; i++) {
        // Add some elements to the dictionary
        parcDictionary_SetValue(dictionary1, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
        parcDictionary_SetValue(dictionary2, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
    }
    for (int i = 20; i < 30; i++) {
        // Add some elements to the dictionary
        parcDictionary_SetValue(dictionary1, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
        parcDictionary_SetValue(dictionary2, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
    }

    int searchKey = 1;

    void *data = parcDictionary_RemoveValue(dictionary1, &searchKey);

    _valueFree(&data);

    assertTrue(parcDictionary_Equals(dictionary1, dictionary2), "Trees dont match after remove");

    parcDictionary_Destroy(&dictionary1);
    parcDictionary_Destroy(&dictionary2);
}

LONGBOW_TEST_CASE(Global, PARC_Dictionary_Remove)
{
    PARCDictionary *dictionary1;
    PARCDictionary *dictionary2;

    dictionary1 = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);
    dictionary2 = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);

    for (int i = 31; i < 40; i++) {
        // Add some elements to the dictionary
        parcDictionary_SetValue(dictionary1, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
        parcDictionary_SetValue(dictionary2, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
    }
    parcDictionary_SetValue(dictionary1, (void *) _keyNewInt(30), (void *) _valueNewInt(31 << 8));
    for (int i = 2; i < 10; i++) {
        // Add some elements to the dictionary
        parcDictionary_SetValue(dictionary1, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
        parcDictionary_SetValue(dictionary2, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
    }
    for (int i = 20; i < 30; i++) {
        // Add some elements to the dictionary
        parcDictionary_SetValue(dictionary1, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
        parcDictionary_SetValue(dictionary2, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
    }

    int searchKey = 30;

    void *data = parcDictionary_RemoveValue(dictionary1, &searchKey);

    _valueFree(&data);

    assertTrue(parcDictionary_Equals(dictionary1, dictionary2), "Trees dont match after remove");

    parcDictionary_Destroy(&dictionary1);
    parcDictionary_Destroy(&dictionary2);
}

LONGBOW_TEST_CASE(Global, PARC_Dictionary_Remove_Last)
{
    PARCDictionary *dictionary1;
    PARCDictionary *dictionary2;

    dictionary1 = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);
    dictionary2 = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);

    for (int i = 30; i < 40; i++) {
        // Add some elements to the dictionary
        parcDictionary_SetValue(dictionary1, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
        parcDictionary_SetValue(dictionary2, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
    }
    parcDictionary_SetValue(dictionary1, (void *) _keyNewInt(100), (void *) _valueNewInt(100 << 8));
    for (int i = 2; i < 10; i++) {
        // Add some elements to the dictionary
        parcDictionary_SetValue(dictionary1, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
        parcDictionary_SetValue(dictionary2, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
    }
    for (int i = 20; i < 30; i++) {
        // Add some elements to the dictionary
        parcDictionary_SetValue(dictionary1, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
        parcDictionary_SetValue(dictionary2, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
    }

    int searchKey = 100;

    void *data = parcDictionary_RemoveValue(dictionary1, &searchKey);

    _valueFree(&data);

    assertTrue(parcDictionary_Equals(dictionary1, dictionary2), "Trees dont match after remove");

    parcDictionary_Destroy(&dictionary1);
    parcDictionary_Destroy(&dictionary2);
}

LONGBOW_TEST_CASE(Global, PARC_Dictionary_RemoveAndDestroy_First)
{
    PARCDictionary *dictionary1 = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);
    PARCDictionary *dictionary2 = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);

    for (int i = 30; i < 40; i++) {
        // Add some elements to the dictionary
        parcDictionary_SetValue(dictionary1, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
        parcDictionary_SetValue(dictionary2, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
    }
    parcDictionary_SetValue(dictionary1, (void *) _keyNewInt(1), (void *) _valueNewInt(1 << 8));
    for (int i = 2; i < 10; i++) {
        // Add some elements to the dictionary
        parcDictionary_SetValue(dictionary1, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
        parcDictionary_SetValue(dictionary2, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
    }
    for (int i = 20; i < 30; i++) {
        // Add some elements to the dictionary
        parcDictionary_SetValue(dictionary1, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
        parcDictionary_SetValue(dictionary2, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
    }

    int searchKey = 1;

    parcDictionary_RemoveAndDestroyValue(dictionary1, &searchKey);

    assertTrue(parcDictionary_Equals(dictionary1, dictionary2), "Trees dont match after remove");

    parcDictionary_Destroy(&dictionary1);
    parcDictionary_Destroy(&dictionary2);
}

LONGBOW_TEST_CASE(Global, PARC_Dictionary_RemoveAndDestroy)
{
    PARCDictionary *dictionary1;
    PARCDictionary *dictionary2;

    dictionary1 = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);
    dictionary2 = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);

    for (int i = 31; i < 40; i++) {
        // Add some elements to the dictionary
        parcDictionary_SetValue(dictionary1, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
        parcDictionary_SetValue(dictionary2, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
    }
    parcDictionary_SetValue(dictionary1, (void *) _keyNewInt(30), (void *) _valueNewInt(31 << 8));
    for (int i = 2; i < 10; i++) {
        // Add some elements to the dictionary
        parcDictionary_SetValue(dictionary1, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
        parcDictionary_SetValue(dictionary2, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
    }
    for (int i = 20; i < 30; i++) {
        // Add some elements to the dictionary
        parcDictionary_SetValue(dictionary1, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
        parcDictionary_SetValue(dictionary2, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
    }

    int searchKey = 30;

    parcDictionary_RemoveAndDestroyValue(dictionary1, &searchKey);

    assertTrue(parcDictionary_Equals(dictionary1, dictionary2), "Trees dont match after remove");

    parcDictionary_Destroy(&dictionary1);
    parcDictionary_Destroy(&dictionary2);
}

LONGBOW_TEST_CASE(Global, PARC_Dictionary_RemoveAndDestroy_Last)
{
    PARCDictionary *dictionary1;
    PARCDictionary *dictionary2;

    dictionary1 = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);
    dictionary2 = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);

    for (int i = 30; i < 40; i++) {
        // Add some elements to the dictionary
        parcDictionary_SetValue(dictionary1, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
        parcDictionary_SetValue(dictionary2, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
    }
    parcDictionary_SetValue(dictionary1, (void *) _keyNewInt(100), (void *) _valueNewInt(100 << 8));
    for (int i = 2; i < 10; i++) {
        // Add some elements to the dictionary
        parcDictionary_SetValue(dictionary1, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
        parcDictionary_SetValue(dictionary2, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
    }
    for (int i = 20; i < 30; i++) {
        // Add some elements to the dictionary
        parcDictionary_SetValue(dictionary1, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
        parcDictionary_SetValue(dictionary2, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
    }

    int searchKey = 100;

    parcDictionary_RemoveAndDestroyValue(dictionary1, &searchKey);

    assertTrue(parcDictionary_Equals(dictionary1, dictionary2), "Trees dont match after remove");

    parcDictionary_Destroy(&dictionary1);
    parcDictionary_Destroy(&dictionary2);
}

LONGBOW_TEST_CASE(Global, PARC_Dictionary_Keys)
{
    PARCDictionary *dictionary =
        parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);

    for (int i = 0; i < 9; i++) {
        // Add some elements to the dictionary
        parcDictionary_SetValue(dictionary, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
    }

    PARCArrayList *keys = parcDictionary_Keys(dictionary);

    assertNotNull(keys,
                  "parcDictionary_Keys returned NULL, expected non-NULL.");

    assertTrue(parcArrayList_Size(keys) == parcDictionary_Size(dictionary),
                "Expected parcDictionary_Keys size %zu, actual %zd, ",
                parcDictionary_Size(dictionary), parcArrayList_Size(keys));

    for (int i = 0; i < 9; i++) {
        bool found = false;
        int *keyToFind = _keyNewInt(i);
        for (int j = 0; j < parcArrayList_Size(keys); j++) {
            int *keyToTest = parcArrayList_Get(keys, j);
            if (*keyToTest == *keyToFind) {
                found = true;
                break;
            }
        }
        assertTrue(found, "Expected to find Key %d, not found", *keyToFind);
        parcMemory_Deallocate((void **) &keyToFind);
    }
    parcArrayList_Destroy(&keys);
    parcDictionary_Destroy(&dictionary);
}

LONGBOW_TEST_CASE(Global, PARC_Dictionary_Values)
{
    PARCDictionary *dictionary =
        parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);

    for (int i = 0; i < 9; i++) {
        // Add some elements to the dictionary
        parcDictionary_SetValue(dictionary, (void *) _keyNewInt(i), (void *) _valueNewInt(i << 8));
    }

    PARCArrayList *values = parcDictionary_Values(dictionary);

    assertNotNull(values,
                  "parcDictionary_Values returned NULL, expected not NULL");

    assertTrue(parcArrayList_Size(values) == parcDictionary_Size(dictionary),
               "parcDictionary_Values size %zd not equal not parcDictionary_Size, %zd",
               parcArrayList_Size(values), parcDictionary_Size(dictionary));

    for (int i = 0; i < 9; i++) {
        bool found = false;
        int *keyToFind = _keyNewInt(i);
        int *valueToFind = parcDictionary_GetValue(dictionary, keyToFind);
        for (int j = 0; j < parcArrayList_Size(values); j++) {
            int *valueToTest = parcArrayList_Get(values, j);
            if (valueToFind == valueToTest) {
                found = true;
                break;
            }
        }
        assertTrue(found,
                   "Expected to find value %d, not found", *valueToFind);
        parcMemory_Deallocate((void **) &keyToFind);
    }
    parcArrayList_Destroy(&values);
    parcDictionary_Destroy(&dictionary);
}


LONGBOW_TEST_CASE(Global, PARC_Dictionary_Equals_Empty)
{
    PARCDictionary *dictionary1;
    PARCDictionary *dictionary2;

    dictionary1 = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);
    dictionary2 = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);

    assertTrue(parcDictionary_Equals(dictionary1, dictionary2), "Empty lists are not equal");

    parcDictionary_Destroy(&dictionary1);
    parcDictionary_Destroy(&dictionary2);
}

LONGBOW_TEST_CASE(Global, PARC_Dictionary_Equals_Not_Values)
{
    PARCDictionary *dictionary1;
    PARCDictionary *dictionary2;

    int compareSetValues = 100;

    dictionary1 = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);
    dictionary2 = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);

    printf("Testing dictionary equals...\n");

    for (int i = 1; i < compareSetValues; i++) {
        parcDictionary_SetValue(dictionary1,
                                (void *) _keyNewInt(i),
                                (void *) _valueNewInt(i << 8));
        parcDictionary_SetValue(dictionary2,
                                (void *) _keyNewInt(compareSetValues - i),
                                (void *) _valueNewInt((compareSetValues + i) << 8));
    }

    assertFalse(parcDictionary_Equals(dictionary1, dictionary2), "Dictionaries are equal and they shouldn't be!");

    parcDictionary_Destroy(&dictionary1);
    parcDictionary_Destroy(&dictionary2);
}

LONGBOW_TEST_CASE(Global, PARC_Dictionary_Equals_Not_Keys)
{
    PARCDictionary *dictionary1;
    PARCDictionary *dictionary2;

    int compareSetValues = 100;

    dictionary1 = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);
    dictionary2 = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);

    for (int i = 1; i < compareSetValues; i++) {
        parcDictionary_SetValue(dictionary1,
                                (void *) _keyNewInt(i),
                                (void *) _valueNewInt(i << 8));
        parcDictionary_SetValue(dictionary2,
                                (void *) _keyNewInt(compareSetValues + i),
                                (void *) _valueNewInt((compareSetValues - i) << 8));
    }

    assertFalse(parcDictionary_Equals(dictionary1, dictionary2), "Lists are equal");

    parcDictionary_Destroy(&dictionary1);
    parcDictionary_Destroy(&dictionary2);
}

LONGBOW_TEST_CASE(Global, PARC_Dictionary_Equals)
{
    PARCDictionary *dictionary1;
    PARCDictionary *dictionary2;

    int compareSetValues = 100;

    dictionary1 = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);
    dictionary2 = parcDictionary_Create(_intKeyComp, _intKeyHash, _keyFree, _valueEquals, _valueFree);

    for (int i = 1; i < compareSetValues; i++) {
        parcDictionary_SetValue(dictionary1,
                                (void *) _keyNewInt(i),
                                (void *) _valueNewInt(i << 8));
        parcDictionary_SetValue(dictionary2,
                                (void *) _keyNewInt(compareSetValues - i),
                                (void *) _valueNewInt((compareSetValues - i) << 8));
    }

    assertTrue(parcDictionary_Equals(dictionary1, dictionary2), "Dictionaries are not equal");

    parcDictionary_Destroy(&dictionary1);
    parcDictionary_Destroy(&dictionary2);
}

LONGBOW_TEST_FIXTURE(Local)
{
}

LONGBOW_TEST_FIXTURE_SETUP(Local)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Local)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(PARC_Dictionary);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
