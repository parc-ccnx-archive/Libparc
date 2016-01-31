/*
 * Copyright (c) 2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
#include <config.h>
#include <LongBow/unit-test.h>
#include <fcntl.h>

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../parc_SafeMemory.h"
#include "../parc_FileOutputStream.c"

#define PATH_SEGMENT "A"

LONGBOW_TEST_RUNNER(parc_FileOutputStream)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Local);
    LONGBOW_RUN_TEST_FIXTURE(AcquireRelease);
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_FileOutputStream)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_FileOutputStream)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(AcquireRelease)
{
    LONGBOW_RUN_TEST_CASE(AcquireRelease, parcFileOutputStream_Create);
    LONGBOW_RUN_TEST_CASE(AcquireRelease, parcFileOutputStream_Release);
    LONGBOW_RUN_TEST_CASE(AcquireRelease, parcFileOutputStream_AcquireRelease);
}

LONGBOW_TEST_FIXTURE_SETUP(AcquireRelease)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(AcquireRelease)
{
    unlink("/tmp/test_parc_FileOutputStream");
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDOUT_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(AcquireRelease, parcFileOutputStream_Create)
{
    PARCFileOutputStream *stream = parcFileOutputStream_Create(open("/tmp/test_parc_FileOutputStream", O_CREAT | O_WRONLY | O_TRUNC, 0600));
    assertNotNull(stream, "Expected a non-null pointer");

    parcFileOutputStream_Release(&stream);
    assertNull(stream, "Expected parcFileOutputStream_Release to null the pointer");
    unlink("/tmp/test_parc_FileOutputStream");
}

LONGBOW_TEST_CASE(AcquireRelease, parcFileOutputStream_Release)
{
    PARCFileOutputStream *stream = parcFileOutputStream_Create(open("/tmp/test_parc_FileOutputStream", O_CREAT | O_WRONLY | O_TRUNC, 0600));
    assertNotNull(stream, "Expected a non-null pointer");

    parcFileOutputStream_Release(&stream);
    assertNull(stream, "Expected parcFileOutputStream_Release to null the pointer");
    unlink("/tmp/test_parc_FileOutputStream");
}

LONGBOW_TEST_CASE(AcquireRelease, parcFileOutputStream_AcquireRelease)
{
    PARCFileOutputStream *stream = parcFileOutputStream_Create(open("/tmp/test_parc_FileOutputStream", O_CREAT | O_WRONLY | O_TRUNC, 0600));
    assertNotNull(stream, "Expected a non-null pointer");

    PARCFileOutputStream *reference = parcFileOutputStream_Acquire(stream);
    assertTrue(stream == reference, "Expected the reference to be equal to the original.");

    parcFileOutputStream_Release(&stream);
    assertNull(stream, "Expected parcFileOutputStream_Release to null the pointer");

    parcFileOutputStream_Release(&reference);
    unlink("/tmp/test_parc_FileOutputStream");
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcFileOutputStream_Write);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    unlink("/tmp/test_parc_FileOutputStream");

    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDOUT_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, parcFileOutputStream_Write)
{
    PARCFileOutputStream *stream =
        parcFileOutputStream_Create(open("/tmp/test_parc_FileOutputStream", O_CREAT | O_WRONLY | O_TRUNC, 0600));

    PARCBuffer *buffer = parcBuffer_Allocate(16 * 1024 * 1024);

    parcFileOutputStream_Write(stream, buffer);

    assertFalse(parcBuffer_HasRemaining(buffer), "Expected the buffer to be emtpy");

    parcBuffer_Release(&buffer);

    parcFileOutputStream_Release(&stream);
    unlink("/tmp/test_parc_FileOutputStream");
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
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_FileOutputStream);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
