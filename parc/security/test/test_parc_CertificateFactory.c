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
#include <config.h>
#include <stdio.h>
#include <LongBow/unit-test.h>

#include "../parc_CertificateFactory.c"
#include <parc/security/parc_X509Certificate.h>
#include <parc/algol/parc_SafeMemory.h>
#include <parc/security/parc_Security.h>
#include <parc/security/parc_CryptoHashType.h>
#include <parc/algol/parc_Buffer.h>

LONGBOW_TEST_RUNNER(parc_CertificateFactory)
{
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

LONGBOW_TEST_RUNNER_SETUP(parc_CertificateFactory)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_CertificateFactory)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parc_CertificateFactory_AcquireRelease);
    LONGBOW_RUN_TEST_CASE(Global, parc_CertificateFactory_Create);
    LONGBOW_RUN_TEST_CASE(Global, parc_CertificateFactory_CreateFromFile);
    LONGBOW_RUN_TEST_CASE(Global, parc_CertificateFactory_CreateFromBuffer);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    parcSecurity_Init();
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    parcSecurity_Fini();
    if (parcSafeMemory_ReportAllocation(STDOUT_FILENO) != 0) {
        printf("('%s' leaks memory by %d (allocs - frees)) ", longBowTestCase_GetName(testCase), parcMemory_Outstanding());
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, parc_CertificateFactory_AcquireRelease)
{
    PARCCertificateFactory *factory = parcCertificateFactory_Create(PARCCertificateType_X509, PARCContainerEncoding_PEM);
    assertNotNull(factory, "Expected non-NULL factory");

    PARCReferenceCount firstCount = parcObject_GetReferenceCount(factory);
    PARCCertificateFactory *copy = parcCertificateFactory_Acquire(factory);
    PARCReferenceCount secondCount = parcObject_GetReferenceCount(copy);

    assertTrue(firstCount == (secondCount - 1), "Expected incremented reference count after Acquire");

    parcCertificateFactory_Release(&copy);
    PARCReferenceCount thirdCount = parcObject_GetReferenceCount(factory);

    assertTrue(firstCount == thirdCount, "Expected equal reference counts after Release");

    parcCertificateFactory_Release(&factory);
}

LONGBOW_TEST_CASE(Global, parc_CertificateFactory_Create)
{
    PARCCertificateFactory *factory = parcCertificateFactory_Create(PARCCertificateType_X509, PARCContainerEncoding_PEM);
    assertNotNull(factory, "Expected non-NULL factory");
    assertTrue(factory->encoding == PARCContainerEncoding_PEM, "Expected PARCContainerEncoding_PEM (%d) encoding, got %d",
               PARCContainerEncoding_PEM, factory->encoding);
    assertTrue(factory->type == PARCCertificateType_X509, "Expected PARCCertificateType_X509 (%d) type, got %d",
               PARCCertificateType_X509, factory->type);
    parcCertificateFactory_Release(&factory);
}

LONGBOW_TEST_CASE(Global, parc_CertificateFactory_CreateFromFile)
{
    PARCCertificateFactory *factory = parcCertificateFactory_Create(PARCCertificateType_X509, PARCContainerEncoding_PEM);
    assertNotNull(factory, "Expected non-NULL factory");

    char *filename = "test.pem";
    PARCCertificate *certificate = parcCertificateFactory_CreateCertificateFromFile(factory, filename, NULL);

    assertNotNull(certificate, "Expected non-NULL certificate");
    assertTrue(parcCertificate_GetContainerEncoding(certificate) == PARCContainerEncoding_PEM,
               "Expected PARCContainerEncoding_PEM encoding, got %d", parcCertificate_GetContainerEncoding(certificate));
    assertTrue(parcCertificate_GetCertificateType(certificate) == PARCCertificateType_X509,
               "Expected PARCCertificateType_X509 type, got %d", parcCertificate_GetCertificateType(certificate));

    parcCertificate_Release(&certificate);
    parcCertificateFactory_Release(&factory);

    factory = parcCertificateFactory_Create(PARCCertificateType_Invalid, PARCContainerEncoding_PEM);

    PARCCertificate *nullCertificate = parcCertificateFactory_CreateCertificateFromFile(factory, filename, NULL);
    assertNull(nullCertificate, "Expected NULL certificate to be returned from factory with an unsupported configuration");

    parcCertificateFactory_Release(&factory);
}

LONGBOW_TEST_CASE(Global, parc_CertificateFactory_CreateFromBuffer)
{
    PARCCertificateFactory *factory = parcCertificateFactory_Create(PARCCertificateType_X509, PARCContainerEncoding_DER);
    assertNotNull(factory, "Expected non-NULL factory");

    char *filename = "test.pem";
    PARCX509Certificate *realCertificate = parcX509Certificate_CreateFromPEMFile(filename);
    PARCBuffer *certificateBuffer = parcX509Certificate_GetDEREncodedCertificate(realCertificate);

    PARCCertificate *certificate = parcCertificateFactory_CreateCertificateFromBuffer(factory, certificateBuffer);

    assertNotNull(certificate, "Expected non-NULL certificate");
    assertTrue(parcCertificate_GetContainerEncoding(certificate) == PARCContainerEncoding_DER,
               "Expected PARCContainerEncoding_DER encoding, got %d", parcCertificate_GetContainerEncoding(certificate));
    assertTrue(parcCertificate_GetCertificateType(certificate) == PARCCertificateType_X509,
               "Expected PARCCertificateType_X509 type, got %d", parcCertificate_GetCertificateType(certificate));

    parcCertificate_Release(&certificate);
    parcCertificateFactory_Release(&factory);
    parcX509Certificate_Release(&realCertificate);

    factory = parcCertificateFactory_Create(PARCCertificateType_Invalid, PARCContainerEncoding_PEM);

    PARCBuffer *invalid = parcBuffer_Allocate(10);
    PARCCertificate *nullCertificate = parcCertificateFactory_CreateCertificateFromBuffer(factory, invalid);
    assertNull(nullCertificate, "Expected NULL certificate to be returned from factory with an unsupported configuration");

    parcBuffer_Release(&invalid);
    parcCertificateFactory_Release(&factory);
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_CertificateFactory);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
