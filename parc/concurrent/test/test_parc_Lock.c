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
#include "../parc_Lock.c"

#include <stdio.h>

#include <LongBow/testing.h>
#include <LongBow/debugging.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_SafeMemory.h>
#include <parc/algol/parc_DisplayIndented.h>

#include <parc/testing/parc_MemoryTesting.h>
#include <parc/testing/parc_ObjectTesting.h>

LONGBOW_TEST_RUNNER(parc_Lock)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(CreateAcquireRelease);
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Locking);
    LONGBOW_RUN_TEST_FIXTURE(WaitNotify);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_Lock)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_Lock)
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
    PARCLock *instance = parcLock_Create();
    assertNotNull(instance, "Expected non-null result from parcLock_Create().");

    parcObjectTesting_AssertAcquire(instance);

    parcObjectTesting_AssertAcquireReleaseContract(parcLock_Acquire, instance);

    parcLock_Release(&instance);
    assertNull(instance, "Expected null result from parcLock_Release().");
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcLock_Display);
    LONGBOW_RUN_TEST_CASE(Global, parcLock_IsValid);
    LONGBOW_RUN_TEST_CASE(Global, parcLock_ToString);
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

LONGBOW_TEST_CASE(Global, parcLock_Display)
{
    PARCLock *lock = parcLock_Create();
    parcLock_Display(lock, 0);
    parcLock_Release(&lock);
}

LONGBOW_TEST_CASE(Global, parcLock_IsValid)
{
    PARCLock *instance = parcLock_Create();
    assertTrue(parcLock_IsValid(instance), "Expected parcLock_Create to result in a valid instance.");

    parcLock_Release(&instance);
    assertFalse(parcLock_IsValid(instance), "Expected parcLock_Release to result in an invalid instance.");
}

LONGBOW_TEST_CASE(Global, parcLock_ToString)
{
    PARCLock *instance = parcLock_Create();

    char *string = parcLock_ToString(instance);

    assertNotNull(string, "Expected non-NULL result from parcLock_ToString");

    parcMemory_Deallocate((void **) &string);
    parcLock_Release(&instance);
}

LONGBOW_TEST_FIXTURE(Locking)
{
    LONGBOW_RUN_TEST_CASE(Locking, parcLock_TryLock_Unlock);
    LONGBOW_RUN_TEST_CASE(Locking, parcLock_TryLock_AlreadyLocked);
    LONGBOW_RUN_TEST_CASE(Locking, parcLock_Lock_Unlock);
    LONGBOW_RUN_TEST_CASE(Locking, parcLock_Lock_AlreadyLocked);
    LONGBOW_RUN_TEST_CASE(Locking, parcLock_Lock_AlreadyLocked);
}

LONGBOW_TEST_FIXTURE_SETUP(Locking)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Locking)
{
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDOUT_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestRunner_GetName(testRunner), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Locking, parcLock_TryLock_Unlock)
{
    PARCLock *lock = parcLock_Create();

    bool actual = parcLock_TryLock(lock);

    assertTrue(actual, "Expected parcObject_TryLock to succeed.");

    actual = parcLock_IsLocked(lock);
    assertTrue(actual, "Expected parcObject_IsLocked to be true.");

    actual = parcLock_Unlock(lock);
    assertTrue(actual, "Expected parcObject_Unlock to succeed.");

    actual = parcLock_IsLocked(lock);
    assertTrue(actual, "Expected parcObject_IsLocked to be false.");

    parcLock_Release((PARCLock **) &lock);
}

LONGBOW_TEST_CASE(Locking, parcLock_Lock_Unlock)
{
    PARCLock *lock = parcLock_Create();

    bool actual = parcLock_Lock(lock);

    assertTrue(actual, "Expected parcObject_Lock to succeed.");

    actual = parcLock_IsLocked(lock);
    assertTrue(actual, "Expected parcObject_IsLocked to be true.");

    actual = parcLock_Unlock(lock);
    assertTrue(actual, "Expected parcObject_Unlock to succeed.");

    actual = parcLock_IsLocked(lock);
    assertTrue(actual, "Expected parcObject_IsLocked to be false.");

    parcLock_Release((PARCLock **) &lock);
}

