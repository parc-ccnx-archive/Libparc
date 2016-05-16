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
 * @author Glenn Scott <Glenn.Scott@parc.com>, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include "../parc_Synchronizer.c"

#include <LongBow/testing.h>
#include <LongBow/debugging.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_SafeMemory.h>
#include <parc/testing/parc_MemoryTesting.h>
#include <parc/testing/parc_ObjectTesting.h>

LONGBOW_TEST_RUNNER(parc_Synchronizer)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(CreateAcquireRelease);
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_Synchronizer)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_Synchronizer)
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
    PARCSynchronizer *instance = parcSynchronizer_Create();
    assertNotNull(instance, "Expeced non-null result from parcSynchronizer_Create();");

    parcObjectTesting_AssertAcquireReleaseContract(parcSynchronizer_Acquire, instance);

    parcSynchronizer_Release(&instance);
    assertNull(instance, "Expeced null result from parcSynchronizer_Release();");
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcSynchronizer_Display);
    LONGBOW_RUN_TEST_CASE(Global, parcSynchronizer_IsValid);

    LONGBOW_RUN_TEST_CASE(Global, parcSynchronizer_TryLock);
    LONGBOW_RUN_TEST_CASE(Global, parcSynchronizer_LockUnlock);
    LONGBOW_RUN_TEST_CASE(Global, parcSynchronizer_TryLock_Fail);
    LONGBOW_RUN_TEST_CASE(Global, parcSynchronizer_IsLocked);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    if (!parcMemoryTesting_ExpectedOutstanding(0, "%s leaked memory.", longBowTestCase_GetFullName(testCase))) {
        return LONGBOW_STATUS_MEMORYLEAK;
    }

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, parcSynchronizer_Display)
{
    PARCSynchronizer *instance = parcSynchronizer_Create();
    assertTrue(parcSynchronizer_IsValid(instance), "Expected parcSynchronizer_Create to result in a valid instance.");

    parcSynchronizer_Display(instance, 0);
    parcSynchronizer_Release(&instance);
}

LONGBOW_TEST_CASE(Global, parcSynchronizer_IsValid)
{
    PARCSynchronizer *instance = parcSynchronizer_Create();
    assertTrue(parcSynchronizer_IsValid(instance), "Expected parcSynchronizer_Create to result in a valid instance.");

    parcSynchronizer_Release(&instance);
    assertFalse(parcSynchronizer_IsValid(instance), "Expected parcSynchronizer_Create to result in an invalid instance.");
}

LONGBOW_TEST_CASE(Global, parcSynchronizer_TryLock)
{
    PARCSynchronizer *instance = parcSynchronizer_Create();

    bool actual = parcSynchronizer_TryLock(instance);
    assertTrue(actual, "Expected parcSynchronizer_TryLock to be successful.");

    parcSynchronizer_Unlock(instance);
    parcSynchronizer_Release(&instance);
}

LONGBOW_TEST_CASE(Global, parcSynchronizer_TryLock_Fail)
{
    PARCSynchronizer *instance = parcSynchronizer_Create();

    parcSynchronizer_Lock(instance);
    bool actual = parcSynchronizer_TryLock(instance);
    assertFalse(actual, "Expected parcSynchronizer_TryLock to be unsuccessful.");

    parcSynchronizer_Unlock(instance);
    parcSynchronizer_Release(&instance);
}

LONGBOW_TEST_CASE(Global, parcSynchronizer_LockUnlock)
{
    PARCSynchronizer *instance = parcSynchronizer_Create();

    parcSynchronizer_Lock(instance);
    parcSynchronizer_Unlock(instance);

    parcSynchronizer_Release(&instance);
}

LONGBOW_TEST_CASE(Global, parcSynchronizer_IsLocked)
{
    PARCSynchronizer *instance = parcSynchronizer_Create();
    assertFalse(parcSynchronizer_IsLocked(instance), "Expected the synchronizer to be unlocked.");

    parcSynchronizer_Lock(instance);
    assertTrue(parcSynchronizer_IsLocked(instance), "Expected the synchronizer to be unlocked.");

    parcSynchronizer_Unlock(instance);
    assertFalse(parcSynchronizer_IsLocked(instance), "Expected the synchronizer to be unlocked.");

    parcSynchronizer_Release(&instance);
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_Synchronizer);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}


