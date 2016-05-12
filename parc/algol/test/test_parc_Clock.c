/*
 * Copyright (c) 2014-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2014-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../parc_Clock.c"
#include <stdio.h>
#include <LongBow/unit-test.h>
#include <parc/algol/parc_SafeMemory.h>

LONGBOW_TEST_RUNNER(parc_Clock)
{
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_Clock)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_Clock)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// ==========================================================

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcClock_Wallclock);
    LONGBOW_RUN_TEST_CASE(Global, parcClock_Wallclock_Acquire);
    LONGBOW_RUN_TEST_CASE(Global, parcClock_Wallclock_GetTime);
    LONGBOW_RUN_TEST_CASE(Global, parcClock_Wallclock_GetTimeval);
    LONGBOW_RUN_TEST_CASE(Global, parcClock_Montonic);
    LONGBOW_RUN_TEST_CASE(Global, parcClock_Monotonic_Acquire);
    LONGBOW_RUN_TEST_CASE(Global, parcClock_Monotonic_GetTime);
    LONGBOW_RUN_TEST_CASE(Global, parcClock_Monotonic_GetTimeval);

    LONGBOW_RUN_TEST_CASE(Global, counterClock_Create);
    LONGBOW_RUN_TEST_CASE(Global, counterClock_Acquire);
    LONGBOW_RUN_TEST_CASE(Global, counterClock_GetTime);
    LONGBOW_RUN_TEST_CASE(Global, counterClock_GetTime_Twice);
    LONGBOW_RUN_TEST_CASE(Global, counterClock_GetTimeval);

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

LONGBOW_TEST_CASE(Global, parcClock_Wallclock)
{
    PARCClock *clock = parcClock_Wallclock();
    assertNotNull(clock, "Got null wall clock");
    parcClock_Release(&clock);
}

LONGBOW_TEST_CASE(Global, parcClock_Wallclock_Acquire)
{
    PARCClock *clock = parcClock_Wallclock();
    PARCClock *copy = parcClock_Acquire(clock);
    assertNotNull(copy, "Got null wall clock");
    parcClock_Release(&copy);
    parcClock_Release(&clock);
}

LONGBOW_TEST_CASE(Global, parcClock_Wallclock_GetTime)
{
    PARCClock *clock = parcClock_Wallclock();
    uint64_t t = parcClock_GetTime(clock);
    parcClock_Release(&clock);
    assertTrue(t > 0, "got 0 time");
}

LONGBOW_TEST_CASE(Global, parcClock_Wallclock_GetTimeval)
{
    PARCClock *clock = parcClock_Wallclock();
    struct timeval tv = { 0, 0};
    parcClock_GetTimeval(clock, &tv);
    parcClock_Release(&clock);
    assertTrue(tv.tv_sec > 0, "Got 0 seconds");
}

// -----

LONGBOW_TEST_CASE(Global, parcClock_Montonic)
{
    PARCClock *clock = parcClock_Monotonic();
    assertNotNull(clock, "Got null wall clock");
    parcClock_Release(&clock);
}

LONGBOW_TEST_CASE(Global, parcClock_Monotonic_Acquire)
{
    PARCClock *clock = parcClock_Monotonic();
    PARCClock *copy = parcClock_Acquire(clock);
    assertNotNull(copy, "Got null wall clock");
}

LONGBOW_TEST_CASE(Global, parcClock_Monotonic_GetTime)
{
    PARCClock *clock = parcClock_Monotonic();
    uint64_t t = parcClock_GetTime(clock);
    parcClock_Release(&clock);
    assertTrue(t > 0, "got 0 time");
}

LONGBOW_TEST_CASE(Global, parcClock_Monotonic_GetTimeval)
{
    PARCClock *clock = parcClock_Monotonic();
    struct timeval tv = { 0, 0};
    parcClock_GetTimeval(clock, &tv);
    parcClock_Release(&clock);
    assertTrue(tv.tv_sec > 0, "Got 0 seconds");
}

// -----

LONGBOW_TEST_CASE(Global, counterClock_Create)
{
    PARCClock *clock = parcClock_Counter();
    assertNotNull(clock, "Got null wall clock");
    parcClock_Release(&clock);
}

LONGBOW_TEST_CASE(Global, counterClock_Acquire)
{
    PARCClock *clock = parcClock_Counter();
    PARCClock *copy = parcClock_Acquire(clock);
    assertNotNull(copy, "Got null wall clock");
    parcClock_Release(&copy);
    parcClock_Release(&clock);
}

LONGBOW_TEST_CASE(Global, counterClock_GetTime)
{
    PARCClock *clock = parcClock_Counter();
    uint64_t t = parcClock_GetTime(clock);
    parcClock_Release(&clock);
    assertTrue(t == 1, "On first call should have gotten 1");
}

LONGBOW_TEST_CASE(Global, counterClock_GetTime_Twice)
{
    PARCClock *clock = parcClock_Counter();
    parcClock_GetTime(clock);
    uint64_t t2 = parcClock_GetTime(clock);
    parcClock_Release(&clock);
    assertTrue(t2 == 2, "On second call should have gotten 2");
}

LONGBOW_TEST_CASE(Global, counterClock_GetTimeval)
{
    PARCClock *clock = parcClock_Counter();
    struct timeval tv = { 0, 0};
    parcClock_GetTimeval(clock, &tv);
    parcClock_Release(&clock);
    assertTrue(tv.tv_usec == 1, "On first call should have gotten 1 usec");
}


// ==========================================================

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_Clock);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}