LONGBOW_TEST_CASE(Locking, parcLock_TryLock_AlreadyLocked)
{
    PARCLock *lock = parcLock_Create();

    bool actual = parcLock_TryLock(lock);

    assertTrue(actual, "Expected parcObject_TryLock to succeed.");

    actual = parcLock_TryLock(lock);

    assertFalse(actual, "Expected parcObject_TryLock to fail when already locked.");

    actual = parcLock_Unlock(lock);
    assertTrue(actual, "Expected parcObject_Unlock to succeed.");

    parcLock_Release((PARCLock **) &lock);
}

LONGBOW_TEST_CASE(Locking, parcLock_Lock_AlreadyLocked)
{
    PARCLock *lock = parcLock_Create();

    bool actual = parcLock_Lock(lock);

    assertTrue(actual, "Expected parcObject_Lock to succeed.");

    actual = parcLock_Lock(lock);

    assertFalse(actual, "Expected parcObject_Lock to fail when already locked by the same thread.");

    actual = parcLock_Unlock(lock);
    assertTrue(actual, "Expected parcObject_Unlock to succeed.");

    parcLock_Release((PARCLock **) &lock);
}

LONGBOW_TEST_FIXTURE(WaitNotify)
{
    LONGBOW_RUN_TEST_CASE(WaitNotify, parcLock_WaitNotify);
    LONGBOW_RUN_TEST_CASE(WaitNotify, parcLock_WaitNotify2);
}

LONGBOW_TEST_FIXTURE_SETUP(WaitNotify)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(WaitNotify)
{
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDOUT_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestRunner_GetName(testRunner), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

static int _sharedValue;

static void *
waiter(void *data)
{
    PARCLock *lock = data;

    while (parcLock_TryLock(lock) == false)
        ;
    parcLock_Wait(lock);

    _sharedValue++;
    parcLock_Unlock(lock);

    return data;
}

LONGBOW_TEST_CASE(WaitNotify, parcLock_WaitNotify)
{
    PARCLock *lock = parcLock_Create();

    _sharedValue = 0;

    pthread_t thread_A;
    pthread_t thread_B;
    pthread_t thread_C;
    pthread_create(&thread_A, NULL, waiter, lock);
    pthread_create(&thread_B, NULL, waiter, lock);
    pthread_create(&thread_C, NULL, waiter, lock);

    while (_sharedValue != 3) {
        while (parcLock_TryLock(lock) == false)
            ;
        parcLock_Notify(lock);
        parcLock_Unlock(lock);
    }

    pthread_join(thread_A, NULL);

    parcLock_Release((PARCLock **) &lock);
}

static void *
decrement(void *data)
{
    PARCLock *lock = data;

    while (parcLock_TryLock(lock) == false) {
        assertTrue(write(1, ".", 1) == 1, "Write failed.");
    }
    while (_sharedValue < 12) {
        parcLock_Wait(lock);
        _sharedValue--;
    }
    parcLock_Unlock(lock);

    return data;
}

LONGBOW_TEST_CASE(WaitNotify, parcLock_WaitNotify2)
{
    PARCLock *lock = parcLock_Create();

    _sharedValue = 0;

    pthread_t thread_A;
    pthread_create(&thread_A, NULL, decrement, lock);

    _sharedValue = 2;
    while (parcLock_TryLock(lock) == false) {
        assertTrue(write(1, ".", 1) == 1, "Write failed.");
    }
    while (_sharedValue <= 12) {
        printf("%d\n", _sharedValue);
        parcLock_Notify(lock);
        _sharedValue += 2;
    }
    parcLock_Unlock(lock);

    parcLock_Notify(lock);
    pthread_join(thread_A, NULL);

    parcLock_Release((PARCLock **) &lock);
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_Lock);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
