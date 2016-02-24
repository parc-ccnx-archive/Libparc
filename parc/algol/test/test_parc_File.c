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
#include "../parc_File.c"

#define PATH_SEGMENT "A"

LONGBOW_TEST_RUNNER(parc_File)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Local);
    LONGBOW_RUN_TEST_FIXTURE(AcquireRelease);
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_File)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_File)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(AcquireRelease)
{
    LONGBOW_RUN_TEST_CASE(AcquireRelease, parcFile_AcquireRelease);
}

LONGBOW_TEST_FIXTURE_SETUP(AcquireRelease)
{
    longBowClipBoard_SetInt(testClipBoard, "initalAllocations", parcMemory_Outstanding());
    
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(AcquireRelease)
{
    uint64_t initialAllocations = longBowClipBoard_GetAsInt(testClipBoard, "initalAllocations");

    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDOUT_FILENO);
    if (initialAllocations < outstandingAllocations) {
        printf("%s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(AcquireRelease, parcFile_AcquireRelease)
{
    char *root = "/tmp/test_parc_File";

    PARCFile *file = parcFile_Create(root);

    PARCFile *reference = parcFile_Acquire(file);

    parcFile_Release(&reference);

    parcFile_AssertValid(file);

    parcFile_Release(&file);
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcFile_CreateChild);
    LONGBOW_RUN_TEST_CASE(Global, parcFile_CreateDeleteNewFile);
    LONGBOW_RUN_TEST_CASE(Global, parcFile_CreateDelete_Directory);
    LONGBOW_RUN_TEST_CASE(Global, parcFile_Exists);
    LONGBOW_RUN_TEST_CASE(Global, parcFile_CreateHome);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    longBowClipBoard_SetInt(testClipBoard, "initalAllocations", parcMemory_Outstanding());
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    uint64_t initialAllocations = longBowClipBoard_GetAsInt(testClipBoard, "initalAllocations");
    
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDOUT_FILENO);
    if (initialAllocations < outstandingAllocations) {
        printf("%s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, parcFile_Exists)
{
    char *root = "/tmp";

    PARCFile *parent = parcFile_Create(root);

    char *child = "foo";
    PARCFile *file = parcFile_CreateChild(parent, child);

    parcFile_CreateNewFile(file);

    bool actual = parcFile_Exists(file);

    assertTrue(actual, "Expected the file to exist.");

    parcFile_Release(&file);
    parcFile_Release(&parent);
}

LONGBOW_TEST_CASE(Global, parcFile_CreateChild)
{
    char *root = "/tmp";

    PARCFile *parent = parcFile_Create(root);

    char *child = "foo";
    PARCFile *file = parcFile_CreateChild(parent, child);

    char *actual = parcFile_ToString(file);

    assertTrue(strcmp("/tmp/foo", actual) == 0,
               "Expected %s, actual %s", "/tmp/foo", actual);

    parcMemory_Deallocate((void **) &actual);
    parcFile_Release(&file);
    parcFile_Release(&parent);
}

LONGBOW_TEST_CASE(Global, parcFile_CreateDeleteNewFile)
{
    char *name = "/tmp/test_parc_File";

    PARCFile *file = parcFile_Create(name);

    parcFile_CreateNewFile(file);

    bool actual = parcFile_Delete(file);
    assertTrue(actual, "Expected parcFile_Delete to return true.");

    parcFile_Release(&file);
}

LONGBOW_TEST_CASE(Global, parcFile_CreateDelete_Directory)
{
    char *name = "/tmp/test_parc_File_directory";

    PARCFile *directory = parcFile_Create(name);

    parcFile_Mkdir(directory);

    char *fileName = "foo";
    PARCFile *file = parcFile_CreateChild(directory, fileName);

    bool success = parcFile_CreateNewFile(file);
    assertTrue(success, "Expected parcFile_CreateNewFile success");

    bool actual = parcFile_Delete(directory);
    assertTrue(actual, "Expected parcFile_Delete to return true.");

    parcFile_Release(&file);
    parcFile_Release(&directory);
}

LONGBOW_TEST_CASE(Global, parcFile_CreateHome)
{
    char *expected = "/tmp";
    
    setenv("HOME", expected, 1);
    
    PARCFile *file = parcFile_CreateHome();
    
    char *actual = parcFile_ToString(file);
    
    assertTrue(strcmp(expected, actual) == 0, "Expected %s, actual %s", expected, actual);

    parcMemory_Deallocate(&actual);
    parcFile_Release(&file);
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
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_File);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
