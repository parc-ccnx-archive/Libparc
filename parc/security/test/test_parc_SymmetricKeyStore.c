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

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../parc_SymmetricKeyStore.c"
#include <parc/algol/parc_SafeMemory.h>

#include <errno.h>
#include <sys/param.h>
#include <fcntl.h>

#include <parc/security/parc_SymmetricKeySigner.h>

#include <LongBow/unit-test.h>

LONGBOW_TEST_RUNNER(parc_SymmetricSignerFileStore)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Local);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_SymmetricSignerFileStore)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_SymmetricSignerFileStore)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcSymmetricKeyStore_Create);
    LONGBOW_RUN_TEST_CASE(Global, parcSymmetricKeyStore_CreateKey);
//    LONGBOW_RUN_TEST_CASE(Global, parcSymmetricKeyStore_CreateFail);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);
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

LONGBOW_TEST_CASE(Global, parcSymmetricKeyStore_Create)
{
    char dirname[] = "/tmp/pubkeystore_XXXXXX";
    char filename[MAXPATHLEN];

    char *tmp_dirname = mkdtemp(dirname);
    assertNotNull(tmp_dirname, "tmp_dirname should not be null");
    sprintf(filename, "%s/pubkeystore.p12", tmp_dirname);

    PARCBuffer *secret_key = parcSymmetricKeyStore_CreateKey(256);

    PARCSymmetricKeyStore *symmetricKeyStore = parcSymmetricKeyStore_Create(secret_key);
    PARCSymmetricKeySigner *privateKeySigner = parcSymmetricKeySigner_Create(symmetricKeyStore, PARCCryptoHashType_SHA256);
    PARCSigner *signer = parcSigner_Create(privateKeySigner, PARCSymmetricKeySignerAsSigner);
    parcSymmetricKeySigner_Release(&privateKeySigner);

    parcSymmetricKeyStore_Release(&symmetricKeyStore);
    parcSigner_Release(&signer);
    parcBuffer_Release(&secret_key);
}

LONGBOW_TEST_CASE(Global, parcSymmetricKeyStore_CreateKey)
{
    PARCBuffer *bb = parcSymmetricKeyStore_CreateKey(256);
    assertTrue(parcBuffer_Remaining(bb) == 32, "Key wrong length expected %d got %zu", 32, parcBuffer_Position(bb));
    parcBuffer_Release(&bb);
}

LONGBOW_TEST_CASE_EXPECTS(Global, parcSymmetricKeyStore_CreateFail, .event = &LongBowTrapIllegalValue)
{
    PARCBuffer *key = parcSymmetricKeyStore_CreateKey(256);
    PARCSymmetricKeyStore *symmetricKeyStore = parcSymmetricKeyStore_Create(key);
    PARCSymmetricKeySigner *privateKeySigner = parcSymmetricKeySigner_Create(symmetricKeyStore, PARCCryptoHashType_CRC32C);

    // fail.

    parcSymmetricKeyStore_Release(&symmetricKeyStore);
    parcSymmetricKeySigner_Release(&privateKeySigner);
    parcBuffer_Release(&key);
}

// ==========================================

LONGBOW_TEST_FIXTURE(Local)
{
    LONGBOW_RUN_TEST_CASE(Local, parcSymmetricKeyStore_GetCryptoHashType);
    LONGBOW_RUN_TEST_CASE(Local, parcSymmetricKeyStore_GetSecretKeyDigest);
    LONGBOW_RUN_TEST_CASE(Local, parcSymmetricKeyStore_GetSigningAlgorithm);
    LONGBOW_RUN_TEST_CASE(Local, parcSymmetricKeyStore_SignDigest_sha256);
    LONGBOW_RUN_TEST_CASE(Local, parcSymmetricKeyStore_SignDigest_sha512);
}

