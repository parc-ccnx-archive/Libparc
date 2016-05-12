/*
 * Copyright (c) 2015-2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @author Christopher A. Wood, Computing Science Laboratory, PARC
 * @copyright (c) 2015-2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include "../parc_SecureRandom.c"
#include <sys/param.h>

#include <fcntl.h>

#include <LongBow/testing.h>
#include <LongBow/debugging.h>

#include <parc/security/parc_Security.h>
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
