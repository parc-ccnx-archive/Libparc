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

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include <config.h>

#include <time.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/time.h>

#include <LongBow/unit-test.h>

#include "../parc_HashCodeTable.c"

#include <parc/algol/parc_SafeMemory.h>

// ==============================
// The objects to put in the hash table.  We have a separate key class and data class

typedef struct test_key_class {
    unsigned key_value;
    unsigned hash_value;
} TestKeyClass;

typedef struct test_data_class {
    unsigned data_value;
} TestDataClass;

static bool
TestKeyClass_Equals(const void *a, const void *b)
{
    return ((TestKeyClass *) a)->key_value == ((TestKeyClass *) b)->key_value;
}

static HashCodeType
TestKeyClass_Hash(const void *a)
{
    return ((TestKeyClass *) a)->hash_value;
}

static void
TestKeyClassDestroy(void **aPtr)
{
    parcMemory_Deallocate((void **) aPtr);
    aPtr = NULL;
}

static void
TestDataClassDestroy(void **aPtr)
{
    parcMemory_Deallocate((void **) aPtr);
    aPtr = NULL;
}

typedef struct truth_table_entry {
    unsigned key_value;
    unsigned hash_code;
    unsigned data_value;
} TruthTableEntry;

// ==============================


LONGBOW_TEST_RUNNER(parc_HashCodeTable)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Local);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_HashCodeTable)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_HashCodeTable)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcHashCodeTable_Add_Get);
    LONGBOW_RUN_TEST_CASE(Global, parcHashCodeTable_Create);
    LONGBOW_RUN_TEST_CASE(Global, parcHashCodeTable_Create_Size);
    LONGBOW_RUN_TEST_CASE(Global, parcHashCodeTable_Del);

    LONGBOW_RUN_TEST_CASE(Global, parcHashCodeTable_Add_DuplicateHashes);
    LONGBOW_RUN_TEST_CASE(Global, parcHashCodeTable_Add_DuplicateValues);

    LONGBOW_RUN_TEST_CASE(Global, parcHashCodeTable_BigTable);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    if (parcSafeMemory_ReportAllocation(STDOUT_FILENO) != 0) {
        printf("('%s' leaks memory by %d (allocs - frees)) ", longBowTestCase_GetName(testCase), parcMemory_Outstanding());
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, parcHashCodeTable_Add_Get)
{
    const int testsize = 4096;
    PARCHashCodeTable *table = parcHashCodeTable_Create(TestKeyClass_Equals, TestKeyClass_Hash, TestKeyClassDestroy, TestDataClassDestroy);
    TruthTableEntry *truthtable = parcMemory_Allocate(sizeof(TruthTableEntry) * testsize);
    assertNotNull(truthtable, "parcMemory_Allocate(%zu) returned NULL", sizeof(TruthTableEntry) * testsize);
    int i;
    int fd = open("/dev/urandom", O_RDONLY);

    if (fd == -1) {
        assertFalse(fd == -1, "Error opening random number generator: %s", strerror(errno));
    }

    ssize_t nread = read(fd, truthtable, sizeof(TruthTableEntry) * testsize);
    assertTrue(nread > 0, "Error using read");
    close(fd);

    for (i = 0; i < testsize; i++) {
        TestKeyClass *key = parcMemory_AllocateAndClear(sizeof(TestKeyClass));
        assertNotNull(key, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(TestKeyClass));
        TestDataClass *data = parcMemory_AllocateAndClear(sizeof(TestDataClass));
        assertNotNull(data, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(TestDataClass));

        key->key_value = truthtable[i].key_value;
        key->hash_value = truthtable[i].hash_code;
        data->data_value = truthtable[i].data_value;

        bool success = parcHashCodeTable_Add(table, key, data);
        assertTrue(success, "Failed inserting value");
    }

    // now retrieve them
    for (i = 0; i < testsize; i++) {
        TestKeyClass lookupkey;
        lookupkey.key_value = truthtable[i].key_value;
        lookupkey.hash_value = truthtable[i].hash_code;

        TestDataClass *data = parcHashCodeTable_Get(table, &lookupkey);
        assertTrue(data->data_value == truthtable[i].data_value, "Data value incorrect");
    }

    parcHashCodeTable_Destroy(&table);
    parcMemory_Deallocate((void **) &truthtable);
}

LONGBOW_TEST_CASE(Global, parcHashCodeTable_Create)
{
    PARCHashCodeTable *table = parcHashCodeTable_Create(TestKeyClass_Equals, TestKeyClass_Hash, TestKeyClassDestroy, TestDataClassDestroy);

    assertNotNull(table, "table came back as null");
    assertTrue(table->hashtable.tableSize == 0, "Hash table initialized to wrong size");
    assertTrue(table->hashtable.tableLimit == MIN_SIZE, "Initial table limit size is wrong");
    assertTrue(table->keyEqualsFunc == TestKeyClass_Equals, "KeyEqualsFunc wrong");
    assertTrue(table->keyHashCodeFunc == TestKeyClass_Hash, "KeyHashFunc wrong");
    assertTrue(table->keyDestroyer == TestKeyClassDestroy, "KeyDestroyer wrong");
    assertTrue(table->dataDestroyer == TestDataClassDestroy, "DataDestroyer wrong");

    parcHashCodeTable_Destroy(&table);
}

