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
/** *
 * @author Glenn Scott <Glenn.Scott@parc.com>, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../parc_Log.c"

#include <parc/logging/parc_LogLevel.h>
#include <parc/logging/parc_LogReporterFile.h>

#include <parc/algol/parc_FileOutputStream.h>
#include <parc/algol/parc_SafeMemory.h>

#include <LongBow/testing.h>
#include <LongBow/debugging.h>

LONGBOW_TEST_RUNNER(test_parc_Log)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(CreateDestroy);
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(test_parc_Log)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(test_parc_Log)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

uint32_t CreationInitialMemoryOutstanding = 0;

LONGBOW_TEST_FIXTURE(CreateDestroy)
{
    LONGBOW_RUN_TEST_CASE(CreateDestroy, parcLog_Create);
    LONGBOW_RUN_TEST_CASE(CreateDestroy, parcLog_Create_DefaultValues);
}

LONGBOW_TEST_FIXTURE_SETUP(CreateDestroy)
{
    CreationInitialMemoryOutstanding = parcMemory_Outstanding();

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(CreateDestroy)
{
    if (parcMemory_Outstanding() != CreationInitialMemoryOutstanding) {
        parcSafeMemory_ReportAllocation(STDOUT_FILENO);
        printf("'%s' leaks memory by %u\n",
               longBowTestCase_GetName(testCase), parcMemory_Outstanding() - CreationInitialMemoryOutstanding);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(CreateDestroy, parcLog_Create)
{
    PARCFileOutputStream *fileOutput = parcFileOutputStream_Create(dup(STDOUT_FILENO));
    PARCOutputStream *output = parcFileOutputStream_AsOutputStream(fileOutput);
    parcFileOutputStream_Release(&fileOutput);

    PARCLogReporter *reporter = parcLogReporterFile_Create(output);
    parcOutputStream_Release(&output);

    PARCLog *log = parcLog_Create("localhost", "test_parc_Log", NULL, reporter);
    parcLogReporter_Release(&reporter);

    assertTrue(parcLogLevel_Equals(parcLog_GetLevel(log), PARCLogLevel_Off), "Expected initial log level to be OFF");

    parcLog_Release(&log);
}

LONGBOW_TEST_CASE(CreateDestroy, parcLog_Create_DefaultValues)
{
    PARCFileOutputStream *fileOutput = parcFileOutputStream_Create(dup(STDOUT_FILENO));
    PARCOutputStream *output = parcFileOutputStream_AsOutputStream(fileOutput);
    parcFileOutputStream_Release(&fileOutput);

    PARCLogReporter *reporter = parcLogReporterFile_Create(output);
    parcOutputStream_Release(&output);

    PARCLog *log = parcLog_Create(NULL, NULL, NULL, reporter);
    parcLogReporter_Release(&reporter);

    assertTrue(parcLogLevel_Equals(parcLog_GetLevel(log), PARCLogLevel_Off), "Expected initial log level to be OFF");

    parcLog_Release(&log);
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcLog_Emergency);
    LONGBOW_RUN_TEST_CASE(Global, parcLog_Warning);
    LONGBOW_RUN_TEST_CASE(Global, parcLog_Alert);
    LONGBOW_RUN_TEST_CASE(Global, parcLog_Critical);
    LONGBOW_RUN_TEST_CASE(Global, parcLog_Error);
    LONGBOW_RUN_TEST_CASE(Global, parcLog_Notice);
    LONGBOW_RUN_TEST_CASE(Global, parcLog_Debug);
    LONGBOW_RUN_TEST_CASE(Global, parcLog_Info);
    LONGBOW_RUN_TEST_CASE(Global, parcLog_Message);
    LONGBOW_RUN_TEST_CASE(Global, parcLog_IsLoggable_True);
    LONGBOW_RUN_TEST_CASE(Global, parcLog_IsLoggable_False);

    LONGBOW_RUN_TEST_CASE(Global, parcLog_Emergency_WrongLevel);
    LONGBOW_RUN_TEST_CASE(Global, parcLog_Warning_WrongLevel);
    LONGBOW_RUN_TEST_CASE(Global, parcLog_Alert_WrongLevel);
    LONGBOW_RUN_TEST_CASE(Global, parcLog_Critical_WrongLevel);
    LONGBOW_RUN_TEST_CASE(Global, parcLog_Error_WrongLevel);
    LONGBOW_RUN_TEST_CASE(Global, parcLog_Notice_WrongLevel);
    LONGBOW_RUN_TEST_CASE(Global, parcLog_Debug_WrongLevel);
    LONGBOW_RUN_TEST_CASE(Global, parcLog_Info_WrongLevel);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    CreationInitialMemoryOutstanding = parcMemory_Outstanding();

    {
        PARCFileOutputStream *fileOutput = parcFileOutputStream_Create(dup(STDOUT_FILENO));
        PARCOutputStream *output = parcFileOutputStream_AsOutputStream(fileOutput);
        parcFileOutputStream_Release(&fileOutput);

        PARCLogReporter *reporter = parcLogReporterFile_Create(output);
        parcOutputStream_Release(&output);

        PARCLog *log = parcLog_Create("localhost", "test_parc_Log", NULL, reporter);
        parcLogReporter_Release(&reporter);

        parcLog_SetLevel(log, PARCLogLevel_All);
        
        longBowTestCase_SetClipBoardData(testCase, log);
    }

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    {
        PARCLog *log =  (PARCLog *) longBowTestCase_GetClipBoardData(testCase);
        parcLog_Release(&log);
    }

    if (parcMemory_Outstanding() != CreationInitialMemoryOutstanding) {
        parcSafeMemory_ReportAllocation(STDOUT_FILENO);
        printf("'%s' leaks memory by %u\n",
               longBowTestCase_GetName(testCase), parcMemory_Outstanding() - CreationInitialMemoryOutstanding);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, parcLog_IsLoggable_True)
{
    PARCLog *log = (PARCLog *) longBowTestCase_GetClipBoardData(testCase);

    assertTrue(parcLog_IsLoggable(log, PARCLogLevel_Alert), "Expected parcLog_IsLoggable to be true.");
}

LONGBOW_TEST_CASE(Global, parcLog_IsLoggable_False)
{
    PARCLog *log = (PARCLog *) longBowTestCase_GetClipBoardData(testCase);

    parcLog_SetLevel(log, PARCLogLevel_Off);

    assertFalse(parcLog_IsLoggable(log, PARCLogLevel_Alert), "Expected parcLog_IsLoggable to be true.");
}

LONGBOW_TEST_CASE(Global, parcLog_Info)
{
    PARCLog *log = (PARCLog *) longBowTestCase_GetClipBoardData(testCase);

    assertTrue(parcLog_Info(log, "This is a info message"),
               "Expected message to be logged successfully");
}

LONGBOW_TEST_CASE(Global, parcLog_Warning)
{
    PARCLog *log = (PARCLog *) longBowTestCase_GetClipBoardData(testCase);

    assertTrue(parcLog_Warning(log, "This is a warning message"),
               "Expected message to be logged successfully");
}

LONGBOW_TEST_CASE(Global, parcLog_Emergency)
{
    PARCLog *log = (PARCLog *) longBowTestCase_GetClipBoardData(testCase);

    assertTrue(parcLog_Emergency(log, "This is an emergency message"),
               "Expected message to be logged successfully");
}

LONGBOW_TEST_CASE(Global, parcLog_Alert)
{
    PARCLog *log = (PARCLog *) longBowTestCase_GetClipBoardData(testCase);

    assertTrue(parcLog_Alert(log, "This is a alert message"),
               "Expected message to be logged successfully");
}

LONGBOW_TEST_CASE(Global, parcLog_Critical)
{
    PARCLog *log = (PARCLog *) longBowTestCase_GetClipBoardData(testCase);

    assertTrue(parcLog_Critical(log, "This is a critical message"),
               "Expected message to be logged successfully");
}

LONGBOW_TEST_CASE(Global, parcLog_Notice)
{
    PARCLog *log = (PARCLog *) longBowTestCase_GetClipBoardData(testCase);

    assertTrue(parcLog_Notice(log, "This is a notice message"),
               "Expected message to be logged successfully");
}

LONGBOW_TEST_CASE(Global, parcLog_Error)
{
    PARCLog *log = (PARCLog *) longBowTestCase_GetClipBoardData(testCase);

    assertTrue(parcLog_Error(log, "This is a error message"),
               "Expected message to be logged successfully");
}

LONGBOW_TEST_CASE(Global, parcLog_Debug)
{
    PARCLog *log = (PARCLog *) longBowTestCase_GetClipBoardData(testCase);

    assertTrue(parcLog_Debug(log, "This is a debug message"),
               "Expected message to be logged successfully");
}

LONGBOW_TEST_CASE(Global, parcLog_Warning_WrongLevel)
{
    PARCLog *log = (PARCLog *) longBowTestCase_GetClipBoardData(testCase);
    parcLog_SetLevel(log, PARCLogLevel_Off);

    assertFalse(parcLog_Warning(log, "This is a warning message"),
               "Expected message to not be logged");
}

LONGBOW_TEST_CASE(Global, parcLog_Emergency_WrongLevel)
{
    PARCLog *log = (PARCLog *) longBowTestCase_GetClipBoardData(testCase);
    parcLog_SetLevel(log, PARCLogLevel_Off);

    // Even if the log level is set to off, you cannot block an emergency message.
    assertTrue(parcLog_Emergency(log, "This is an emergency message"),
               "Expected message to not be logged");
}

LONGBOW_TEST_CASE(Global, parcLog_Alert_WrongLevel)
{
    PARCLog *log = (PARCLog *) longBowTestCase_GetClipBoardData(testCase);
    parcLog_SetLevel(log, PARCLogLevel_Off);

    assertFalse(parcLog_Alert(log, "This is a finest message"),
               "Expected message to not be logged");
}

LONGBOW_TEST_CASE(Global, parcLog_Critical_WrongLevel)
{
    PARCLog *log = (PARCLog *) longBowTestCase_GetClipBoardData(testCase);
    parcLog_SetLevel(log, PARCLogLevel_Off);

    assertFalse(parcLog_Critical(log, "This is a finer message"),
               "Expected message to not be logged");
}

LONGBOW_TEST_CASE(Global, parcLog_Notice_WrongLevel)
{
    PARCLog *log = (PARCLog *) longBowTestCase_GetClipBoardData(testCase);
    parcLog_SetLevel(log, PARCLogLevel_Off);

    assertFalse(parcLog_Notice(log, "This is a fine message"),
               "Expected message to not be logged");
}

LONGBOW_TEST_CASE(Global, parcLog_Debug_WrongLevel)
{
    PARCLog *log = (PARCLog *) longBowTestCase_GetClipBoardData(testCase);
    parcLog_SetLevel(log, PARCLogLevel_Off);

    assertFalse(parcLog_Debug(log, "This is a debug message"),
               "Expected message to not be logged");
}

LONGBOW_TEST_CASE(Global, parcLog_Error_WrongLevel)
{
    PARCLog *log = (PARCLog *) longBowTestCase_GetClipBoardData(testCase);
    parcLog_SetLevel(log, PARCLogLevel_Off);

    assertFalse(parcLog_Error(log, "This is a debug message"),
                "Expected message to not be logged");
}

LONGBOW_TEST_CASE(Global, parcLog_Info_WrongLevel)
{
    PARCLog *log = (PARCLog *) longBowTestCase_GetClipBoardData(testCase);
    parcLog_SetLevel(log, PARCLogLevel_Off);

    assertFalse(parcLog_Info(log, "This is a debug message"),
                "Expected message to not be logged");
}

LONGBOW_TEST_CASE(Global, parcLog_Message)
{
    PARCLog *log = (PARCLog *) longBowTestCase_GetClipBoardData(testCase);
    parcLog_SetLevel(log, PARCLogLevel_Alert);

    assertTrue(parcLog_Message(log, PARCLogLevel_Alert, 0, "This is an alert message"),
                "Expected message to be logged");
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(test_parc_Log);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
