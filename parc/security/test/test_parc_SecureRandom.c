/*
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * Copyright 2015 Palo Alto Research Center, Inc. (PARC), a Xerox company.  All Rights Reserved.
 * The content of this file, whole or in part, is subject to licensing terms.
 * If distributing this software, include this License Header Notice in each
 * file and provide the accompanying LICENSE file.
 */
/**
 * @author Christopher A. Wood, Computing Science Laboratory, PARC
 * @copyright 2016 Palo Alto Research Center, Inc. (PARC), A Xerox Company.  All Rights Reserved.
 */
#include "../parc_SecureRandom.c"
#include <sys/param.h>

#include <fcntl.h>

#include <LongBow/testing.h>
#include <LongBow/debugging.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_SafeMemory.h>
#include <parc/algol/parc_LinkedList.h>

#include <parc/testing/parc_MemoryTesting.h>
#include <parc/testing/parc_ObjectTesting.h>

#define NUM_TESTS 1000
#define EPSILON 0.01

LONGBOW_TEST_RUNNER(parc_SecureRandom)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(CreateAcquireRelease);
    LONGBOW_RUN_TEST_FIXTURE(Object);
    LONGBOW_RUN_TEST_FIXTURE(Specialization);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_SecureRandom)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_SecureRandom)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(CreateAcquireRelease)
{
    LONGBOW_RUN_TEST_CASE(CreateAcquireRelease, CreateRelease);
}

LONGBOW_TEST_FIXTURE_SETUP(CreateAcquireRelease)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);
    parcSecurity_Init();
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(CreateAcquireRelease)
{
    parcSecurity_Fini();
    if (!parcMemoryTesting_ExpectedOutstanding(0, "%s leaked memory.", longBowTestCase_GetFullName(testCase))) {
        return LONGBOW_STATUS_MEMORYLEAK;
    }

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(CreateAcquireRelease, CreateRelease)
{
    PARCSecureRandom *rng = parcSecureRandom_Create();
    assertNotNull(rng, "Expected non-null result from parcSecureRandom_Create();");

    parcObjectTesting_AssertAcquireReleaseContract(parcSecureRandom_Acquire, rng);

    parcSecureRandom_Release(&rng);
    assertNull(rng, "Expected null result from parcSecureRandom_Release();");
}

LONGBOW_TEST_FIXTURE(Object)
{
    LONGBOW_RUN_TEST_CASE(Object, parcSecureRandom_IsValid);
}

LONGBOW_TEST_FIXTURE_SETUP(Object)
{
    parcSecurity_Init();
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Object)
{
    parcSecurity_Fini();
    if (!parcMemoryTesting_ExpectedOutstanding(0, "%s mismanaged memory.", longBowTestCase_GetFullName(testCase))) {
        return LONGBOW_STATUS_MEMORYLEAK;
    }

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Object, parcSecureRandom_IsValid)
{
    PARCSecureRandom *rng = parcSecureRandom_Create();
    assertNotNull(rng, "Expected a non-NULL PARCSecureRandom");

    assertTrue(parcSecureRandom_IsValid(rng), "Expected parcSecureRandom_Create to result in a valid instance.");

    parcSecureRandom_Release(&rng);
    assertFalse(parcSecureRandom_IsValid(rng), "Expected parcSecureRandom_Release to result in an invalid instance.");
}

LONGBOW_TEST_FIXTURE(Specialization)
{
    LONGBOW_RUN_TEST_CASE(Specialization, parcSecureRandom_Create);
    LONGBOW_RUN_TEST_CASE(Specialization, parcSecureRandom_CreateWithSeed);
    LONGBOW_RUN_TEST_CASE(Specialization, parcSecureRandom_Next);
    LONGBOW_RUN_TEST_CASE(Specialization, parcSecureRandom_NextBytes);
}

LONGBOW_TEST_FIXTURE_SETUP(Specialization)
{
    parcSecurity_Init();
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Specialization)
{
    parcSecurity_Fini();
    if (!parcMemoryTesting_ExpectedOutstanding(0, "%s mismanaged memory.", longBowTestCase_GetFullName(testCase))) {
        return LONGBOW_STATUS_MEMORYLEAK;
    }

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Specialization, parcSecureRandom_Create)
{
    PARCSecureRandom *rng = parcSecureRandom_Create();
    assertTrue(parcSecureRandom_IsValid(rng), "Expected parcSecureRandom_Create to result in a valid instance.");
    parcSecureRandom_Release(&rng);
}

LONGBOW_TEST_CASE(Specialization, parcSecureRandom_CreateWithSeed)
{
    PARCBuffer *seed = parcBuffer_Allocate(1024);
    PARCSecureRandom *rng = parcSecureRandom_CreateWithSeed(seed);

    assertTrue(parcSecureRandom_IsValid(rng), "Expected parcSecureRandom_CreateWithSeed to result in a valid instance.");

    parcSecureRandom_Release(&rng);
    parcBuffer_Release(&seed);
}

static void
_stressTestNext(PARCSecureRandom *rng)
{
    PARCLinkedList *seen = parcLinkedList_Create();
    size_t duplicates = 0;
    for (size_t i = 0; i < NUM_TESTS; i++) {
        uint32_t next = parcSecureRandom_Next(rng);
        PARCBuffer *buffer = parcBuffer_Allocate(sizeof(next));
        parcBuffer_Flip(parcBuffer_PutUint32(buffer, next));

        if (parcLinkedList_Contains(seen, buffer)) {
            duplicates++;
        } else {
            parcLinkedList_Append(seen, buffer);
        }

        parcBuffer_Release(&buffer);
    }

    assertFalse(duplicates > (NUM_TESTS * EPSILON), "The RNG failed to generate random values: saw %zu duplicates", duplicates);
    parcLinkedList_Release(&seen);
}

LONGBOW_TEST_CASE(Specialization, parcSecureRandom_Next)
{
    PARCSecureRandom *rng = parcSecureRandom_Create();
    assertTrue(parcSecureRandom_IsValid(rng), "Expected parcSecureRandom_Create to result in a valid instance");
    _stressTestNext(rng);
    parcSecureRandom_Release(&rng);
}

static void
_stressTestNextBytes(PARCSecureRandom *rng)
{
    PARCLinkedList *seen = parcLinkedList_Create();
    size_t duplicates = 0;
    for (size_t i = 0; i < NUM_TESTS; i++) {
        PARCBuffer *buffer = parcBuffer_Allocate(32);

        int numBytes = parcSecureRandom_NextBytes(rng, buffer);
        assertTrue(numBytes == 32, "Expected 32 bytes from the RNG, got %d", numBytes);

        if (parcLinkedList_Contains(seen, buffer)) {
            duplicates++;
        } else {
            parcLinkedList_Append(seen, buffer);
        }

        parcBuffer_Release(&buffer);
    }

    assertFalse(duplicates > (NUM_TESTS * EPSILON), "The RNG failed to generate random values: saw %zu duplicates", duplicates);
    parcLinkedList_Release(&seen);
}

LONGBOW_TEST_CASE(Specialization, parcSecureRandom_NextBytes)
{
    PARCBuffer *seed = parcBuffer_Allocate(1024);
    PARCSecureRandom *rng = parcSecureRandom_CreateWithSeed(seed);

    assertTrue(parcSecureRandom_IsValid(rng), "Expected parcSecureRandom_CreateWithSeed to result in a valid instance.");
    _stressTestNextBytes(rng);

    parcSecureRandom_Release(&rng);
    parcBuffer_Release(&seed);
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_SecureRandom);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