LONGBOW_TEST_CASE(Global, parcHashCodeTable_Create_Size)
{
    PARCHashCodeTable *table = parcHashCodeTable_Create_Size(TestKeyClass_Equals, TestKeyClass_Hash, TestKeyClassDestroy, TestDataClassDestroy, 16);

    assertNotNull(table, "table came back as null");
    assertTrue(table->hashtable.tableLimit == 16, "Initial table limit size is wrong");
    parcHashCodeTable_Destroy(&table);
}

LONGBOW_TEST_CASE(Global, parcHashCodeTable_Del)
{
    const int testsize = 6;
    PARCHashCodeTable *table = parcHashCodeTable_Create(TestKeyClass_Equals, TestKeyClass_Hash, TestKeyClassDestroy, TestDataClassDestroy);
    TruthTableEntry *truthtable = parcMemory_Allocate(sizeof(TruthTableEntry) * testsize);
    assertNotNull(truthtable, "parcMemory_Allocate(%zu) returned NULL", sizeof(TruthTableEntry) * testsize);
    int i;
    int fd = open("/dev/urandom", O_RDONLY);

    if (fd == -1) {
        assertFalse(fd == -1, "Error opening random number generator: %s", strerror(errno));
    }

    ssize_t nread = read(fd, truthtable, sizeof(TruthTableEntry) * testsize);
    assertTrue(nread > 0, "Error using read");
    close(fd);

    for (i = 0; i < testsize; i++) {
        TestKeyClass *key = parcMemory_AllocateAndClear(sizeof(TestKeyClass));
        assertNotNull(key, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(TestKeyClass));
        TestDataClass *data = parcMemory_AllocateAndClear(sizeof(TestDataClass));
        assertNotNull(data, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(TestDataClass));

        key->key_value = truthtable[i].key_value;
        key->hash_value = truthtable[i].hash_code;
        data->data_value = truthtable[i].data_value;

        bool success = parcHashCodeTable_Add(table, key, data);
        assertTrue(success, "Failed inserting value");
    }

    // delete the last one
    {
        TestKeyClass lookupkey;

        lookupkey.key_value = truthtable[testsize - 1].key_value;
        lookupkey.hash_value = truthtable[testsize - 1].hash_code;

        parcHashCodeTable_Del(table, &lookupkey);
        assertTrue(table->hashtable.tableSize == testsize - 1, "tableSize wrong");
    }

    for (i = 0; i < testsize - 1; i++) {
        TestKeyClass lookupkey;
        lookupkey.key_value = truthtable[i].key_value;
        lookupkey.hash_value = truthtable[i].hash_code;

        TestDataClass *data = parcHashCodeTable_Get(table, &lookupkey);
        assertTrue(data->data_value == truthtable[i].data_value, "Data value incorrect");
    }

    for (i = testsize - 1; i < testsize; i++) {
        TestKeyClass lookupkey;
        lookupkey.key_value = truthtable[i].key_value;
        lookupkey.hash_value = truthtable[i].hash_code;

        TestDataClass *data = parcHashCodeTable_Get(table, &lookupkey);
        assertNull(data, "Should not have returned deleted value");
    }
    parcHashCodeTable_Destroy(&table);
    parcMemory_Deallocate((void **) &truthtable);
}

LONGBOW_TEST_CASE(Global, parcHashCodeTable_Add_DuplicateHashes)
{
    PARCHashCodeTable *table = parcHashCodeTable_Create(TestKeyClass_Equals, TestKeyClass_Hash, TestKeyClassDestroy, TestDataClassDestroy);

    TestKeyClass *key1 = parcMemory_AllocateAndClear(sizeof(TestKeyClass));
    assertNotNull(key1, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(TestKeyClass));
    TestKeyClass *key2 = parcMemory_AllocateAndClear(sizeof(TestKeyClass));
    assertNotNull(key2, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(TestKeyClass));
    TestDataClass *data1 = parcMemory_AllocateAndClear(sizeof(TestDataClass));
    assertNotNull(data1, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(TestDataClass));
    TestDataClass *data2 = parcMemory_AllocateAndClear(sizeof(TestDataClass));
    assertNotNull(data2, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(TestDataClass));
    TestDataClass *test;
    bool success;

    *key1 = (TestKeyClass) { .key_value = 1, .hash_value = 2 };
    *key2 = (TestKeyClass) { .key_value = 3, .hash_value = 2 };
    data1->data_value = 11;
    data2->data_value = 22;

    success = parcHashCodeTable_Add(table, key1, data1);
    assertTrue(success, "Failed to add value");

    success = parcHashCodeTable_Add(table, key2, data2);
    assertTrue(success, "Failed to add value");

    // value 3 should be in position 3
    test = parcHashCodeTable_Get(table, key1);
    assertNotNull(test, "returned null on get");
    assertTrue(test->data_value == 11, "Got wrong value back for key1");

    test = parcHashCodeTable_Get(table, key2);
    assertNotNull(test, "returned null on get");
    assertTrue(test->data_value == 22, "Got wrong value back for key1");

    parcHashCodeTable_Destroy(&table);
}

