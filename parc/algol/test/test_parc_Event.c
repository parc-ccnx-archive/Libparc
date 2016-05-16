/*
 * Copyright (c) 2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
#include <config.h>

#include <LongBow/unit-test.h>

#include <parc/algol/parc_SafeMemory.h>

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../parc_Event.c"

LONGBOW_TEST_RUNNER(parc_Event)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_Event)
{
    parcEvent_EnableDebug();
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_Event)
{
    parcEvent_DisableDebug();
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parc_Event_Create_Destroy);
    LONGBOW_RUN_TEST_CASE(Global, parc_Event_Start);
    LONGBOW_RUN_TEST_CASE(Global, parc_Event_Stop);
    LONGBOW_RUN_TEST_CASE(Global, parc_Event_Poll);
    LONGBOW_RUN_TEST_CASE(Global, parc_Event_SetPriority);
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

LONGBOW_TEST_CASE(Global, parc_Event_Create_Destroy)
{
    int fds[2];
    int result = socketpair(AF_LOCAL, SOCK_DGRAM, 0, fds);
    assertFalse(result, "Socketpair creation failed.\n");

    PARCEventScheduler *parcEventScheduler = parcEventScheduler_Create();
    assertNotNull(parcEventScheduler, "parcEventScheduler_Create returned a null reference");
    PARCEvent *parcEvent = parcEvent_Create(parcEventScheduler, fds[0], PARCEventType_Write, _test_event, NULL);
    assertNotNull(parcEvent, "parcEvent_Create returned a null reference");
    parcEvent_Destroy(&parcEvent);
    parcEventScheduler_Destroy(&parcEventScheduler);

    close(fds[0]);
    close(fds[1]);
}

LONGBOW_TEST_CASE(Global, parc_Event_Start)
{
    int fds[2];
    int result = socketpair(AF_LOCAL, SOCK_DGRAM, 0, fds);
    assertFalse(result, "Socketpair creation failed.\n");

    PARCEventScheduler *parcEventScheduler = parcEventScheduler_Create();
    assertNotNull(parcEventScheduler, "parcEventScheduler_Create returned a null reference");
    PARCEvent *parcEvent = parcEvent_Create(parcEventScheduler, fds[0], PARCEventType_Read | PARCEventType_Write, _test_event, NULL);
    assertNotNull(parcEvent, "parcEvent_Create returned a null reference");

    _test_event_called = 0;
    parcEvent_Start(parcEvent);
    parcEventScheduler_Start(parcEventScheduler, PARCEventSchedulerDispatchType_Blocking);
    assertTrue(_test_event_called == 1, "Event never called.");

    parcEvent_Destroy(&parcEvent);
    parcEventScheduler_Destroy(&parcEventScheduler);
    close(fds[0]);
    close(fds[1]);
}

static int _test_stop_event_called = 0;

static void
_test_stop_event(int fd, PARCEventType flags, void *data)
{
    PARCEvent **parcEvent = (PARCEvent **) data;
    _test_stop_event_called++;
    parcEvent_Stop(*parcEvent);
}

LONGBOW_TEST_CASE(Global, parc_Event_Stop)
{
    int fds[2];
    int result = socketpair(AF_LOCAL, SOCK_DGRAM, 0, fds);
    assertFalse(result, "Socketpair creation failed.\n");

    PARCEventScheduler *parcEventScheduler = parcEventScheduler_Create();
    assertNotNull(parcEventScheduler, "parcEventScheduler_Create returned a null reference");
    PARCEvent *parcEvent = parcEvent_Create(parcEventScheduler, fds[0], PARCEventType_Write | PARCEventType_Persist, _test_stop_event, &parcEvent);
    assertNotNull(parcEvent, "parcEvent_Create returned a null reference");

    parcEvent_Start(parcEvent);
    parcEventScheduler_Start(parcEventScheduler, PARCEventSchedulerDispatchType_Blocking);
    assertTrue(_test_stop_event_called > 0, "Event never called.");
    assertFalse(_test_stop_event_called != 1, "Event called more than once.");

    parcEvent_Destroy(&parcEvent);
    parcEventScheduler_Destroy(&parcEventScheduler);
    close(fds[0]);
    close(fds[1]);
}

LONGBOW_TEST_CASE(Global, parc_Event_Poll)
{
    int fds[2];
    int result = socketpair(AF_LOCAL, SOCK_DGRAM, 0, fds);
    assertFalse(result, "Socketpair creation failed.\n");

    PARCEventScheduler *parcEventScheduler = parcEventScheduler_Create();
    assertNotNull(parcEventScheduler, "parcEventScheduler_Create returned a null reference");
    PARCEvent *parcEvent = parcEvent_Create(parcEventScheduler, fds[0], PARCEventType_Write, _test_event, NULL);
    assertNotNull(parcEvent, "parcEvent_Create returned a null reference");

    result = parcEvent_Poll(parcEvent, PARCEventType_Read);
    // should be no outstanding events
    assertTrue(result == 0, "parcEvent_Poll returned %d\n", result);

    parcEvent_Destroy(&parcEvent);
    parcEventScheduler_Destroy(&parcEventScheduler);
    close(fds[0]);
    close(fds[1]);
}

static int _test_writeMaxPriority_event_called = 0;

static void
_test_writeMaxPriority_event(int fd, PARCEventType flags, void *data)
{
    PARCEvent *parcEvent = *((PARCEvent **)data);
    parcEvent_Stop(parcEvent);
    _test_writeMaxPriority_event_called++;
}

static int _test_writeMinPriority_event_called = 0;

static void
_test_writeMinPriority_event(int fd, PARCEventType flags, void *data)
{
    PARCEvent *parcEvent = *((PARCEvent **)data);
    parcEvent_Stop(parcEvent);
    _test_writeMinPriority_event_called++;
}

LONGBOW_TEST_CASE(Global, parc_Event_SetPriority)
{
    int fds[2];
    int result = socketpair(AF_LOCAL, SOCK_DGRAM, 0, fds);
    assertFalse(result, "Socketpair creation failed.\n");

    PARCEventScheduler *parcEventScheduler = parcEventScheduler_Create();
    assertNotNull(parcEventScheduler, "parcEventScheduler_Create returned a null reference");

    //
    // First event to be called back disables its partner's event
    //
    PARCEvent *parcEventWriteMax, *parcEventWriteMin;
    parcEventWriteMax = parcEvent_Create(parcEventScheduler, fds[0],
                                         PARCEventType_Write,
                                         _test_writeMaxPriority_event,
                                         (void *)&parcEventWriteMin);
    assertNotNull(parcEventWriteMax, "parcEvent_Create returned a null reference");
    parcEventWriteMin = parcEvent_Create(parcEventScheduler, fds[1],
                                         PARCEventType_Write,
                                         _test_writeMinPriority_event,
                                         (void *)&parcEventWriteMax);
    assertNotNull(parcEventWriteMin, "parcEvent_Create returned a null reference");

    result = parcEvent_SetPriority(parcEventWriteMin, PARCEventPriority_Minimum);
    assertTrue(result == 0, "parcEvent_SetPriority write returned %d\n", result);
    result = parcEvent_SetPriority(parcEventWriteMax, PARCEventPriority_Maximum);
    assertTrue(result == 0, "parcEvent_SetPriority read returned %d\n", result);

    parcEvent_Start(parcEventWriteMin);
    parcEvent_Start(parcEventWriteMax);

    parcEventScheduler_Start(parcEventScheduler, PARCEventSchedulerDispatchType_NonBlocking);

    assertTrue(_test_writeMaxPriority_event_called == 1, "Read event called before priority write event handled");
    assertTrue(_test_writeMinPriority_event_called == 0, "Write event never triggered");

    parcEvent_Destroy(&parcEventWriteMax);
    parcEvent_Destroy(&parcEventWriteMin);
    parcEventScheduler_Destroy(&parcEventScheduler);
    close(fds[0]);
    close(fds[1]);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_Event);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