LONGBOW_TEST_FIXTURE_SETUP(Local)
{
    parcSecurity_Init();
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Local)
{
    parcSecurity_Fini();
    if (parcSafeMemory_ReportAllocation(STDOUT_FILENO) != 0) {
        printf("('%s' leaks memory by %d (allocs - frees)) ", longBowTestCase_GetName(testCase), parcMemory_Outstanding());
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Local, parcSymmetricKeyStore_GetCryptoHashType)
{
    PARCBuffer *secret_key = parcSymmetricKeyStore_CreateKey(256);

    PARCSymmetricKeyStore *symmetricKeyStore = parcSymmetricKeyStore_Create(secret_key);
    PARCSymmetricKeySigner *privateKeySigner = parcSymmetricKeySigner_Create(symmetricKeyStore, PARCCryptoHashType_SHA256);
    PARCSigner *signer = parcSigner_Create(privateKeySigner, PARCSymmetricKeySignerAsSigner);
    parcSymmetricKeySigner_Release(&privateKeySigner);

    PARCCryptoHashType hashType = parcSigner_GetCryptoHashType(signer);
    assertTrue(hashType == PARCCryptoHashType_SHA256,
               "Got wrong hash Type, expected %d got %d", PARCCryptoHashType_SHA256, hashType);

    parcSymmetricKeyStore_Release(&symmetricKeyStore);
    parcSigner_Release(&signer);
    parcBuffer_Release(&secret_key);
}

LONGBOW_TEST_CASE(Local, parcSymmetricKeyStore_GetSecretKeyDigest)
{
    int fd;
    uint8_t key_buffer[128];
    uint8_t key_sha256[128];
    ssize_t read_len;

    fd = open("test_symmetric_key.bin", O_RDONLY);
    read_len = read(fd, key_buffer, sizeof(key_buffer));
    assertTrue(read_len == 32, "read wrong size, expected 32, got %zd", read_len);
    close(fd);

    fd = open("test_symmetric_key.sha256", O_RDONLY);
    read_len = read(fd, key_sha256, sizeof(key_sha256));
    assertTrue(read_len == 32, "read wrong size, expected 32, got %zd", read_len);
    close(fd);

    PARCBufferComposer *composer = parcBufferComposer_Create();
    parcBufferComposer_PutArray(composer, key_buffer, 32);
    PARCBuffer *secret_key = parcBufferComposer_ProduceBuffer(composer);

    PARCBufferComposer *composer2 = parcBufferComposer_Create();
    parcBufferComposer_PutArray(composer2, key_sha256, 32);
    PARCBuffer *secret_sha = parcBufferComposer_ProduceBuffer(composer2);

    PARCSymmetricKeyStore *symmetricKeyStore = parcSymmetricKeyStore_Create(secret_key);
    PARCSymmetricKeySigner *privateKeySigner = parcSymmetricKeySigner_Create(symmetricKeyStore, PARCCryptoHashType_SHA256);
    PARCSigner *signer = parcSigner_Create(privateKeySigner, PARCSymmetricKeySignerAsSigner);
    parcSymmetricKeySigner_Release(&privateKeySigner);

    PARCCryptoHash *key_hash = parcKeyStore_GetVerifierKeyDigest(parcSigner_GetKeyStore(signer));
    assertTrue(parcBuffer_Equals(parcCryptoHash_GetDigest(key_hash), secret_sha),
               "sha256 digests of secret key did not match");
    parcCryptoHash_Release(&key_hash);

    parcSymmetricKeyStore_Release(&symmetricKeyStore);
    parcBuffer_Release(&secret_sha);
    parcBufferComposer_Release(&composer);
    parcBuffer_Release(&secret_key);
    parcBufferComposer_Release(&composer2);
    parcSigner_Release(&signer);
}

LONGBOW_TEST_CASE(Local, parcSymmetricKeyStore_GetSigningAlgorithm)
{
    PARCBuffer *secret_key = parcSymmetricKeyStore_CreateKey(256);

    PARCSymmetricKeyStore *symmetricKeyStore = parcSymmetricKeyStore_Create(secret_key);
    PARCSymmetricKeySigner *privateKeySigner = parcSymmetricKeySigner_Create(symmetricKeyStore, PARCCryptoHashType_SHA256);
    PARCSigner *signer = parcSigner_Create(privateKeySigner, PARCSymmetricKeySignerAsSigner);
    parcSymmetricKeySigner_Release(&privateKeySigner);

    PARCSigningAlgorithm alg = parcSigner_GetSigningAlgorithm(signer);
    assertTrue(alg == PARCSigningAlgorithm_HMAC,
               "Got wrong signing algorithm, expected %d got %d", PARCSigningAlgorithm_HMAC, alg);

    parcSymmetricKeyStore_Release(&symmetricKeyStore);
    parcSigner_Release(&signer);
    parcBuffer_Release(&secret_key);
}

LONGBOW_TEST_CASE(Local, parcSymmetricKeyStore_SignDigest_sha256)
{
    uint8_t to_digest_buffer[MAXPATHLEN];
    ssize_t to_digest_length;
    uint8_t true_hmac_buffer[MAXPATHLEN];
    char key[] = "apple_pie_is_good";

    PARCBufferComposer *composer = parcBufferComposer_Create();
    parcBufferComposer_PutString(composer, key);
    PARCBuffer *key_buffer = parcBufferComposer_ProduceBuffer(composer);

    int fd = open("test_random_bytes", O_RDONLY);
    assertTrue(fd > 0, "Could not open input file: %s", strerror(errno));
    to_digest_length = read(fd, to_digest_buffer, sizeof(to_digest_buffer));
    assertTrue(to_digest_length > 0, "Could not read input file: %s", strerror(errno));
    close(fd);

    fd = open("test_random_bytes.hmac_sha256", O_RDONLY);
    assertTrue(fd > 0, "Could not open input file: %s", strerror(errno));
    ssize_t true_hmac_length = read(fd, true_hmac_buffer, sizeof(true_hmac_buffer));
    assertTrue(true_hmac_length > 0, "Could not read input file: %s", strerror(errno));
    close(fd);

    PARCBufferComposer *composer2 = parcBufferComposer_Create();
    parcBufferComposer_PutArray(composer2, true_hmac_buffer, true_hmac_length);
    PARCBuffer *true_hash = parcBufferComposer_ProduceBuffer(composer2);

    PARCSymmetricKeyStore *symmetricKeyStore = parcSymmetricKeyStore_Create(key_buffer);
    PARCSymmetricKeySigner *privateKeySigner = parcSymmetricKeySigner_Create(symmetricKeyStore, PARCCryptoHashType_SHA256);
    PARCSigner *signer = parcSigner_Create(privateKeySigner, PARCSymmetricKeySignerAsSigner);
    parcSymmetricKeySigner_Release(&privateKeySigner);

    PARCCryptoHasher *hasher = parcSigner_GetCryptoHasher(signer);
    parcCryptoHasher_Init(hasher);
    parcCryptoHasher_UpdateBytes(hasher, to_digest_buffer, to_digest_length);
    PARCCryptoHash *hash = parcCryptoHasher_Finalize(hasher);
    PARCSignature *sig = parcSigner_SignDigest(signer, hash);

    assertTrue(parcBuffer_Equals(parcSignature_GetSignature(sig), parcCryptoHash_GetDigest(hash)),
               "Hashes are not equal");
    assertTrue(parcBuffer_Equals(parcSignature_GetSignature(sig), true_hash),
               "Hash does not match file");
    assertTrue(parcSignature_GetSigningAlgorithm(sig) == PARCSigningAlgorithm_HMAC,
               "Signing alg incorrect, expected %d got %d",
               PARCSigningAlgorithm_HMAC, parcSignature_GetSigningAlgorithm(sig));
    assertTrue(parcSignature_GetHashType(sig) == PARCCryptoHashType_SHA256,
               "Digest alg incorrect, expected %d got %d",
               PARCCryptoHashType_SHA256, parcSignature_GetSigningAlgorithm(sig));

    parcSymmetricKeyStore_Release(&symmetricKeyStore);
    parcSignature_Release(&sig);
    parcCryptoHash_Release(&hash);
    parcBuffer_Release(&true_hash);
    parcBuffer_Release(&key_buffer);
    parcBufferComposer_Release(&composer);
    parcBufferComposer_Release(&composer2);
    parcSigner_Release(&signer);
}

LONGBOW_TEST_CASE(Local, parcSymmetricKeyStore_SignDigest_sha512)
{
    char key[] = "apple_pie_is_good";

    PARCBufferComposer *composer = parcBufferComposer_Create();
    parcBufferComposer_PutString(composer, key);
    PARCBuffer *key_buffer = parcBufferComposer_ProduceBuffer(composer);

    int fd;
    uint8_t to_digest_buffer[MAXPATHLEN];
    ssize_t to_digest_length;

    uint8_t true_hmac_buffer[MAXPATHLEN];
    ssize_t true_hmac_length;

    fd = open("test_random_bytes", O_RDONLY);
    assertTrue(fd > 0, "Could not open input file: %s", strerror(errno));
    to_digest_length = read(fd, to_digest_buffer, sizeof(to_digest_buffer));
    assertTrue(to_digest_length > 0, "Could not read input file: %s", strerror(errno));
    close(fd);

    fd = open("test_random_bytes.hmac_sha512", O_RDONLY);
    assertTrue(fd > 0, "Could not open input file: %s", strerror(errno));
    true_hmac_length = read(fd, true_hmac_buffer, sizeof(true_hmac_buffer));
    assertTrue(true_hmac_length > 0, "Could not read input file: %s", strerror(errno));
    close(fd);

    PARCBufferComposer *composer2 = parcBufferComposer_Create();
    parcBufferComposer_PutArray(composer2, true_hmac_buffer, true_hmac_length);
    PARCBuffer *true_hash = parcBufferComposer_ProduceBuffer(composer2);

    PARCSymmetricKeyStore *symmetricKeyStore = parcSymmetricKeyStore_Create(key_buffer);
    PARCSymmetricKeySigner *privateKeySigner = parcSymmetricKeySigner_Create(symmetricKeyStore, PARCCryptoHashType_SHA512);
    PARCSigner *signer = parcSigner_Create(privateKeySigner, PARCSymmetricKeySignerAsSigner);
    parcSymmetricKeySigner_Release(&privateKeySigner);

    PARCCryptoHasher *hasher = parcSigner_GetCryptoHasher(signer);
    parcCryptoHasher_Init(hasher);
    parcCryptoHasher_UpdateBytes(hasher, to_digest_buffer, to_digest_length);
    PARCCryptoHash *hash = parcCryptoHasher_Finalize(hasher);
    PARCSignature *sig = parcSigner_SignDigest(signer, hash);

    assertTrue(parcBuffer_Equals(parcSignature_GetSignature(sig), parcCryptoHash_GetDigest(hash)),
               "Hashes are not equal");
    assertTrue(parcBuffer_Equals(parcSignature_GetSignature(sig), true_hash),
               "Hash does not match file");
    assertTrue(parcSignature_GetSigningAlgorithm(sig) == PARCSigningAlgorithm_HMAC,
               "Signing alg incorrect, expected %d got %d",
               PARCSigningAlgorithm_HMAC, parcSignature_GetSigningAlgorithm(sig));
    assertTrue(parcSignature_GetHashType(sig) == PARCCryptoHashType_SHA512,
               "Digest alg incorrect, expected %d got %d",
               PARCCryptoHashType_SHA512, parcSignature_GetSigningAlgorithm(sig));

    parcSymmetricKeyStore_Release(&symmetricKeyStore);
    parcSignature_Release(&sig);
    parcCryptoHash_Release(&hash);
    parcBuffer_Release(&true_hash);
    parcBuffer_Release(&key_buffer);
    parcBufferComposer_Release(&composer);
    parcBufferComposer_Release(&composer2);
    parcSigner_Release(&signer);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_SymmetricSignerFileStore);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
