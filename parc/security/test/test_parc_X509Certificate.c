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

#include <unistd.h>
#include <fcntl.h>

#include "../parc_X509Certificate.c"
#include <parc/algol/parc_SafeMemory.h>
#include <parc/security/parc_CryptoHashType.h>
#include <parc/algol/parc_Buffer.h>

LONGBOW_TEST_RUNNER(parc_X509Certificate)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

LONGBOW_TEST_RUNNER_SETUP(parc_X509Certificate)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_X509Certificate)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parc_X509Certificate_AcquireRelease);
    LONGBOW_RUN_TEST_CASE(Global, parc_X509Certificate_Create);
    LONGBOW_RUN_TEST_CASE(Global, parc_X509Certificate_CreateFromDERBuffer);
    LONGBOW_RUN_TEST_CASE(Global, parc_X509Certificate_GetPublicKeyDigest);
    LONGBOW_RUN_TEST_CASE(Global, parc_X509Certificate_GetCertificateDigest);
    LONGBOW_RUN_TEST_CASE(Global, parc_X509Certificate_GetDEREncodedCertificate);
    LONGBOW_RUN_TEST_CASE(Global, parc_X509Certificate_GetDEREncodedPublicKey);
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

LONGBOW_TEST_CASE(Global, parc_X509Certificate_AcquireRelease)
{
    char *fileName = "test.pem";
    PARCX509Certificate *certificate = parcX509Certificate_CreateFromPEMFile(fileName);
    assertNotNull(certificate, "Expected non-NULL certificate");

    PARCReferenceCount firstCount = parcObject_GetReferenceCount(certificate);
    PARCX509Certificate *copy = parcX509Certificate_Acquire(certificate);
    PARCReferenceCount secondCount = parcObject_GetReferenceCount(copy);

    assertTrue(firstCount == (secondCount - 1), "Expected incremented reference count after Acquire");

    parcX509Certificate_Release(&copy);
    PARCReferenceCount thirdCount = parcObject_GetReferenceCount(certificate);

    assertTrue(firstCount == thirdCount, "Expected equal reference counts after Release");

    parcX509Certificate_Release(&certificate);
}

LONGBOW_TEST_CASE(Global, parc_X509Certificate_Create)
{
    char *fileName = "bad.pem";
    PARCX509Certificate *certificate = _parcX509Certificate_CreateFromPEMFile(fileName);
    assertNull(certificate, "Expected NULL certificate with non-existent file");
}

LONGBOW_TEST_CASE(Global, parc_X509Certificate_CreateFromDERBuffer)
{
    char *fileName = "test.pem";
    PARCX509Certificate *certificate = _parcX509Certificate_CreateFromPEMFile(fileName);
    assertNotNull(certificate, "Expected non-NULL certificate");

    PARCBuffer *certificateBuffer = _getDEREncodedCertificate(certificate);
    PARCX509Certificate *realCertificate = parcX509Certificate_CreateFromDERBuffer(certificateBuffer);
    assertNotNull(realCertificate, "Expected non-NULL certificate to be parsed from DER buffer");

    parcX509Certificate_Release(&certificate);
    parcX509Certificate_Release(&realCertificate);
}

LONGBOW_TEST_CASE(Global, parc_X509Certificate_GetPublicKeyDigest)
{
    char *fileName = "test.pem";
    PARCX509Certificate *certificate = _parcX509Certificate_CreateFromPEMFile(fileName);

    PARCCryptoHash *digest = _getPublicKeyDigest(certificate);
    PARCBuffer *digestBuffer = parcCryptoHash_GetDigest(digest);

    size_t expectedLength = SHA256_DIGEST_LENGTH;
    size_t actualLength = parcBuffer_Remaining(digestBuffer);
    assertTrue(actualLength == expectedLength, "Digest unexpected size: got %zu expected %zu", actualLength, expectedLength);

    int fd = open("test_pubkey.bin", O_RDONLY);
    uint8_t rawDigest[SHA256_DIGEST_LENGTH];
    ssize_t numBytes = read(fd, rawDigest, SHA256_DIGEST_LENGTH);
    assertTrue(numBytes == SHA256_DIGEST_LENGTH, "Expected to read %d bytes, got %zu", SHA256_DIGEST_LENGTH, numBytes);
    close(fd);

    PARCBuffer *rawBuffer = parcBuffer_Flip(parcBuffer_CreateFromArray(rawDigest, SHA256_DIGEST_LENGTH));

    assertTrue(parcBuffer_Remaining(rawBuffer) == SHA256_DIGEST_LENGTH, "Expected %d length buffer", SHA256_DIGEST_LENGTH);

    parcBuffer_Release(&rawBuffer);
    parcX509Certificate_Release(&certificate);
}

