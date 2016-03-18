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
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include "../parc_Stopwatch.c"

#include <LongBow/testing.h>
#include <LongBow/debugging.h>

#include <stdio.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_SafeMemory.h>
#include <parc/algol/parc_DisplayIndented.h>

#include <parc/testing/parc_MemoryTesting.h>
#include <parc/testing/parc_ObjectTesting.h>

LONGBOW_TEST_RUNNER(parc_Timer)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(CreateAcquireRelease);
    LONGBOW_RUN_TEST_FIXTURE(Object);
    LONGBOW_RUN_TEST_FIXTURE(Specialization);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_Timer)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_Timer)
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
    PARCStopwatch *instance = parcStopwatch_Create();
    assertNotNull(instance, "Expected non-null result from parcStopwatch_Create();");

    parcObjectTesting_AssertAcquireReleaseContract(parcStopwatch_Acquire, instance);

    parcStopwatch_Release(&instance);
    assertNull(instance, "Expected null result from parcStopwatch_Release();");
}

LONGBOW_TEST_FIXTURE(Object)
{
    LONGBOW_RUN_TEST_CASE(Object, parcStopwatch_Compare);
    LONGBOW_RUN_TEST_CASE(Object, parcStopwatch_Copy);
    LONGBOW_RUN_TEST_CASE(Object, parcStopwatch_Display);
    LONGBOW_RUN_TEST_CASE(Object, parcStopwatch_Equals);
    LONGBOW_RUN_TEST_CASE(Object, parcStopwatch_HashCode);
    LONGBOW_RUN_TEST_CASE(Object, parcStopwatch_IsValid);
    LONGBOW_RUN_TEST_CASE(Object, parcStopwatch_ToJSON);
    LONGBOW_RUN_TEST_CASE(Object, parcStopwatch_ToString);
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

LONGBOW_TEST_CASE(Object,  parcStopwatch_Compare)
{
    testUnimplemented("");

}

LONGBOW_TEST_CASE(Object, parcStopwatch_Copy)
{
    PARCStopwatch *instance = parcStopwatch_Create();
    PARCStopwatch *copy = parcStopwatch_Copy(instance);
    assertTrue(parcStopwatch_Equals(instance, copy), "Expected the copy to be equal to the original");

    parcStopwatch_Release(&instance);
    parcStopwatch_Release(&copy);
}

LONGBOW_TEST_CASE(Object, parcStopwatch_Display)
{
    PARCStopwatch *instance = parcStopwatch_Create();
    parcStopwatch_Display(instance, 0);
    parcStopwatch_Release(&instance);
}

LONGBOW_TEST_CASE(Object, parcStopwatch_Equals)
{
    PARCStopwatch *x = parcStopwatch_Create();
    PARCStopwatch *y = parcStopwatch_Create();
    PARCStopwatch *z = parcStopwatch_Create();

    parcObjectTesting_AssertEquals(x, y, z, NULL);

    parcStopwatch_Release(&x);
    parcStopwatch_Release(&y);
    parcStopwatch_Release(&z);
}

LONGBOW_TEST_CASE(Object, parcStopwatch_HashCode)
{
    PARCStopwatch *x = parcStopwatch_Create();
    PARCStopwatch *y = parcStopwatch_Create();

    parcObjectTesting_AssertHashCode(x, y);

    parcStopwatch_Release(&x);
    parcStopwatch_Release(&y);
}

LONGBOW_TEST_CASE(Object, parcStopwatch_IsValid)
{
    PARCStopwatch *instance = parcStopwatch_Create();
    assertTrue(parcStopwatch_IsValid(instance), "Expected parcStopwatch_Create to result in a valid instance.");

    parcStopwatch_Release(&instance);
    assertFalse(parcStopwatch_IsValid(instance), "Expected parcStopwatch_Release to result in an invalid instance.");
}

LONGBOW_TEST_CASE(Object, parcStopwatch_ToJSON)
{
    PARCStopwatch *instance = parcStopwatch_Create();

    PARCJSON *json = parcStopwatch_ToJSON(instance);

    parcJSON_Release(&json);

    parcStopwatch_Release(&instance);
}

LONGBOW_TEST_CASE(Object, parcStopwatch_ToString)
{
    PARCStopwatch *instance = parcStopwatch_Create();

    char *string = parcStopwatch_ToString(instance);

    assertNotNull(string, "Expected non-NULL result from parcStopwatch_ToString");

    parcMemory_Deallocate((void **) &string);
    parcStopwatch_Release(&instance);
}

LONGBOW_TEST_FIXTURE(Specialization)
{
    LONGBOW_RUN_TEST_CASE(Specialization, parcStopwatch_Multi);
    LONGBOW_RUN_TEST_CASE(Specialization, parcStopwatch_ElapsedTimeNanos);
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

LONGBOW_TEST_CASE(Specialization, parcStopwatch_Multi)
{
    PARCStopwatch *a = parcStopwatch_Create();
    PARCStopwatch *b = parcStopwatch_Create();
    PARCStopwatch *c = parcStopwatch_Create();
    
    parcStopwatch_Start(a, b, c);
    sleep(2);
    uint64_t nanos = parcStopwatch_ElapsedTimeNanos(a);
    printf("%llu %llu\n", nanos, nanos/1000000000);
    if (nanos > (3000000000)) {
        parcStopwatch_Display(a, 0);
    }
    
    parcStopwatch_Release(&a);
    parcStopwatch_Release(&b);
    parcStopwatch_Release(&c);
}

LONGBOW_TEST_CASE(Specialization, parcStopwatch_ElapsedTimeNanos)
{
    PARCStopwatch *instance = parcStopwatch_Create();

    parcStopwatch_StartImpl(instance, NULL);
    sleep(2);
    uint64_t nanos = parcStopwatch_ElapsedTimeNanos(instance);
    printf("%llu %llu\n", nanos, nanos/1000000000);
    if (nanos > (3000000000)) {
        parcStopwatch_Display(instance, 0);
    }
    
    parcStopwatch_Release(&instance);
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_Timer);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}


