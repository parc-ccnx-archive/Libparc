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

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../parc_IdentityFile.c"
#include "../parc_Security.h"

#include <stdio.h>
#include <fcntl.h>
#include <inttypes.h>

#include <LongBow/unit-test.h>

#include <parc/algol/parc_SafeMemory.h>
#include <parc/testing/parc_ObjectTesting.h>

LONGBOW_TEST_RUNNER(parc_IdentityFile)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_IdentityFile)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_IdentityFile)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcIdentityFile_Create);
    LONGBOW_RUN_TEST_CASE(Global, parcIdentityFile_Release);
    LONGBOW_RUN_TEST_CASE(Global, parcIdentityFile_Acquire);
    LONGBOW_RUN_TEST_CASE(Global, parcIdentityFile_Equals);
    LONGBOW_RUN_TEST_CASE(Global, parcIdentityFile_GetFileName);
    LONGBOW_RUN_TEST_CASE(Global, parcIdentityFile_GetPassWord);
    LONGBOW_RUN_TEST_CASE(Global, parcIdentityFile_CreateSigner);
//    LONGBOW_RUN_TEST_CASE(Global, parcIdentityFile_Display);
//    LONGBOW_RUN_TEST_CASE(Global, parcIdentityFile_Exists_True);
//    LONGBOW_RUN_TEST_CASE(Global, parcIdentityFile_Exists_False);
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

LONGBOW_TEST_CASE(Global, parcIdentityFile_Acquire)
{
    const char *keystoreName = "test_rsa.p12";
    const char *keystorePassword = "blueberry";

    PARCIdentityFile *identityFile = parcIdentityFile_Create(keystoreName, keystorePassword);

    assertNotNull(identityFile, "Expected non-null");

    parcObjectTesting_AssertAcquireReleaseContract(parcIdentityFile_Acquire, identityFile);

    parcIdentityFile_Release(&identityFile);
}

LONGBOW_TEST_CASE(Global, parcIdentityFile_Create)
{
    const char *keystoreName = "test_rsa.p12";
    const char *keystorePassword = "blueberry";

    PARCIdentityFile *identityFile = parcIdentityFile_Create(keystoreName, keystorePassword);

    assertNotNull(identityFile, "Expected non-null");

    parcIdentityFile_Release(&identityFile);
}

LONGBOW_TEST_CASE(Global, parcIdentityFile_GetFileName)
{
    const char *keystoreName = "test_rsa.p12";
    const char *keystorePassword = "blueberry";

    PARCIdentityFile *identityFile = parcIdentityFile_Create(keystoreName, keystorePassword);

    assertNotNull(identityFile, "Expected non-null");

    assertEqualStrings(keystoreName, parcIdentityFile_GetFileName(identityFile));

    parcIdentityFile_Release(&identityFile);
}

LONGBOW_TEST_CASE(Global, parcIdentityFile_GetPassWord)
{
    const char *keystoreName = "test_rsa.p12";
    const char *keystorePassword = "blueberry";

    PARCIdentityFile *identityFile = parcIdentityFile_Create(keystoreName, keystorePassword);

    assertNotNull(identityFile, "Expected non-null");

    assertEqualStrings(keystorePassword, parcIdentityFile_GetPassWord(identityFile));

    parcIdentityFile_Release(&identityFile);
}

LONGBOW_TEST_CASE(Global, parcIdentityFile_Exists_True)
{
    const char *keystoreName = "test_rsa.p12";
    const char *keystorePassword = "blueberry";

    PARCIdentityFile *identityFile = parcIdentityFile_Create(keystoreName, keystorePassword);

    bool actual = parcIdentityFile_Exists(identityFile);

    assertTrue(actual, "Expected %s to exist.", parcIdentityFile_GetFileName(identityFile));

    parcIdentityFile_Release(&identityFile);
}

LONGBOW_TEST_CASE(Global, parcIdentityFile_Exists_False)
{
    const char *keystoreName = "/dev/notgoingtoexist";
    const char *keystorePassword = "blueberry";

    PARCIdentityFile *identityFile = parcIdentityFile_Create(keystoreName, keystorePassword);

    bool actual = parcIdentityFile_Exists(identityFile);

    assertFalse(actual, "Expected %s to not exist.", parcIdentityFile_GetFileName(identityFile));

    parcIdentityFile_Release(&identityFile);
}

LONGBOW_TEST_CASE(Global, parcIdentityFile_CreateSigner)
{
    parcSecurity_Init();

    const char *keystoreName = "test_rsa.p12";
    const char *keystorePassword = "blueberry";

    PARCIdentityFile *identityFile = parcIdentityFile_Create(keystoreName, keystorePassword);

    assertNotNull(identityFile, "Expected non-null");

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        fprintf(stdout, "Current working dir: %s\n", cwd);
    else
        perror("getcwd() error");
    PARCSigner *signer = parcIdentityFile_CreateSigner(identityFile);

    assertNotNull(signer, "Expected non-null");

    parcIdentityFile_Release(&identityFile);
    parcSigner_Release(&signer);

    parcSecurity_Fini();
}

LONGBOW_TEST_CASE(Global, parcIdentityFile_Release)
{
    const char *keystoreName = "test_rsa.p12";
    const char *keystorePassword = "blueberry";

    PARCIdentityFile *identityFile = parcIdentityFile_Create(keystoreName, keystorePassword);

    assertNotNull(identityFile, "Expected non-null");

    parcIdentityFile_Release(&identityFile);
    assertNull(identityFile, "Identity File was not nulled out after Release()");
}

LONGBOW_TEST_CASE(Global, parcIdentityFile_Equals)
{
    const char *keystoreName = "test_rsa.p12";
    const char *keystorePassword = "blueberry";

    PARCIdentityFile *x = parcIdentityFile_Create(keystoreName, keystorePassword);
    PARCIdentityFile *y = parcIdentityFile_Create(keystoreName, keystorePassword);
    PARCIdentityFile *z = parcIdentityFile_Create(keystoreName, keystorePassword);
    PARCIdentityFile *u1 = parcIdentityFile_Create("foo", keystorePassword);
    PARCIdentityFile *u2 = parcIdentityFile_Create(keystoreName, "bar");

    parcObjectTesting_AssertEqualsFunction(parcIdentityFile_Equals, x, y, z, u1, u2);

    parcIdentityFile_Release(&x);
    parcIdentityFile_Release(&y);
    parcIdentityFile_Release(&z);
    parcIdentityFile_Release(&u1);
    parcIdentityFile_Release(&u2);
}

LONGBOW_TEST_CASE(Global, parcIdentityFile_Display)
{
    const char *keystoreName = "test_rsa.p12";
    const char *keystorePassword = "blueberry";

    PARCIdentityFile *identityFile = parcIdentityFile_Create(keystoreName, keystorePassword);
    assertNotNull(identityFile, "Expected non-null");

    parcIdentityFile_Display(identityFile, 0);

    parcIdentityFile_Release(&identityFile);
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_IdentityFile);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
