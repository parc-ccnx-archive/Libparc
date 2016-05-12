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
#include "../parc_Hash.c"

#include <LongBow/testing.h>
#include <stdio.h>

#include <parc/algol/parc_SafeMemory.h>

LONGBOW_TEST_RUNNER(parc_Hash)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Local);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_Hash)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_Hash)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcHash32Bit_Create);
    LONGBOW_RUN_TEST_CASE(Global, parcHash32Bit_Acquire);
    LONGBOW_RUN_TEST_CASE(Global, parcHash32Bit_Release);
    LONGBOW_RUN_TEST_CASE(Global, parcHash32Bit_Update);
    LONGBOW_RUN_TEST_CASE(Global, parcHash32Bit_UpdateUint32);
    LONGBOW_RUN_TEST_CASE(Global, parcHash32Bit_Hash);

    LONGBOW_RUN_TEST_CASE(Global, parc_Hash32_Data);
    LONGBOW_RUN_TEST_CASE(Global, parc_Hash32_Int32);
    LONGBOW_RUN_TEST_CASE(Global, parc_Hash32_Int64);
    LONGBOW_RUN_TEST_CASE(Global, parc_Hash64_Data);
    LONGBOW_RUN_TEST_CASE(Global, parc_Hash64_Int32);
    LONGBOW_RUN_TEST_CASE(Global, parc_Hash64_Int64);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDOUT_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, parcHash32Bit_Create)
{
    testUnimplemented("");
}

LONGBOW_TEST_CASE(Global, parcHash32Bit_Acquire)
{
    testUnimplemented("");
}

LONGBOW_TEST_CASE(Global, parcHash32Bit_Release)
{
    testUnimplemented("");
}

LONGBOW_TEST_CASE(Global, parcHash32Bit_Update)
{
    PARCHash32Bits *hash = parcHash32Bits_Create();
    parcHash32Bits_Update(hash, "123", 3);
    uint32_t value = parcHash32Bits_Hash(hash);
    assertTrue(value != 0, "Expected a non-zero (non-initial) value");

    parcHash32Bits_Release(&hash);
}

LONGBOW_TEST_CASE(Global, parcHash32Bit_UpdateUint32)
{
    PARCHash32Bits *hash = parcHash32Bits_Create();
    parcHash32Bits_UpdateUint32(hash, 123);
    uint32_t value = parcHash32Bits_Hash(hash);
    assertTrue(value != 0, "Expected a non-zero (non-initial) value");

    parcHash32Bits_Release(&hash);
}

LONGBOW_TEST_CASE(Global, parcHash32Bit_Hash)
{
    testUnimplemented("");
}

LONGBOW_TEST_CASE(Global, parc_Hash32_Data)
{
    char *data1 = "Hello World";
    char *data2 = "Hello World1";
    char *data3 = "Hello World2";

    char data4[20];
    strncpy(data4, data1, sizeof(data4));

    uint32_t hash1 = parcHash32_Data(data1, strlen(data1));
    uint32_t hash2 = parcHash32_Data(data2, strlen(data2));
    uint32_t hash3 = parcHash32_Data(data3, strlen(data3));
    uint32_t hash4 = parcHash32_Data(data4, strlen(data4));

    assertTrue(hash1 != 0, "Hash is 0, unlikely");
    assertTrue(hash2 != 0, "Hash is 0, unlikely");
    assertTrue(hash3 != 0, "Hash is 0, unlikely");
    assertTrue(hash4 != 0, "Hash is 0, unlikely");
    assertTrue(hash1 != hash2, "Hash collision, unlikely");
    assertTrue(hash3 != hash2, "Hash collision, unlikely");
    assertTrue(hash3 != hash1, "Hash collision, unlikely");
    assertTrue(hash1 == hash4, "Hash different for same content");
}

