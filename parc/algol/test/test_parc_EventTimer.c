/*
 * Copyright (c) 2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
#include <config.h>
#include <stdio.h>
#include <pthread.h>

#include <LongBow/unit-test.h>

#include <parc/algol/parc_SafeMemory.h>
#include <parc/algol/parc_EventTimer.h>

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../parc_EventTimer.c"

LONGBOW_TEST_RUNNER(parc_EventTimer)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_EventTimer)
{
    parcEventTimer_EnableDebug();
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_EventTimer)
{
    parcEventTimer_DisableDebug();
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parc_EventTimer_Create_Destroy);
    LONGBOW_RUN_TEST_CASE(Global, parc_EventTimer_Start);
    LONGBOW_RUN_TEST_CASE(Global, parc_EventTimer_Stop);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDERR_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

static int _test_event_called = 0;

static void
_test_event(int fd, PARCEventType flags, void *data)
{
    _test_event_called++;
}

LONGBOW_TEST_CASE(Global, parc_EventTimer_Create_Destroy)
{
    PARCEventScheduler *parcEventScheduler = parcEventScheduler_Create();
    assertNotNull(parcEventScheduler, "parcEventScheduler_Create returned a null reference");

    PARCEventTimer *parcEventTimer = parcEventTimer_Create(parcEventScheduler, PARCEventType_None, _test_event, NULL);
    assertNotNull(parcEventTimer, "parcEventTimer_Create returned a null reference");

    parcEventTimer_Destroy(&parcEventTimer);
    parcEventScheduler_Destroy(&parcEventScheduler);
}

LONGBOW_TEST_CASE(Global, parc_EventTimer_Start)
{
    PARCEventScheduler *parcEventScheduler = parcEventScheduler_Create();
    assertNotNull(parcEventScheduler, "parcEventScheduler_Create returned a null reference");

    PARCEventTimer *parcEventTimer = parcEventTimer_Create(parcEventScheduler, PARCEventType_None, _test_event, NULL);
    assertNotNull(parcEventTimer, "parcEventTimer_Create returned a null reference");

    struct timeval timeout = { 1, 0 };
    parcEventTimer_Start(parcEventTimer, &timeout);
    parcEventScheduler_Start(parcEventScheduler, PARCEventSchedulerDispatchType_Blocking);
    assertTrue(_test_event_called == 1, "Event never called.");

    parcEventTimer_Destroy(&parcEventTimer);
    parcEventScheduler_Destroy(&parcEventScheduler);
}

static void
_test_stop_event(int fd, PARCEventType flags, void *data)
{
    PARCEventTimer **parcEventTimer = (PARCEventTimer **) data;
    _test_event_called++;
    parcEventTimer_Stop(*parcEventTimer);
}

LONGBOW_TEST_CASE(Global, parc_EventTimer_Stop)
{
    PARCEventScheduler *parcEventScheduler = parcEventScheduler_Create();
    assertNotNull(parcEventScheduler, "parcEventScheduler_Create returned a null reference");

    PARCEventTimer *parcEventTimer = parcEventTimer_Create(parcEventScheduler, PARCEventType_None | PARCEventType_Persist, _test_stop_event, &parcEventTimer);
    assertNotNull(parcEventTimer, "parcEventTimer_Create returned a null reference");

    _test_event_called = 0;
    struct timeval timeout = { 1, 0 };
    parcEventTimer_Start(parcEventTimer, &timeout);
    parcEventScheduler_Start(parcEventScheduler, PARCEventSchedulerDispatchType_Blocking);
    assertTrue(_test_event_called == 1, "Event never called.");

    parcEventTimer_Destroy(&parcEventTimer);
    parcEventScheduler_Destroy(&parcEventScheduler);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_EventTimer);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
