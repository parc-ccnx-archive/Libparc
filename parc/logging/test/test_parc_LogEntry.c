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

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Runner.
#include "../parc_LogEntry.c"

#include <LongBow/unit-test.h>

#include <stdio.h>
#include <pthread.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_SafeMemory.h>
#include <parc/testing/parc_ObjectTesting.h>

#include <parc/logging/parc_LogReporter.h>
#include <parc/logging/parc_LogReporterTextStdout.h>
#include <parc/logging/parc_LogFormatText.h>

LONGBOW_TEST_RUNNER(parc_LogEntry)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified here, but every test must be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Creation);
    LONGBOW_RUN_TEST_FIXTURE(Static);
    LONGBOW_RUN_TEST_FIXTURE(Global);

    LONGBOW_RUN_TEST_FIXTURE(MultiThreaded);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_LogEntry)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_LogEntry)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Creation)
{
    LONGBOW_RUN_TEST_CASE(Creation, parcLogEntry_CreateRelease);
}

uint32_t CreationInitialMemoryOutstanding = 0;

LONGBOW_TEST_FIXTURE_SETUP(Creation)
{
    CreationInitialMemoryOutstanding = parcMemory_Outstanding();

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Creation)
{
    if (parcMemory_Outstanding() != CreationInitialMemoryOutstanding) {
        parcSafeMemory_ReportAllocation(STDOUT_FILENO);
        printf("'%s' leaks memory by %u\n",
               longBowTestCase_GetName(testCase), parcMemory_Outstanding() - CreationInitialMemoryOutstanding);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Creation, parcLogEntry_CreateRelease)
{
    PARCBuffer *payload = parcBuffer_AllocateCString("hello");

    struct timeval timeStamp;
    gettimeofday(&timeStamp, NULL);
    PARCLogEntry *entry =
        parcLogEntry_Create(PARCLogLevel_Info, "hostname", "applicationname", "processid", 12345, timeStamp, payload);
    parcBuffer_Release(&payload);

    parcObjectTesting_AssertAcquireReleaseContract(parcLogEntry_Acquire, entry);
    parcLogEntry_Release(&entry);
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcLogEntry_AcquireRelease);
    LONGBOW_RUN_TEST_CASE(Global, parcLogEntry_GetBuffer);
    LONGBOW_RUN_TEST_CASE(Global, parcLogEntry_GetTimeStamp);
    LONGBOW_RUN_TEST_CASE(Global, parcLogEntry_ToString);
    LONGBOW_RUN_TEST_CASE(Global, parcLogEntry_GetMessageId);
    LONGBOW_RUN_TEST_CASE(Global, parcLogEntry_GetApplicationName);
    LONGBOW_RUN_TEST_CASE(Global, parcLogEntry_GetLevel);
    LONGBOW_RUN_TEST_CASE(Global, parcLogEntry_GetHostName);
    LONGBOW_RUN_TEST_CASE(Global, parcLogEntry_GetLevel);
    LONGBOW_RUN_TEST_CASE(Global, parcLogEntry_GetProcessName);
    LONGBOW_RUN_TEST_CASE(Global, parcLogEntry_GetVersion);
}

uint32_t GlobalInitialMemoryOutstanding = 0;

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    GlobalInitialMemoryOutstanding = parcMemory_Outstanding();

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    if (parcMemory_Outstanding() != GlobalInitialMemoryOutstanding) {
        parcSafeMemory_ReportAllocation(STDOUT_FILENO);
        printf("'%s' leaks memory by %u\n",
               longBowTestCase_GetName(testCase), parcMemory_Outstanding() - GlobalInitialMemoryOutstanding);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, parcLogEntry_AcquireRelease)
{
    PARCBuffer *payload = parcBuffer_AllocateCString("hello");

    struct timeval timeStamp;
    gettimeofday(&timeStamp, NULL);
    PARCLogEntry *entry = parcLogEntry_Create(PARCLogLevel_Info, "hostname", "applicationname", "processid", 1234, timeStamp, payload);
    parcBuffer_Release(&payload);

    parcObjectTesting_AssertAcquireReleaseContract(parcLogEntry_Acquire, entry);

    parcLogEntry_Release(&entry);
}

LONGBOW_TEST_CASE(Global, parcLogEntry_GetBuffer)
{
    PARCBuffer *payload = parcBuffer_AllocateCString("hello");

    struct timeval timeStamp;
    gettimeofday(&timeStamp, NULL);
    PARCLogEntry *entry = parcLogEntry_Create(PARCLogLevel_Info, "hostname", "applicationname", "processid", 1234, timeStamp, payload);
    PARCBuffer *actual = parcLogEntry_GetPayload(entry);

    assertTrue(payload == actual, "Expected %p, actual %p", (void *) payload, (void *) actual);
    parcBuffer_Release(&payload);

    parcLogEntry_Release(&entry);
}