LONGBOW_TEST_CASE(Global, parc_Hash32_Int32)
{
    uint32_t data1 = 12345;
    uint32_t data2 = 12346;
    uint32_t data3 = 12345;

    uint32_t hash1 = parcHash32_Int32(data1);
    uint32_t hash2 = parcHash32_Int32(data2);
    uint32_t hash3 = parcHash32_Int32(data3);

    assertTrue(hash1 != 0, "Hash is 0, unlikely");
    assertTrue(hash2 != 0, "Hash is 0, unlikely");
    assertTrue(hash3 != 0, "Hash is 0, unlikely");
    assertTrue(hash1 != hash2, "Hash collision, unlikely");
    assertTrue(hash1 == hash3, "Hash different for same content");
}

LONGBOW_TEST_CASE(Global, parc_Hash32_Int64)
{
    uint64_t data1 = 10010010012345;
    uint64_t data2 = 10010010012346;
    uint64_t data3 = 10010010012345;

    uint32_t hash1 = parcHash32_Int64(data1);
    uint32_t hash2 = parcHash32_Int64(data2);
    uint32_t hash3 = parcHash32_Int64(data3);

    assertTrue(hash1 != 0, "Hash is 0, unlikely");
    assertTrue(hash2 != 0, "Hash is 0, unlikely");
    assertTrue(hash3 != 0, "Hash is 0, unlikely");
    assertTrue(hash1 != hash2, "Hash collision, unlikely");
    assertTrue(hash1 == hash3, "Hash different for same content");
}

LONGBOW_TEST_CASE(Global, parc_Hash64_Data)
{
    char *data1 = "Hello World";
    char *data2 = "Hello World1";
    char *data3 = "Hello World2";

    char data4[20];
    strncpy(data4, data1, sizeof(data4));

    uint64_t hash1 = parcHash64_Data(data1, strlen(data1));
    uint64_t hash2 = parcHash64_Data(data2, strlen(data2));
    uint64_t hash3 = parcHash64_Data(data3, strlen(data3));
    uint64_t hash4 = parcHash64_Data(data4, strlen(data4));

    assertTrue(hash1 != 0, "Hash is 0, unlikely");
    assertTrue(hash2 != 0, "Hash is 0, unlikely");
    assertTrue(hash3 != 0, "Hash is 0, unlikely");
    assertTrue(hash4 != 0, "Hash is 0, unlikely");
    assertTrue(hash1 != hash2, "Hash collision, unlikely");
    assertTrue(hash3 != hash2, "Hash collision, unlikely");
    assertTrue(hash3 != hash1, "Hash collision, unlikely");
    assertTrue(hash1 == hash4, "Hash different for same content");
}

LONGBOW_TEST_CASE(Global, parc_Hash64_Int32)
{
    uint32_t data1 = 12345;
    uint32_t data2 = 12346;
    uint32_t data3 = 12345;

    uint64_t hash1 = parcHash64_Int32(data1);
    uint64_t hash2 = parcHash64_Int32(data2);
    uint64_t hash3 = parcHash64_Int32(data3);

    assertTrue(hash1 != 0, "Hash is 0, unlikely");
    assertTrue(hash2 != 0, "Hash is 0, unlikely");
    assertTrue(hash3 != 0, "Hash is 0, unlikely");
    assertTrue(hash1 != hash2, "Hash collision, unlikely");
    assertTrue(hash1 == hash3, "Hash different for same content");
}

LONGBOW_TEST_CASE(Global, parc_Hash64_Int64)
{
    uint64_t data1 = 10010010012345;
    uint64_t data2 = 10010010012346;
    uint64_t data3 = 10010010012345;

    uint64_t hash1 = parcHash64_Int64(data1);
    uint64_t hash2 = parcHash64_Int64(data2);
    uint64_t hash3 = parcHash64_Int64(data3);

    assertTrue(hash1 != 0, "Hash is 0, unlikely");
    assertTrue(hash2 != 0, "Hash is 0, unlikely");
    assertTrue(hash3 != 0, "Hash is 0, unlikely");
    assertTrue(hash1 != hash2, "Hash collision, unlikely");
    assertTrue(hash1 == hash3, "Hash different for same content");
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
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_Hash);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
