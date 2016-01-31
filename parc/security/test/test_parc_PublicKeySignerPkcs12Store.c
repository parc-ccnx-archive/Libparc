/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @author Marc Mosko, Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>

#include <LongBow/testing.h>

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../parc_PublicKeySignerPkcs12Store.c"

#include <parc/algol/parc_SafeMemory.h>
#include <parc/security/parc_Security.h>

const char *filename = "/tmp/filekeystore.p12";

LONGBOW_TEST_RUNNER(ccnx_FileKeystore)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(openssl_commandline);
    LONGBOW_RUN_TEST_FIXTURE(ccnx_internal);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(ccnx_FileKeystore)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(ccnx_FileKeystore)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcPublicKeySignerPkcs12Store_Open);
    LONGBOW_RUN_TEST_CASE(Global, parcPublicKeySignerPkcs12Store_badpass);
    LONGBOW_RUN_TEST_CASE(Global, parcPublicKeySignerPkcs12Store_CreateAndOpen);
    LONGBOW_RUN_TEST_CASE(Global, parcPublicKeySignerPkcs12Store_CreateFile_Fail);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    parcSecurity_Init();
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    unlink(filename);
    parcSecurity_Fini();
    if (parcSafeMemory_ReportAllocation(STDOUT_FILENO) != 0) {
        printf("('%s' leaks memory by %d (allocs - frees)) ", longBowTestCase_GetName(testCase), parcMemory_Outstanding());
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, parcPublicKeySignerPkcs12Store_Open)
{
    // open our test p12 file created with openssl
    parcSecurity_Init();

    PARCSigningInterface *interface = parcPublicKeySignerPkcs12Store_Open("test_rsa.p12", "blueberry", PARC_HASH_SHA256);

    assertNotNull(interface, "Got null result from opening openssl pkcs12 file");

    interface->Destroy(&interface);
    parcSecurity_Fini();
}

LONGBOW_TEST_CASE(Global, parcPublicKeySignerPkcs12Store_badpass)
{
    // open our test p12 file created with openssl

    fprintf(stderr, "The next openssl error is expected, we're using the wrong password\n");
    PARCSigningInterface *interface = parcPublicKeySignerPkcs12Store_Open("test_rsa.p12", "orange", PARC_HASH_SHA256);

    assertNull(interface, "Got null result from opening openssl pkcs12 file");
}

LONGBOW_TEST_CASE(Global, parcPublicKeySignerPkcs12Store_CreateAndOpen)
{
    // create a file and open it
    const char *filename = "/tmp/parcPublicKeySignerPkcs12Store_CreateAndOpen.p12";
    const char *password = "12345";
    const char *subject = "alice";
    bool result;

    result = parcPublicKeySignerPkcs12Store_CreateFile(filename, password, subject, 1024, 32);
    assertTrue(result, "got error from parcPublicKeySignerPkcs12Store_CreatePkcs12File");

    PARCSigningInterface *interface = parcPublicKeySignerPkcs12Store_Open(filename, password, PARC_HASH_SHA256);

    assertNotNull(interface, "Got null result from opening openssl pkcs12 file");

    interface->Destroy(&interface);
    unlink(filename);
}

LONGBOW_TEST_CASE(Global, parcPublicKeySignerPkcs12Store_CreateFile_Fail)
{
    // create a file and open it
    const char *filename = "/tmp/parcPublicKeySignerPkcs12Store_CreateAndOpen.p12";
    const char *password = "12345";
    const char *subject = "alice";
    bool result;

    result = parcPublicKeySignerPkcs12Store_CreateFile(filename, password, subject, -1, 32);
    assertFalse(result, "Expected false result from parcPublicKeySignerPkcs12Store_CreateFile()");

    unlink(filename);
}


// =====================================================
// These are tests based on internally-generated pkcs12

