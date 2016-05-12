/*
 * Copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
#include "../parc_JSONArray.c"

#include <LongBow/unit-test.h>
#include <stdio.h>

#include "../parc_List.h"
#include "../parc_ArrayList.h"
#include "../parc_SafeMemory.h"
#include "../parc_Memory.h"
#include <parc/testing/parc_ObjectTesting.h>

LONGBOW_TEST_RUNNER(parc_JSONArray)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Static);
    LONGBOW_RUN_TEST_FIXTURE(parc_JSONArray);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_JSONArray)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_JSONArray)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(parc_JSONArray)
{
    LONGBOW_RUN_TEST_CASE(parc_JSONArray, parcJSONArray_CreateRelease);
    LONGBOW_RUN_TEST_CASE(parc_JSONArray, parcJSONArray_Equals);
    LONGBOW_RUN_TEST_CASE(parc_JSONArray, parcJSONArray_AddValue);
    LONGBOW_RUN_TEST_CASE(parc_JSONArray, parcJSONArray_GetLength);
    LONGBOW_RUN_TEST_CASE(parc_JSONArray, parcJSONArray_GetValue);
    LONGBOW_RUN_TEST_CASE(parc_JSONArray, parcJSONArray_BuildString);
    LONGBOW_RUN_TEST_CASE(parc_JSONArray, parcJSONArray_ToString);
    LONGBOW_RUN_TEST_CASE(parc_JSONArray, parcJSONArray_Display);
}

LONGBOW_TEST_FIXTURE_SETUP(parc_JSONArray)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(parc_JSONArray)
{
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDOUT_FILENO);
    if (outstandingAllocations != 0) {
        printf("Errors %s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(parc_JSONArray, parcJSONArray_CreateRelease)
{
    PARCJSONArray *expected = parcJSONArray_Create();
    parcJSONArray_AssertValid(expected);
    assertNotNull(expected, "Expected non-null return value from parcJSONArray_Create");

    PARCJSONArray *actual = parcJSONArray_Acquire(expected);
    parcJSONArray_AssertValid(actual);

    parcJSONArray_Release(&actual);
    assertNull(actual, "Expected null value set by parcJSONArray_Release");
    parcJSONArray_AssertValid(expected);

    parcJSONArray_Release(&expected);
    assertNull(expected, "Expected null value set by parcJSONArray_Release");
}

LONGBOW_TEST_CASE(parc_JSONArray, parcJSONArray_Equals)
{
    PARCJSONArray *x = parcJSONArray_Create();
    PARCJSONArray *y = parcJSONArray_Create();
    PARCJSONArray *z = parcJSONArray_Create();

    PARCJSONArray *notEqual1 = parcJSONArray_Create();
    PARCJSONValue *value = parcJSONValue_CreateFromCString("Hello");
    parcJSONArray_AddValue(notEqual1, value);
    parcJSONValue_Release(&value);

    parcObjectTesting_AssertEqualsFunction(parcJSONArray_Equals, x, y, z, notEqual1);

    parcJSONArray_Release(&x);
    parcJSONArray_Release(&y);
    parcJSONArray_Release(&z);
    parcJSONArray_Release(&notEqual1);
}

LONGBOW_TEST_CASE(parc_JSONArray, parcJSONArray_AddValue)
{
    PARCJSONArray *expected = parcJSONArray_Create();
    PARCJSONValue *value = parcJSONValue_CreateFromInteger(10);
    parcJSONArray_AddValue(expected, value);
    parcJSONValue_Release(&value);

    parcJSONArray_Release(&expected);
}

LONGBOW_TEST_CASE(parc_JSONArray, parcJSONArray_GetLength)
{
    PARCJSONArray *expected = parcJSONArray_Create();
    PARCJSONValue *value = parcJSONValue_CreateFromInteger(10);
    parcJSONArray_AddValue(expected, value);
    parcJSONValue_Release(&value);
    assertTrue(parcJSONArray_GetLength(expected) == 1, "Expected a length of 1");

    parcJSONArray_Release(&expected);
}

LONGBOW_TEST_CASE(parc_JSONArray, parcJSONArray_GetValue)
{
    PARCJSONArray *array = parcJSONArray_Create();
    PARCJSONValue *expected = parcJSONValue_CreateFromInteger(10);
    parcJSONArray_AddValue(array, expected);

    PARCJSONValue *actual = parcJSONArray_GetValue(array, 0);

    assertTrue(expected == actual, "Expected different value");

    parcJSONValue_Release(&expected);
    parcJSONArray_Release(&array);
}

LONGBOW_TEST_CASE(parc_JSONArray, parcJSONArray_BuildString)
{
    PARCJSONArray *array = parcJSONArray_Create();
    PARCJSONValue *expected = parcJSONValue_CreateFromInteger(10);
    parcJSONArray_AddValue(array, expected);

    PARCBufferComposer *composer = parcBufferComposer_Create();
    parcJSONArray_BuildString(array, composer, false);

    PARCBuffer *tempBuffer = parcBufferComposer_ProduceBuffer(composer);
    parcBufferComposer_Release(&composer);
    char *result = parcBuffer_ToString(tempBuffer);
    parcBuffer_Release(&tempBuffer);

    assertTrue(strlen(result) > 0, "Expected non-empty string result");

    parcMemory_Deallocate((void **) &result);

    composer = parcBufferComposer_Create();
    parcJSONArray_BuildString(array, composer, true);
    tempBuffer = parcBufferComposer_ProduceBuffer(composer);
    parcBufferComposer_Release(&composer);
    result = parcBuffer_ToString(tempBuffer);
    parcBuffer_Release(&tempBuffer);

    assertTrue(strlen(result) > 0, "Expected non-empty string result");

    parcMemory_Deallocate((void **) &result);

    parcJSONValue_Release(&expected);
    parcJSONArray_Release(&array);
}

LONGBOW_TEST_CASE(parc_JSONArray, parcJSONArray_ToString)
{
    PARCJSONArray *array = parcJSONArray_Create();
    PARCJSONValue *expected = parcJSONValue_CreateFromInteger(10);
    parcJSONArray_AddValue(array, expected);
    parcJSONValue_Release(&expected);

    const char *string = parcJSONArray_ToString(array);

    parcMemory_Deallocate((void **) &string);

    parcJSONArray_Release(&array);
}

LONGBOW_TEST_CASE(parc_JSONArray, parcJSONArray_Display)
{
    PARCJSONArray *array = parcJSONArray_Create();
    PARCJSONValue *expected = parcJSONValue_CreateFromInteger(10);
    parcJSONArray_AddValue(array, expected);
    parcJSONValue_Release(&expected);

    parcJSONArray_Display(array, 0);

    parcJSONArray_Release(&array);
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
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_JSONArray);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
