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
#include "../parc_HashMap.c"

#include <LongBow/unit-test.h>
#include <parc/algol/parc_SafeMemory.h>
#include <parc/algol/parc_StdlibMemory.h>

#include <parc/testing/parc_ObjectTesting.h>
#include <parc/testing/parc_MemoryTesting.h>

LONGBOW_TEST_RUNNER(parc_HashMap)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Static);
    LONGBOW_RUN_TEST_FIXTURE(CreateAcquireRelease);
    LONGBOW_RUN_TEST_FIXTURE(ObjectContract);
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_HashMap)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_HashMap)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(CreateAcquireRelease)
{
    LONGBOW_RUN_TEST_CASE(CreateAcquireRelease, CreateRelease);
    LONGBOW_RUN_TEST_CASE(CreateAcquireRelease, CreateCapacity0);
    LONGBOW_RUN_TEST_CASE(CreateAcquireRelease, CreateCapacityNominal);
}

LONGBOW_TEST_FIXTURE_SETUP(CreateAcquireRelease)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(CreateAcquireRelease)
{
    if (!parcMemoryTesting_ExpectedOutstanding(0, "%s leaked memory.", longBowTestCase_GetFullName(testCase))) {
        return LONGBOW_STATUS_MEMORYLEAK;
    }

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(CreateAcquireRelease, CreateRelease)
{
    PARCHashMap *instance = parcHashMap_Create();
    assertNotNull(instance, "Expeced non-null result from parcHashMap_Create();");
    parcObjectTesting_AssertAcquireReleaseContract(parcHashMap_Acquire, instance);

    parcHashMap_Release(&instance);
    assertNull(instance, "Expeced null result from parcHashMap_Release();");
}

LONGBOW_TEST_CASE(CreateAcquireRelease, CreateCapacity0)
{
    const size_t CAPACITY = 0;
    PARCHashMap *instance = parcHashMap_CreateCapacity(CAPACITY);
    assertNotNull(instance, "Expeced non-null result from parcHashMap_Create();");
    parcObjectTesting_AssertAcquireReleaseContract(parcHashMap_Acquire, instance);

    parcHashMap_Release(&instance);
}

LONGBOW_TEST_CASE(CreateAcquireRelease, CreateCapacityNominal)
{
    const size_t CAPACITY = 10000;
    PARCHashMap *instance = parcHashMap_CreateCapacity(CAPACITY);
    assertNotNull(instance, "Expeced non-null result from parcHashMap_Create();");
    parcObjectTesting_AssertAcquireReleaseContract(parcHashMap_Acquire, instance);

    //Make sure all the buckets exist
    PARCBuffer *key = parcBuffer_WrapCString("key");
    PARCBuffer *value = parcBuffer_WrapCString("value");
    _PARCHashMapEntry *entry = _parcHashMapEntry_Create(key, value);
    for (size_t i = 0; i < CAPACITY; ++i) {
        parcLinkedList_Append(instance->buckets[i], entry);
        ++instance->size;
    }
    _parcHashMapEntry_Release(&entry);
    parcBuffer_Release(&key);
    parcBuffer_Release(&value);

    parcHashMap_Release(&instance);
    assertNull(instance, "Expeced null result from parcHashMap_Release();");
}

LONGBOW_TEST_FIXTURE(ObjectContract)
{
    LONGBOW_RUN_TEST_CASE(ObjectContract, parcHashMap_Copy);
    LONGBOW_RUN_TEST_CASE(ObjectContract, parcHashMap_Display);
    LONGBOW_RUN_TEST_CASE(ObjectContract, parcHashMap_Equals);
    LONGBOW_RUN_TEST_CASE(ObjectContract, parcHashMap_HashCode_Empty);
    LONGBOW_RUN_TEST_CASE(ObjectContract, parcHashMap_HashCode_NonEmpty);
    LONGBOW_RUN_TEST_CASE(ObjectContract, parcHashMap_IsValid);
    LONGBOW_RUN_TEST_CASE(ObjectContract, parcHashMap_AssertValid);
    LONGBOW_RUN_TEST_CASE(ObjectContract, parcHashMap_ToJSON);
    LONGBOW_RUN_TEST_CASE(ObjectContract, parcHashMap_ToString);
}

LONGBOW_TEST_FIXTURE_SETUP(ObjectContract)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(ObjectContract)
{
    if (!parcMemoryTesting_ExpectedOutstanding(0, "%s leaked memory.", longBowTestCase_GetFullName(testCase))) {
        parcSafeMemory_ReportAllocation(1);

        return LONGBOW_STATUS_MEMORYLEAK;
    }

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(ObjectContract, parcHashMap_Copy)
{
    PARCHashMap *instance = parcHashMap_Create();
    PARCHashMap *copy = parcHashMap_Copy(instance);

    assertTrue(parcHashMap_Equals(instance, copy), "Expected the copy to be equal to the original");

    parcHashMap_Release(&instance);
    parcHashMap_Release(&copy);
}

LONGBOW_TEST_CASE(ObjectContract, parcHashMap_Display)
{
    PARCHashMap *x = parcHashMap_Create();
    PARCBuffer *key = parcBuffer_WrapCString("key1");
    PARCBuffer *value = parcBuffer_WrapCString("value1");
    parcHashMap_Put(x, key, value);
    parcHashMap_Display(x, 0);

    parcBuffer_Release(&key);
    parcBuffer_Release(&value);

    parcHashMap_Release(&x);
}

LONGBOW_TEST_CASE(ObjectContract, parcHashMap_Equals)
{
    PARCBuffer *key = parcBuffer_WrapCString("key1");
    PARCBuffer *value = parcBuffer_WrapCString("value1");

    PARCHashMap *x = parcHashMap_Create();
    parcHashMap_Put(x, key, value);
    PARCHashMap *y = parcHashMap_Create();
    parcHashMap_Put(y, key, value);
    PARCHashMap *z = parcHashMap_Create();
    parcHashMap_Put(z, key, value);

    PARCHashMap *u1 = parcHashMap_Create();

    PARCHashMap *u2 = parcHashMap_Create();
    parcHashMap_Put(u2, key, value);

    parcObjectTesting_AssertEquals(x, y, z, u1, NULL);

    parcHashMap_Release(&x);
    parcHashMap_Release(&y);
    parcHashMap_Release(&z);
    parcHashMap_Release(&u1);
    parcHashMap_Release(&u2);

    parcBuffer_Release(&key);
    parcBuffer_Release(&value);
}

LONGBOW_TEST_CASE(ObjectContract, parcHashMap_HashCode_Empty)
{
    PARCHashMap *instance = parcHashMap_Create();

    PARCHashCode code = parcHashMap_HashCode(instance);

    assertTrue(code == 0, "Expected 0, actual %" PRIPARCHashCode, code);
    parcHashMap_Release(&instance);
}

LONGBOW_TEST_CASE(ObjectContract, parcHashMap_HashCode_NonEmpty)
{
    PARCHashMap *instance = parcHashMap_Create();
    PARCBuffer *key = parcBuffer_WrapCString("key1");
    PARCBuffer *value = parcBuffer_WrapCString("value1");
    parcHashMap_Put(instance, key, value);
    parcBuffer_Release(&key);
    parcBuffer_Release(&value);

    PARCHashCode code = parcHashMap_HashCode(instance);
    assertTrue(code != 0, "Expected a non-zero hash code, actual %" PRIPARCHashCode, code);

    parcHashMap_Release(&instance);
}

LONGBOW_TEST_CASE(ObjectContract, parcHashMap_IsValid)
{
    PARCHashMap *instance = parcHashMap_Create();

    PARCBuffer *key = parcBuffer_WrapCString("key1");
    PARCBuffer *value = parcBuffer_WrapCString("value1");
    parcHashMap_Put(instance, key, value);
    assertTrue(parcHashMap_IsValid(instance), "Expected parcHashMap_Create to result in a valid instance.");

    parcHashMap_Release(&instance);
    assertFalse(parcHashMap_IsValid(instance), "Expected parcHashMap_Create to result in an invalid instance.");

    parcBuffer_Release(&key);
    parcBuffer_Release(&value);
}

LONGBOW_TEST_CASE(ObjectContract, parcHashMap_AssertValid)
{
    PARCHashMap *instance = parcHashMap_Create();

    PARCBuffer *key = parcBuffer_WrapCString("key1");
    PARCBuffer *value = parcBuffer_WrapCString("value1");
    parcHashMap_Put(instance, key, value);
    parcHashMap_AssertValid(instance);

    parcBuffer_Release(&key);
    parcBuffer_Release(&value);
    parcHashMap_Release(&instance);
}

LONGBOW_TEST_CASE(ObjectContract, parcHashMap_ToJSON)
{
    PARCHashMap *instance = parcHashMap_Create();
    PARCBuffer *key = parcBuffer_WrapCString("key1");
    PARCBuffer *value = parcBuffer_WrapCString("value1");
    parcHashMap_Put(instance, key, value);

    PARCJSON *json = parcHashMap_ToJSON(instance);

    parcJSON_Release(&json);
    parcBuffer_Release(&key);
    parcBuffer_Release(&value);

    parcHashMap_Release(&instance);
}

LONGBOW_TEST_CASE(ObjectContract, parcHashMap_ToString)
{
    PARCHashMap *instance = parcHashMap_Create();
    PARCBuffer *key = parcBuffer_WrapCString("key1");
    PARCBuffer *value = parcBuffer_WrapCString("value1");
    parcHashMap_Put(instance, key, value);

    char *string = parcHashMap_ToString(instance);

    assertNotNull(string, "Expected non-NULL result from parcHashMap_ToString");

    parcMemory_Deallocate(&string);
    parcBuffer_Release(&key);
    parcBuffer_Release(&value);
    parcHashMap_Release(&instance);
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcHashMap_Put);
    LONGBOW_RUN_TEST_CASE(Global, parcHashMap_PutN);
    LONGBOW_RUN_TEST_CASE(Global, parcHashMap_Put_Replace);
    LONGBOW_RUN_TEST_CASE(Global, parcHashMap_Get_NoValue);
    LONGBOW_RUN_TEST_CASE(Global, parcHashMap_Contains_True);
    LONGBOW_RUN_TEST_CASE(Global, parcHashMap_Contains_False);
    LONGBOW_RUN_TEST_CASE(Global, parcHashMap_Remove);
    LONGBOW_RUN_TEST_CASE(Global, parcHashMap_Remove_False);
    LONGBOW_RUN_TEST_CASE(Global, parcHashMap_CreateValueIterator);
    LONGBOW_RUN_TEST_CASE(Global, parcHashMap_CreateValueIterator_HasNext);
    LONGBOW_RUN_TEST_CASE(Global, parcHashMap_CreateValueIterator_Next);
    LONGBOW_RUN_TEST_CASE(Global, parcHashMap_KeyIterator);
    LONGBOW_RUN_TEST_CASE(Global, parcHashMap_KeyIterator_HasNext);
    LONGBOW_RUN_TEST_CASE(Global, parcHashMap_KeyIterator_Next);
    LONGBOW_RUN_TEST_CASE(Global, parcHashMap_KeyIterator_Remove);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    if (!parcMemoryTesting_ExpectedOutstanding(0, "%s leaked memory.", longBowTestCase_GetFullName(testCase))) {
        parcSafeMemory_ReportAllocation(1);
        return LONGBOW_STATUS_MEMORYLEAK;
    }

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, parcHashMap_Put)
{
    PARCHashMap *instance = parcHashMap_Create();

    PARCBuffer *key = parcBuffer_WrapCString("key1");
    PARCBuffer *value = parcBuffer_WrapCString("value1");

    //size_t keyReferences = parcObject_GetReferenceCount(key);
    size_t valueReferences = parcObject_GetReferenceCount(value);

    parcHashMap_Put(instance, key, value);
    //M.S. Put now results in a copy of the key.
    //assertTrue(keyReferences + 1 == parcObject_GetReferenceCount(key), "Expected key reference to be incremented by 1.");
    assertTrue(valueReferences + 1 == parcObject_GetReferenceCount(value), "Expected value reference to be incremented by 1.");

    PARCBuffer *actual = (PARCBuffer *) parcHashMap_Get(instance, key);

    assertTrue(parcBuffer_Equals(value, actual), "Expected value was not returned from Get");

    parcBuffer_Release(&key);
    parcBuffer_Release(&value);

    parcHashMap_Release(&instance);
}

LONGBOW_TEST_CASE(Global, parcHashMap_PutN)
{
    size_t testRunSize = 100;

    PARCHashMap *instance = parcHashMap_CreateCapacity(testRunSize);

    PARCBuffer *key = parcBuffer_Allocate(sizeof(uint32_t));
    PARCBuffer *value = parcBuffer_WrapCString("value1");
    PARCBuffer *value42 = parcBuffer_WrapCString("value42");
    for (uint32_t i = 0; i < testRunSize * 2; ++i) {
        parcBuffer_PutUint32(key, i);
        parcHashMap_Put(instance, parcBuffer_Flip(key), value);
        if (i == 42) {
            parcHashMap_Put(instance, key, value42);
        }
    }

    parcBuffer_PutUint32(key, 42);
    PARCBuffer *actual = (PARCBuffer *) parcHashMap_Get(instance, parcBuffer_Flip(key));
    assertTrue(parcBuffer_Equals(value42, actual), "Expect to get back value42");

    parcBuffer_Release(&key);
    parcBuffer_Release(&value);
    parcBuffer_Release(&value42);
    parcHashMap_Release(&instance);
}

LONGBOW_TEST_CASE(Global, parcHashMap_Put_Replace)
{
    PARCHashMap *instance = parcHashMap_Create();

    PARCBuffer *key = parcBuffer_WrapCString("key1");
    PARCBuffer *value1 = parcBuffer_WrapCString("value1");
    PARCBuffer *value2 = parcBuffer_WrapCString("value2");

    parcHashMap_Put(instance, key, value1);

    parcHashMap_Put(instance, key, value2);

    PARCBuffer *actual = (PARCBuffer *) parcHashMap_Get(instance, key);

    assertTrue(parcBuffer_Equals(value2, actual), "Expected value was not returned from Get");

    parcBuffer_Release(&key);
    parcBuffer_Release(&value1);
    parcBuffer_Release(&value2);
    parcHashMap_Release(&instance);
}

LONGBOW_TEST_CASE(Global, parcHashMap_Get_NoValue)
{
    PARCHashMap *instance = parcHashMap_Create();

    PARCBuffer *key = parcBuffer_WrapCString("key1");

    PARCBuffer *actual = (PARCBuffer *) parcHashMap_Get(instance, key);

    assertNull(actual, "Expected parcHashMap_Get to return NULL for non-existent key.");

    parcBuffer_Release(&key);

    parcHashMap_Release(&instance);
}

LONGBOW_TEST_CASE(Global, parcHashMap_Contains_True)
{
    PARCHashMap *instance = parcHashMap_Create();

    PARCBuffer *key = parcBuffer_WrapCString("key1");
    PARCBuffer *value = parcBuffer_WrapCString("value1");

    parcHashMap_Put(instance, key, value);

    bool actual = parcHashMap_Contains(instance, key);

    assertTrue(actual, "Expected parcHashMap_Contains to return true");

    parcBuffer_Release(&key);
    parcBuffer_Release(&value);

    parcHashMap_Release(&instance);
}

LONGBOW_TEST_CASE(Global, parcHashMap_Contains_False)
{
    PARCHashMap *instance = parcHashMap_Create();

    PARCBuffer *key = parcBuffer_WrapCString("key1");

    bool actual = parcHashMap_Contains(instance, key);

    assertFalse(actual, "Expected parcHashMap_Contains to return NULL for non-existent key.");

    parcBuffer_Release(&key);

    parcHashMap_Release(&instance);
}

LONGBOW_TEST_CASE(Global, parcHashMap_Remove)
{
    PARCHashMap *instance = parcHashMap_Create();

    PARCBuffer *key = parcBuffer_WrapCString("key1");
    PARCBuffer *value = parcBuffer_WrapCString("value1");

    parcHashMap_Put(instance, key, value);

    bool actual = parcHashMap_Remove(instance, key);

    assertTrue(actual, "Expected parcHashMap_Remove to return true.");

    parcBuffer_Release(&key);
    parcBuffer_Release(&value);

    parcHashMap_Release(&instance);
}

LONGBOW_TEST_CASE(Global, parcHashMap_Remove_False)
{
    PARCHashMap *instance = parcHashMap_Create();

    PARCBuffer *key = parcBuffer_WrapCString("key1");
    PARCBuffer *key2 = parcBuffer_WrapCString("key2");
    PARCBuffer *value = parcBuffer_WrapCString("value1");

    parcHashMap_Put(instance, key, value);

    bool actual = parcHashMap_Remove(instance, key2);

    assertFalse(actual, "Expected parcHashMap_Remove to return false.");

    parcBuffer_Release(&key);
    parcBuffer_Release(&key2);
    parcBuffer_Release(&value);

    parcHashMap_Release(&instance);
}

LONGBOW_TEST_CASE(Global, parcHashMap_CreateValueIterator)
{
    PARCHashMap *instance = parcHashMap_Create();

    PARCBuffer *key = parcBuffer_WrapCString("key1");
    PARCBuffer *value = parcBuffer_WrapCString("value1");

    parcHashMap_Put(instance, key, value);

    PARCIterator *iterator = parcHashMap_CreateValueIterator(instance);

    assertNotNull(iterator, "Expected parcHashMap_ValueIterator to return non-null result");

    parcIterator_Release(&iterator);
    parcBuffer_Release(&key);
    parcBuffer_Release(&value);

    parcHashMap_Release(&instance);
}

LONGBOW_TEST_CASE(Global, parcHashMap_CreateValueIterator_HasNext)
{
    PARCHashMap *instance = parcHashMap_Create();

    PARCBuffer *key = parcBuffer_WrapCString("key1");
    PARCBuffer *value = parcBuffer_WrapCString("value1");

    parcHashMap_Put(instance, key, value);

    PARCIterator *iterator = parcHashMap_CreateValueIterator(instance);

    assertTrue(parcIterator_HasNext(iterator), "Expected parcIterator_HasNext to return true");

    parcIterator_Release(&iterator);
    parcBuffer_Release(&key);
    parcBuffer_Release(&value);

    parcHashMap_Release(&instance);
}

LONGBOW_TEST_CASE(Global, parcHashMap_CreateValueIterator_Next)
{
    PARCHashMap *instance = parcHashMap_Create();

    PARCBuffer *key1 = parcBuffer_WrapCString("key1");
    PARCBuffer *value1 = parcBuffer_WrapCString("1");
    PARCBuffer *key2 = parcBuffer_WrapCString("key2");
    PARCBuffer *value2 = parcBuffer_WrapCString("2");
    PARCBuffer *key3 = parcBuffer_WrapCString("key3");
    PARCBuffer *value3 = parcBuffer_WrapCString("3");
    PARCBuffer *key4 = parcBuffer_WrapCString("key4");
    PARCBuffer *value4 = parcBuffer_WrapCString("4");

    parcHashMap_Put(instance, key1, value1);
    parcHashMap_Put(instance, key2, value2);
    parcHashMap_Put(instance, key3, value3);
    parcHashMap_Put(instance, key4, value4);

    PARCIterator *iterator = parcHashMap_CreateValueIterator(instance);

    while (parcIterator_HasNext(iterator)) {
        PARCBuffer *actual = parcIterator_Next(iterator);
        assertNotNull(actual, "Expected parcIterator_Next to return non-null");
        assertTrue(parcBuffer_Remaining(actual) > 0, "The same value appeared more than once in the iteration");
        parcBuffer_SetPosition(actual, parcBuffer_Limit(actual));
    }
    parcIterator_Release(&iterator);

    parcBuffer_Release(&key1);
    parcBuffer_Release(&value1);
    parcBuffer_Release(&key2);
    parcBuffer_Release(&value2);
    parcBuffer_Release(&key3);
    parcBuffer_Release(&value3);
    parcBuffer_Release(&key4);
    parcBuffer_Release(&value4);

    parcHashMap_Release(&instance);
}

LONGBOW_TEST_CASE(Global, parcHashMap_KeyIterator)
{
    PARCHashMap *instance = parcHashMap_Create();

    PARCBuffer *key = parcBuffer_WrapCString("key1");
    PARCBuffer *value = parcBuffer_WrapCString("value1");

    parcHashMap_Put(instance, key, value);

    PARCIterator *iterator = parcHashMap_CreateKeyIterator(instance);

    assertNotNull(iterator, "Expected parcHashMap_KeyIterator to return non-null result");

    parcIterator_Release(&iterator);
    parcBuffer_Release(&key);
    parcBuffer_Release(&value);

    parcHashMap_Release(&instance);
}

LONGBOW_TEST_CASE(Global, parcHashMap_KeyIterator_HasNext)
{
    PARCHashMap *instance = parcHashMap_Create();

    PARCBuffer *key = parcBuffer_WrapCString("key1");
    PARCBuffer *value = parcBuffer_WrapCString("value1");

    parcHashMap_Put(instance, key, value);

    PARCIterator *iterator = parcHashMap_CreateKeyIterator(instance);

    assertTrue(parcIterator_HasNext(iterator), "Expected parcIterator_HasNext to return true");

    parcIterator_Release(&iterator);
    parcBuffer_Release(&key);
    parcBuffer_Release(&value);

    parcHashMap_Release(&instance);
}

LONGBOW_TEST_CASE(Global, parcHashMap_KeyIterator_Next)
{
    PARCHashMap *instance = parcHashMap_Create();

    PARCBuffer *key1 = parcBuffer_WrapCString("key1");
    PARCBuffer *value1 = parcBuffer_WrapCString("1");
    PARCBuffer *key2 = parcBuffer_WrapCString("key2");
    PARCBuffer *value2 = parcBuffer_WrapCString("2");
    PARCBuffer *key3 = parcBuffer_WrapCString("key3");
    PARCBuffer *value3 = parcBuffer_WrapCString("3");
    PARCBuffer *key4 = parcBuffer_WrapCString("key4");
    PARCBuffer *value4 = parcBuffer_WrapCString("4");

    parcHashMap_Put(instance, key1, value1);
    parcHashMap_Put(instance, key2, value2);
    parcHashMap_Put(instance, key3, value3);
    parcHashMap_Put(instance, key4, value4);

    PARCIterator *iterator = parcHashMap_CreateKeyIterator(instance);

    while (parcIterator_HasNext(iterator)) {
        PARCBuffer *actual = parcIterator_Next(iterator);
        assertNotNull(actual, "Expected parcIterator_Next to return non-null");
        assertTrue(parcBuffer_Remaining(actual) > 0, "The same value appeared more than once in the iteration");
        parcBuffer_SetPosition(actual, parcBuffer_Limit(actual));
    }
    parcIterator_Release(&iterator);

    parcBuffer_Release(&key1);
    parcBuffer_Release(&value1);
    parcBuffer_Release(&key2);
    parcBuffer_Release(&value2);
    parcBuffer_Release(&key3);
    parcBuffer_Release(&value3);
    parcBuffer_Release(&key4);
    parcBuffer_Release(&value4);

    parcHashMap_Release(&instance);
}

LONGBOW_TEST_CASE(Global, parcHashMap_KeyIterator_Remove)
{
    PARCHashMap *instance = parcHashMap_Create();

    PARCBuffer *key1 = parcBuffer_WrapCString("key1");
    PARCBuffer *value1 = parcBuffer_WrapCString("1");
    PARCBuffer *key2 = parcBuffer_WrapCString("key2");
    PARCBuffer *value2 = parcBuffer_WrapCString("2");
    PARCBuffer *key3 = parcBuffer_WrapCString("key3");
    PARCBuffer *value3 = parcBuffer_WrapCString("3");
    PARCBuffer *key4 = parcBuffer_WrapCString("key4");
    PARCBuffer *value4 = parcBuffer_WrapCString("4");

    parcHashMap_Put(instance, key1, value1);
    parcHashMap_Put(instance, key2, value2);
    parcHashMap_Put(instance, key3, value3);
    parcHashMap_Put(instance, key4, value4);

    assertTrue(parcHashMap_Size(instance) == 4, "Expected 4, actual %zd", parcHashMap_Size(instance));
    PARCIterator *iterator = parcHashMap_CreateKeyIterator(instance);

    while (parcIterator_HasNext(iterator)) {
        PARCBuffer *key = parcBuffer_Acquire(parcIterator_Next(iterator));
        parcIterator_Remove(iterator);
        assertNull(parcHashMap_Get(instance, key), "Expected deleted entry to not be gettable.");
        parcBuffer_Release(&key);
    }
    parcIterator_Release(&iterator);

    assertTrue(parcHashMap_Size(instance) == 0, "Expected 0, actual %zd", parcHashMap_Size(instance));
    parcBuffer_Release(&key1);
    parcBuffer_Release(&value1);
    parcBuffer_Release(&key2);
    parcBuffer_Release(&value2);
    parcBuffer_Release(&key3);
    parcBuffer_Release(&value3);
    parcBuffer_Release(&key4);
    parcBuffer_Release(&value4);

    parcHashMap_Release(&instance);
}

LONGBOW_TEST_FIXTURE(Static)
{
    LONGBOW_RUN_TEST_CASE(Static, parcHashMapEntry);
}

LONGBOW_TEST_FIXTURE_SETUP(Static)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Static)
{
    if (!parcMemoryTesting_ExpectedOutstanding(0, "%s leaked memory.", longBowTestCase_GetFullName(testCase))) {
        parcSafeMemory_ReportAllocation(1);
        return LONGBOW_STATUS_MEMORYLEAK;
    }

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Static, parcHashMapEntry)
{
    PARCBuffer *key = parcBuffer_WrapCString("key1");
    PARCBuffer *value = parcBuffer_WrapCString("value1");

    _PARCHashMapEntry *instance = _parcHashMapEntry_Create(key, value);
    parcBuffer_Release(&key);
    parcBuffer_Release(&value);

    _parcHashMapEntry_Release(&instance);
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_HashMap);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