LONGBOW_TEST_CASE(Global, parcLogEntry_GetTimeStamp)
{
    PARCBuffer *payload = parcBuffer_AllocateCString("hello");

    struct timeval timeStamp;
    gettimeofday(&timeStamp, NULL);
    PARCLogEntry *entry =
        parcLogEntry_Create(PARCLogLevel_Info, "hostname", "applicationname", "processid", 1234, timeStamp, payload);
    const struct timeval *actual = parcLogEntry_GetTimeStamp(entry);

    assertTrue(memcmp(&timeStamp, actual, sizeof(struct timeval)) == 0, "Expected timeStamp to be identical");
    parcBuffer_Release(&payload);

    parcLogEntry_Release(&entry);
}

LONGBOW_TEST_CASE(Global, parcLogEntry_GetLevel)
{
    PARCBuffer *payload = parcBuffer_AllocateCString("hello");

    struct timeval timeStamp;
    gettimeofday(&timeStamp, NULL);
    PARCLogEntry *entry =
        parcLogEntry_Create(PARCLogLevel_Info, "hostname", "applicationname", "processid", 1234, timeStamp, payload);
    const PARCLogLevel actual = parcLogEntry_GetLevel(entry);

    assertTrue(PARCLogLevel_Info == actual, "Expected %d, actual %d", PARCLogLevel_Info, actual);
    parcBuffer_Release(&payload);

    parcLogEntry_Release(&entry);
}

LONGBOW_TEST_CASE(Global, parcLogEntry_GetVersion)
{
    PARCBuffer *payload = parcBuffer_AllocateCString("hello");

    struct timeval timeStamp;
    gettimeofday(&timeStamp, NULL);
    PARCLogEntry *entry =
        parcLogEntry_Create(PARCLogLevel_Info, "hostname", "applicationname", "processid", 1234, timeStamp, payload);
    const PARCLogLevel actual = parcLogEntry_GetVersion(entry);

    assertTrue(_parcLog_Version == actual, "Expected %d, actual %d", _parcLog_Version, actual);
    parcBuffer_Release(&payload);

    parcLogEntry_Release(&entry);
}

LONGBOW_TEST_CASE(Global, parcLogEntry_GetHostName)
{
    PARCBuffer *payload = parcBuffer_AllocateCString("hello");
    char *expected = "hostname";

    struct timeval timeStamp;
    gettimeofday(&timeStamp, NULL);
    PARCLogEntry *entry =
        parcLogEntry_Create(PARCLogLevel_Info, expected, "applicationname", "processid", 1234, timeStamp, payload);
    const char *actual = parcLogEntry_GetHostName(entry);

    assertTrue(strcmp(expected, actual) == 0, "Expected %s, actual %s", expected, actual);
    parcBuffer_Release(&payload);

    parcLogEntry_Release(&entry);
}

LONGBOW_TEST_CASE(Global, parcLogEntry_GetApplicationName)
{
    PARCBuffer *payload = parcBuffer_AllocateCString("hello");
    char *expected = "applicationname";

    struct timeval timeStamp;
    gettimeofday(&timeStamp, NULL);
    PARCLogEntry *entry =
        parcLogEntry_Create(PARCLogLevel_Info, "hostname", "applicationname", "processid", 1234, timeStamp, payload);
    const char *actual = parcLogEntry_GetApplicationName(entry);

    assertTrue(strcmp(expected, actual) == 0, "Expected %s, actual %s", expected, actual);
    parcBuffer_Release(&payload);

    parcLogEntry_Release(&entry);
}

LONGBOW_TEST_CASE(Global, parcLogEntry_GetProcessName)
{
    PARCBuffer *payload = parcBuffer_AllocateCString("hello");
    char *expected = "processid";

    struct timeval timeStamp;
    gettimeofday(&timeStamp, NULL);
    PARCLogEntry *entry =
        parcLogEntry_Create(PARCLogLevel_Info, "hostname", "applicationname", expected, 1234, timeStamp, payload);
    const char *actual = parcLogEntry_GetProcessName(entry);

    assertTrue(strcmp(expected, actual) == 0, "Expected %s, actual %s", expected, actual);
    parcBuffer_Release(&payload);

    parcLogEntry_Release(&entry);
}

LONGBOW_TEST_CASE(Global, parcLogEntry_GetMessageId)
{
    PARCBuffer *payload = parcBuffer_AllocateCString("hello");

    struct timeval timeStamp;
    gettimeofday(&timeStamp, NULL);

    uint64_t expected = 1234;
    PARCLogEntry *entry =
        parcLogEntry_Create(PARCLogLevel_Info, "hostname", "applicationname", "processid", expected, timeStamp, payload);
    const uint64_t actual = parcLogEntry_GetMessageId(entry);

    assertTrue(expected == actual, "Expected %" PRId64 " actual %" PRId64 "", expected, actual);
    parcBuffer_Release(&payload);

    parcLogEntry_Release(&entry);
}

LONGBOW_TEST_CASE(Global, parcLogEntry_ToString)
{
    PARCBuffer *payload = parcBuffer_AllocateCString("hello");

    struct timeval timeStamp;
    gettimeofday(&timeStamp, NULL);
    PARCLogEntry *entry =
        parcLogEntry_Create(PARCLogLevel_Info, "hostname", "applicationname", "processid", 1234, timeStamp, payload);

    parcBuffer_Release(&payload);

    char *actual = parcLogEntry_ToString(entry);

    parcMemory_Deallocate((void **) &actual);

    parcLogEntry_Release(&entry);
}

