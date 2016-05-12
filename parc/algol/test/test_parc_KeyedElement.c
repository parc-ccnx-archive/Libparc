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
#include "../parc_KeyedElement.c"

#include <LongBow/unit-test.h>
#include <stdio.h>

#include <parc/algol/parc_SafeMemory.h>

LONGBOW_TEST_RUNNER(parc_KeyedElement)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Local);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_KeyedElement)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_KeyedElement)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcKeyedElement_CreateDestroy);
    LONGBOW_RUN_TEST_CASE(Global, parcKeyedElement_GetData);
    LONGBOW_RUN_TEST_CASE(Global, parcKeyedElement_GetKey);
    LONGBOW_RUN_TEST_CASE(Global, parcKeyedElement_SetData);
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

LONGBOW_TEST_CASE(Global, parcKeyedElement_CreateDestroy)
{
    char *key1 = "key001";
    size_t keylen1 = 7;
    PARCKeyedElement *keyedElement = parcKeyedElement_Create("Some data", key1, keylen1);
    parcKeyedElement_Destroy(&keyedElement);
}

LONGBOW_TEST_CASE(Global, parcKeyedElement_GetData)
{
    char *initial_data = "some data here";
    char *key1 = "key001";
    size_t keylen1 = 7;
    PARCKeyedElement *keyedElement = parcKeyedElement_Create(initial_data, key1, keylen1);
    char *data = parcKeyedElement_GetData(keyedElement);
    assertTrue(initial_data == data, "We got different data from element");
    parcKeyedElement_Destroy(&keyedElement);
}

LONGBOW_TEST_CASE(Global, parcKeyedElement_GetKey)
{
    char *initial_data = "some data here";
    char *key1 = "key001";
    size_t keylen1 = 7;
    PARCKeyedElement *keyedElement = parcKeyedElement_Create(initial_data, key1, keylen1);
    char *thekey = parcKeyedElement_GetKey(keyedElement);
    size_t thekeylen = parcKeyedElement_GetKeyLen(keyedElement);
    assertTrue(keylen1 == thekeylen, "We got different key size?");
    assertTrue(memcmp(key1, thekey, keylen1) == 0, "We got different keys?");
    parcKeyedElement_Destroy(&keyedElement);
}

LONGBOW_TEST_CASE(Global, parcKeyedElement_SetData)
{
    char *key1 = "key001";
    size_t keylen1 = 7;
    char *initial_data = "some data here";

    PARCKeyedElement *keyedElement = parcKeyedElement_Create("Hello World", key1, keylen1);

    parcKeyedElement_SetData(keyedElement, initial_data);
    char *data = parcKeyedElement_GetData(keyedElement);
    assertTrue(initial_data == data, "We got different data from element");
    parcKeyedElement_Destroy(&keyedElement);
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
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_KeyedElement);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
