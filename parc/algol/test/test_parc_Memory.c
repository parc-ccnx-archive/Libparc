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

#include <LongBow/unit-test.h>

#include <parc/algol/parc_SafeMemory.h>

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../parc_Memory.c"

LONGBOW_TEST_RUNNER(parc_Memory)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Static);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_Memory)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_Memory)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcMemory_RoundUpToMultiple);
    LONGBOW_RUN_TEST_CASE(Global, parcMemory_RoundUpToCacheLine);
    LONGBOW_RUN_TEST_CASE(Global, parcMemory_Allocate);
    LONGBOW_RUN_TEST_CASE(Global, parcMemory_AllocateAndClear);
    LONGBOW_RUN_TEST_CASE(Global, parcMemory_MemAlign);
    LONGBOW_RUN_TEST_CASE(Global, parcMemory_Reallocate);
    LONGBOW_RUN_TEST_CASE(Global, parcMemory_StringDuplicate);
    LONGBOW_RUN_TEST_CASE(Global, parcMemory_Outstanding);
    LONGBOW_RUN_TEST_CASE(Global, parcMemory_SetInterface);
    LONGBOW_RUN_TEST_CASE(Global, parcMemory_Format);
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

LONGBOW_TEST_CASE(Global, parcMemory_RoundUpToMultiple)
{
    size_t actual = parcMemory_RoundUpToMultiple(14, 12);
    assertTrue((actual % 12) == 0, "Expected %zd to be a multiple of %d", actual, 12);
    assertTrue(24 == actual, "Expected 24, actual %zd", actual);

    actual = parcMemory_RoundUpToMultiple(14, 20);
    assertTrue((actual % 20) == 0, "Expected %zd to be a multiple of %d", actual, 20);
    assertTrue(20 == actual, "Expected 20, actual %zd", actual);

    actual = parcMemory_RoundUpToMultiple(20, 20);
    assertTrue((actual % 20) == 0, "Expected %zd to be a multiple of %d", actual, 20);
    assertTrue(20 == actual, "Expected 20, actual %zd", actual);

    actual = parcMemory_RoundUpToMultiple(0, 20);
    assertTrue((actual % 20) == 0, "Expected %zd to be a multiple of %d", actual, 20);
    assertTrue(20 == actual, "Expected 20, actual %zd", actual);

    actual = parcMemory_RoundUpToMultiple(8, 0);
    assertTrue(actual == 8, "Expected %d, actual %zd", 8, actual);
}

LONGBOW_TEST_CASE(Global, parcMemory_RoundUpToCacheLine)
{
    size_t actual = parcMemory_RoundUpToCacheLine(LEVEL1_DCACHE_LINESIZE - 1);
    assertTrue((actual % LEVEL1_DCACHE_LINESIZE) == 0,
               "Expected %zd to be a multiple of %d", actual, LEVEL1_DCACHE_LINESIZE);
}

LONGBOW_TEST_CASE(Global, parcMemory_Allocate)
{
    void *pointer;
    pointer = parcMemory_Allocate(sizeof(int));
    assertNotNull(pointer, "Expected pointer to not be NULL");

    parcMemory_Deallocate(&pointer);
    assertNull(pointer, "Expected pointer to not be NULL");
}

LONGBOW_TEST_CASE(Global, parcMemory_MemAlign)
{
    void *pointer;
    int actual = parcMemory_MemAlign(&pointer, sizeof(void *), sizeof(int));
    assertTrue(actual == 0, "Expected successful return value.");
    assertNotNull(pointer, "Expected pointer to not be NULL");

    parcMemory_Deallocate(&pointer);
    assertNull(pointer, "Expected pointer to not be NULL");
}

LONGBOW_TEST_CASE(Global, parcMemory_Reallocate)
{
    void *pointer;
    int actual = parcMemory_MemAlign(&pointer, sizeof(void *), sizeof(int));
    assertTrue(actual == 0, "Expected successful return value.");

    pointer = parcMemory_Reallocate(pointer, sizeof(int) * 2);
    assertNotNull(pointer, "Expected pointer to not be NULL");

    parcMemory_Deallocate(&pointer);
    assertNull(pointer, "Expected pointer to not be NULL");
}

LONGBOW_TEST_CASE(Global, parcMemory_AllocateAndClear)
{
    void *pointer;
    pointer = parcMemory_AllocateAndClear(sizeof(int));
    assertNotNull(pointer, "Expected pointer to not be NULL");

    for (int i = 0; i < sizeof(int); i++) {
        assertTrue(((char *) pointer)[i] == 0, "Expected cell to be zero.");
    }

    parcMemory_Deallocate(&pointer);
    assertNull(pointer, "Expected pointer to not be NULL");
}

LONGBOW_TEST_CASE(Global, parcMemory_StringDuplicate)
{
    char *expected = "Hello";

    char *actual = parcMemory_StringDuplicate(expected, strlen(expected));

    assertTrue(strcmp(expected, actual) == 0, "Expected %s, actual %s", expected, actual);

    parcMemory_Deallocate((void **) &actual);
}

LONGBOW_TEST_CASE(Global, parcMemory_Outstanding)
{
    void *pointer;
    pointer = parcMemory_Allocate(sizeof(int));

    size_t expected = 1;
    size_t actual = parcMemory_Outstanding();
    assertTrue(expected == actual, "Expected %zd, actual %zd", expected, actual);

    parcMemory_Deallocate(&pointer);

    expected = 0;
    actual = parcMemory_Outstanding();
    assertTrue(expected == actual, "Expected %zd, actual %zd", expected, actual);
}

LONGBOW_TEST_CASE(Global, parcMemory_SetInterface)
{
    const PARCMemoryInterface *old = parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);

    parcMemory_SetInterface(old);
}

LONGBOW_TEST_CASE(Global, parcMemory_Format)
{
    char *expected = "Hello World";
    char *actual = parcMemory_Format("Hello %s", "World");

    assertTrue(strcmp(expected, actual) == 0,
               "Expected '%s', actual '%s'", expected, actual);

    parcMemory_Deallocate(&actual);
}

LONGBOW_TEST_FIXTURE(Static)
{
}

LONGBOW_TEST_FIXTURE_SETUP(Static)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Static)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_Memory);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