LONGBOW_TEST_CASE(Global, parc_X509Certificate_GetCertificateDigest)
{
    char *fileName = "test.pem";
    PARCX509Certificate *certificate = _parcX509Certificate_CreateFromPEMFile(fileName);

    PARCCryptoHash *digest = _getCertificateDigest(certificate);
    PARCBuffer *digestBuffer = parcCryptoHash_GetDigest(digest);

    size_t expectedLength = SHA256_DIGEST_LENGTH;
    size_t actualLength = parcBuffer_Remaining(digestBuffer);
    assertTrue(actualLength == expectedLength, "Digest unexpected size: got %zu expected %zu", actualLength, expectedLength);

    int fd = open("test_crt_sha256.bin", O_RDONLY);
    uint8_t rawDigest[SHA256_DIGEST_LENGTH];
    ssize_t numBytes = read(fd, rawDigest, SHA256_DIGEST_LENGTH);
    assertTrue(numBytes == SHA256_DIGEST_LENGTH, "Expected to read %d bytes, got %zu", SHA256_DIGEST_LENGTH, numBytes);
    close(fd);

    PARCBuffer *rawBuffer = parcBuffer_Flip(parcBuffer_CreateFromArray(rawDigest, SHA256_DIGEST_LENGTH));

    assertTrue(parcBuffer_Equals(rawBuffer, digestBuffer), "Expected raw binary to equal the computed result.");

    parcBuffer_Release(&rawBuffer);
    parcX509Certificate_Release(&certificate);
}

LONGBOW_TEST_CASE(Global, parc_X509Certificate_GetDEREncodedCertificate)
{
    char *fileName = "test.pem";
    PARCX509Certificate *certificate = _parcX509Certificate_CreateFromPEMFile(fileName);

    PARCBuffer *digestBuffer = _getDEREncodedCertificate(certificate);

    size_t expectedLength = 517;
    size_t actualLength = parcBuffer_Remaining(digestBuffer);
    assertTrue(actualLength == expectedLength, "Digest unexpected size: got %zu expected %zu", actualLength, expectedLength);

    int fd = open("test_crt_der.bin", O_RDONLY);
    uint8_t rawDigest[expectedLength];
    ssize_t numBytes = read(fd, rawDigest, expectedLength);
    assertTrue(numBytes == expectedLength, "Expected to read %zu bytes, got %zu", expectedLength, numBytes);
    close(fd);

    PARCBuffer *rawBuffer = parcBuffer_Flip(parcBuffer_CreateFromArray(rawDigest, expectedLength));

    assertTrue(parcBuffer_Equals(rawBuffer, digestBuffer), "Expected raw binary to equal the computed result.");

    parcBuffer_Release(&rawBuffer);
    parcX509Certificate_Release(&certificate);
}

LONGBOW_TEST_CASE(Global, parc_X509Certificate_GetDEREncodedPublicKey)
{
    char *fileName = "test.pem";
    PARCX509Certificate *certificate = _parcX509Certificate_CreateFromPEMFile(fileName);

    PARCBuffer *digestBuffer = _getDEREncodedPublicKey(certificate);

    size_t expectedLength = 162;
    size_t actualLength = parcBuffer_Remaining(digestBuffer);
    assertTrue(actualLength == expectedLength, "Digest unexpected size: got %zu expected %zu", actualLength, expectedLength);

    int fd = open("test_der.bin", O_RDONLY);
    uint8_t rawDigest[expectedLength];
    ssize_t numBytes = read(fd, rawDigest, expectedLength);
    assertTrue(numBytes == expectedLength, "Expected to read %zu bytes, got %zu", expectedLength, numBytes);
    close(fd);

    PARCBuffer *rawBuffer = parcBuffer_Flip(parcBuffer_CreateFromArray(rawDigest, expectedLength));

    assertTrue(parcBuffer_Equals(rawBuffer, digestBuffer), "Expected raw binary to equal the computed result.");

    parcBuffer_Release(&rawBuffer);
    parcX509Certificate_Release(&certificate);
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_X509Certificate);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