LONGBOW_TEST_FIXTURE(Static)
{
    LONGBOW_RUN_TEST_CASE(Static, _parcLogEntry_Destroy);
    LONGBOW_RUN_TEST_CASE(Static, _toString);
}

LONGBOW_TEST_FIXTURE_SETUP(Static)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Static)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Static, _parcLogEntry_Destroy)
{
    testUnimplemented("");
}

LONGBOW_TEST_CASE(Static, _toString)
{
    testUnimplemented("");
}

// Multi-threaded test

LONGBOW_TEST_FIXTURE(MultiThreaded)
{
    LONGBOW_RUN_TEST_CASE(MultiThreaded, fgThreadTest);
    LONGBOW_RUN_TEST_CASE(MultiThreaded, bgThreadTest);
}

LONGBOW_TEST_FIXTURE_SETUP(MultiThreaded)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(MultiThreaded)
{
   if (parcSafeMemory_ReportAllocation(STDOUT_FILENO) != 0) {
        //parcSafeMemory_ReportAllocation(STDOUT_FILENO);
        printf("'%s' leaks memory by %u allocations\n",
               longBowTestCase_GetName(testCase), parcMemory_Outstanding() - CreationInitialMemoryOutstanding);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}


void *_runInThread(void *threadLabel);
void *_runInThread_Stripped(void *threadLabel);

static int _loopCount = INT32_MAX;

void *
_runInThread(void *threadLabel)  // Look at _runInThread_Stripped(), below, instead of this one.
{
    PARCLogReporter *reporter = parcLogReporterTextStdout_Create();
    PARCBuffer *payload = parcBuffer_AllocateCString(threadLabel);
    struct timeval timeStamp;
    gettimeofday(&timeStamp, NULL);

    PARCLogEntry *entry = parcLogEntry_Create(PARCLogLevel_Info, "hostName", "applicationName", "processName", _loopCount, timeStamp, payload);

    while (_loopCount > 0) {

        PARCBuffer *buf = parcLogFormatText_FormatEntry(entry);

        parcLogReporterTextStdout_Report(reporter, entry);
        parcBuffer_Release(&buf);

        _loopCount--;

        usleep(10 * 1000); // yield for a bit to let another thread have at it.
    }

    parcLogEntry_Release(&entry);

    parcBuffer_Release(&payload);
    parcLogReporter_Release(&reporter);

    return threadLabel; // Unchanged from what was passed in.
}


void *
_runInThread_Stripped(void *threadLabel)
{
    PARCBuffer *payload = parcBuffer_AllocateCString(threadLabel);

    while (_loopCount > 0) {

        //
        // The code below taken from parcLogReporterTextStdout_Report(). I stripped it down some.
        // If you switch the thread's job from _runInThread_Stripped to _runInThread you can run
        // the original, which shows the same thing.
        //

        PARCBufferComposer *composer = parcBufferComposer_Allocate(128);

        parcBufferComposer_Format(composer, "%d [ ", _loopCount);
        parcBufferComposer_PutBuffer(composer, payload);
        parcBufferComposer_PutStrings(composer, " ]\n", NULL);

        PARCBuffer *result = parcBuffer_Flip(parcBuffer_Acquire(parcBufferComposer_GetBuffer(composer)));
        parcBufferComposer_Release(&composer);

        char *string = parcBuffer_ToString(result);
        parcBuffer_Release(&result);

        printf("%s", string);

        parcMemory_Deallocate((void **)&string);

        _loopCount--;

        usleep(10 * 1000); // yield for a bit to let another thread have at it.
    }

    parcBuffer_Release(&payload);

    return threadLabel; // Unchanged from what was passed in.
}


LONGBOW_TEST_CASE(MultiThreaded, bgThreadTest)
{
    int numThreads = 2;
    pthread_t workerThreads[numThreads];
    char *threadLabel[numThreads];

    _loopCount = INT32_MAX; // We'll set it to 0 after a second
    for (int i = 0; i < numThreads; i++) {

        if (asprintf(&threadLabel[i], "bg thread #%d", i) > 0) {
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

            pthread_create(&workerThreads[i], &attr, _runInThread_Stripped, threadLabel[i]);
        }
        //pthread_create(&workerThreads[i], &attr, _runInThread, threadLabel[i]);
    }

    sleep(2);  // Let the bg threads run

    _loopCount = 0;  // tell the bg threads to stop

    for (int i = 0; i < numThreads; i++) {
        int status = pthread_join(workerThreads[i], NULL);
        printf("Child %d (out of %d) joined with status %d\n", i, numThreads, status);
        free(threadLabel[i]);
    }
}

LONGBOW_TEST_CASE(MultiThreaded, fgThreadTest)
{
    _loopCount = 10;
    //_runInThread("main thread");   // Run the same logging loop, but in a single thread
    _runInThread_Stripped("main thread");   // Run the same logging loop, but in a single thread
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_LogEntry);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
