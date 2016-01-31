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
/**
 * @author Kevin Fox, Palo Alto Research Center (Xerox PARC)
 * @copyright 2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>
#include <stdio.h>

#include <pthread.h>

#include <LongBow/unit-test.h>

#include "../internal_parc_Event.h"
#include <parc/algol/parc_SafeMemory.h>
#include <parc/algol/parc_EventScheduler.h>
#include <parc/algol/parc_EventTimer.h>

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../parc_EventScheduler.c"

LONGBOW_TEST_RUNNER(parc_EventScheduler)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_EventScheduler)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_EventScheduler)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parc_EventScheduler_Create_Destroy);
    LONGBOW_RUN_TEST_CASE(Global, parc_EventScheduler_Run);
    LONGBOW_RUN_TEST_CASE(Global, parc_EventScheduler_Dispatch);
    LONGBOW_RUN_TEST_CASE(Global, parc_EventScheduler_Stop);
    LONGBOW_RUN_TEST_CASE(Global, parc_EventScheduler_Abort);
    LONGBOW_RUN_TEST_CASE(Global, parc_EventScheduler_Memory);
    LONGBOW_RUN_TEST_CASE(Global, parc_EventScheduler_GetEvBase);
    LONGBOW_RUN_TEST_CASE(Global, parc_EventScheduler_GetLogger);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    parcEventScheduler_EnableDebug();
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDERR_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }

    parcEventScheduler_DisableDebug();
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, parc_EventScheduler_Create_Destroy)
{
    PARCEventScheduler *parcEventScheduler = parcEventScheduler_Create();
    assertNotNull(parcEventScheduler, "parcEventScheduler_Create returned a null reference");
    parcEventScheduler_Destroy(&parcEventScheduler);
    assertNull(parcEventScheduler, "parcEventScheduler_Destroy failed to null reference");
}

static void
_event_callback(int fd, PARCEventType flags, void *data)
{
    (*(unsigned *) data)++;
}

static int _callback_event_called = 0;

LONGBOW_TEST_CASE(Global, parc_EventScheduler_Run)
{
    _callback_event_called = 0;

    PARCEventScheduler *parcEventScheduler = parcEventScheduler_Create();
    assertNotNull(parcEventScheduler, "parcEventScheduler_Create returned a null reference");
    PARCEventTimer *parcEvent = parcEventTimer_Create(parcEventScheduler, 0, _event_callback, (void *) &_callback_event_called);
    assertNotNull(parcEvent, "parcEventTimer_Create returned a null reference");

    struct timeval fs = { 0, 1 };
    parcEventTimer_Start(parcEvent, &fs);

    parcEventScheduler_Start(parcEventScheduler, PARCEventSchedulerDispatchType_Blocking);
    assertTrue(_callback_event_called == 1, "Timer event never called back");

    parcEventTimer_Destroy(&parcEvent);
    parcEventScheduler_Destroy(&parcEventScheduler);
}

LONGBOW_TEST_CASE(Global, parc_EventScheduler_Dispatch)
{
    _callback_event_called = 0;

    PARCEventScheduler *parcEventScheduler = parcEventScheduler_Create();
    assertNotNull(parcEventScheduler, "parcEventScheduler_Create returned a null reference");
    PARCEventTimer *parcEvent = parcEventTimer_Create(parcEventScheduler, 0, _event_callback, (void *) &_callback_event_called);
    assertNotNull(parcEvent, "parcEventTimer_Create returned a null reference");

    struct timeval fs = { 0, 1 };
    parcEventTimer_Start(parcEvent, &fs);

    // This will block until the event is processed
    parcEventScheduler_DispatchBlocking(parcEventScheduler);
    assertTrue(_callback_event_called == 1, "Timer event never called back");

    // Start the timer
    struct timeval longerfs = { 1, 0 }; // 1s
    parcEventTimer_Start(parcEvent, &longerfs);
    parcEventScheduler_DispatchNonBlocking(parcEventScheduler);
    assertTrue(_callback_event_called == 1, "Timer event called again prematurely");
    usleep(2000000); // 2s
    parcEventScheduler_DispatchNonBlocking(parcEventScheduler);
    assertTrue(_callback_event_called == 2, "Timer event never called back");

    parcEventTimer_Destroy(&parcEvent);
    parcEventScheduler_Destroy(&parcEventScheduler);
}

static void
_stop_callback(int fd, PARCEventType flags, void *data)
{
    PARCEventScheduler *parcEventScheduler = (PARCEventScheduler *) data;
    struct timeval fs = { 0, 0 };
    parcEventScheduler_Stop(parcEventScheduler, &fs);
    _callback_event_called++;
}

LONGBOW_TEST_CASE(Global, parc_EventScheduler_Stop)
{
    _callback_event_called = 0;

    // Create a new scheduler
    PARCEventScheduler *parcEventScheduler = parcEventScheduler_Create();
    assertNotNull(parcEventScheduler, "parcEventScheduler_Create returned a null reference");

    // Create a persistent event which will repeat until stopped
    PARCEventTimer *parcEvent = parcEventTimer_Create(parcEventScheduler, PARCEventType_Persist, _stop_callback, parcEventScheduler);
    assertNotNull(parcEvent, "parcEventTimer_Create returned a null reference");

    struct timeval fs = { 1, 0 };
    parcEventTimer_Start(parcEvent, &fs);

    parcEventScheduler_Start(parcEventScheduler, PARCEventSchedulerDispatchType_Blocking);
    assertTrue(_callback_event_called == 1, "Timer event never called back");

    parcEventTimer_Destroy(&parcEvent);
    parcEventScheduler_Destroy(&parcEventScheduler);
}

static void
_abort_callback(int fd, PARCEventType flags, void *data)
{
    PARCEventScheduler *parcEventScheduler = (PARCEventScheduler *) data;
    parcEventScheduler_Abort(parcEventScheduler);
}

LONGBOW_TEST_CASE(Global, parc_EventScheduler_Abort)
{
    PARCEventScheduler *parcEventScheduler = parcEventScheduler_Create();
    assertNotNull(parcEventScheduler, "parcEventScheduler_Create returned a null reference");
    PARCEventTimer *parcEvent = parcEventTimer_Create(parcEventScheduler, PARCEventType_Persist, _abort_callback, parcEventScheduler);
    assertNotNull(parcEvent, "parcEventTimer_Create returned a null reference");

    struct timeval fs = { 1, 0 };
    parcEventTimer_Start(parcEvent, &fs);

    parcEventScheduler_Start(parcEventScheduler, PARCEventSchedulerDispatchType_Blocking);

    parcEventTimer_Destroy(&parcEvent);
    parcEventScheduler_Destroy(&parcEventScheduler);
}

static void
_test_memory_event(int fd, short flags, void *data)
{
}

/**
 * Ensure that the scheduler is using parc memory inside libevent
 */
