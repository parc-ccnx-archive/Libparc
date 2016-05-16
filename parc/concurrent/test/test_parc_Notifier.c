/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../parc_Notifier.c"

#include <sys/time.h>
#include <pthread.h>
#include <poll.h>

#include <parc/algol/parc_SafeMemory.h>
#include <LongBow/unit-test.h>

LONGBOW_TEST_RUNNER(parc_Notifier)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Local);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_Notifier)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_Notifier)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcNotifier_Acquire);
    LONGBOW_RUN_TEST_CASE(Global, parcNotifier_Create_Release);
    LONGBOW_RUN_TEST_CASE(Global, parcNotifier_PauseEvent_NotPaused);
    LONGBOW_RUN_TEST_CASE(Global, parcNotifier_PauseEvent_AlreadyPaused);
    LONGBOW_RUN_TEST_CASE(Global, parcNotifier_StartEvents);

    LONGBOW_RUN_TEST_CASE(Global, parcNotifier_Notify_First);
    LONGBOW_RUN_TEST_CASE(Global, parcNotifier_Notify_Twice);

    LONGBOW_RUN_TEST_CASE(Global, parcNotifier_ThreadedTest);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    if (parcSafeMemory_ReportAllocation(STDOUT_FILENO) != 0) {
        printf("('%s' leaks memory by %d (allocs - frees)) ", longBowTestCase_GetName(testCase), parcMemory_Outstanding());
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

// ------
typedef struct test_data {
    volatile int barrier;
    PARCNotifier *notifier;

    unsigned notificationsToSend;
    unsigned notificationsSent;

    unsigned notificationsToRecieve;
    unsigned notificationsReceived;

    pthread_t producerThread;
    pthread_t consumerThread;
} TestData;


void *
consumer(void *p)
{
    TestData *data = (TestData *) p;
    --data->barrier;
    while (data->barrier) {
        ;
    }

    struct pollfd pfd;
    pfd.fd = parcNotifier_Socket(data->notifier);
    pfd.events = POLLIN;

    while (data->notificationsReceived < data->notificationsToRecieve) {
        if (poll(&pfd, 1, -1)) {
            data->notificationsReceived++;
            parcNotifier_PauseEvents(data->notifier);
            usleep(rand() % 1024 + 1024);
            printf("skipped = %d\n", data->notifier->skippedNotify);
            parcNotifier_StartEvents(data->notifier);
        }
    }

    --data->barrier;

    printf("Consumer exiting: received %d\n", data->notificationsReceived);
    pthread_exit((void *) NULL);
}

void *
producer(void *p)
{
    TestData *data = (TestData *) p;
    --data->barrier;
    while (data->barrier) {
        ;
    }

    while (data->barrier == 0) {
        if (parcNotifier_Notify(data->notifier)) {
        }
        data->notificationsSent++;
        usleep(rand() % 1024 + 512);
    }

    printf("Producer exiting: sent %d\n", data->notificationsSent);
    pthread_exit((void *) NULL);
}

LONGBOW_TEST_CASE(Global, parcNotifier_Acquire)
{
    testUnimplemented("");
}

LONGBOW_TEST_CASE(Global, parcNotifier_Create_Release)
{
    testUnimplemented("");
}

LONGBOW_TEST_CASE(Global, parcNotifier_PauseEvent_NotPaused)
{
    PARCNotifier *notifier = parcNotifier_Create();

    parcNotifier_PauseEvents(notifier);
    assertTrue(notifier->paused == 1, "Not paused, got %d expected %d", notifier->paused, 1);
    assertTrue(notifier->skippedNotify == 0, "Wrong skipped, got %d expected %d", notifier->skippedNotify, 0);

    parcNotifier_Release(&notifier);
}

LONGBOW_TEST_CASE(Global, parcNotifier_PauseEvent_AlreadyPaused)
{
    PARCNotifier *notifier = parcNotifier_Create();

    parcNotifier_PauseEvents(notifier);

    // now pause again
    parcNotifier_PauseEvents(notifier);

    assertTrue(notifier->paused == 1, "Not paused, got %d expected %d", notifier->paused, 1);
    assertTrue(notifier->skippedNotify == 0, "Wrong skipped, got %d expected %d", notifier->skippedNotify, 0);

    parcNotifier_Release(&notifier);
}


LONGBOW_TEST_CASE(Global, parcNotifier_ThreadedTest)
{
    TestData *data = parcMemory_AllocateAndClear(sizeof(TestData));
    assertNotNull(data, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(TestData));

    data->notifier = parcNotifier_Create();
    data->notificationsToSend = 10;
    data->notificationsToRecieve = data->notificationsToSend;
    data->notificationsSent = 0;
    data->notificationsReceived = 0;
    data->barrier = 2;

    pthread_create(&data->consumerThread, NULL, consumer, data);
    pthread_create(&data->producerThread, NULL, producer, data);

    // wait for them to exit
    pthread_join(data->producerThread, NULL);
    pthread_join(data->consumerThread, NULL);

    assertTrue(data->notificationsReceived >= data->notificationsToRecieve,
               "Did not write all items got %u expected %u\n",
               data->notificationsReceived,
               data->notificationsToRecieve);

    parcNotifier_Release(&data->notifier);
    parcMemory_Deallocate((void **) &data);
}

LONGBOW_TEST_CASE(Global, parcNotifier_StartEvents)
{
    testUnimplemented("unimplemented");
}

LONGBOW_TEST_CASE(Global, parcNotifier_Notify_First)
{
    PARCNotifier *notifier = parcNotifier_Create();

    bool success = parcNotifier_Notify(notifier);
    assertTrue(success, "Did not succeed on first notify");
    assertTrue(notifier->paused == 1, "Not paused, got %d expected %d", notifier->paused, 1);
    assertTrue(notifier->skippedNotify == 0, "Wrong skipped, got %d expected %d", notifier->skippedNotify, 0);

    parcNotifier_Release(&notifier);
}


LONGBOW_TEST_CASE(Global, parcNotifier_Notify_Twice)
{
    PARCNotifier *notifier = parcNotifier_Create();

    parcNotifier_Notify(notifier);

    bool success = parcNotifier_Notify(notifier);
    assertFalse(success, "Should have failed on second notify");
    assertTrue(notifier->paused == 1, "Not paused, got %d expected %d", notifier->paused, 1);
    assertTrue(notifier->skippedNotify == 1, "Wrong skipped, got %d expected %d", notifier->skippedNotify, 1);

    parcNotifier_Release(&notifier);
}

// ===============================================================

LONGBOW_TEST_FIXTURE(Local)
{
}

LONGBOW_TEST_FIXTURE_SETUP(Local)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Local)
{
    if (parcSafeMemory_ReportAllocation(STDOUT_FILENO) != 0) {
        printf("('%s' leaks memory by %d (allocs - frees)) ", longBowTestCase_GetName(testCase), parcMemory_Outstanding());
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_Notifier);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
