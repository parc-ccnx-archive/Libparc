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
#include "../parc_Timer.c"

#include <LongBow/testing.h>
#include <LongBow/debugging.h>
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
    PARCTimer *instance = parcTimer_Create();
    assertNotNull(instance, "Expected non-null result from parcTimer_Create();");

    parcObjectTesting_AssertAcquireReleaseContract(parcTimer_Acquire, instance);
   
    parcTimer_Release(&instance);
    assertNull(instance, "Expected null result from parcTimer_Release();");
}

LONGBOW_TEST_FIXTURE(Object)
{
    LONGBOW_RUN_TEST_CASE(Object, parcTimer_Compare);
    LONGBOW_RUN_TEST_CASE(Object, parcTimer_Copy);
    LONGBOW_RUN_TEST_CASE(Object, parcTimer_Display);
    LONGBOW_RUN_TEST_CASE(Object, parcTimer_Equals);
    LONGBOW_RUN_TEST_CASE(Object, parcTimer_HashCode);
    LONGBOW_RUN_TEST_CASE(Object, parcTimer_IsValid);
    LONGBOW_RUN_TEST_CASE(Object, parcTimer_ToJSON);
    LONGBOW_RUN_TEST_CASE(Object, parcTimer_ToString);
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

LONGBOW_TEST_CASE(Object,  parcTimer_Compare)
{
    testUnimplemented("");

}

LONGBOW_TEST_CASE(Object, parcTimer_Copy)
{
    PARCTimer *instance = parcTimer_Create();
    PARCTimer *copy = parcTimer_Copy(instance);
    assertTrue(parcTimer_Equals(instance, copy), "Expected the copy to be equal to the original");

    parcTimer_Release(&instance);
    parcTimer_Release(&copy);
}

LONGBOW_TEST_CASE(Object, parcTimer_Display)
{
    PARCTimer *instance = parcTimer_Create();
    parcTimer_Display(instance, 0);
    parcTimer_Release(&instance);
}

LONGBOW_TEST_CASE(Object, parcTimer_Equals)
{
    PARCTimer *x = parcTimer_Create();
    PARCTimer *y = parcTimer_Create();
    PARCTimer *z = parcTimer_Create();

    parcObjectTesting_AssertEquals(x, y, z, NULL);

    parcTimer_Release(&x);
    parcTimer_Release(&y);
    parcTimer_Release(&z);
}

LONGBOW_TEST_CASE(Object, parcTimer_HashCode)
{
    PARCTimer *x = parcTimer_Create();
    PARCTimer *y = parcTimer_Create();
   
    parcObjectTesting_AssertHashCode(x, y);
   
    parcTimer_Release(&x);
    parcTimer_Release(&y);
}

LONGBOW_TEST_CASE(Object, parcTimer_IsValid)
{
    PARCTimer *instance = parcTimer_Create();
    assertTrue(parcTimer_IsValid(instance), "Expected parcTimer_Create to result in a valid instance.");
   
    parcTimer_Release(&instance);
    assertFalse(parcTimer_IsValid(instance), "Expected parcTimer_Release to result in an invalid instance.");
}

LONGBOW_TEST_CASE(Object, parcTimer_ToJSON)
{
    PARCTimer *instance = parcTimer_Create();
   
    PARCJSON *json = parcTimer_ToJSON(instance);

    parcJSON_Release(&json);

    parcTimer_Release(&instance);
}

LONGBOW_TEST_CASE(Object, parcTimer_ToString)
{
    PARCTimer *instance = parcTimer_Create();
   
    char *string = parcTimer_ToString(instance);
   
    assertNotNull(string, "Expected non-NULL result from parcTimer_ToString");
   
    parcMemory_Deallocate((void **) &string);
    parcTimer_Release(&instance);
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
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_Timer);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}