LONGBOW_TEST_FIXTURE(ccnx_internal)
{
    LONGBOW_RUN_TEST_CASE(ccnx_internal, parcPublicKeySignerPkcs12Store_GetCertificateDigest);
    LONGBOW_RUN_TEST_CASE(ccnx_internal, parcPublicKeySignerPkcs12Store_GetPublicKeyDigest);
    LONGBOW_RUN_TEST_CASE(ccnx_internal, parcPublicKeySignerPkcs12Store_GetEncodedCertificate);
    LONGBOW_RUN_TEST_CASE(ccnx_internal, parcPublicKeySignerPkcs12Store_GetEncodedPublicKey);
    LONGBOW_RUN_TEST_CASE(ccnx_internal, parcPublicKeySignerPkcs12Store_SignBuffer);
}

LONGBOW_TEST_FIXTURE_SETUP(ccnx_internal)
{
    parcSecurity_Init();
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(ccnx_internal)
{
    unlink(filename);
    parcSecurity_Fini();
    if (parcSafeMemory_ReportAllocation(STDOUT_FILENO) != 0) {
        printf("('%s' leaks memory by %d (allocs - frees)) ", longBowTestCase_GetName(testCase), parcMemory_Outstanding());
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(ccnx_internal, parcPublicKeySignerPkcs12Store_GetCertificateDigest)
{
    // create a file and open it
    const char *password = "12345";
    const char *subject = "alice";
    bool result;

    result = parcPublicKeySignerPkcs12Store_CreateFile(filename, password, subject, 1024, 32);
    assertTrue(result, "got error from parcPublicKeySignerPkcs12Store_CreatePkcs12File");

    PARCSigningInterface *interface = parcPublicKeySignerPkcs12Store_Open(filename, password, PARC_HASH_SHA256);
    assertNotNull(interface, "Got null result from opening openssl pkcs12 file");

    PARCCryptoHash *cert_digest = interface->GetCertificateDigest(interface->interfaceContext);
    assertNotNull(cert_digest, "got null public key digest for external pkcs12");

    size_t bb_length = parcBuffer_Remaining(parcCryptoHash_GetDigest(cert_digest));
    assertTrue(bb_length == SHA256_DIGEST_LENGTH,
               "Incorrect digest length returned from GetPublicKeyDigest: %zu", bb_length);

    interface->Destroy(&interface);
    parcCryptoHash_Release(&cert_digest);
}

/**
 * Use a ccnx-generated pkcs12 file
 */
LONGBOW_TEST_CASE(ccnx_internal, parcPublicKeySignerPkcs12Store_GetPublicKeyDigest)
{
    // create a file and open it
    const char *password = "12345";
    const char *subject = "alice";
    bool result;

    result = parcPublicKeySignerPkcs12Store_CreateFile(filename, password, subject, 1024, 32);
    assertTrue(result, "got error from parcPublicKeySignerPkcs12Store_CreatePkcs12File");

    PARCSigningInterface *interface = parcPublicKeySignerPkcs12Store_Open(filename, password, PARC_HASH_SHA256);
    assertNotNull(interface, "Got null result from opening openssl pkcs12 file");

    PARCCryptoHash *pkd = interface->GetVerifierKeyDigest(interface->interfaceContext);
    assertNotNull(pkd, "got null public key digest for external pkcs12");

    size_t bb_length = parcBuffer_Remaining(parcCryptoHash_GetDigest(pkd));
    assertTrue(bb_length == SHA256_DIGEST_LENGTH, "Incorrect digest length returned from GetPublicKeyDigest: %zu", bb_length);

    interface->Destroy(&interface);
    parcCryptoHash_Release(&pkd);
}


LONGBOW_TEST_CASE(ccnx_internal, parcPublicKeySignerPkcs12Store_GetEncodedCertificate)
{
    // create a file and open it
    const char *password = "12345";
    const char *subject = "alice";
    bool result;

    result = parcPublicKeySignerPkcs12Store_CreateFile(filename, password, subject, 1024, 32);
    assertTrue(result, "got error from parcPublicKeySignerPkcs12Store_CreatePkcs12File");

    PARCSigningInterface *interface = parcPublicKeySignerPkcs12Store_Open(filename, password, PARC_HASH_SHA256);
    assertNotNull(interface, "Got null result from opening openssl pkcs12 file");

    PARCBuffer *certificate_der = interface->GetDEREncodedCertificate(interface->interfaceContext);
    assertNotNull(certificate_der, "got null public key digest for external pkcs12");

    // 557 (64-bit) and 553 (32-bit) are pre-etermined sizes of how big a DER encoded
    // certificate with a 1024-bit key should be
    size_t expectedMinimumLength = 545;
    size_t expectedMaximumLength = 560;
    size_t bb_length = parcBuffer_Remaining(certificate_der);
    assertTrue(expectedMinimumLength <= bb_length && bb_length <= expectedMaximumLength,
               "Digest unexpected size: got %zu expected %zu - %zu", bb_length, expectedMinimumLength, expectedMaximumLength);

    interface->Destroy(&interface);
    parcBuffer_Release(&certificate_der);
}

LONGBOW_TEST_CASE(ccnx_internal, parcPublicKeySignerPkcs12Store_GetEncodedPublicKey)
{
    // create a file and open it
    const char *password = "12345";
    const char *subject = "alice";
    bool result;

    result = parcPublicKeySignerPkcs12Store_CreateFile(filename, password, subject, 1024, 32);
    assertTrue(result, "got error from parcPublicKeySignerPkcs12Store_CreatePkcs12File");

    PARCSigningInterface *interface = parcPublicKeySignerPkcs12Store_Open(filename, password, PARC_HASH_SHA256);
    assertNotNull(interface, "Got null result from opening openssl pkcs12 file");

    PARCBuffer *pubkey_der = interface->GetDEREncodedPublicKey(interface->interfaceContext);
    assertNotNull(pubkey_der, "got null public key digest for external pkcs12");

    size_t bb_length = parcBuffer_Remaining(pubkey_der);
    assertTrue(bb_length == 162, "Incorrect digest length returned from GetPublicKeyDigest: %zu", bb_length);

    interface->Destroy(&interface);
    parcBuffer_Release(&pubkey_der);
}

LONGBOW_TEST_CASE(ccnx_internal, parcPublicKeySignerPkcs12Store_SignBuffer)
{
    const char *password = "12345";
    const char *subject = "alice";
    bool result;

    result = parcPublicKeySignerPkcs12Store_CreateFile(filename, password, subject, 1024, 32);
    assertTrue(result, "got error from parcPublicKeySignerPkcs12Store_CreatePkcs12File");

    for (PARCCryptoHashType hashType = PARC_HASH_SHA256; hashType <= PARC_HASH_SHA512; ++hashType) {
        PARCSigningInterface *interface = parcPublicKeySignerPkcs12Store_Open(filename, password, hashType);
        assertNotNull(interface, "Got null result from opening openssl pkcs12 file");

        assertTrue(interface->GetCryptoHashType(interface->interfaceContext) == hashType, "Expect GetCryptoHashType() to match hashType");

        // read the buffer to sign
        int fd = open("test_random_bytes", O_RDONLY);
        uint8_t buffer_to_sign[2048];
        ssize_t read_bytes = read(fd, buffer_to_sign, 2048);
        close(fd);

        // Digest it
        PARCCryptoHasher *digester = interface->GetCryptoHasher(interface->interfaceContext);
        parcCryptoHasher_Init(digester);
        parcCryptoHasher_UpdateBytes(digester, buffer_to_sign, read_bytes);
        PARCCryptoHash *parcDigest = parcCryptoHasher_Finalize(digester);

        PARCSignature *bb_test_sign = interface->SignDigest(interface->interfaceContext, parcDigest);

        assertNotNull(bb_test_sign,
                      "Got null byte buffer from SignBuffer");
        assertTrue(parcBuffer_Remaining(parcSignature_GetSignature(bb_test_sign)) == 128,
                   "Incorrect signature size: %zu",
                   parcBuffer_Remaining(parcSignature_GetSignature(bb_test_sign)));



        interface->Destroy(&interface);
        parcSignature_Release(&bb_test_sign);
        parcCryptoHash_Release(&parcDigest);
    }
}

LONGBOW_TEST_CASE(ccnx_internal, parcPublicKeySignerPkcs12Store_VerifySignature_Cert)
{
}

// =====================================================
// These are tests based on pre-generated material from the openssl command line

LONGBOW_TEST_FIXTURE(openssl_commandline)
{
    LONGBOW_RUN_TEST_CASE(openssl_commandline, parcPublicKeySignerPkcs12Store_GetCertificateDigest);
    LONGBOW_RUN_TEST_CASE(openssl_commandline, parcPublicKeySignerPkcs12Store_GetPublicKeyDigest);
    LONGBOW_RUN_TEST_CASE(openssl_commandline, parcPublicKeySignerPkcs12Store_GetEncodedCertificate);
    LONGBOW_RUN_TEST_CASE(openssl_commandline, parcPublicKeySignerPkcs12Store_GetEncodedPublicKey);
    LONGBOW_RUN_TEST_CASE(openssl_commandline, compare_private_key);
    LONGBOW_RUN_TEST_CASE(openssl_commandline, parcPublicKeySignerPkcs12Store_SignBuffer);
}

LONGBOW_TEST_FIXTURE_SETUP(openssl_commandline)
{
    parcSecurity_Init();
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(openssl_commandline)
{
    unlink(filename);
    parcSecurity_Fini();
    if (parcSafeMemory_ReportAllocation(STDOUT_FILENO) != 0) {
        printf("('%s' leaks memory by %d (allocs - frees)) ", longBowTestCase_GetName(testCase), parcMemory_Outstanding());
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

/**
 * read in the openssl command-line generated pkcs12 file
 */
LONGBOW_TEST_CASE(openssl_commandline, parcPublicKeySignerPkcs12Store_GetPublicKeyDigest)
{
    PARCSigningInterface *interface = parcPublicKeySignerPkcs12Store_Open("test_rsa.p12", "blueberry", PARC_HASH_SHA256);
    assertNotNull(interface, "parcPublicKeySignerPkcs12Store_Open(\"test_rsa.p12\", \"blueberry\", PARC_HASH_SHA256) returned NULL");

    PARCCryptoHash *pkd = interface->GetVerifierKeyDigest(interface->interfaceContext);
    assertNotNull(pkd, "got null public key digest for external pkcs12");

    // read in the "truth" from the command line utilities

    int fd = open("test_rsa_pub_sha256.bin", O_RDONLY);
    uint8_t true_digest[SHA256_DIGEST_LENGTH];
    ssize_t read_bytes = read(fd, true_digest, SHA256_DIGEST_LENGTH);
    close(fd);

    assertTrue(read_bytes == SHA256_DIGEST_LENGTH, "could not read %d byte digest from test_rsa_pub_sha256.bin", SHA256_DIGEST_LENGTH);

    PARCBuffer *digest = parcCryptoHash_GetDigest(pkd);
    const uint8_t *bb_buffer = parcByteArray_Array(parcBuffer_Array(digest));
    size_t bb_length = parcBuffer_Remaining(digest);
    assertTrue(bb_length == SHA256_DIGEST_LENGTH,
               "Incorrect digest length returned from GetPublicKeyDigest: %zu", bb_length);

    assertTrue(memcmp(bb_buffer, true_digest, SHA256_DIGEST_LENGTH) == 0, "digests did not match");


    interface->Destroy(&interface);
    parcCryptoHash_Release(&pkd);
}

/**
 * Get the certificate digest from the openssl command line pkcs12
 */
LONGBOW_TEST_CASE(openssl_commandline, parcPublicKeySignerPkcs12Store_GetCertificateDigest)
{
    PARCSigningInterface *interface = parcPublicKeySignerPkcs12Store_Open("test_rsa.p12", "blueberry", PARC_HASH_SHA256);
    assertNotNull(interface, "Got null result from opening openssl pkcs12 file");

    PARCCryptoHash *cert_digest = interface->GetCertificateDigest(interface->interfaceContext);
    assertNotNull(cert_digest, "got null public key digest for external pkcs12");

    // read in the "truth" from the command line utilities

    int fd = open("test_rsa_crt_sha256.bin", O_RDONLY);
    uint8_t true_digest[SHA256_DIGEST_LENGTH];
    ssize_t read_bytes = read(fd, true_digest, SHA256_DIGEST_LENGTH);
    close(fd);

    assertTrue(read_bytes == SHA256_DIGEST_LENGTH, "could not read %d byte digest from test_rsa_pub_sha256.bin", SHA256_DIGEST_LENGTH);

    const uint8_t *bb_buffer = parcByteArray_Array(parcBuffer_Array(parcCryptoHash_GetDigest(cert_digest)));
    size_t bb_length = parcBuffer_Remaining(parcCryptoHash_GetDigest(cert_digest));
    assertTrue(bb_length == SHA256_DIGEST_LENGTH,
               "Incorrect digest length returned from GetCertificateDigest: %zu", bb_length);

    assertTrue(memcmp(bb_buffer, true_digest, SHA256_DIGEST_LENGTH) == 0, "digests did not match");

    interface->Destroy(&interface);
    parcCryptoHash_Release(&cert_digest);
}

LONGBOW_TEST_CASE(openssl_commandline, parcPublicKeySignerPkcs12Store_GetEncodedCertificate)
{
    PARCSigningInterface *interface = parcPublicKeySignerPkcs12Store_Open("test_rsa.p12", "blueberry", PARC_HASH_SHA256);
    assertNotNull(interface, "Got null result from opening openssl pkcs12 file");

    PARCBuffer *certificate_der = interface->GetDEREncodedCertificate(interface->interfaceContext);
    assertNotNull(certificate_der, "got null der certificate for external pkcs12");

    // read in the "truth" from the command line utilities

    int fd = open("test_rsa_crt.der", O_RDONLY);
    uint8_t true_der[1024];
    ssize_t read_bytes = read(fd, true_der, 1024);
    close(fd);

    assertTrue(read_bytes == 517,
               "could not read %d byte digest from test_rsa_pub_sha256.bin", 517);

    const uint8_t *bb_buffer = parcByteArray_Array(parcBuffer_Array(certificate_der));
    size_t bb_length = parcBuffer_Remaining(certificate_der);
    assertTrue(bb_length == read_bytes,
               "Incorrect digest length returned from GetCertificateDigest: %zu", bb_length);

    assertTrue(memcmp(bb_buffer, true_der, read_bytes) == 0, "digests did not match");

    interface->Destroy(&interface);
    parcBuffer_Release(&certificate_der);
}

/**
 * Gets the DER encoded public key
 */
LONGBOW_TEST_CASE(openssl_commandline, parcPublicKeySignerPkcs12Store_GetEncodedPublicKey)
{
    PARCSigningInterface *interface = parcPublicKeySignerPkcs12Store_Open("test_rsa.p12", "blueberry", PARC_HASH_SHA256);
    assertNotNull(interface, "Got null result from opening openssl pkcs12 file");

    PARCBuffer *pubkey_der = interface->GetDEREncodedPublicKey(interface->interfaceContext);
    assertNotNull(pubkey_der, "got null public key der for external pkcs12");

    // read in the "truth" from the command line utilities

    int fd = open("test_rsa_pub.der", O_RDONLY);
    uint8_t true_der[1024];
    ssize_t read_bytes = read(fd, true_der, 1024);
    close(fd);

    assertTrue(read_bytes == 162, "could not read %d byte digest from test_rsa_pub_sha256.bin", 162);

    const uint8_t *bb_buffer = parcByteArray_Array(parcBuffer_Array(pubkey_der));
    size_t bb_length = parcBuffer_Remaining(pubkey_der);
    assertTrue(bb_length == read_bytes, "Incorrect digest length returned from GetCertificateDigest: %zu", bb_length);

    assertTrue(memcmp(bb_buffer, true_der, read_bytes) == 0, "digests did not match");


    interface->Destroy(&interface);
    parcBuffer_Release(&pubkey_der);
}

/**
 * Pokes around in the keystore memory and compares the private key to the
 * DER form on disk
 */
LONGBOW_TEST_CASE(openssl_commandline, compare_private_key)
{
    PARCSigningInterface *interface = parcPublicKeySignerPkcs12Store_Open("test_rsa.p12", "blueberry", PARC_HASH_SHA256);
    assertNotNull(interface, "Got null result from opening openssl pkcs12 file");

    PARCPublicKeySignerPkcs12Store *fks = (PARCPublicKeySignerPkcs12Store *) interface->interfaceContext;

    uint8_t *priv_key_der = NULL;

    LONGBOW_STOP_DEPRECATED_WARNINGS
    int len = i2d_PrivateKey(fks->private_key, &priv_key_der);
    LONGBOW_START_DEPRECATED_WARNINGS

    assertTrue(len == 608, "wrong size private key DER: %d", len);

    // read in the "truth" from the command line utilities

    int fd = open("test_rsa_key.der", O_RDONLY);
    uint8_t true_der[1024];
    ssize_t read_bytes = read(fd, true_der, 1024);
    close(fd);

    assertTrue(read_bytes == 608, "could not read %d byte digest from test_rsa_key.der", 608);

    assertTrue(memcmp(priv_key_der, true_der, read_bytes) == 0, "digests did not match");


    interface->Destroy(&interface);
    free(priv_key_der);
}

/**
 * Sign the file "test_rsa_pub_sha256.bin" using the test_rsa.p12 private key.
 */
LONGBOW_TEST_CASE(openssl_commandline, parcPublicKeySignerPkcs12Store_SignBuffer)
{
    PARCSigningInterface *interface = parcPublicKeySignerPkcs12Store_Open("test_rsa.p12", "blueberry", PARC_HASH_SHA256);
    assertNotNull(interface, "Got null result from opening openssl pkcs12 file");

    // read the buffer to sign
    int fd = open("test_random_bytes", O_RDONLY);
    assertTrue(fd != -1, "Cannot open test_random_bytes file.");
    uint8_t buffer_to_sign[2048];
    ssize_t read_bytes = read(fd, buffer_to_sign, 2048);
    close(fd);

    // Digest it
    PARCCryptoHasher *digester = interface->GetCryptoHasher(interface->interfaceContext);
    parcCryptoHasher_Init(digester);
    parcCryptoHasher_UpdateBytes(digester, buffer_to_sign, read_bytes);
    PARCCryptoHash *parcDigest = parcCryptoHasher_Finalize(digester);

    PARCSignature *bb_test_sign = interface->SignDigest(interface->interfaceContext, parcDigest);

    assertNotNull(bb_test_sign, "Got null byte buffer from SignBuffer");
    assertTrue(parcBuffer_Remaining(parcSignature_GetSignature(bb_test_sign)) == 128,
               "Incorrect signature size: %zu",
               parcBuffer_Position(parcSignature_GetSignature(bb_test_sign)));

    // now read the "true" signature
    uint8_t scratch_buffer[1024];
    fd = open("test_random_bytes.sig", O_RDONLY);
    assertTrue(fd != -1, "Cannot open test_random_bytes.sig file.");
    read_bytes = read(fd, scratch_buffer, 1024);
    assertTrue(read_bytes == 128, "read incorrect size signature from disk: %zu", read_bytes);
    close(fd);

    const unsigned char *actual = parcByteArray_Array(parcBuffer_Array(parcSignature_GetSignature(bb_test_sign)));

    assertTrue(memcmp(scratch_buffer, actual, read_bytes) == 0,
               "signatures did not match");

    interface->Destroy(&interface);
    parcSignature_Release(&bb_test_sign);
    parcCryptoHash_Release(&parcDigest);
}

LONGBOW_TEST_CASE(openssl_commandline, parcPublicKeySignerPkcs12Store_VerifySignature_Cert)
{
    testUnimplemented("Not Implemented");
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(ccnx_FileKeystore);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
