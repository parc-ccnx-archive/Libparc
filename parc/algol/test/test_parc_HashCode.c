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
/** *
 * @author Glenn Scott <Glenn.Scott@parc.com>, Palo Alto Research Center (Xerox PARC)
 * @copyright 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../parc_HashCode.c"

#include <LongBow/testing.h>
#include <LongBow/debugging.h>

LONGBOW_TEST_RUNNER(test_parc_HashCode)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(test_parc_HashCode)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(test_parc_HashCode)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcHashCode_HashImpl);
    LONGBOW_RUN_TEST_CASE(Global, parcHashCode_HashHashCode);
    LONGBOW_RUN_TEST_CASE(Global, parcHashCode_Hash);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, parcHashCode_HashImpl)
{
    PARCHashCode lastValue = 0;
    uint8_t *memory = (uint8_t *) "1234";
    size_t length = 4;

    PARCHashCode expected = 3316911679945239212ULL;
    PARCHashCode actual = parcHashCode_HashImpl(memory, length, lastValue);

    assertTrue(expected == actual, "Expected %" PRIPARCHashCode " actual %" PRIPARCHashCode, expected, actual);
}

LONGBOW_TEST_CASE(Global, parcHashCode_HashHashCode)
{
    PARCHashCode lastValue = 0;
    uint8_t *memory = (uint8_t *) "1234";
    size_t length = 4;

    PARCHashCode lastHash = parcHashCode_HashImpl(memory, length, lastValue);

    PARCHashCode hashedHash = parcHashCode_HashHashCode(lastHash, 123456);

    assertTrue(lastHash != 0, "Expected a non zero hash value for lastHash");
    assertTrue(hashedHash != 0, "Expected a non zero hash value for hashedHash");
    assertTrue(lastHash != hashedHash, "Expected different hash values for hashedHash and lastHash");
}

LONGBOW_TEST_CASE(Global, parcHashCode_Hash)
{
    char *testString1 = "this is some test data";
    char *testString2 = "this is different test data";

    PARCHashCode hash1 = parcHashCode_Hash((uint8_t *) testString1, strlen(testString1));
    PARCHashCode hash2 = parcHashCode_Hash((uint8_t *) testString2, strlen(testString2));

    assertTrue(hash1 != 0, "Expected a non zero hash value for testString1");
    assertTrue(hash2 != 0, "Expected a non zero hash value for testString2");
    assertTrue(hash1 != hash2, "Expected different hash values for testString1 and testString2");
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(test_parc_HashCode);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