LONGBOW_TEST_CASE(Global, parcHashCodeTable_Add_DuplicateValues)
{
    PARCHashCodeTable *table = parcHashCodeTable_Create(TestKeyClass_Equals, TestKeyClass_Hash, TestKeyClassDestroy, TestDataClassDestroy);

    TestKeyClass *key1 = parcMemory_AllocateAndClear(sizeof(TestKeyClass));
    assertNotNull(key1, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(TestKeyClass));
    TestKeyClass *key2 = parcMemory_AllocateAndClear(sizeof(TestKeyClass));
    assertNotNull(key2, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(TestKeyClass));
    TestDataClass *data1 = parcMemory_AllocateAndClear(sizeof(TestDataClass));
    assertNotNull(data1, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(TestDataClass));
    TestDataClass *data2 = parcMemory_AllocateAndClear(sizeof(TestDataClass));
    assertNotNull(data2, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(TestDataClass));
    TestDataClass *test;
    bool success;

    *key1 = (TestKeyClass) { .key_value = 1, .hash_value = 2 };
    *key2 = (TestKeyClass) { .key_value = 1, .hash_value = 2 };
    data1->data_value = 11;
    data2->data_value = 22;

    success = parcHashCodeTable_Add(table, key1, data1);
    assertTrue(success, "Failed to add value");

    success = parcHashCodeTable_Add(table, key2, data2);
    assertFalse(success, "Second add should have failed on duplicate key");

    // value 3 should be in position 3
    test = parcHashCodeTable_Get(table, key1);
    assertNotNull(test, "returned null on get");
    assertTrue(test->data_value == 11, "Got wrong value back for key1");

    parcHashCodeTable_Destroy(&table);
    parcMemory_Deallocate((void **) &key2);
    parcMemory_Deallocate((void **) &data2);
}

LONGBOW_TEST_CASE(Global, parcHashCodeTable_BigTable)
{
    PARCHashCodeTable *table = parcHashCodeTable_Create(TestKeyClass_Equals, TestKeyClass_Hash, TestKeyClassDestroy, TestDataClassDestroy);

    struct timeval t0, t1;

    gettimeofday(&t0, NULL);

    int loops = 1000;
    for (int i = 0; i < loops; i++) {
        TestKeyClass *key = parcMemory_AllocateAndClear(sizeof(TestKeyClass));
        assertNotNull(key, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(TestKeyClass));
        TestDataClass *data = parcMemory_AllocateAndClear(sizeof(TestDataClass));
        assertNotNull(data, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(TestDataClass));

        *key = (TestKeyClass) { .key_value = i, .hash_value = i };
        data->data_value = i;

        bool success = parcHashCodeTable_Add(table, key, data);
        assertTrue(success, "Failed to add value");
    }
    gettimeofday(&t1, NULL);

    timersub(&t1, &t0, &t1);
    double sec = t1.tv_sec + t1.tv_usec * 1E-6;
    printf("expand count %u, sec = %.3f, sec/add = %.9f\n", table->expandCount, sec, sec / loops);

    gettimeofday(&t0, NULL);
    parcHashCodeTable_Destroy(&table);
    gettimeofday(&t1, NULL);

    timersub(&t1, &t0, &t1);
    sec = t1.tv_sec + t1.tv_usec * 1E-6;
    printf("destroy sec = %.3f, sec/add = %.9f\n", sec, sec / loops);
}

LONGBOW_TEST_FIXTURE(Local)
{
    LONGBOW_RUN_TEST_CASE(Local, _findIndex);
}

LONGBOW_TEST_FIXTURE_SETUP(Local)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Local)
{
    if (parcSafeMemory_ReportAllocation(STDOUT_FILENO) != 0) {
        printf("('%s' leaks memory by %d (allocs - frees)) ", longBowTestCase_GetName(testCase), parcMemory_Outstanding());
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Local, _findIndex)
{
    PARCHashCodeTable *table = parcHashCodeTable_Create(TestKeyClass_Equals, TestKeyClass_Hash, TestKeyClassDestroy, TestDataClassDestroy);

    // stick a data element in the middle of the table
    TestKeyClass *key = parcMemory_AllocateAndClear(sizeof(TestKeyClass));
    assertNotNull(key, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(TestKeyClass));
    TestDataClass *data = parcMemory_AllocateAndClear(sizeof(TestDataClass));
    assertNotNull(data, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(TestDataClass));

    key->key_value = 1;
    key->hash_value = 37;
    data->data_value = 7;

    table->hashtable.entries[37].key = key;
    table->hashtable.entries[37].hashcode = key->hash_value;
    table->hashtable.entries[37].data = data;

    size_t index;
    bool success = _findIndex(table, key, &index);
    assertTrue(success, "FindIndex did not find known value");
    assertTrue(index == 37, "FindIndex returned wrong value");


    parcHashCodeTable_Destroy(&table);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_HashCodeTable);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
