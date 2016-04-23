/*
 * Copyright (c) 2016, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @author <#__FULLUSERNAME___#>, Palo Alto Research Center (PARC)
 * @copyright 2016, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include "../parc_BufferPool.c"

#include <LongBow/testing.h>
#include <LongBow/debugging.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_SafeMemory.h>
#include <parc/algol/parc_DisplayIndented.h>

#include <parc/testing/parc_MemoryTesting.h>
#include <parc/testing/parc_ObjectTesting.h>

LONGBOW_TEST_RUNNER(parc_BufferPool)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(CreateAcquireRelease);
    LONGBOW_RUN_TEST_FIXTURE(Object);
    LONGBOW_RUN_TEST_FIXTURE(Specialization);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_BufferPool)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_BufferPool)
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
    PARCBufferPool *instance = parcBufferPool_Create();
    assertNotNull(instance, "Expected non-null result from parcBufferPool_Create();");

    parcObjectTesting_AssertAcquireReleaseContract(parcBufferPool_Acquire, instance);
    
    parcBufferPool_Release(&instance);
    assertNull(instance, "Expected null result from parcBufferPool_Release();");
}

LONGBOW_TEST_FIXTURE(Object)
{
    LONGBOW_RUN_TEST_CASE(Object, parcBufferPool_Compare);
    LONGBOW_RUN_TEST_CASE(Object, parcBufferPool_Copy);
    LONGBOW_RUN_TEST_CASE(Object, parcBufferPool_Display);
    LONGBOW_RUN_TEST_CASE(Object, parcBufferPool_Equals);
    LONGBOW_RUN_TEST_CASE(Object, parcBufferPool_HashCode);
    LONGBOW_RUN_TEST_CASE(Object, parcBufferPool_IsValid);
    LONGBOW_RUN_TEST_CASE(Object, parcBufferPool_ToJSON);
    LONGBOW_RUN_TEST_CASE(Object, parcBufferPool_ToString);
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

LONGBOW_TEST_CASE(Object,  parcBufferPool_Compare)
{
    testUnimplemented("");

}

LONGBOW_TEST_CASE(Object, parcBufferPool_Copy)
{
    PARCBufferPool *instance = parcBufferPool_Create();
    PARCBufferPool *copy = parcBufferPool_Copy(instance);
    assertTrue(parcBufferPool_Equals(instance, copy), "Expected the copy to be equal to the original");

    parcBufferPool_Release(&instance);
    parcBufferPool_Release(&copy);
}

LONGBOW_TEST_CASE(Object, parcBufferPool_Display)
{
    PARCBufferPool *instance = parcBufferPool_Create();
    parcBufferPool_Display(instance, 0);
    parcBufferPool_Release(&instance);
}

LONGBOW_TEST_CASE(Object, parcBufferPool_Equals)
{
    PARCBufferPool *x = parcBufferPool_Create();
    PARCBufferPool *y = parcBufferPool_Create();
    PARCBufferPool *z = parcBufferPool_Create();

    parcObjectTesting_AssertEquals(x, y, z, NULL);

    parcBufferPool_Release(&x);
    parcBufferPool_Release(&y);
    parcBufferPool_Release(&z);
}

LONGBOW_TEST_CASE(Object, parcBufferPool_HashCode)
{
    PARCBufferPool *x = parcBufferPool_Create();
    PARCBufferPool *y = parcBufferPool_Create();
    
    parcObjectTesting_AssertHashCode(x, y);
    
    parcBufferPool_Release(&x);
    parcBufferPool_Release(&y);
}

LONGBOW_TEST_CASE(Object, parcBufferPool_IsValid)
{
    PARCBufferPool *instance = parcBufferPool_Create();
    assertTrue(parcBufferPool_IsValid(instance), "Expected parcBufferPool_Create to result in a valid instance.");
    
    parcBufferPool_Release(&instance);
    assertFalse(parcBufferPool_IsValid(instance), "Expected parcBufferPool_Release to result in an invalid instance.");
}

LONGBOW_TEST_CASE(Object, parcBufferPool_ToJSON)
{
    PARCBufferPool *instance = parcBufferPool_Create();
    
    PARCJSON *json = parcBufferPool_ToJSON(instance);

    parcJSON_Release(&json);

    parcBufferPool_Release(&instance);
}

LONGBOW_TEST_CASE(Object, parcBufferPool_ToString)
{
    PARCBufferPool *instance = parcBufferPool_Create();
    
    char *string = parcBufferPool_ToString(instance);
    
    assertNotNull(string, "Expected non-NULL result from parcBufferPool_ToString");
    
    parcMemory_Deallocate((void **) &string);
    parcBufferPool_Release(&instance);
}

LONGBOW_TEST_FIXTURE(Specialization)
{
}

LONGBOW_TEST_FIXTURE_SETUP(Specialization)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Specialization)
{
    if (!parcMemoryTesting_ExpectedOutstanding(0, "%s mismanaged memory.", longBowTestCase_GetFullName(testCase))) {
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    
    return LONGBOW_STATUS_SUCCEEDED;
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_BufferPool);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}


