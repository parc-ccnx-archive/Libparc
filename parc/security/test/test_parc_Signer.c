/*
 * Copyright (c) 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
#include <LongBow/unit-test.h>
#include <sys/param.h>

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../parc_Signer.c"
#include <parc/algol/parc_SafeMemory.h>
#include <parc/security/parc_Security.h>
#include <parc/testing/parc_ObjectTesting.h>

#include <errno.h>

#include <parc/security/parc_PublicKeySignerPkcs12Store.h>
#include <parc/security/parc_SymmetricSignerFileStore.h>

LONGBOW_TEST_RUNNER(parc_Signer)
{
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_Signer)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_Signer)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcSigner_Create_PubKey);
    LONGBOW_RUN_TEST_CASE(Global, parcSigner_Create_HMAC);
    LONGBOW_RUN_TEST_CASE(Global, parcSigner_Acquire);
    LONGBOW_RUN_TEST_CASE(Global, parcSigner_GetCertificateDigest);
    LONGBOW_RUN_TEST_CASE(Global, parcSigner_GetDEREncodedCertificate);
    LONGBOW_RUN_TEST_CASE(Global, parcSigner_CreatePublicKey);
    LONGBOW_RUN_TEST_CASE(Global, parcSigner_CreateKeyId);
    LONGBOW_RUN_TEST_CASE(Global, parcSigner_GetSigningAlgorithm);
    LONGBOW_RUN_TEST_CASE(Global, parcSigner_GetCryptoHashType);
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

LONGBOW_TEST_CASE(Global, parcSigner_Create_PubKey)
{
    char dirname[] = "/tmp/pubkeystore_XXXXXX";
    char filename[MAXPATHLEN];
    const char *password = "flumox";
    unsigned key_bits = 1024;
    unsigned valid_days = 30;

    const char to_sign[] = "it was a dark and stormy night, and all through the house not a digest was creeping";

    char *tmp_dirname = mkdtemp(dirname);
    assertNotNull(tmp_dirname, "tmp_dirname should not be null");
    sprintf(filename, "%s/pubkeystore.p12", tmp_dirname);

    // create the file
    parcPublicKeySignerPkcs12Store_CreateFile(filename, password, "alice", key_bits, valid_days);

    // open it as an RSA provider for the signer
    PARCSigner *signer = parcSigner_Create(parcPublicKeySignerPkcs12Store_Open(filename, password, PARC_HASH_SHA256));
    PARCCryptoHasher *hasher = parcSigner_GetCryptoHasher(signer);
    parcCryptoHasher_Init(hasher);
    parcCryptoHasher_UpdateBytes(hasher, to_sign, sizeof(to_sign));
    PARCCryptoHash *hash = parcCryptoHasher_Finalize(hasher);
    PARCSignature *sig = parcSigner_SignDigest(signer, hash);

    unlink(filename);
    int rc = rmdir(tmp_dirname);
    assertTrue(rc == 0, "directory cleanup failed");

    char *s = parcSignature_ToString(sig);
    printf("Signature: %s\n", s);
    parcMemory_Deallocate((void **) &s);

    parcCryptoHash_Release(&hash);
    parcSignature_Release(&sig);
    parcSigner_Release(&signer);
}

LONGBOW_TEST_CASE(Global, parcSigner_Create_HMAC)
{
    char dirname[] = "/tmp/pubkeystore_XXXXXX";
    char filename[MAXPATHLEN];
    const char *password = "flumox";

    const char to_sign[] = "it was a dark and stormy night, and all through the house not a digest was creeping";

    char *tmp_dirname = mkdtemp(dirname);
    assertNotNull(tmp_dirname, "tmp_dirname should not be null");
    sprintf(filename, "%s/pubkeystore.hmac", tmp_dirname);

    // create the file
    PARCBuffer *secret_key = parcSymmetricSignerFileStore_CreateKey(256);
    parcSymmetricSignerFileStore_CreateFile(filename, password, secret_key);

    // open it as a symmetric key provider for the signer
    PARCSigner *signer = parcSigner_Create(parcSymmetricSignerFileStore_OpenFile(filename, password, PARC_HASH_SHA256));

    // the rest is identical to public key
    PARCCryptoHasher *hasher = parcSigner_GetCryptoHasher(signer);
    parcCryptoHasher_Init(hasher);
    parcCryptoHasher_UpdateBytes(hasher, to_sign, sizeof(to_sign));
    PARCCryptoHash *hash = parcCryptoHasher_Finalize(hasher);
    PARCSignature *sig = parcSigner_SignDigest(signer, hash);

    unlink(filename);
    int rc = rmdir(tmp_dirname);
    assertTrue(rc == 0, "directory cleanup failed");

    char *s = parcSignature_ToString(sig);
    printf("Signature: %s\n", s);
    parcMemory_Deallocate((void **) &s);

    parcBuffer_Release(&secret_key);
    parcCryptoHash_Release(&hash);
    parcSignature_Release(&sig);
    parcSigner_Release(&signer);
}

LONGBOW_TEST_CASE(Global, parcSigner_Acquire)
{
    char dirname[] = "/tmp/pubkeystore_XXXXXX";
    char filename[MAXPATHLEN];
    const char *password = "flumox";
    unsigned key_bits = 1024;
    unsigned valid_days = 30;

    const char to_sign[] = "it was a dark and stormy night, and all through the house not a digest was creeping";

    char *tmp_dirname = mkdtemp(dirname);
    assertNotNull(tmp_dirname, "tmp_dirname should not be null");
    sprintf(filename, "%s/pubkeystore.p12", tmp_dirname);

    // create the file
    parcPublicKeySignerPkcs12Store_CreateFile(filename, password, "alice", key_bits, valid_days);

    // open it as an RSA provider for the signer
    PARCSigner *signer = parcSigner_Create(parcPublicKeySignerPkcs12Store_Open(filename, password, PARC_HASH_SHA256));
    PARCCryptoHasher *hasher = parcSigner_GetCryptoHasher(signer);
    parcCryptoHasher_Init(hasher);
    parcCryptoHasher_UpdateBytes(hasher, to_sign, sizeof(to_sign));
    PARCCryptoHash *hash = parcCryptoHasher_Finalize(hasher);
    PARCSignature *sig = parcSigner_SignDigest(signer, hash);

    unlink(filename);
    int rc = rmdir(tmp_dirname);
    assertTrue(rc == 0, "directory cleanup failed");

    char *s = parcSignature_ToString(sig);
    printf("Signature: %s\n", s);
    parcMemory_Deallocate((void **) &s);

    parcObjectTesting_AssertAcquireReleaseContract(parcSigner_Acquire, signer);

    parcCryptoHash_Release(&hash);
    parcSignature_Release(&sig);
    parcSigner_Release(&signer);
}

LONGBOW_TEST_CASE(Global, parcSigner_GetCertificateDigest)
{
    char dirname[] = "/tmp/pubkeystore_XXXXXX";
    char filename[MAXPATHLEN];
    const char *password = "flumox";
    unsigned key_bits = 1024;
    unsigned valid_days = 30;

    const char to_sign[] = "it was a dark and stormy night, and all through the house not a digest was creeping";

    char *tmp_dirname = mkdtemp(dirname);
    assertNotNull(tmp_dirname, "tmp_dirname should not be null");
    sprintf(filename, "%s/pubkeystore.p12", tmp_dirname);

    // create the file
    parcPublicKeySignerPkcs12Store_CreateFile(filename, password, "alice", key_bits, valid_days);

    // open it as an RSA provider for the signer
    PARCSigner *signer = parcSigner_Create(parcPublicKeySignerPkcs12Store_Open(filename, password, PARC_HASH_SHA256));
    PARCCryptoHasher *hasher = parcSigner_GetCryptoHasher(signer);
    parcCryptoHasher_Init(hasher);
    parcCryptoHasher_UpdateBytes(hasher, to_sign, sizeof(to_sign));
    PARCCryptoHash *hash = parcCryptoHasher_Finalize(hasher);
    PARCSignature *sig = parcSigner_SignDigest(signer, hash);

    unlink(filename);
    int rc = rmdir(tmp_dirname);
    assertTrue(rc == 0, "directory cleanup failed");

    char *s = parcSignature_ToString(sig);
    printf("Signature: %s\n", s);
    parcMemory_Deallocate((void **) &s);

    PARCCryptoHash *certDigest = parcSigner_GetCertificateDigest(signer);
    assertNotNull(certDigest, "Expected a non NULL value");
    parcCryptoHash_Release(&certDigest);

    parcCryptoHash_Release(&hash);
    parcSignature_Release(&sig);
    parcSigner_Release(&signer);
}

LONGBOW_TEST_CASE(Global, parcSigner_GetDEREncodedCertificate)
{
    char dirname[] = "/tmp/pubkeystore_XXXXXX";
    char filename[MAXPATHLEN];
    const char *password = "flumox";
    unsigned key_bits = 1024;
    unsigned valid_days = 30;

    const char to_sign[] = "it was a dark and stormy night, and all through the house not a digest was creeping";

    char *tmp_dirname = mkdtemp(dirname);
    assertNotNull(tmp_dirname, "tmp_dirname should not be null");
    sprintf(filename, "%s/pubkeystore.p12", tmp_dirname);

    // create the file
    parcPublicKeySignerPkcs12Store_CreateFile(filename, password, "alice", key_bits, valid_days);

    // open it as an RSA provider for the signer
    PARCSigner *signer = parcSigner_Create(parcPublicKeySignerPkcs12Store_Open(filename, password, PARC_HASH_SHA256));
    PARCCryptoHasher *hasher = parcSigner_GetCryptoHasher(signer);
    parcCryptoHasher_Init(hasher);
    parcCryptoHasher_UpdateBytes(hasher, to_sign, sizeof(to_sign));
    PARCCryptoHash *hash = parcCryptoHasher_Finalize(hasher);
    PARCSignature *sig = parcSigner_SignDigest(signer, hash);

    unlink(filename);
    int rc = rmdir(tmp_dirname);
    assertTrue(rc == 0, "directory cleanup failed");

    char *s = parcSignature_ToString(sig);
    printf("Signature: %s\n", s);
    parcMemory_Deallocate((void **) &s);

    PARCBuffer *certificate_der = parcSigner_GetDEREncodedCertificate(signer);
    assertNotNull(certificate_der, "Expected a non NULL value");
    parcBuffer_Release(&certificate_der);

    parcCryptoHash_Release(&hash);
    parcSignature_Release(&sig);
    parcSigner_Release(&signer);
}

LONGBOW_TEST_CASE(Global, parcSigner_CreatePublicKey)
{
    char dirname[] = "/tmp/pubkeystore_XXXXXX";
    char filename[MAXPATHLEN];
    const char *password = "flumox";
    unsigned key_bits = 1024;
    unsigned valid_days = 30;

    const char to_sign[] = "it was a dark and stormy night, and all through the house not a digest was creeping";

    char *tmp_dirname = mkdtemp(dirname);
    assertNotNull(tmp_dirname, "tmp_dirname should not be null");
    sprintf(filename, "%s/pubkeystore.p12", tmp_dirname);

    // create the file
    parcPublicKeySignerPkcs12Store_CreateFile(filename, password, "alice", key_bits, valid_days);

    // open it as an RSA provider for the signer
    PARCSigner *signer = parcSigner_Create(parcPublicKeySignerPkcs12Store_Open(filename, password, PARC_HASH_SHA256));
    PARCCryptoHasher *hasher = parcSigner_GetCryptoHasher(signer);
    parcCryptoHasher_Init(hasher);
    parcCryptoHasher_UpdateBytes(hasher, to_sign, sizeof(to_sign));
    PARCCryptoHash *hash = parcCryptoHasher_Finalize(hasher);
    PARCSignature *sig = parcSigner_SignDigest(signer, hash);

    unlink(filename);
    int rc = rmdir(tmp_dirname);
    assertTrue(rc == 0, "directory cleanup failed");

    char *s = parcSignature_ToString(sig);
    printf("Signature: %s\n", s);
    parcMemory_Deallocate((void **) &s);

    PARCKey *key = parcSigner_CreatePublicKey(signer);
    assertNotNull(key, "Expected a non NULL value");
    parcKey_Release(&key);

    parcCryptoHash_Release(&hash);
    parcSignature_Release(&sig);
    parcSigner_Release(&signer);
}

LONGBOW_TEST_CASE(Global, parcSigner_CreateKeyId)
{
    char dirname[] = "/tmp/pubkeystore_XXXXXX";
    char filename[MAXPATHLEN];
    const char *password = "flumox";
    unsigned key_bits = 1024;
    unsigned valid_days = 30;

    const char to_sign[] = "it was a dark and stormy night, and all through the house not a digest was creeping";

    char *tmp_dirname = mkdtemp(dirname);
    assertNotNull(tmp_dirname, "tmp_dirname should not be null");
    sprintf(filename, "%s/pubkeystore.p12", tmp_dirname);

    // create the file
    parcPublicKeySignerPkcs12Store_CreateFile(filename, password, "alice", key_bits, valid_days);

    // open it as an RSA provider for the signer
    PARCSigner *signer = parcSigner_Create(parcPublicKeySignerPkcs12Store_Open(filename, password, PARC_HASH_SHA256));
    PARCCryptoHasher *hasher = parcSigner_GetCryptoHasher(signer);
    parcCryptoHasher_Init(hasher);
    parcCryptoHasher_UpdateBytes(hasher, to_sign, sizeof(to_sign));
    PARCCryptoHash *hash = parcCryptoHasher_Finalize(hasher);
    PARCSignature *sig = parcSigner_SignDigest(signer, hash);

    unlink(filename);
    int rc = rmdir(tmp_dirname);
    assertTrue(rc == 0, "directory cleanup failed");

    char *s = parcSignature_ToString(sig);
    printf("Signature: %s\n", s);
    parcMemory_Deallocate((void **) &s);

    PARCKeyId *keyId = parcSigner_CreateKeyId(signer);
    assertNotNull(keyId, "Expected a non NULL value");
    parcKeyId_Release(&keyId);

    parcCryptoHash_Release(&hash);
    parcSignature_Release(&sig);
    parcSigner_Release(&signer);
}

LONGBOW_TEST_CASE(Global, parcSigner_GetSigningAlgorithm)
{
    char dirname[] = "/tmp/pubkeystore_XXXXXX";
    char filename[MAXPATHLEN];
    const char *password = "flumox";

    const char to_sign[] = "it was a dark and stormy night, and all through the house not a digest was creeping";

    char *tmp_dirname = mkdtemp(dirname);
    assertNotNull(tmp_dirname, "tmp_dirname should not be null");
    sprintf(filename, "%s/pubkeystore.hmac", tmp_dirname);

    // create the file
    PARCBuffer *secret_key = parcSymmetricSignerFileStore_CreateKey(256);
    parcSymmetricSignerFileStore_CreateFile(filename, password, secret_key);

    // open it as a symmetric key provider for the signer
    PARCSigner *signer = parcSigner_Create(parcSymmetricSignerFileStore_OpenFile(filename, password, PARC_HASH_SHA256));

    // the rest is identical to public key
    PARCCryptoHasher *hasher = parcSigner_GetCryptoHasher(signer);
    parcCryptoHasher_Init(hasher);
    parcCryptoHasher_UpdateBytes(hasher, to_sign, sizeof(to_sign));
    PARCCryptoHash *hash = parcCryptoHasher_Finalize(hasher);
    PARCSignature *sig = parcSigner_SignDigest(signer, hash);

    unlink(filename);
    int rc = rmdir(tmp_dirname);
    assertTrue(rc == 0, "directory cleanup failed");

    char *s = parcSignature_ToString(sig);
    printf("Signature: %s\n", s);
    parcMemory_Deallocate((void **) &s);

    assertTrue(parcSigner_GetSigningAlgorithm(signer) == PARCSigningAlgorithm_HMAC, "Expected to be true");

    parcBuffer_Release(&secret_key);
    parcCryptoHash_Release(&hash);
    parcSignature_Release(&sig);
    parcSigner_Release(&signer);
}

LONGBOW_TEST_CASE(Global, parcSigner_GetCryptoHashType)
{
    char dirname[] = "/tmp/pubkeystore_XXXXXX";
    char filename[MAXPATHLEN];
    const char *password = "flumox";

    const char to_sign[] = "it was a dark and stormy night, and all through the house not a digest was creeping";

    char *tmp_dirname = mkdtemp(dirname);
    assertNotNull(tmp_dirname, "tmp_dirname should not be null");
    sprintf(filename, "%s/pubkeystore.hmac", tmp_dirname);

    // create the file
    PARCBuffer *secret_key = parcSymmetricSignerFileStore_CreateKey(256);
    parcSymmetricSignerFileStore_CreateFile(filename, password, secret_key);

    // open it as a symmetric key provider for the signer
    PARCSigner *signer = parcSigner_Create(parcSymmetricSignerFileStore_OpenFile(filename, password, PARC_HASH_SHA256));

    // the rest is identical to public key
    PARCCryptoHasher *hasher = parcSigner_GetCryptoHasher(signer);
    parcCryptoHasher_Init(hasher);
    parcCryptoHasher_UpdateBytes(hasher, to_sign, sizeof(to_sign));
    PARCCryptoHash *hash = parcCryptoHasher_Finalize(hasher);
    PARCSignature *sig = parcSigner_SignDigest(signer, hash);

    unlink(filename);
    int rc = rmdir(tmp_dirname);
    assertTrue(rc == 0, "directory cleanup failed");

    char *s = parcSignature_ToString(sig);
    printf("Signature: %s\n", s);
    parcMemory_Deallocate((void **) &s);

    assertTrue(parcSigner_GetCryptoHashType(signer) == PARC_HASH_SHA256, "Expected to be true");

    parcBuffer_Release(&secret_key);
    parcCryptoHash_Release(&hash);
    parcSignature_Release(&sig);
    parcSigner_Release(&signer);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_Signer);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
