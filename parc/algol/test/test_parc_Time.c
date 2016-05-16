/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>

#include <LongBow/unit-test.h>
#include <LongBow/debugging.h>

#include <parc/algol/parc_SafeMemory.h>
#include <parc/algol/parc_Memory.h>

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../parc_Time.c"

LONGBOW_TEST_RUNNER(test_parc_Time)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(test_parc_Time)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(test_parc_Time)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcTime_TimevalAsString);
    LONGBOW_RUN_TEST_CASE(Global, parcTime_TimevalAsISO8601);
    LONGBOW_RUN_TEST_CASE(Global, parcTime_TimevalAsRFC3339);
    LONGBOW_RUN_TEST_CASE(Global, parcTime_RFC3339_Now);
    LONGBOW_RUN_TEST_CASE(Global, parcTime_NowTimeval);
    LONGBOW_RUN_TEST_CASE(Global, parcTime_NowMicroseconds);
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

LONGBOW_TEST_CASE(Global, parcTime_TimevalAsString)
{
    struct timeval timeval;
    timeval.tv_sec = 0;
    timeval.tv_usec = 1000;

    char *expected = "0.001000";
    char *actual = parcTime_TimevalAsString(timeval);
    assertTrue(strcmp(expected, actual) == 0, "Expected %s, actual %s", expected, actual);
    parcMemory_Deallocate((void **) &actual);
}

LONGBOW_TEST_CASE(Global, parcTime_TimevalAsISO8601)
{
    struct timeval timeval;
    timeval.tv_sec = 0;
    timeval.tv_usec = 1000;

    char *expected = "1970-01-01 00:00:00.001000Z";

    char actual[64];
    parcTime_TimevalAsISO8601(&timeval, actual);

    assertTrue(strcmp(expected, actual) == 0, "Expected %s, actual %s", expected, actual);
}

LONGBOW_TEST_CASE(Global, parcTime_TimevalAsRFC3339)
{
    struct timeval timeval;
    timeval.tv_sec = 0;
    timeval.tv_usec = 1000;

    char *expected = "1970-01-01T00:00:00.001000Z";

    char actual[64];
    parcTime_TimevalAsRFC3339(&timeval, actual);

    assertTrue(strcmp(expected, actual) == 0, "Expected %s, actual %s", expected, actual);
}

LONGBOW_TEST_CASE(Global, parcTime_RFC3339_Now)
{

    struct timeval timeval;
    gettimeofday(&timeval, NULL);

    char actual[64];
    parcTime_TimevalAsRFC3339(&timeval, actual);
    printf("%s\n", actual);
}

LONGBOW_TEST_CASE(Global, parcTime_NowTimeval)
{
    struct timeval result = parcTime_NowTimeval();
    assertTrue(result.tv_sec != 0, "Expected NOW to not be zero.");
}

LONGBOW_TEST_CASE(Global, parcTime_NowMicroseconds)
{
    uint64_t result = parcTime_NowMicroseconds();
    assertTrue(result != 0, "Expected NOW to not be zero.");
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(test_parc_Time);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
