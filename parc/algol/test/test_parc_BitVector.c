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
#include <LongBow/unit-test.h>

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../parc_BitVector.c"

#include <stdio.h>
#include <parc/algol/parc_SafeMemory.h>
#include <limits.h>

LONGBOW_TEST_RUNNER(parc_BitVector)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Local);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_BitVector)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);

    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_BitVector)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcBitVector_Create_Release);
    LONGBOW_RUN_TEST_CASE(Global, parcBitVector_SetClear);
    LONGBOW_RUN_TEST_CASE(Global, parcBitVector_SetVector);
    LONGBOW_RUN_TEST_CASE(Global, parcBitVector_Reset);
    LONGBOW_RUN_TEST_CASE(Global, parcBitVector_ClearVector);
    LONGBOW_RUN_TEST_CASE(Global, parcBitVector_NextBitSet);
    LONGBOW_RUN_TEST_CASE(Global, parcBitVector_Get);
    LONGBOW_RUN_TEST_CASE(Global, parcBitVector_ToString);
    LONGBOW_RUN_TEST_CASE(Global, parcBitVector_Copy);
    LONGBOW_RUN_TEST_CASE(Global, parcBitVector_Equals);
    LONGBOW_RUN_TEST_CASE(Global, parcBitVector_Contains);
    LONGBOW_RUN_TEST_CASE(Global, parcBitVector_Set);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDOUT_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks %d memory allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, parcBitVector_Create_Release)
{
    PARCBitVector *parcBitVector = parcBitVector_Create();
    assertTrue(parcBitVector, "parcBitVector_Create returned a NULL pointer");
    PARCBitVector *reference = parcBitVector;
    parcBitVector_Acquire(reference);
    parcBitVector_Release(&parcBitVector);
    parcBitVector_Release(&reference);
}

LONGBOW_TEST_CASE(Global, parcBitVector_Set)
{
    PARCBitVector *parcBitVector = parcBitVector_Create();
    assertTrue(parcBitVector, "parcBitVector_Create returned a NULL pointer");
    assertTrue(parcBitVector_NumberOfBitsSet(parcBitVector) == 0, "parcBitVector_Create created a non-empty vector");

    parcBitVector_Set(parcBitVector, 0);
    assertTrue(parcBitVector_NumberOfBitsSet(parcBitVector) == 1, "Expect number of bits set to be 1");
    assertTrue(parcBitVector->firstBitSet == 0, "Expect first bit set to be 0");
    assertTrue(parcBitVector->bitLength == 8, "Expect the bitLength to be 8");
    assertTrue(parcBitVector->bitArray[0] == (uint8_t) 1, "Expect the bitArray as a unsigned char to be = 1");

    parcBitVector_Set(parcBitVector, 7);
    assertTrue(parcBitVector_NumberOfBitsSet(parcBitVector) == 2, "Expect number of bits set to be 2");
    assertTrue(parcBitVector->firstBitSet == 0, "Expect first bit set to be 0");
    assertTrue(parcBitVector->bitLength == 8, "Expect the bitLength to be 8");
    assertTrue(parcBitVector->bitArray[0] == (uint8_t) 0x81, "Expect the bitArray as a unsigned char to be = 0x81");

    parcBitVector_Set(parcBitVector, 8);
    assertTrue(parcBitVector_NumberOfBitsSet(parcBitVector) == 3, "Expect number of bits set to be 3");
    assertTrue(parcBitVector->firstBitSet == 0, "Expect first bit set to be 0");
    assertTrue(parcBitVector->bitLength == 16, "Expect the bitLength to be 16");
    assertTrue(parcBitVector->bitArray[0] == (uint8_t) 0x81, "Expect the bitArray as a unsigned char to be = 0x81");
    assertTrue(parcBitVector->bitArray[1] == (uint8_t) 0x1, "Expect the bitArray as a unsigned char to be = 0x1");

    parcBitVector_Release(&parcBitVector);
}

LONGBOW_TEST_CASE(Global, parcBitVector_SetClear)
{
    PARCBitVector *parcBitVector = parcBitVector_Create();
    assertTrue(parcBitVector, "parcBitVector_Create returned a NULL pointer");
    assertTrue(parcBitVector_NumberOfBitsSet(parcBitVector) == 0, "parcBitVector_Create created a non-empty vector");

    parcBitVector_Set(parcBitVector, 10);
    assertTrue(parcBitVector_NumberOfBitsSet(parcBitVector) == 1, "parcBitVector_Set failed");

    parcBitVector_Clear(parcBitVector, 10);
    assertTrue(parcBitVector_NumberOfBitsSet(parcBitVector) == 0, "parcBitVector_Clear failed");

    parcBitVector_Clear(parcBitVector, 20);
    assertTrue(parcBitVector_NumberOfBitsSet(parcBitVector) == 0, "parcBitVector_Clear failed");

    parcBitVector_Release(&parcBitVector);
}

