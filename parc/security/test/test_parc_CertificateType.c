/*
 * Copyright (c) 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
#include <LongBow/unit-test.h>

#include "../parc_CertificateType.c"
#include <parc/algol/parc_SafeMemory.h>
#include <parc/security/parc_CryptoHashType.h>
#include <parc/algol/parc_Buffer.h>

LONGBOW_TEST_RUNNER(parc_CertificateType)
{
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

LONGBOW_TEST_RUNNER_SETUP(parc_CertificateType)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_CertificateType)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parc_CertificateType_FromString);
    LONGBOW_RUN_TEST_CASE(Global, parc_CertificateType_ToString);
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

LONGBOW_TEST_CASE(Global, parc_CertificateType_FromString)
{
    char *inputString = "PARCCertificateType_X509";
    PARCCertificateType encoding = parcCertificateType_FromString(inputString);
    assertTrue(encoding == PARCCertificateType_X509, "Expected PARCCertificateType_X509 (%d), for %d", PARCCertificateType_X509, encoding);

    inputString = "the cake is a lie";
    encoding = parcCertificateType_FromString(inputString);
    assertTrue(encoding == PARCCertificateType_Invalid, "Expected PARCCertificateType_Invalid (%d), for %d", PARCCertificateType_Invalid, encoding);
}

LONGBOW_TEST_CASE(Global, parc_CertificateType_ToString)
{
    char *expected = "PARCCertificateType_X509";
    PARCCertificateType encoding = PARCCertificateType_X509;
    const char *actual = parcCertificateType_ToString(encoding);
    assertTrue(strcmp(actual, expected) == 0, "Expected %s, got %s", expected, actual);

    actual = parcCertificateType_ToString(PARCCertificateType_Invalid);
    assertNull(actual, "Expected NULL string to be returned with invalid certificate type");
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_CertificateType);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
