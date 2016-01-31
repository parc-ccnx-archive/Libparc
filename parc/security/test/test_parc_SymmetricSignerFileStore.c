/*
 * Copyright (c) 2014-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../parc_SymmetricSignerFileStore.c"
#include <parc/algol/parc_SafeMemory.h>
#include <errno.h>

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
    LONGBOW_RUN_TEST_CASE(Global, parcSymmetricSignerFileStore_Create);
    LONGBOW_RUN_TEST_CASE(Global, parcSymmetricSignerFileStore_CreateFile);
    LONGBOW_RUN_TEST_CASE(Global, parcSymmetricSignerFileStore_CreateKey);
    LONGBOW_RUN_TEST_CASE(Global, parcSymmetricSignerFileStore_OpenFile);
    LONGBOW_RUN_TEST_CASE(Global, parcSymmetricSignerFileStore_CreateFail);
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

LONGBOW_TEST_CASE(Global, parcSymmetricSignerFileStore_Create)
{
    PARCBuffer *key = parcSymmetricSignerFileStore_CreateKey(256);
    PARCSigningInterface *interface = parcSymmetricSignerFileStore_Create(key, PARC_HASH_SHA256);
    interface->Destroy(&interface);
    parcBuffer_Release(&key);
}

LONGBOW_TEST_CASE(Global, parcSymmetricSignerFileStore_CreateKey)
{
    PARCBuffer *bb;

    bb = parcSymmetricSignerFileStore_CreateKey(128);
    assertTrue(parcBuffer_Remaining(bb) == 16, "Key wrong length expected %d got %zu", 16, parcBuffer_Position(bb));
    parcBuffer_Release(&bb);

    bb = parcSymmetricSignerFileStore_CreateKey(256);
    assertTrue(parcBuffer_Remaining(bb) == 32, "Key wrong length expected %d got %zu", 32, parcBuffer_Position(bb));
    parcBuffer_Release(&bb);

    bb = parcSymmetricSignerFileStore_CreateKey(512);
    assertTrue(parcBuffer_Remaining(bb) == 64, "Key wrong length expected %d got %zu", 64, parcBuffer_Position(bb));
    parcBuffer_Release(&bb);
}

LONGBOW_TEST_CASE(Global, parcSymmetricSignerFileStore_CreateFile)
{
    const char *filename = "/tmp/aes_keystore.bin";
    const char *password = "yellow_vortex";

    PARCBuffer *secret_key = parcSymmetricSignerFileStore_CreateKey(256);

    int result = parcSymmetricSignerFileStore_CreateFile(filename, password, secret_key);
    assertTrue(result == 0, "got error");

    parcBuffer_Release(&secret_key);
    unlink(filename);
}

LONGBOW_TEST_CASE(Global, parcSymmetricSignerFileStore_OpenFile)
{
    const char *filename = "/tmp/aes_keystore.bin";
    const char *password = "yellow_vortex";

    PARCBuffer *secret_key = parcSymmetricSignerFileStore_CreateKey(256);
    parcSymmetricSignerFileStore_CreateFile(filename, password, secret_key);

    PARCSigningInterface *interface = parcSymmetricSignerFileStore_OpenFile(filename, password, PARC_HASH_SHA256);
    assertNotNull(interface, "Expected non null");

    parcBuffer_Release(&secret_key);
    interface->Destroy(&interface);
    unlink(filename);
}

LONGBOW_TEST_CASE_EXPECTS(Global, parcSymmetricSignerFileStore_CreateFail, .event = &LongBowTrapIllegalValue)
{
    PARCBuffer *key = parcSymmetricSignerFileStore_CreateKey(256);
    PARCSigningInterface *interface = parcSymmetricSignerFileStore_Create(key, PARC_HASH_CRC32C);
    interface->Destroy(&interface);
    parcBuffer_Release(&key);
}

// ==========================================

LONGBOW_TEST_FIXTURE(Local)
{
    LONGBOW_RUN_TEST_CASE(Local, test_hmac_sha256);
    LONGBOW_RUN_TEST_CASE(Local, test_hmac_sha512);
    LONGBOW_RUN_TEST_CASE(Local, parcSymmetricSignerFileStore_GetCryptoHashType);
    LONGBOW_RUN_TEST_CASE(Local, parcSymmetricSignerFileStore_GetSecretKeyDigest);
    LONGBOW_RUN_TEST_CASE(Local, parcSymmetricSignerFileStore_GetSigningAlgorithm);
    LONGBOW_RUN_TEST_CASE(Local, parcSymmetricSignerFileStore_SignDigest_sha256);
    LONGBOW_RUN_TEST_CASE(Local, parcSymmetricSignerFileStore_SignDigest_sha512);
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

LONGBOW_TEST_CASE(Local, test_hmac_sha256)
{
    HMAC_CTX ctx;
    char key[] = "apple_pie_is_good";
    int fd;
    uint8_t to_digest_buffer[1024];
    ssize_t to_digest_length;

    uint8_t true_hmac_buffer[1024];
    ssize_t true_hmac_length;

#ifdef __APPLE__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif
    HMAC_CTX_init(&ctx);
    HMAC_Init_ex(&ctx, key, sizeof(key), EVP_sha256(), NULL);
#ifdef __APPLE__
#pragma clang diagnostic pop
#endif

    fd = open("test_random_bytes", O_RDONLY);
    assertTrue(fd > 0, "Could not open input file: %s", strerror(errno));
    to_digest_length = read(fd, to_digest_buffer, sizeof(to_digest_buffer));
    assertTrue(to_digest_length > 0, "Could not read input file: %s", strerror(errno));
    close(fd);

    fd = open("test_random_bytes.hmac_sha256", O_RDONLY);
    assertTrue(fd > 0, "Could not open input file: %s", strerror(errno));
    true_hmac_length = read(fd, true_hmac_buffer, sizeof(true_hmac_buffer));
    assertTrue(true_hmac_length > 0, "Could not read input file: %s", strerror(errno));
    close(fd);

    _hmacInit(&ctx);
    _hmacUpdate(&ctx, to_digest_buffer, to_digest_length);
    PARCBuffer *output = _hmacFinalize(&ctx);

    assertTrue(parcBuffer_Position(output) == true_hmac_length,
               "hmac wrong length, expected %zu got %zu",
               true_hmac_length,
               parcBuffer_Position(output));

    assertTrue(memcmp(parcByteArray_Array(parcBuffer_Array(output)), true_hmac_buffer, true_hmac_length) == 0,
               "hmac values did not match");

#ifdef __APPLE__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif
    HMAC_cleanup(&ctx);
#ifdef __APPLE__
#pragma clang diagnostic pop
#endif

    parcBuffer_Release(&output);
}

LONGBOW_TEST_CASE(Local, test_hmac_sha512)
{
    HMAC_CTX ctx;
    char key[] = "apple_pie_is_good";
    int fd;
    uint8_t to_digest_buffer[1024];
    ssize_t to_digest_length;

    uint8_t true_hmac_buffer[1024];
    ssize_t true_hmac_length;

#ifdef __APPLE__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif
    HMAC_CTX_init(&ctx);
    HMAC_Init_ex(&ctx, key, sizeof(key), EVP_sha512(), NULL);
#ifdef __APPLE__
#pragma clang diagnostic pop
#endif
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

    _hmacInit(&ctx);
    _hmacUpdate(&ctx, to_digest_buffer, to_digest_length);
    PARCBuffer *output = _hmacFinalize(&ctx);

    assertTrue(parcBuffer_Position(output) == true_hmac_length,
               "hmac wrong length, expected %zu got %zu",
               true_hmac_length,
               parcBuffer_Position(output));

    assertTrue(memcmp(parcByteArray_Array(parcBuffer_Array(output)), true_hmac_buffer, true_hmac_length) == 0,
               "hmac values did not match");

#ifdef __APPLE__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif
    HMAC_cleanup(&ctx);
#ifdef __APPLE__
#pragma clang diagnostic pop
#endif
    parcBuffer_Release(&output);
}

LONGBOW_TEST_CASE(Local, parcSymmetricSignerFileStore_GetCryptoHashType)
{
    PARCBuffer *key = parcSymmetricSignerFileStore_CreateKey(256);
    PARCSigningInterface *interface = parcSymmetricSignerFileStore_Create(key, PARC_HASH_SHA256);
    PARCCryptoHashType hashType = interface->GetCryptoHashType(interface->interfaceContext);
    assertTrue(hashType == PARC_HASH_SHA256,
               "Got wrong hash Type, expected %d got %d", PARC_HASH_SHA256, hashType);
    interface->Destroy(&interface);
    parcBuffer_Release(&key);
}

LONGBOW_TEST_CASE(Local, parcSymmetricSignerFileStore_GetSecretKeyDigest)
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

    PARCSigningInterface *interface = parcSymmetricSignerFileStore_Create(secret_key, PARC_HASH_SHA256);

    PARCCryptoHash *key_hash = interface->GetVerifierKeyDigest(interface->interfaceContext);
    assertTrue(parcBuffer_Equals(parcCryptoHash_GetDigest(key_hash), secret_sha),
               "sha256 digests of secret key did not match");

    parcBuffer_Release(&secret_sha);
    parcBufferComposer_Release(&composer);
    parcBuffer_Release(&secret_key);
    parcBufferComposer_Release(&composer2);
    interface->Destroy(&interface);
}

LONGBOW_TEST_CASE(Local, parcSymmetricSignerFileStore_GetSigningAlgorithm)
{
    PARCBuffer *key = parcSymmetricSignerFileStore_CreateKey(256);
    PARCSigningInterface *interface = parcSymmetricSignerFileStore_Create(key, PARC_HASH_SHA256);
    PARCSigningAlgorithm alg = interface->GetSigningAlgorithm(interface->interfaceContext);
    assertTrue(alg == PARCSigningAlgorithm_HMAC,
               "Got wrong signing algorithm, expected %d got %d", PARCSigningAlgorithm_HMAC, alg);
    interface->Destroy(&interface);
    parcBuffer_Release(&key);
}

LONGBOW_TEST_CASE(Local, parcSymmetricSignerFileStore_SignDigest_sha256)
{
    uint8_t to_digest_buffer[1024];
    ssize_t to_digest_length;
    uint8_t true_hmac_buffer[1024];
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

    PARCSigningInterface *interface = parcSymmetricSignerFileStore_Create(key_buffer, PARC_HASH_SHA256);
    PARCCryptoHasher *hasher = interface->GetCryptoHasher(interface->interfaceContext);
    parcCryptoHasher_Init(hasher);
    parcCryptoHasher_UpdateBytes(hasher, to_digest_buffer, to_digest_length);
    PARCCryptoHash *hash = parcCryptoHasher_Finalize(hasher);
    PARCSignature *sig = interface->SignDigest(interface->interfaceContext, hash);

    assertTrue(parcBuffer_Equals(parcSignature_GetSignature(sig), parcCryptoHash_GetDigest(hash)),
               "Hashes are not equal");
    assertTrue(parcBuffer_Equals(parcSignature_GetSignature(sig), true_hash),
               "Hash does not match file");
    assertTrue(parcSignature_GetSigningAlgorithm(sig) == PARCSigningAlgorithm_HMAC,
               "Signing alg incorrect, expected %d got %d",
               PARCSigningAlgorithm_HMAC, parcSignature_GetSigningAlgorithm(sig));
    assertTrue(parcSignature_GetHashType(sig) == PARC_HASH_SHA256,
               "Digest alg incorrect, expected %d got %d",
               PARC_HASH_SHA256, parcSignature_GetSigningAlgorithm(sig));

    parcSignature_Release(&sig);
    parcCryptoHash_Release(&hash);
    parcBuffer_Release(&true_hash);
    parcBuffer_Release(&key_buffer);
    parcBufferComposer_Release(&composer);
    parcBufferComposer_Release(&composer2);
    interface->Destroy(&interface);
}

LONGBOW_TEST_CASE(Local, parcSymmetricSignerFileStore_SignDigest_sha512)
{
    char key[] = "apple_pie_is_good";

    PARCBufferComposer *composer = parcBufferComposer_Create();
    parcBufferComposer_PutString(composer, key);
    PARCBuffer *key_buffer = parcBufferComposer_ProduceBuffer(composer);

    int fd;
    uint8_t to_digest_buffer[1024];
    ssize_t to_digest_length;

    uint8_t true_hmac_buffer[1024];
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

    PARCSigningInterface *interface = parcSymmetricSignerFileStore_Create(key_buffer, PARC_HASH_SHA512);
    PARCCryptoHasher *hasher = interface->GetCryptoHasher(interface->interfaceContext);
    parcCryptoHasher_Init(hasher);
    parcCryptoHasher_UpdateBytes(hasher, to_digest_buffer, to_digest_length);
    PARCCryptoHash *hash = parcCryptoHasher_Finalize(hasher);
    PARCSignature *sig = interface->SignDigest(interface->interfaceContext, hash);

    assertTrue(parcBuffer_Equals(parcSignature_GetSignature(sig), parcCryptoHash_GetDigest(hash)),
               "Hashes are not equal");
    assertTrue(parcBuffer_Equals(parcSignature_GetSignature(sig), true_hash),
               "Hash does not match file");
    assertTrue(parcSignature_GetSigningAlgorithm(sig) == PARCSigningAlgorithm_HMAC,
               "Signing alg incorrect, expected %d got %d",
               PARCSigningAlgorithm_HMAC, parcSignature_GetSigningAlgorithm(sig));
    assertTrue(parcSignature_GetHashType(sig) == PARC_HASH_SHA512,
               "Digest alg incorrect, expected %d got %d",
               PARC_HASH_SHA512, parcSignature_GetSigningAlgorithm(sig));

    parcSignature_Release(&sig);
    parcCryptoHash_Release(&hash);
    parcBuffer_Release(&true_hash);
    parcBuffer_Release(&key_buffer);
    parcBufferComposer_Release(&composer);
    parcBufferComposer_Release(&composer2);
    interface->Destroy(&interface);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_SymmetricSignerFileStore);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