LONGBOW_TEST_CASE(Global, parcBitVector_SetVector)
{
    PARCBitVector *parcBitVector = parcBitVector_Create();
    assertTrue(parcBitVector, "parcBitVector_Create returned a NULL pointer");
    PARCBitVector *setVector = parcBitVector_Create();
    parcBitVector_Set(parcBitVector, 1);
    assertTrue(parcBitVector_NumberOfBitsSet(parcBitVector) == 1, "parcBitVector_Set failed");

    parcBitVector_Set(setVector, 20);
    parcBitVector_SetVector(parcBitVector, setVector);
    assertTrue(parcBitVector_NumberOfBitsSet(parcBitVector) == 2, "parcBitVector_SetVector failed");
    assertTrue(parcBitVector_NextBitSet(parcBitVector, 0) == 1, "parcBitVector_Set failed to set bit 1");
    assertTrue(parcBitVector_NextBitSet(parcBitVector, 2) == 20, "parcBitVector_SetVector failed to set bit 20");

    parcBitVector_Set(setVector, 10);
    parcBitVector_SetVector(parcBitVector, setVector);
    assertTrue(parcBitVector_NumberOfBitsSet(parcBitVector) == 3, "parcBitVector_SetVector failed");
    parcBitVector_Release(&parcBitVector);
    parcBitVector_Release(&setVector);
}

LONGBOW_TEST_CASE(Global, parcBitVector_Reset)
{
    PARCBitVector *parcBitVector = parcBitVector_Create();
    assertTrue(parcBitVector, "parcBitVector_Create returned a NULL pointer");

    // Reset and empty vector test
    parcBitVector_Reset(parcBitVector);
    assertTrue(parcBitVector_NumberOfBitsSet(parcBitVector) == 0, "parcBitVector_Reset failed");

    parcBitVector_Set(parcBitVector, 1);
    parcBitVector_Set(parcBitVector, 42);
    assertTrue(parcBitVector_NumberOfBitsSet(parcBitVector) == 2, "parcBitVector_Set failed");
    assertTrue(parcBitVector->bitLength == 48, "Expected a bitLength of 48");

    parcBitVector_Reset(parcBitVector);
    assertTrue(parcBitVector_NumberOfBitsSet(parcBitVector) == 0, "parcBitVector_Reset failed");
    assertTrue(parcBitVector->bitLength == 48, "Expected a bitLength of 48");

    parcBitVector_Release(&parcBitVector);
}

LONGBOW_TEST_CASE(Global, parcBitVector_ClearVector)
{
    PARCBitVector *parcBitVector = parcBitVector_Create();
    assertTrue(parcBitVector, "parcBitVector_Create returned a NULL pointer");

    PARCBitVector *setVector = parcBitVector_Create();
    parcBitVector_Set(parcBitVector, 1);
    assertTrue(parcBitVector_NumberOfBitsSet(parcBitVector) == 1, "parcBitVector_Set failed to set bit");

    parcBitVector_Set(setVector, 1);
    parcBitVector_Set(setVector, 20);
    parcBitVector_ClearVector(parcBitVector, setVector);
    assertTrue(parcBitVector_NumberOfBitsSet(parcBitVector) == 0, "parcBitVector_ClearVector failed to clear vector");

    parcBitVector_Set(parcBitVector, 12);
    parcBitVector_Set(parcBitVector, 17);
    parcBitVector_ClearVector(parcBitVector, parcBitVector);
    assertTrue(parcBitVector_NumberOfBitsSet(parcBitVector) == 0, "parcBitVector_ClearVector failed to clear vector");

    parcBitVector_Release(&parcBitVector);
    parcBitVector_Release(&setVector);
}

LONGBOW_TEST_CASE(Global, parcBitVector_NextBitSet)
{
    PARCBitVector *parcBitVector = parcBitVector_Create();
    assertTrue(parcBitVector, "parcBitVector_Create returned a NULL pointer");
    assertTrue(parcBitVector_NumberOfBitsSet(parcBitVector) == 0, "parcBitVector_Create created a non-empty vector");

    int nextBit = parcBitVector_NextBitSet(parcBitVector, 0);
    assertTrue(nextBit == -1, "parcBitVector_NextBitSet should have failed (%d)", nextBit);

    parcBitVector_Set(parcBitVector, 10);
    nextBit = parcBitVector_NextBitSet(parcBitVector, 0);
    assertTrue(nextBit == 10, "parcBitVector_NextBitSet failed (%d)", nextBit);

    nextBit = parcBitVector_NextBitSet(parcBitVector, 20);
    assertTrue(nextBit == -1, "parcBitVector_NextBitSet read past end of vector (%d)", nextBit);

    nextBit = parcBitVector_NextBitSet(parcBitVector, 10);
    assertTrue(nextBit == 10, "parcBitVector_NextBitSet failed (%d)", nextBit);

    nextBit = parcBitVector_NextBitSet(parcBitVector, 11);
    assertTrue(nextBit == -1, "parcBitVector_NextBitSet should have failed (%d)", nextBit);

    parcBitVector_Set(parcBitVector, 20);
    nextBit = parcBitVector_NextBitSet(parcBitVector, 11);
    assertTrue(nextBit == 20, "parcBitVector_NextBitSet failed (%d)", nextBit);
    parcBitVector_Release(&parcBitVector);
}

