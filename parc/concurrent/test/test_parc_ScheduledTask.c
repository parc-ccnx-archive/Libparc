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
#include "../parc_ScheduledTask.c"

#include <LongBow/testing.h>
#include <LongBow/debugging.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_SafeMemory.h>
#include <parc/algol/parc_DisplayIndented.h>

#include <parc/testing/parc_MemoryTesting.h>
#include <parc/testing/parc_ObjectTesting.h>

LONGBOW_TEST_RUNNER(parc_ScheduledTask)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(CreateAcquireRelease);
    LONGBOW_RUN_TEST_FIXTURE(Object);
    LONGBOW_RUN_TEST_FIXTURE(Specialization);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_ScheduledTask)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_ScheduledTask)
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

void *_function(PARCFutureTask *task, void *parameter)
{
    return parameter;
}

LONGBOW_TEST_CASE(CreateAcquireRelease, CreateRelease)
{
    PARCFutureTask *task = parcFutureTask_Create(_function, _function);
    
    PARCScheduledTask *instance = parcScheduledTask_Create(task, 0);
    assertNotNull(instance, "Expected non-null result from parcScheduledTask_Create();");

    parcObjectTesting_AssertAcquireReleaseContract(parcScheduledTask_Acquire, instance);
    
    parcScheduledTask_Release(&instance);
    assertNull(instance, "Expected null result from parcScheduledTask_Release();");
    parcFutureTask_Release(&task);
}

LONGBOW_TEST_FIXTURE(Object)
{
    LONGBOW_RUN_TEST_CASE(Object, parcScheduledTask_Compare);
    LONGBOW_RUN_TEST_CASE(Object, parcScheduledTask_Copy);
    LONGBOW_RUN_TEST_CASE(Object, parcScheduledTask_Display);
    LONGBOW_RUN_TEST_CASE(Object, parcScheduledTask_Equals);
    LONGBOW_RUN_TEST_CASE(Object, parcScheduledTask_HashCode);
    LONGBOW_RUN_TEST_CASE(Object, parcScheduledTask_IsValid);
    LONGBOW_RUN_TEST_CASE(Object, parcScheduledTask_ToJSON);
    LONGBOW_RUN_TEST_CASE(Object, parcScheduledTask_ToString);
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

LONGBOW_TEST_CASE(Object,  parcScheduledTask_Compare)
{
    testUnimplemented("");

}

LONGBOW_TEST_CASE(Object, parcScheduledTask_Copy)
{
    PARCFutureTask *task = parcFutureTask_Create(_function, _function);
    
    PARCScheduledTask *instance = parcScheduledTask_Create(task, 0);
    PARCScheduledTask *copy = parcScheduledTask_Copy(instance);
    assertTrue(parcScheduledTask_Equals(instance, copy), "Expected the copy to be equal to the original");

    parcScheduledTask_Release(&instance);
    parcScheduledTask_Release(&copy);
    parcFutureTask_Release(&task);
}

LONGBOW_TEST_CASE(Object, parcScheduledTask_Display)
{
    PARCFutureTask *task = parcFutureTask_Create(_function, _function);
    
    PARCScheduledTask *instance = parcScheduledTask_Create(task, 0);
    parcScheduledTask_Display(instance, 0);
    parcScheduledTask_Release(&instance);
    parcFutureTask_Release(&task);
}

LONGBOW_TEST_CASE(Object, parcScheduledTask_Equals)
{
    PARCFutureTask *task = parcFutureTask_Create(_function, _function);
    
    PARCScheduledTask *x = parcScheduledTask_Create(task, 0);
    PARCScheduledTask *y = parcScheduledTask_Create(task, 0);
    PARCScheduledTask *z = parcScheduledTask_Create(task, 0);

    parcObjectTesting_AssertEquals(x, y, z, NULL);

    parcScheduledTask_Release(&x);
    parcScheduledTask_Release(&y);
    parcScheduledTask_Release(&z);
    parcFutureTask_Release(&task);
}

LONGBOW_TEST_CASE(Object, parcScheduledTask_HashCode)
{
    PARCFutureTask *task = parcFutureTask_Create(_function, _function);
    
    PARCScheduledTask *x = parcScheduledTask_Create(task, 0);
    PARCScheduledTask *y = parcScheduledTask_Create(task, 0);
    
    parcObjectTesting_AssertHashCode(x, y);
    
    parcScheduledTask_Release(&x);
    parcScheduledTask_Release(&y);
    parcFutureTask_Release(&task);
}

LONGBOW_TEST_CASE(Object, parcScheduledTask_IsValid)
{
    PARCFutureTask *task = parcFutureTask_Create(_function, _function);
    
    PARCScheduledTask *instance = parcScheduledTask_Create(task, 0);
    assertTrue(parcScheduledTask_IsValid(instance), "Expected parcScheduledTask_Create to result in a valid instance.");
    
    parcScheduledTask_Release(&instance);
    assertFalse(parcScheduledTask_IsValid(instance), "Expected parcScheduledTask_Release to result in an invalid instance.");
    
    parcFutureTask_Release(&task);
}

LONGBOW_TEST_CASE(Object, parcScheduledTask_ToJSON)
{
    PARCFutureTask *task = parcFutureTask_Create(_function, _function);
    
    PARCScheduledTask *instance = parcScheduledTask_Create(task, 0);
    
    PARCJSON *json = parcScheduledTask_ToJSON(instance);

    parcJSON_Release(&json);

    parcScheduledTask_Release(&instance);
    parcFutureTask_Release(&task);
}

LONGBOW_TEST_CASE(Object, parcScheduledTask_ToString)
{
    PARCFutureTask *task = parcFutureTask_Create(_function, _function);
    
    PARCScheduledTask *instance = parcScheduledTask_Create(task, 0);
    
    char *string = parcScheduledTask_ToString(instance);
    
    assertNotNull(string, "Expected non-NULL result from parcScheduledTask_ToString");
    
    parcMemory_Deallocate((void **) &string);
    parcScheduledTask_Release(&instance);
    parcFutureTask_Release(&task);
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
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_ScheduledTask);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}


