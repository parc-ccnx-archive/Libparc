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

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Runner.
#include "../parc_LogLevel.c"

#include <LongBow/unit-test.h>

LONGBOW_TEST_RUNNER(parc_LogLevel)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified here, but every test must be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Static);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_LogLevel)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_LogLevel)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcLogLevel_Compare);
    LONGBOW_RUN_TEST_CASE(Global, parcLogLevel_Equals);
    LONGBOW_RUN_TEST_CASE(Global, parcLogLevel_ToString);
    LONGBOW_RUN_TEST_CASE(Global, parcLogLevel_ToString_All);
    LONGBOW_RUN_TEST_CASE(Global, parcLogLevel_ToString_Off);
    LONGBOW_RUN_TEST_CASE(Global, parcLogLevel_FromString_Debug);
    LONGBOW_RUN_TEST_CASE(Global, parcLogLevel_FromString_All);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, parcLogLevel_Compare)
{
    assertTrue(parcLogLevel_Compare(PARCLogLevel_Off, PARCLogLevel_All) < 0, "Expected PARCLogLevel_Off to be less that All");
}

LONGBOW_TEST_CASE(Global, parcLogLevel_Equals)
{
    assertTrue(parcLogLevel_Equals(PARCLogLevel_Emergency, PARCLogLevel_Emergency), "Expected equality");
    assertFalse(parcLogLevel_Equals(PARCLogLevel_Emergency, PARCLogLevel_Debug), "Expected inequality");
}

LONGBOW_TEST_CASE(Global, parcLogLevel_ToString)
{
    char *expected = "Debug";
    const char *actual = parcLogLevel_ToString(PARCLogLevel_Debug);

    assertTrue(strcmp(expected, actual) == 0, "Expected '%s', actual '%s'", expected, actual);
}

LONGBOW_TEST_CASE(Global, parcLogLevel_ToString_Off)
{
    char *expected = "Off";
    const char *actual = parcLogLevel_ToString(PARCLogLevel_Off);

    assertTrue(strcmp(expected, actual) == 0, "Expected '%s', actual '%s'", expected, actual);
}

LONGBOW_TEST_CASE(Global, parcLogLevel_ToString_All)
{
    char *expected = "All";
    const char *actual = parcLogLevel_ToString(PARCLogLevel_All);

    assertTrue(strcmp(expected, actual) == 0, "Expected '%s', actual '%s'", expected, actual);
}

LONGBOW_TEST_CASE(Global, parcLogLevel_FromString_Debug)
{
    PARCLogLevel expected = PARCLogLevel_Debug;
    PARCLogLevel actual = parcLogLevel_FromString("DEBUG");

    assertTrue(expected == actual, "Expected '%d', actual '%d'", expected, actual);
}

LONGBOW_TEST_CASE(Global, parcLogLevel_FromString_All)
{
    PARCLogLevel expected = PARCLogLevel_All;
    PARCLogLevel actual = parcLogLevel_FromString("AlL");

    assertTrue(expected == actual, "Expected '%d', actual '%d'", expected, actual);
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
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_LogLevel);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