LONGBOW_TEST_CASE(Global, parc_EventScheduler_Memory)
{
    PARCEventScheduler *parcEventScheduler = parcEventScheduler_Create();
    assertNotNull(parcEventScheduler, "parcEventScheduler_Create returned a null reference");

    size_t baseline = parcMemory_Outstanding();

    struct event *testEvent = event_new(parcEventScheduler_GetEvBase(parcEventScheduler), -1, 0, _test_memory_event, NULL);

    assertTrue(parcMemory_Outstanding() > baseline,
               "event_new() did not increase parcMemory_Outstanding: baseline %zu now %u",
               baseline,
               parcMemory_Outstanding());

    event_free(testEvent);

    assertTrue(parcMemory_Outstanding() == baseline,
               "event_free() did reduce to baseline: baseline %zu now %u",
               baseline,
               parcMemory_Outstanding());

    parcEventScheduler_Destroy(&parcEventScheduler);

    assertTrue(parcSafeMemory_ReportAllocation(STDOUT_FILENO) == 0, "Memory imbalance on create/destroy: %u", parcMemory_Outstanding());
}

LONGBOW_TEST_CASE(Global, parc_EventScheduler_GetEvBase)
{
    PARCEventScheduler *parcEventScheduler = parcEventScheduler_Create();
    assertNotNull(parcEventScheduler, "parcEventScheduler_Create returned a null reference");

    assertNotNull(parcEventScheduler_GetEvBase(parcEventScheduler), "Expected a non-null EV pointer.");

    parcEventScheduler_Destroy(&parcEventScheduler);
}

LONGBOW_TEST_CASE(Global, parc_EventScheduler_GetLogger)
{
    PARCEventScheduler *parcEventScheduler = parcEventScheduler_Create();
    assertNotNull(parcEventScheduler, "parcEventScheduler_Create returned a null reference");

    assertNotNull(parcEventScheduler_GetLogger(parcEventScheduler), "Expected a non-null logger.");

    parcEventScheduler_Destroy(&parcEventScheduler);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_EventScheduler);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