LONGBOW_TEST_CASE(Global, parcBitVector_Get)
{
    PARCBitVector *parcBitVector = parcBitVector_Create();
    assertTrue(parcBitVector, "parcBitVector_Create returned a NULL pointer");
    assertTrue(parcBitVector_NumberOfBitsSet(parcBitVector) == 0, "parcBitVector_Create created a non-empty vector");

    parcBitVector_Set(parcBitVector, 10);
    int bitValue = parcBitVector_Get(parcBitVector, 10);
    assertTrue(bitValue == 1, "parcBitVector_Get returned wrong value (%d)", bitValue);

    bitValue = parcBitVector_Get(parcBitVector, 11);
    assertTrue(bitValue == 0, "parcBitVector_Get returned wrong value (%d)", bitValue);

    bitValue = parcBitVector_Get(parcBitVector, 100);
    assertTrue(bitValue == -1, "parcBitVector_NextBitSet should have failed (%d)", bitValue);

    parcBitVector_Release(&parcBitVector);
}

LONGBOW_TEST_CASE(Global, parcBitVector_ToString)
{
    PARCBitVector *parcBitVector = parcBitVector_Create();
    assertTrue(parcBitVector, "parcBitVector_Create returned a NULL pointer");

    char *string = parcBitVector_ToString(parcBitVector);
    assertTrue(strcmp(string, "[ ]") == 0, "parcBitVector_ToString returned unexpected representation (%s != [ ])", string);
    parcMemory_Deallocate(&string);

    parcBitVector_Set(parcBitVector, 10);
    parcBitVector_Set(parcBitVector, 1);
    string = parcBitVector_ToString(parcBitVector);
    assertTrue(strcmp(string, "[ 1 10 ]") == 0, "parcBitVector_ToString returned unexpected representation (%s != [ 1 10 ])", string);
    parcMemory_Deallocate(&string);

    parcBitVector_Release(&parcBitVector);
}

LONGBOW_TEST_CASE(Global, parcBitVector_Copy)
{
    PARCBitVector *parcBitVector = parcBitVector_Create();
    assertTrue(parcBitVector, "parcBitVector_Create returned a NULL pointer");

    parcBitVector_Set(parcBitVector, 10);
    PARCBitVector *copy = parcBitVector_Copy(parcBitVector);
    assertTrue(parcBitVector_NumberOfBitsSet(copy) == 1, "parcBitVector_Copy failed to copy set bit");
    assertTrue(parcBitVector_NextBitSet(copy, 0) == 10, "parcBitVector_Copy failed to copy correct bit");

    parcBitVector_Release(&copy);
    parcBitVector_Release(&parcBitVector);
}

LONGBOW_TEST_CASE(Global, parcBitVector_Equals)
{
    PARCBitVector *parcBitVector = parcBitVector_Create();
    assertTrue(parcBitVector, "parcBitVector_Create returned a NULL pointer");

    parcBitVector_Set(parcBitVector, 10);
    PARCBitVector *copy = parcBitVector_Copy(parcBitVector);
    assertTrue(parcBitVector_Equals(parcBitVector, copy), "Duplicate vector found unequal");

    parcBitVector_Set(copy, 9);
    assertFalse(parcBitVector_Equals(parcBitVector, copy), "Unequal vector found equal");

    parcBitVector_Clear(copy, 9);
    parcBitVector_Set(copy, 29);
    assertFalse(parcBitVector_Equals(parcBitVector, copy), "Unequal long vector found equal");

    parcBitVector_Clear(copy, 29);
    assertTrue(parcBitVector_Equals(parcBitVector, copy), "Equal long vector found unequal");
    assertTrue(parcBitVector_Equals(copy, parcBitVector), "Equal long vector found unequal");

    parcBitVector_Release(&copy);
    parcBitVector_Release(&parcBitVector);
}

LONGBOW_TEST_CASE(Global, parcBitVector_Contains)
{
    PARCBitVector *supersetVector = parcBitVector_Create();

    parcBitVector_Set(supersetVector, 10);
    parcBitVector_Set(supersetVector, 11);

    PARCBitVector *testVector = parcBitVector_Create();
    parcBitVector_Set(testVector, 10);
    assertTrue(parcBitVector_Contains(supersetVector, testVector), "Expect superset to contain testVector");

    parcBitVector_Set(testVector, 12);
    assertFalse(parcBitVector_Contains(supersetVector, testVector), "Expect superset to not contain testVector");

    parcBitVector_Release(&supersetVector);
    parcBitVector_Release(&testVector);
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
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDOUT_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks %d memory allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_BitVector);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
