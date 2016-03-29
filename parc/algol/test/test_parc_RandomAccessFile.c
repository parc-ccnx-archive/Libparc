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
/**
 * @author Christopher A. Wood, Palo Alto Research Center (Xerox PARC)
 * @copyright 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include "../parc_RandomAccessFile.c"

#include <LongBow/testing.h>
#include <LongBow/debugging.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_SafeMemory.h>
#include <parc/algol/parc_DisplayIndented.h>

#include <parc/testing/parc_MemoryTesting.h>
#include <parc/testing/parc_ObjectTesting.h>

LONGBOW_TEST_RUNNER(parcRandomAccessFile_RandomAccessFile)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(CreateAcquireRelease);
    LONGBOW_RUN_TEST_FIXTURE(Object);
    LONGBOW_RUN_TEST_FIXTURE(Specialization);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parcRandomAccessFile_RandomAccessFile)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parcRandomAccessFile_RandomAccessFile)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(CreateAcquireRelease)
{
    LONGBOW_RUN_TEST_CASE(CreateAcquireRelease, CreateRelease);
}

LONGBOW_TEST_FIXTURE_SETUP(CreateAcquireRelease)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(CreateAcquireRelease)
{
    if (!parcMemoryTesting_ExpectedOutstanding(0, "%s leaked memory.", longBowTestCase_GetFullName(testCase))) {
        return LONGBOW_STATUS_MEMORYLEAK;
    }

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(CreateAcquireRelease, CreateRelease)
{
    PARCFile *file = parcFile_Create("/tmp/tmpfile");
    PARCRandomAccessFile *instance = parcRandomAccessFile_Open(file);
    assertNotNull(instance, "Expected non-null result from parcRandomAccessFile_Open();");

    parcObjectTesting_AssertAcquireReleaseContract(parcRandomAccessFile_Acquire, instance);

    parcRandomAccessFile_Release(&instance);
    assertNull(instance, "Expected null result from parcRandomAccessFile_Release();");

    parcFile_Release(&file);
}

LONGBOW_TEST_FIXTURE(Object)
{
    LONGBOW_RUN_TEST_CASE(Object, parcRandomAccessFile_Display);
	// XXX: Disable this test until fixed
    //LONGBOW_RUN_TEST_CASE(Object, parcRandomAccessFile_Equals);
    LONGBOW_RUN_TEST_CASE(Object, parcRandomAccessFile_IsValid);
    LONGBOW_RUN_TEST_CASE(Object, parcRandomAccessFile_ToJSON);
    LONGBOW_RUN_TEST_CASE(Object, parcRandomAccessFile_ToString);
}

LONGBOW_TEST_FIXTURE_SETUP(Object)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Object)
{
    if (!parcMemoryTesting_ExpectedOutstanding(0, "%s mismanaged memory.", longBowTestCase_GetFullName(testCase))) {
        return LONGBOW_STATUS_MEMORYLEAK;
    }

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Object, parcRandomAccessFile_Display)
{
    PARCFile *file = parcFile_Create("/tmp/tmpfile");
    PARCRandomAccessFile *instance = parcRandomAccessFile_Open(file);
    parcFile_Release(&file);

    parcRandomAccessFile_Display(instance, 0);
    parcRandomAccessFile_Release(&instance);
}

LONGBOW_TEST_CASE(Object, parcRandomAccessFile_Equals)
{
    PARCFile *fileX = parcFile_Create("/tmp/tmpfileX");
    PARCFile *fileY = parcFile_Create("/tmp/tmpfileX");
    PARCFile *fileZ = parcFile_Create("/tmp/tmpfileX");

    PARCRandomAccessFile *x = parcRandomAccessFile_Open(fileX);
    PARCRandomAccessFile *y = parcRandomAccessFile_Open(fileY);
    PARCRandomAccessFile *z = parcRandomAccessFile_Open(fileZ);
    parcFile_Release(&fileX);
    parcFile_Release(&fileY);
    parcFile_Release(&fileZ);

    parcObjectTesting_AssertEquals(x, y, z, NULL);

    parcRandomAccessFile_Close(x);
    parcRandomAccessFile_Close(y);
    parcRandomAccessFile_Close(z);

    parcRandomAccessFile_Release(&x);
    parcRandomAccessFile_Release(&y);
    parcRandomAccessFile_Release(&z);
}

LONGBOW_TEST_CASE(Object, parcRandomAccessFile_IsValid)
{
    PARCFile *file = parcFile_Create("/tmp/tmpfile");
    parcFile_CreateNewFile(file);

    PARCRandomAccessFile *instance = parcRandomAccessFile_Open(file);
    parcFile_Release(&file);
    assertTrue(parcRandomAccessFile_IsValid(instance), "Expected parcRandomAccessFile_Create to result in a valid instance.");

    parcRandomAccessFile_Release(&instance);
    assertFalse(parcRandomAccessFile_IsValid(instance), "Expected parcRandomAccessFile_Release to result in an invalid instance.");
}

LONGBOW_TEST_CASE(Object, parcRandomAccessFile_ToJSON)
{
    PARCFile *file = parcFile_Create("/tmp/tmpfile");
    PARCRandomAccessFile *instance = parcRandomAccessFile_Open(file);
    parcFile_Release(&file);

    PARCJSON *json = parcRandomAccessFile_ToJSON(instance);

    const PARCJSONPair *pair = parcJSON_GetPairByName(json, "fname");
    PARCJSONValue *value = parcJSONPair_GetValue(pair);
    PARCBuffer *buffer = parcJSONValue_GetString(value);

    char *string = parcBuffer_ToString(buffer);
    assertTrue(strcmp("/tmp/tmpfile", string) == 0, "The file was stored correctly");

    parcMemory_Deallocate(&string);

    parcJSON_Release(&json);

    parcRandomAccessFile_Release(&instance);
}

LONGBOW_TEST_CASE(Object, parcRandomAccessFile_ToString)
{
    PARCFile *file = parcFile_Create("/tmp/tmpfile");
    PARCRandomAccessFile *instance = parcRandomAccessFile_Open(file);
    parcFile_Release(&file);

    char *string = parcRandomAccessFile_ToString(instance);

    assertNotNull(string, "Expected non-NULL result from parcRandomAccessFile_ToString");

    parcMemory_Deallocate((void **) &string);
    parcRandomAccessFile_Release(&instance);
}

LONGBOW_TEST_FIXTURE(Specialization)
{
    LONGBOW_RUN_TEST_CASE(Object, parcRandomAccessFile_Read);
    LONGBOW_RUN_TEST_CASE(Object, parcRandomAccessFile_Write);
    LONGBOW_RUN_TEST_CASE(Object, parcRandomAccessFile_Seek);
}

LONGBOW_TEST_FIXTURE_SETUP(Specialization)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Specialization)
{
    if (!parcMemoryTesting_ExpectedOutstanding(0, "%s mismanaged memory.", longBowTestCase_GetFullName(testCase))) {
        parcSafeMemory_ReportAllocation(STDOUT_FILENO);
        return LONGBOW_STATUS_MEMORYLEAK;
    }

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Object, parcRandomAccessFile_Read)
{
    char *fname = "tmpfile";

    PARCFile *file = parcFile_Create(fname);

    parcFile_CreateNewFile(file);
    FILE *fp = fopen(fname, "w");
    fseek(fp, 0, SEEK_SET);

    uint8_t data[128];
    for (int i = 0; i < 128; i++) {
        data[i] = i;
    }
    fwrite(data, 1, 128, fp);
    fclose(fp);

    PARCRandomAccessFile *instance = parcRandomAccessFile_Open(file);
    parcFile_Release(&file);

    PARCBuffer *buffer = parcBuffer_Allocate(128);
    size_t numBytes = parcRandomAccessFile_Read(instance, buffer);
    assertTrue(numBytes == 128, "Expected 128 bytes to be read, but got %zu", numBytes);

    parcBuffer_Flip(buffer);
    uint8_t *bytes = parcBuffer_Overlay(buffer, parcBuffer_Remaining(buffer));
    assertTrue(memcmp(data, bytes, 128) == 0, "Expected buffers to be equal");

    parcBuffer_Release(&buffer);
    parcRandomAccessFile_Close(instance);
    parcRandomAccessFile_Release(&instance);
}

LONGBOW_TEST_CASE(Object, parcRandomAccessFile_Write)
{
    char *fname = "tmpfile";

    PARCFile *file = parcFile_Create(fname);

    parcFile_CreateNewFile(file);

    uint8_t data[128];
    for (int i = 0; i < 128; i++) {
        data[i] = i;
    }

    PARCRandomAccessFile *instance = parcRandomAccessFile_Open(file);
    PARCBuffer *buffer = parcBuffer_Allocate(128);
    parcBuffer_PutArray(buffer, 128, data);
    parcBuffer_Flip(buffer);
    size_t numBytes = parcRandomAccessFile_Write(instance, buffer);
    assertTrue(numBytes == 128, "Expected 128 bytes to be read, but got %zu", numBytes);
    parcBuffer_Release(&buffer);

    parcRandomAccessFile_Close(instance);
    parcRandomAccessFile_Release(&instance);

    uint8_t bytes[128];
    FILE *fp = fopen(fname, "r");
    numBytes = fread(bytes, 1, 128, fp);
    assertTrue(numBytes == 128, "Expected 128 bytes to be read, but got %zu", numBytes);

    fclose(fp);

    assertTrue(memcmp(data, bytes, 128) == 0, "Expected buffers to be equal");

    parcFile_Release(&file);
}

LONGBOW_TEST_CASE(Object, parcRandomAccessFile_Seek)
{
    char *fname = "tmpfile";

    PARCFile *file = parcFile_Create(fname);

    parcFile_CreateNewFile(file);
    FILE *fp = fopen(fname, "w");
    fseek(fp, 0, SEEK_SET);

    uint8_t data[128];
    for (int i = 0; i < 128; i++) {
        data[i] = i;
    }
    fwrite(data, 1, 128, fp);
    fclose(fp);

    PARCRandomAccessFile *instance = parcRandomAccessFile_Open(file);
    PARCBuffer *buffer = parcBuffer_Allocate(128);
    parcRandomAccessFile_Seek(instance, 64, PARCRandomAccessFilePosition_Start);
    size_t numBytes = parcRandomAccessFile_Read(instance, buffer);
    assertTrue(numBytes == 64, "Expected 64 bytes to be read, but got %zu", numBytes);

    parcRandomAccessFile_Seek(instance, 0, PARCRandomAccessFilePosition_End);
    parcBuffer_Flip(buffer);
    numBytes = parcRandomAccessFile_Read(instance, buffer);
    assertTrue(numBytes == 0, "Expected 0 bytes to be read, but got %zu", numBytes);

    parcRandomAccessFile_Seek(instance, 0, PARCRandomAccessFilePosition_Start);
    parcBuffer_Flip(buffer);
    numBytes = parcRandomAccessFile_Read(instance, buffer);
    assertTrue(numBytes == 128, "Expected 128 bytes to be read, but got %zu", numBytes);

    parcBuffer_Release(&buffer);
    parcRandomAccessFile_Close(instance);
    parcRandomAccessFile_Release(&instance);

    parcFile_Release(&file);
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parcRandomAccessFile_RandomAccessFile);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}


