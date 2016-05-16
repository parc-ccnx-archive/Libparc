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
 * @author <#Glenn Scott <Glenn.Scott@parc.com>#>, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include "../parc_Properties.c"

#include <LongBow/testing.h>
#include <LongBow/debugging.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_SafeMemory.h>
#include <parc/algol/parc_DisplayIndented.h>

#include <parc/testing/parc_MemoryTesting.h>
#include <parc/testing/parc_ObjectTesting.h>

LONGBOW_TEST_RUNNER(parc_Properties)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(CreateAcquireRelease);
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Specialized);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_Properties)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_Properties)
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
    PARCProperties *instance = parcProperties_Create();
    assertNotNull(instance, "Expected non-null result from parcProperties_Create();");

    parcObjectTesting_AssertAcquire(instance);

    parcProperties_Release(&instance);
    assertNull(instance, "Expected null result from parcProperties_Release();");
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcProperties_Compare);
    LONGBOW_RUN_TEST_CASE(Global, parcProperties_Copy);
    LONGBOW_RUN_TEST_CASE(Global, parcProperties_Display);
    LONGBOW_RUN_TEST_CASE(Global, parcProperties_Equals);
    LONGBOW_RUN_TEST_CASE(Global, parcProperties_HashCode);
    LONGBOW_RUN_TEST_CASE(Global, parcProperties_IsValid);
    LONGBOW_RUN_TEST_CASE(Global, parcProperties_ToJSON);
    LONGBOW_RUN_TEST_CASE(Global, parcProperties_ToString);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    if (!parcMemoryTesting_ExpectedOutstanding(0, "%s mismanaged memory.", longBowTestCase_GetFullName(testCase))) {
        return LONGBOW_STATUS_MEMORYLEAK;
    }

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global,  parcProperties_Compare)
{
    testUnimplemented("");

}

LONGBOW_TEST_CASE(Global, parcProperties_Copy)
{
    PARCProperties *instance = parcProperties_Create();
    PARCProperties *copy = parcProperties_Copy(instance);
    assertTrue(parcProperties_Equals(instance, copy), "Expected the copy to be equal to the original");

    parcProperties_Release(&instance);
    parcProperties_Release(&copy);
}

LONGBOW_TEST_CASE(Global, parcProperties_Display)
{
    PARCProperties *instance = parcProperties_Create();
    parcProperties_SetProperty(instance, "foo", "bar");
    parcProperties_SetProperty(instance, "xyzzy", "plugh");

    parcProperties_Display(instance, 0);
    parcProperties_Release(&instance);
}

LONGBOW_TEST_CASE(Global, parcProperties_Equals)
{
    PARCProperties *x = parcProperties_Create();
    PARCProperties *y = parcProperties_Create();
    PARCProperties *z = parcProperties_Create();

    parcObjectTesting_AssertEquals(x, y, z, NULL);

    parcProperties_Release(&x);
    parcProperties_Release(&y);
    parcProperties_Release(&z);
}

LONGBOW_TEST_CASE(Global, parcProperties_HashCode)
{
    testUnimplemented("");

}

LONGBOW_TEST_CASE(Global, parcProperties_IsValid)
{
    PARCProperties *instance = parcProperties_Create();
    assertTrue(parcProperties_IsValid(instance), "Expected parcProperties_Create to result in a valid instance.");

    parcProperties_Release(&instance);
    assertFalse(parcProperties_IsValid(instance), "Expected parcProperties_Release to result in an invalid instance.");
}

LONGBOW_TEST_CASE(Global, parcProperties_ToJSON)
{
    PARCProperties *instance = parcProperties_Create();

    parcProperties_SetProperty(instance, "foo", "bar");
    PARCJSON *json = parcProperties_ToJSON(instance);

    parcJSON_Release(&json);

    parcProperties_Release(&instance);
}

LONGBOW_TEST_CASE(Global, parcProperties_ToString)
{
    PARCProperties *instance = parcProperties_Create();

    parcProperties_SetProperty(instance, "foo", "bar");
    parcProperties_SetProperty(instance, "bar", "baz");
    char *string = parcProperties_ToString(instance);

    assertNotNull(string, "Expected non-NULL result from parcProperties_ToString");

    parcMemory_Deallocate((void **) &string);
    parcProperties_Release(&instance);
}

LONGBOW_TEST_FIXTURE(Specialized)
{
    LONGBOW_RUN_TEST_CASE(Specialized, parcProperties_SetProperty);
    LONGBOW_RUN_TEST_CASE(Specialized, parcProperties_GetProperty);
    LONGBOW_RUN_TEST_CASE(Specialized, parcProperties_GetPropertyDefault);
    LONGBOW_RUN_TEST_CASE(Specialized, parcProperties_GetAsBoolean_true);
    LONGBOW_RUN_TEST_CASE(Specialized, parcProperties_GetAsBoolean_false);
}

LONGBOW_TEST_FIXTURE_SETUP(Specialized)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Specialized)
{
    if (!parcMemoryTesting_ExpectedOutstanding(0, "%s mismanaged memory.", longBowTestCase_GetFullName(testCase))) {
        return LONGBOW_STATUS_MEMORYLEAK;
    }

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Specialized, parcProperties_SetProperty)
{
    PARCProperties *instance = parcProperties_Create();
    char *expected = "bar";
    parcProperties_SetProperty(instance, "foo", expected);

    const char *actual = parcProperties_GetProperty(instance, "foo");
    assertTrue(strcmp("bar", actual) == 0, "Expected %s, actual %s", expected, actual);

    parcProperties_Release(&instance);
}

LONGBOW_TEST_CASE(Specialized, parcProperties_GetProperty)
{
    PARCProperties *instance = parcProperties_Create();
    char *expected = "bar";
    parcProperties_SetProperty(instance, "foo", expected);

    const char *actual = parcProperties_GetProperty(instance, "foo");
    assertTrue(strcmp("bar", actual) == 0, "Expected %s, actual %s", expected, actual);

    parcProperties_Release(&instance);
}

LONGBOW_TEST_CASE(Specialized, parcProperties_GetPropertyDefault)
{
    PARCProperties *instance = parcProperties_Create();
    char *expected = "bar";
    parcProperties_SetProperty(instance, "foo", expected);

    const char *actual = parcProperties_GetPropertyDefault(instance, "blurfl", "defaultValue");
    assertTrue(strcmp("defaultValue", actual) == 0, "Expected %s, actual %s", "defaultValue", actual);

    parcProperties_Release(&instance);
}

LONGBOW_TEST_CASE(Specialized, parcProperties_GetAsBoolean_true)
{
    PARCProperties *instance = parcProperties_Create();
    char *expected = "true";
    parcProperties_SetProperty(instance, "foo", expected);

    bool actual = parcProperties_GetAsBoolean(instance, "foo", false);
    assertTrue(actual, "Expected true");

    parcProperties_Release(&instance);
}

LONGBOW_TEST_CASE(Specialized, parcProperties_GetAsBoolean_false)
{
    PARCProperties *instance = parcProperties_Create();
    char *expected = "false";
    parcProperties_SetProperty(instance, "foo", expected);

    bool actual = parcProperties_GetAsBoolean(instance, "foo", true);
    assertFalse(actual, "Expected false");

    parcProperties_Release(&instance);
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_Properties);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}


