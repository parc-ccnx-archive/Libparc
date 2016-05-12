/*
 * Copyright (c) 2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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

/**
 * @author Glenn Scott, Computing Science Laboratory, PARC
 * @copyright (c) 2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include "../parc_ScheduledThreadPool.c"

#include <LongBow/testing.h>
#include <LongBow/debugging.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_SafeMemory.h>
#include <parc/algol/parc_DisplayIndented.h>

#include <parc/testing/parc_MemoryTesting.h>
#include <parc/testing/parc_ObjectTesting.h>

LONGBOW_TEST_RUNNER(parc_ScheduledThreadPool)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(CreateAcquireRelease);
    LONGBOW_RUN_TEST_FIXTURE(Object);
    LONGBOW_RUN_TEST_FIXTURE(Specialization);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_ScheduledThreadPool)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_ScheduledThreadPool)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(CreateAcquireRelease)
{
    LONGBOW_RUN_TEST_CASE(CreateAcquireRelease, CreateRelease);
}

LONGBOW_TEST_FIXTURE_SETUP(CreateAcquireRelease)
{
    longBowTestCase_SetInt(testCase, "initalAllocations", parcMemory_Outstanding());
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(CreateAcquireRelease)
{
    int initialAllocations = longBowTestCase_GetInt(testCase, "initalAllocations");
    if (!parcMemoryTesting_ExpectedOutstanding(initialAllocations, "%s leaked memory.", longBowTestCase_GetFullName(testCase))) {
        parcSafeMemory_ReportAllocation(1);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(CreateAcquireRelease, CreateRelease)
{
    PARCScheduledThreadPool *instance = parcScheduledThreadPool_Create(3);
    assertNotNull(instance, "Expected non-null result from parcScheduledThreadPool_Create();");

    parcObjectTesting_AssertAcquireReleaseContract(parcScheduledThreadPool_Acquire, instance);
    parcScheduledThreadPool_ShutdownNow(instance);
    
    assertTrue(parcObject_GetReferenceCount(instance) == 1, "Expected 1 reference count. Actual %llu", parcObject_GetReferenceCount(instance) );

    parcScheduledThreadPool_Release(&instance);
    assertNull(instance, "Expected null result from parcScheduledThreadPool_Release();");
}

LONGBOW_TEST_FIXTURE(Object)
{
    LONGBOW_RUN_TEST_CASE(Object, parcScheduledThreadPool_Compare);
    LONGBOW_RUN_TEST_CASE(Object, parcScheduledThreadPool_Copy);
    LONGBOW_RUN_TEST_CASE(Object, parcScheduledThreadPool_Display);
    LONGBOW_RUN_TEST_CASE(Object, parcScheduledThreadPool_Equals);
    LONGBOW_RUN_TEST_CASE(Object, parcScheduledThreadPool_HashCode);
    LONGBOW_RUN_TEST_CASE(Object, parcScheduledThreadPool_IsValid);
    LONGBOW_RUN_TEST_CASE(Object, parcScheduledThreadPool_ToJSON);
    LONGBOW_RUN_TEST_CASE(Object, parcScheduledThreadPool_ToString);
}

LONGBOW_TEST_FIXTURE_SETUP(Object)
{
    longBowTestCase_SetInt(testCase, "initalAllocations", parcMemory_Outstanding());
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Object)
{
    int initialAllocations = longBowTestCase_GetInt(testCase, "initalAllocations");
    if (!parcMemoryTesting_ExpectedOutstanding(initialAllocations, "%s leaked memory.", longBowTestCase_GetFullName(testCase))) {
        parcSafeMemory_ReportAllocation(1);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Object,  parcScheduledThreadPool_Compare)
{
    testUnimplemented("");

}

LONGBOW_TEST_CASE(Object, parcScheduledThreadPool_Copy)
{
    PARCScheduledThreadPool *instance = parcScheduledThreadPool_Create(3);
    PARCScheduledThreadPool *copy = parcScheduledThreadPool_Copy(instance);
    assertTrue(parcScheduledThreadPool_Equals(instance, copy), "Expected the copy to be equal to the original");

    parcScheduledThreadPool_ShutdownNow(instance);
    parcScheduledThreadPool_ShutdownNow(copy);
    
    parcScheduledThreadPool_Release(&instance);
    parcScheduledThreadPool_Release(&copy);
}

LONGBOW_TEST_CASE(Object, parcScheduledThreadPool_Display)
{
    PARCScheduledThreadPool *instance = parcScheduledThreadPool_Create(2);
    parcScheduledThreadPool_Display(instance, 0);
    parcScheduledThreadPool_ShutdownNow(instance);
    parcScheduledThreadPool_Release(&instance);
}

LONGBOW_TEST_CASE(Object, parcScheduledThreadPool_Equals)
{
    PARCScheduledThreadPool *x = parcScheduledThreadPool_Create(2);
    PARCScheduledThreadPool *y = parcScheduledThreadPool_Create(2);
    PARCScheduledThreadPool *z = parcScheduledThreadPool_Create(2);
    PARCScheduledThreadPool *u1 = parcScheduledThreadPool_Create(3);

    parcObjectTesting_AssertEquals(x, y, z, u1, NULL);

    parcScheduledThreadPool_ShutdownNow(x);
    parcScheduledThreadPool_ShutdownNow(y);
    parcScheduledThreadPool_ShutdownNow(z);
    parcScheduledThreadPool_ShutdownNow(u1);
    
    parcScheduledThreadPool_Release(&x);
    parcScheduledThreadPool_Release(&y);
    parcScheduledThreadPool_Release(&z);
    parcScheduledThreadPool_Release(&u1);
}

LONGBOW_TEST_CASE(Object, parcScheduledThreadPool_HashCode)
{
    PARCScheduledThreadPool *x = parcScheduledThreadPool_Create(2);
    PARCScheduledThreadPool *y = parcScheduledThreadPool_Create(2);
    
    parcObjectTesting_AssertHashCode(x, y);
    
    parcScheduledThreadPool_ShutdownNow(x);
    parcScheduledThreadPool_ShutdownNow(y);
    
    parcScheduledThreadPool_Release(&x);
    parcScheduledThreadPool_Release(&y);
}

LONGBOW_TEST_CASE(Object, parcScheduledThreadPool_IsValid)
{
    PARCScheduledThreadPool *instance = parcScheduledThreadPool_Create(2);
    assertTrue(parcScheduledThreadPool_IsValid(instance), "Expected parcScheduledThreadPool_Create to result in a valid instance.");
    
    parcScheduledThreadPool_ShutdownNow(instance);
    
    parcScheduledThreadPool_Release(&instance);
    assertFalse(parcScheduledThreadPool_IsValid(instance), "Expected parcScheduledThreadPool_Release to result in an invalid instance.");
}

LONGBOW_TEST_CASE(Object, parcScheduledThreadPool_ToJSON)
{
    PARCScheduledThreadPool *instance = parcScheduledThreadPool_Create(2);
    
    PARCJSON *json = parcScheduledThreadPool_ToJSON(instance);

    parcJSON_Release(&json);
    
    parcScheduledThreadPool_ShutdownNow(instance);
    parcScheduledThreadPool_Release(&instance);
}

LONGBOW_TEST_CASE(Object, parcScheduledThreadPool_ToString)
{
    PARCScheduledThreadPool *instance = parcScheduledThreadPool_Create(2);
    
    char *string = parcScheduledThreadPool_ToString(instance);
    
    assertNotNull(string, "Expected non-NULL result from parcScheduledThreadPool_ToString");
    
    parcMemory_Deallocate((void **) &string);
    parcScheduledThreadPool_ShutdownNow(instance);
    parcScheduledThreadPool_Release(&instance);
}

LONGBOW_TEST_FIXTURE(Specialization)
{
    LONGBOW_RUN_TEST_CASE(Specialization, OneJob);
    LONGBOW_RUN_TEST_CASE(Specialization, Idle);
    LONGBOW_RUN_TEST_CASE(Specialization, parcScheduledThreadPool_Schedule);
}

LONGBOW_TEST_FIXTURE_SETUP(Specialization)
{
    longBowTestCase_SetInt(testCase, "initalAllocations", parcMemory_Outstanding());
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Specialization)
{
    int initialAllocations = longBowTestCase_GetInt(testCase, "initalAllocations");
    if (!parcMemoryTesting_ExpectedOutstanding(initialAllocations, "%s leaked memory.", longBowTestCase_GetFullName(testCase))) {
        parcSafeMemory_ReportAllocation(1);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Specialization, Idle)
{
    PARCScheduledThreadPool *pool = parcScheduledThreadPool_Create(3);
    
    sleep(2);
    
    parcScheduledThreadPool_ShutdownNow(pool);
    
    parcScheduledThreadPool_Release(&pool);
}

static void *
_function(PARCFutureTask *task, void *parameter)
{
    printf("Hello World\n");
    return parameter;
}

LONGBOW_TEST_CASE(Specialization, OneJob)
{
    PARCScheduledThreadPool *pool = parcScheduledThreadPool_Create(3);
    
    PARCFutureTask *task = parcFutureTask_Create(_function, _function);
    
    parcScheduledThreadPool_Schedule(pool, task, parcTimeout_MilliSeconds(2000));
    printf("references %lld\n", parcObject_GetReferenceCount(task));
    parcFutureTask_Release(&task);
    
    sleep(5);
    
    parcScheduledThreadPool_ShutdownNow(pool);
    
    parcScheduledThreadPool_Release(&pool);
}

LONGBOW_TEST_CASE(Specialization, parcScheduledThreadPool_Schedule)
{
    PARCScheduledThreadPool *pool = parcScheduledThreadPool_Create(3);
    
    PARCFutureTask *task = parcFutureTask_Create(_function, _function);
    
    parcScheduledThreadPool_Schedule(pool, task, parcTimeout_MilliSeconds(2000));
    
    parcFutureTask_Release(&task);
    
    parcScheduledThreadPool_Shutdown(pool);
//    parcScheduledThreadPool_AwaitTermination(pool, PARCTimeout_Never);
    
//    uint64_t count = parcScheduledThreadPool_GetCompletedTaskCount(pool);
//    assertTrue(count == 5, "Expected 5, actual %lld", count);
    
    parcScheduledThreadPool_ShutdownNow(pool);
    
    parcScheduledThreadPool_Release(&pool);
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_ScheduledThreadPool);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
