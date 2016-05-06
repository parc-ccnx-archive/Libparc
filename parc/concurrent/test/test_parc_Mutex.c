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
#include "../parc_Mutex.c"

#include <LongBow/testing.h>
#include <LongBow/debugging.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_SafeMemory.h>
#include <parc/algol/parc_DisplayIndented.h>

#include <parc/testing/parc_MemoryTesting.h>
#include <parc/testing/parc_ObjectTesting.h>

LONGBOW_TEST_RUNNER(parc_Mutex)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(CreateAcquireRelease);
    LONGBOW_RUN_TEST_FIXTURE(Object);
    LONGBOW_RUN_TEST_FIXTURE(Specialization);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_Mutex)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_Mutex)
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
    PARCMutex *instance = parcMutex_Create();
    assertNotNull(instance, "Expected non-null result from parcMutex_Create();");

    parcObjectTesting_AssertAcquireReleaseContract(parcMutex_Acquire, instance);
    
    parcMutex_Release(&instance);
    assertNull(instance, "Expected null result from parcMutex_Release();");
}

LONGBOW_TEST_FIXTURE(Object)
{
    LONGBOW_RUN_TEST_CASE(Object, parcMutex_Compare);
    LONGBOW_RUN_TEST_CASE(Object, parcMutex_Copy);
    LONGBOW_RUN_TEST_CASE(Object, parcMutex_Display);
    LONGBOW_RUN_TEST_CASE(Object, parcMutex_Equals);
    LONGBOW_RUN_TEST_CASE(Object, parcMutex_HashCode);
    LONGBOW_RUN_TEST_CASE(Object, parcMutex_IsValid);
    LONGBOW_RUN_TEST_CASE(Object, parcMutex_ToJSON);
    LONGBOW_RUN_TEST_CASE(Object, parcMutex_ToString);
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

LONGBOW_TEST_CASE(Object,  parcMutex_Compare)
{
    testUnimplemented("");

}

LONGBOW_TEST_CASE(Object, parcMutex_Copy)
{
    PARCMutex *instance = parcMutex_Create();
    PARCMutex *copy = parcMutex_Copy(instance);
    assertTrue(parcMutex_Equals(instance, copy), "Expected the copy to be equal to the original");

    parcMutex_Release(&instance);
    parcMutex_Release(&copy);
}

LONGBOW_TEST_CASE(Object, parcMutex_Display)
{
    PARCMutex *instance = parcMutex_Create();
    parcMutex_Display(instance, 0);
    parcMutex_Release(&instance);
}

LONGBOW_TEST_CASE(Object, parcMutex_Equals)
{
    PARCMutex *x = parcMutex_Create();
    PARCMutex *y = parcMutex_Create();
    PARCMutex *z = parcMutex_Create();

    parcObjectTesting_AssertEquals(x, y, z, NULL);

    parcMutex_Release(&x);
    parcMutex_Release(&y);
    parcMutex_Release(&z);
}

LONGBOW_TEST_CASE(Object, parcMutex_HashCode)
{
    PARCMutex *x = parcMutex_Create();
    PARCMutex *y = parcMutex_Create();
    
    parcObjectTesting_AssertHashCode(x, y);
    
    parcMutex_Release(&x);
    parcMutex_Release(&y);
}

LONGBOW_TEST_CASE(Object, parcMutex_IsValid)
{
    PARCMutex *instance = parcMutex_Create();
    assertTrue(parcMutex_IsValid(instance), "Expected parcMutex_Create to result in a valid instance.");
    
    parcMutex_Release(&instance);
    assertFalse(parcMutex_IsValid(instance), "Expected parcMutex_Release to result in an invalid instance.");
}

LONGBOW_TEST_CASE(Object, parcMutex_ToJSON)
{
    PARCMutex *instance = parcMutex_Create();
    
    PARCJSON *json = parcMutex_ToJSON(instance);

    parcJSON_Release(&json);

    parcMutex_Release(&instance);
}

LONGBOW_TEST_CASE(Object, parcMutex_ToString)
{
    PARCMutex *instance = parcMutex_Create();
    
    char *string = parcMutex_ToString(instance);
    
    assertNotNull(string, "Expected non-NULL result from parcMutex_ToString");
    
    parcMemory_Deallocate((void **) &string);
    parcMutex_Release(&instance);
}

LONGBOW_TEST_FIXTURE(Specialization)
{
    LONGBOW_RUN_TEST_CASE(Specialization, parcMutex_Example);
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

LONGBOW_TEST_CASE(Specialization, parcMutex_Example)
{
    
#define parcMutex_Synchronize(_object_) for (bool once = true; once && parcObject_Lock(_object_); parcObject_Unlock(_object_), once = false)
    
    PARCMutex *instance = parcMutex_Create();
    
    //        parcMutex_Synchronize(object) {
    //
    //        }
    PARCAtomicBool *atomic = parcAtomicBool_Create(false);
    
    parcAtomicBool_Toggle(atomic, true);
    
    parcAtomicBool_Toggle(atomic, false);
    
    parcMutex_Release(&instance);
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_Mutex);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}


