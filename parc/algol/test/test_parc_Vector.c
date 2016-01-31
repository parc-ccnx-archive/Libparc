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
#include <LongBow/debugging.h>

#include <parc/algol/parc_SafeMemory.h>

#include "../parc_Vector.c"

LONGBOW_TEST_RUNNER(PARCVector)
{
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

LONGBOW_TEST_RUNNER_SETUP(PARCVector)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_RUNNER_TEARDOWN(PARCVector)
{
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDERR_FILENO);
    if (outstandingAllocations != 0) {
        printf("Tests leak memory by %d allocations\n", outstandingAllocations);
        exit(1);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcVectorDestroy);
    LONGBOW_RUN_TEST_CASE(Global, parcVectorGetLength);
    LONGBOW_RUN_TEST_CASE(Global, parcVectorGetPointer);
    LONGBOW_RUN_TEST_CASE(Global, parcVectorCreate);
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

LONGBOW_TEST_CASE(Global, parcVectorDestroy)
{
    const char *string = "Hello World";

    PARCVector *x = parcVector_Create((void *) string, strlen(string));
    parcVector_Destroy(&x);
    assertNull(x, "Destroy did not nullify.");
}

LONGBOW_TEST_CASE(Global, parcVectorGetLength)
{
    const char *string = "Hello World";

    PARCVector *x = parcVector_Create((void *) string, strlen(string));
    size_t expected = strlen(string);
    size_t actual = parcVector_GetLength(x);
    assertEqual(expected, actual, "%zd");
    parcVector_Destroy(&x);
}

LONGBOW_TEST_CASE(Global, parcVectorGetPointer)
{
    const char *expected = "Hello World";

    PARCVector *x = parcVector_Create((void *) expected, strlen(expected));
    const char *actual = parcVector_GetPointer(x);

    assertEqual((void *)expected, (void *)actual, "%p");

    parcVector_Destroy(&x);
}

LONGBOW_TEST_CASE(Global, parcVectorCreate)
{
    const char *string = "Hello World";

    PARCVector *x = parcVector_Create((void *) string, strlen(string));
    assertNotNull(x, "Probably out of memory.");
    parcVector_Destroy(&x);
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(PARCVector);
    int status = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(status);
}
