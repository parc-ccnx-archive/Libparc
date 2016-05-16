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

/**
 * @author <#gscott#>, Computing Science Laboratory, PARC
 * @copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include "../parc_FutureTask.c"

#include <LongBow/testing.h>
#include <LongBow/debugging.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_SafeMemory.h>
#include <parc/algol/parc_DisplayIndented.h>

#include <parc/testing/parc_MemoryTesting.h>
#include <parc/testing/parc_ObjectTesting.h>

LONGBOW_TEST_RUNNER(parc_FutureTask)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(CreateAcquireRelease);
    LONGBOW_RUN_TEST_FIXTURE(Object);
    LONGBOW_RUN_TEST_FIXTURE(Specialization);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_FutureTask)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_FutureTask)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(CreateAcquireRelease)
{
    LONGBOW_RUN_TEST_CASE(CreateAcquireRelease, CreateRelease);
    LONGBOW_RUN_TEST_CASE(CreateAcquireRelease, CreateRelease_PARCObject);
}

LONGBOW_TEST_FIXTURE_SETUP(CreateAcquireRelease)
{
    longBowTestCase_SetInt(testCase, "initialAllocations", parcMemory_Outstanding());
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(CreateAcquireRelease)
{
    int initialAllocations = longBowTestCase_GetInt(testCase, "initialAllocations");
   
    if (!parcMemoryTesting_ExpectedOutstanding(initialAllocations, "%s leaked memory.", longBowTestCase_GetFullName(testCase))) {
        return LONGBOW_STATUS_MEMORYLEAK;
    }
   
    return LONGBOW_STATUS_SUCCEEDED;
}

static void *
_function(PARCFutureTask *task, void *parameter)
{
    return parameter;
}

LONGBOW_TEST_CASE(CreateAcquireRelease, CreateRelease)
{
    PARCFutureTask *instance = parcFutureTask_Create(_function, _function);
    assertNotNull(instance, "Expected non-null result from parcFutureTask_Create(_function, _function);");

    parcObjectTesting_AssertAcquireReleaseContract(parcFutureTask_Acquire, instance);
   
    parcFutureTask_Release(&instance);
    assertNull(instance, "Expected null result from parcFutureTask_Release();");
}

LONGBOW_TEST_CASE(CreateAcquireRelease, CreateRelease_PARCObject)
{
    PARCBuffer *object = parcBuffer_Allocate(10);
   
    PARCFutureTask *instance = parcFutureTask_Create(_function, object);
    parcBuffer_Release(&object);
   
    assertNotNull(instance, "Expected non-null result from parcFutureTask_Create(_function, object);");
   
    parcObjectTesting_AssertAcquireReleaseContract(parcFutureTask_Acquire, instance);
   
    parcFutureTask_Release(&instance);
    assertNull(instance, "Expected null result from parcFutureTask_Release();");
}

LONGBOW_TEST_FIXTURE(Object)
{
    LONGBOW_RUN_TEST_CASE(Object, parcFutureTask_Compare);
    LONGBOW_RUN_TEST_CASE(Object, parcFutureTask_Copy);
    LONGBOW_RUN_TEST_CASE(Object, parcFutureTask_Display);
    LONGBOW_RUN_TEST_CASE(Object, parcFutureTask_Equals);
    LONGBOW_RUN_TEST_CASE(Object, parcFutureTask_HashCode);
    LONGBOW_RUN_TEST_CASE(Object, parcFutureTask_IsValid);
    LONGBOW_RUN_TEST_CASE(Object, parcFutureTask_ToJSON);
    LONGBOW_RUN_TEST_CASE(Object, parcFutureTask_ToString);
}

LONGBOW_TEST_FIXTURE_SETUP(Object)
{
    longBowTestCase_SetInt(testCase, "initialAllocations", parcMemory_Outstanding());
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Object)
{
    int initialAllocations = longBowTestCase_GetInt(testCase, "initialAllocations");
   
    if (!parcMemoryTesting_ExpectedOutstanding(initialAllocations, "%s leaked memory.", longBowTestCase_GetFullName(testCase))) {
        return LONGBOW_STATUS_MEMORYLEAK;
    }
   
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Object,  parcFutureTask_Compare)
{
    testUnimplemented("");

}

LONGBOW_TEST_CASE(Object, parcFutureTask_Copy)
{
    PARCFutureTask *instance = parcFutureTask_Create(_function, _function);
    PARCFutureTask *copy = parcFutureTask_Copy(instance);
    assertTrue(parcFutureTask_Equals(instance, copy), "Expected the copy to be equal to the original");

    parcFutureTask_Release(&instance);
    parcFutureTask_Release(&copy);
}

LONGBOW_TEST_CASE(Object, parcFutureTask_Display)
{
    PARCFutureTask *instance = parcFutureTask_Create(_function, _function);
    parcFutureTask_Display(instance, 0);
    parcFutureTask_Release(&instance);
}

LONGBOW_TEST_CASE(Object, parcFutureTask_Equals)
{
    PARCFutureTask *x = parcFutureTask_Create(_function, _function);
    PARCFutureTask *y = parcFutureTask_Create(_function, _function);
    PARCFutureTask *z = parcFutureTask_Create(_function, _function);
    PARCFutureTask *u1 = parcFutureTask_Create(_function, NULL);

    parcObjectTesting_AssertEquals(x, y, z, u1, NULL);

    parcFutureTask_Release(&x);
    parcFutureTask_Release(&y);
    parcFutureTask_Release(&z);
    parcFutureTask_Release(&u1);
}

LONGBOW_TEST_CASE(Object, parcFutureTask_HashCode)
{
    PARCFutureTask *x = parcFutureTask_Create(_function, _function);
    PARCFutureTask *y = parcFutureTask_Create(_function, _function);
   
    parcObjectTesting_AssertHashCode(x, y);
   
    parcFutureTask_Release(&x);
    parcFutureTask_Release(&y);
}

LONGBOW_TEST_CASE(Object, parcFutureTask_IsValid)
{
    PARCFutureTask *instance = parcFutureTask_Create(_function, _function);
    assertTrue(parcFutureTask_IsValid(instance), "Expected parcFutureTask_Create to result in a valid instance.");
   
    parcFutureTask_Release(&instance);
    assertFalse(parcFutureTask_IsValid(instance), "Expected parcFutureTask_Release to result in an invalid instance.");
}

LONGBOW_TEST_CASE(Object, parcFutureTask_ToJSON)
{
    PARCFutureTask *instance = parcFutureTask_Create(_function, _function);
   
    PARCJSON *json = parcFutureTask_ToJSON(instance);

    parcJSON_Release(&json);

    parcFutureTask_Release(&instance);
}

LONGBOW_TEST_CASE(Object, parcFutureTask_ToString)
{
    PARCFutureTask *instance = parcFutureTask_Create(_function, _function);
   
    char *string = parcFutureTask_ToString(instance);
   
    assertNotNull(string, "Expected non-NULL result from parcFutureTask_ToString");
   
    parcMemory_Deallocate((void **) &string);
    parcFutureTask_Release(&instance);
}

LONGBOW_TEST_FIXTURE(Specialization)
{
    LONGBOW_RUN_TEST_CASE(Specialization, parcFutureTask_Cancel);
    LONGBOW_RUN_TEST_CASE(Specialization, parcFutureTask_Get);
    LONGBOW_RUN_TEST_CASE(Specialization, parcFutureTask_IsCancelled);
    LONGBOW_RUN_TEST_CASE(Specialization, parcFutureTask_IsDone);
    LONGBOW_RUN_TEST_CASE(Specialization, parcFutureTask_Run);
    LONGBOW_RUN_TEST_CASE(Specialization, parcFutureTask_RunAndReset);
}

LONGBOW_TEST_FIXTURE_SETUP(Specialization)
{
    longBowTestCase_SetInt(testCase, "initialAllocations", parcMemory_Outstanding());
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Specialization)
{
    int initialAllocations = longBowTestCase_GetInt(testCase, "initialAllocations");
   
    if (!parcMemoryTesting_ExpectedOutstanding(initialAllocations, "%s leaked memory.", longBowTestCase_GetFullName(testCase))) {
        return LONGBOW_STATUS_MEMORYLEAK;
    }
   
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Specialization, parcFutureTask_Cancel)
{
    PARCFutureTask *task = parcFutureTask_Create(_function, _function);

    bool parcFutureTask_Cancel(PARCFutureTask *task, bool mayInterruptIfRunning);

    parcFutureTask_Release(&task);
}

LONGBOW_TEST_CASE(Specialization, parcFutureTask_Get)
{
    PARCFutureTask *task = parcFutureTask_Create(_function, _function);

    PARCFutureTaskResult result = parcFutureTask_Get(task, PARCTimeout_Immediate);
   
    assertTrue(parcExecution_Is(result.execution, PARCExecution_Timeout), "Expected Timeout, actual %s",
               parcExecution_GetMessage(result.execution));
    parcFutureTask_Release(&task);
}

LONGBOW_TEST_CASE(Specialization, parcFutureTask_IsCancelled)
{
    PARCFutureTask *task = parcFutureTask_Create(_function, _function);

    bool actual = parcFutureTask_IsCancelled(task);
    assertFalse(actual, "Expected true.");
   
    parcFutureTask_Release(&task);
}

LONGBOW_TEST_CASE(Specialization, parcFutureTask_IsDone)
{
    PARCFutureTask *task = parcFutureTask_Create(_function, _function);

    bool actual = parcFutureTask_IsDone(task);
   
    assertFalse(actual, "Expected false.");
   
    parcFutureTask_Release(&task);
}

LONGBOW_TEST_CASE(Specialization, parcFutureTask_Run)
{
    PARCFutureTask *task = parcFutureTask_Create(_function, _function);

    parcFutureTask_Run(task);
   
    PARCFutureTaskResult actual = parcFutureTask_Get(task, PARCTimeout_Immediate);
   
    assertTrue(parcFutureTask_IsDone(task), "Expected parcFutureTask_IsDone to be true.");
    assertTrue(parcExecution_Is(actual.execution, PARCExecution_OK),
               "Expected OK, actual %s", parcExecution_GetMessage(actual.execution));
    assertTrue(actual.value == _function, "Expected actual to point to _function");
    parcFutureTask_Release(&task);
}

LONGBOW_TEST_CASE(Specialization, parcFutureTask_RunAndReset)
{
    PARCFutureTask *task = parcFutureTask_Create(_function, _function);

    bool actual = parcFutureTask_RunAndReset(task);
   
    assertTrue(actual, "Expectd parcFutureTask_RunAndReset to return true.");
    assertFalse(parcFutureTask_IsDone(task), "Expected parcFutureTask_IsDone to be false");
    parcFutureTask_Release(&task);
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_FutureTask);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
