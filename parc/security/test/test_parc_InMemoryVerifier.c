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
#include "../parc_InMemoryVerifier.c"

#include <parc/algol/parc_SafeMemory.h>
#include <parc/algol/parc_BufferComposer.h>
#include <parc/security/parc_Security.h>

#include <parc/security/parc_Pkcs12KeyStore.h>
#include <parc/security/parc_KeyStore.h>
#include <parc/security/parc_PublicKeySigner.h>
#include <parc/security/parc_Signer.h>

#include <fcntl.h>
#include <LongBow/unit-test.h>

LONGBOW_TEST_RUNNER(parc_InMemoryVerifier)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Local);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_InMemoryVerifier)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_InMemoryVerifier)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    parcSecurity_Init();
    LONGBOW_RUN_TEST_CASE(Global, parcInMemoryVerifier_Create);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    parcSecurity_Fini();
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

LONGBOW_TEST_CASE(Global, parcInMemoryVerifier_Create)
{
    PARCInMemoryVerifier *verifier = parcInMemoryVerifier_Create();

    assertNotNull(verifier, "Got null result from parcInMemoryVerifier_Create");

    parcInMemoryVerifier_Release(&verifier);
}

// ===========
// We use the known keys on disk for these tests

typedef struct test_data {
    PARCSigner *signer;
    PARCInMemoryVerifier *inMemoryInterface;
} TestData;

LONGBOW_TEST_FIXTURE(Local)
{
    LONGBOW_RUN_TEST_CASE(Local, parcInMemoryVerifier_AddKeyId);
    LONGBOW_RUN_TEST_CASE(Local, parcInMemoryVerifier_GetCryptoHasher);
    LONGBOW_RUN_TEST_CASE(Local, parcInMemoryVerifier_AllowedCryptoSuite_RSA);

    LONGBOW_RUN_TEST_CASE(Local, parcInMemoryVerifier_RemoveKeyId);
    LONGBOW_RUN_TEST_CASE(Local, parcInMemoryVerifier_VerifySignature);
    LONGBOW_RUN_TEST_CASE(Local, parcInMemoryVerifier_VerifySignature_BadHashAlg);
    LONGBOW_RUN_TEST_CASE(Local, parcInMemoryVerifier_VerifySignature_BadSigAlg);
    LONGBOW_RUN_TEST_CASE(Local, parcInMemoryVerifier_VerifySignature_BadHash);
}

LONGBOW_TEST_FIXTURE_SETUP(Local)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);
    parcSecurity_Init();

    TestData *data = parcMemory_AllocateAndClear(sizeof(TestData));
    assertNotNull(data, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(TestData));

    PARCPkcs12KeyStore *publicKeyStore = parcPkcs12KeyStore_Open("test_rsa.p12", "blueberry", PARC_HASH_SHA256);
    PARCKeyStore *keyStore = parcKeyStore_Create(publicKeyStore, PARCPkcs12KeyStoreAsKeyStore);
    parcPkcs12KeyStore_Release(&publicKeyStore);

    PARCPublicKeySigner *publicKeySigner = parcPublicKeySigner_Create(keyStore, PARCSigningAlgorithm_RSA, PARC_HASH_SHA256);
    parcKeyStore_Release(&keyStore);

    data->signer = parcSigner_Create(publicKeySigner, PARCPublicKeySignerAsSigner);
    parcPublicKeySigner_Release(&publicKeySigner);
    assertNotNull(data->signer, "Got null result from opening openssl pkcs12 file");

    data->inMemoryInterface = parcInMemoryVerifier_Create();

    longBowTestCase_SetClipBoardData(testCase, data);

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Local)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);

    parcInMemoryVerifier_Release(&data->inMemoryInterface);
    parcSigner_Release(&data->signer);
    parcMemory_Deallocate((void **) &data);

    parcSecurity_Fini();

    if (parcSafeMemory_ReportAllocation(STDOUT_FILENO) != 0) {
        printf("('%s' leaks memory by %d (allocs - frees)) ", longBowTestCase_GetName(testCase), parcMemory_Outstanding());
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Local, parcInMemoryVerifier_GetCryptoHasher)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);

    PARCKey *key = parcSigner_CreatePublicKey(data->signer);
    _parcInMemoryVerifier_AddKey(data->inMemoryInterface, key);

    PARCCryptoHasher *hasher = _parcInMemoryVerifier_GetCryptoHasher(data->inMemoryInterface, parcKey_GetKeyId(key), PARC_HASH_SHA256);
    parcKey_Release(&key);
    assertNotNull(hasher, "Got a null hasher");
}

LONGBOW_TEST_CASE(Local, parcInMemoryVerifier_AddKeyId)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);

    // create the key with copies of the byte buffers
    PARCKey *key = parcSigner_CreatePublicKey(data->signer);

    _parcInMemoryVerifier_AddKey(data->inMemoryInterface, key);

    // now do something that uses the key
    bool success = _parcInMemoryVerifier_AllowedCryptoSuite(data->inMemoryInterface, parcKey_GetKeyId(key), PARCCryptoSuite_RSA_SHA256);
    parcKey_Release(&key);
    assertTrue(success, "Should have allowed PARCCryptoSuite_RSA_SHA256 for an RSA keystore");
}

LONGBOW_TEST_CASE(Local, parcInMemoryVerifier_AllowedCryptoSuite_RSA)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);

    PARCKey *key = parcSigner_CreatePublicKey(data->signer);
    _parcInMemoryVerifier_AddKey(data->inMemoryInterface, key);

    // now do something that uses the key
    bool success = _parcInMemoryVerifier_AllowedCryptoSuite(data->inMemoryInterface, parcKey_GetKeyId(key), PARCCryptoSuite_RSA_SHA256);
    parcKey_Release(&key);
    assertTrue(success, "Should have allowed PARCCryptoSuite_RSA_SHA256 for an RSA keystore");
}

LONGBOW_TEST_CASE(Local, parcInMemoryVerifier_RemoveKeyId)
{
    testUnimplemented("This test is unimplemented");
}

/**
 * Verify the openssl signature using the public key and our locally computed hash
 */
LONGBOW_TEST_CASE(Local, parcInMemoryVerifier_VerifySignature)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);

    // Setup the key in the verifier

    PARCKey *key = parcSigner_CreatePublicKey(data->signer);

    _parcInMemoryVerifier_AddKey(data->inMemoryInterface, key);

    // read the buffer to sign
    int fd = open("test_random_bytes", O_RDONLY);
    uint8_t buffer_to_sign[2048];
    ssize_t read_bytes = read(fd, buffer_to_sign, 2048);
    close(fd);

    // Digest it
    PARCCryptoHasher *digester = _parcInMemoryVerifier_GetCryptoHasher(data->inMemoryInterface, parcKey_GetKeyId(key), PARC_HASH_SHA256);
    assertNotNull(digester, "got null cryptohasher from inmemory verifier");

    parcCryptoHasher_Init(digester);
    parcCryptoHasher_UpdateBytes(digester, buffer_to_sign, read_bytes);
    PARCCryptoHash *localHash = parcCryptoHasher_Finalize(digester);

    // now read the "true" signature
    uint8_t scratch_buffer[1024];
    fd = open("test_random_bytes.sig", O_RDONLY);
    read_bytes = read(fd, scratch_buffer, 1024);
    assertTrue(read_bytes == 128,
               "read incorrect size signature from disk: %zu", read_bytes);
    close(fd);

    PARCBufferComposer *composer = parcBufferComposer_Create();
    parcBufferComposer_PutArray(composer, scratch_buffer, read_bytes);
    PARCBuffer *bb_sig = parcBufferComposer_ProduceBuffer(composer);

    PARCSignature *signatureToVerify = parcSignature_Create(PARCSigningAlgorithm_RSA, PARC_HASH_SHA256, bb_sig);
    parcBuffer_Release(&bb_sig);
    parcBufferComposer_Release(&composer);

    bool success = _parcInMemoryVerifier_VerifyDigest(data->inMemoryInterface, parcKey_GetKeyId(key), localHash, PARCCryptoSuite_RSA_SHA256, signatureToVerify);

    parcSignature_Release(&signatureToVerify);
    parcCryptoHash_Release(&localHash);
    parcKey_Release(&key);

    assertTrue(success, "Could not validate signature");
}

/**
 * Same as the "good" code above, but calculate the hash with the wrong hash algorithm.  This is
 * what would happen if the signer and the verifier did not use the same hash algorithym.
 */
LONGBOW_TEST_CASE(Local, parcInMemoryVerifier_VerifySignature_BadHashAlg)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);
    bool success;

    // Setup the key in the verifier

    PARCKey *key = parcSigner_CreatePublicKey(data->signer);

    _parcInMemoryVerifier_AddKey(data->inMemoryInterface, key);

    // read the buffer to sign
    int fd = open("test_random_bytes", O_RDONLY);
    uint8_t buffer_to_sign[2048];
    ssize_t read_bytes = read(fd, buffer_to_sign, 2048);
    close(fd);

    // Digest it WITH THE WRONG HASH
    PARCCryptoHasher *digester = _parcInMemoryVerifier_GetCryptoHasher(data->inMemoryInterface, parcKey_GetKeyId(key), PARC_HASH_SHA512);
    assertNotNull(digester, "got null cryptohasher from inmemory verifier");

    parcCryptoHasher_Init(digester);
    parcCryptoHasher_UpdateBytes(digester, buffer_to_sign, read_bytes);
    PARCCryptoHash *localHash = parcCryptoHasher_Finalize(digester);

    // now read the "true" signature
    uint8_t scratch_buffer[1024];
    fd = open("test_random_bytes.sig", O_RDONLY);
    read_bytes = read(fd, scratch_buffer, 1024);
    assertTrue(read_bytes == 128, "read incorrect size signature from disk: %zu", read_bytes);
    close(fd);

    PARCBufferComposer *composer = parcBufferComposer_Create();
    parcBufferComposer_PutArray(composer, scratch_buffer, read_bytes);
    PARCBuffer *bb_sig = parcBufferComposer_ProduceBuffer(composer);

    PARCSignature *signatureToVerify = parcSignature_Create(PARCSigningAlgorithm_RSA,
                                                            PARC_HASH_SHA256,
                                                            bb_sig);
    parcBuffer_Release(&bb_sig);
    parcBufferComposer_Release(&composer);

    success = _parcInMemoryVerifier_VerifyDigest(data->inMemoryInterface, parcKey_GetKeyId(key), localHash, PARCCryptoSuite_RSA_SHA256, signatureToVerify);

    parcSignature_Release(&signatureToVerify);
    parcCryptoHash_Release(&localHash);
    parcKey_Release(&key);

    assertFalse(success, "Signatures should not have verified!  Wrong hash types!");
}


/**
 * Same as the "good" code, but tell the verifier the wrong key type.  This is what would
 * happen if the verifier somehow picked the wrong cryptosuite.
 */
LONGBOW_TEST_CASE(Local, parcInMemoryVerifier_VerifySignature_BadSigAlg)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);

    // Setup the key in the verifier
    PARCKey *key = parcSigner_CreatePublicKey(data->signer);

    _parcInMemoryVerifier_AddKey(data->inMemoryInterface, key);

    // read the buffer to sign
    int fd = open("test_random_bytes", O_RDONLY);
    uint8_t buffer_to_sign[2048];
    ssize_t read_bytes = read(fd, buffer_to_sign, 2048);
    close(fd);

    // Digest it
    PARCCryptoHasher *digester = _parcInMemoryVerifier_GetCryptoHasher(data->inMemoryInterface, parcKey_GetKeyId(key), PARC_HASH_SHA256);
    assertNotNull(digester, "got null cryptohasher from inmemory verifier");

    parcCryptoHasher_Init(digester);
    parcCryptoHasher_UpdateBytes(digester, buffer_to_sign, read_bytes);
    PARCCryptoHash *localHash = parcCryptoHasher_Finalize(digester);

    // now read the "true" signature
    uint8_t scratch_buffer[1024];
    fd = open("test_random_bytes.sig", O_RDONLY);
    read_bytes = read(fd, scratch_buffer, 1024);
    assertTrue(read_bytes == 128, "read incorrect size signature from disk: %zu", read_bytes);
    close(fd);

    PARCBufferComposer *composer = parcBufferComposer_Create();
    parcBufferComposer_PutArray(composer, scratch_buffer, read_bytes);
    PARCBuffer *bb_sig = parcBufferComposer_ProduceBuffer(composer);

    // HERE WE TELL IT DSA, NOT RSA
    PARCSignature *signatureToVerify = parcSignature_Create(PARCSigningAlgorithm_DSA,
                                                            PARC_HASH_SHA256,
                                                            bb_sig);
    parcBuffer_Release(&bb_sig);
    parcBufferComposer_Release(&composer);

    bool success = _parcInMemoryVerifier_VerifyDigest(data->inMemoryInterface, parcKey_GetKeyId(key), localHash, PARCCryptoSuite_RSA_SHA256, signatureToVerify);

    parcSignature_Release(&signatureToVerify);
    parcCryptoHash_Release(&localHash);
    parcKey_Release(&key);

    assertFalse(success, "Signatures should not have verified!  Wrong hash types!");
}

/**
 * THis tests the locally computed digest not matching te digest used for the signature.
 */
LONGBOW_TEST_CASE(Local, parcInMemoryVerifier_VerifySignature_BadHash)
{
    TestData *data = longBowTestCase_GetClipBoardData(testCase);

    // Setup the key in the verifier

    PARCKey *key = parcSigner_CreatePublicKey(data->signer);

    _parcInMemoryVerifier_AddKey(data->inMemoryInterface, key);

    // read the buffer to sign
    int fd = open("test_random_bytes", O_RDONLY);
    uint8_t buffer_to_sign[2048];
    ssize_t read_bytes = read(fd, buffer_to_sign, 2048);
    close(fd);

    // Digest it
    PARCCryptoHasher *digester = _parcInMemoryVerifier_GetCryptoHasher(data->inMemoryInterface, parcKey_GetKeyId(key), PARC_HASH_SHA256);
    assertNotNull(digester, "got null cryptohasher from inmemory verifier");

    // DIGEST THE BYTES TWICE TO GIVE WRONG HASH
    parcCryptoHasher_Init(digester);
    parcCryptoHasher_UpdateBytes(digester, buffer_to_sign, read_bytes);
    parcCryptoHasher_UpdateBytes(digester, buffer_to_sign, read_bytes);
    PARCCryptoHash *localHash = parcCryptoHasher_Finalize(digester);

    // now read the "true" signature
    uint8_t scratch_buffer[1024];
    fd = open("test_random_bytes.sig", O_RDONLY);
    read_bytes = read(fd, scratch_buffer, 1024);
    assertTrue(read_bytes == 128, "read incorrect size signature from disk: %zu", read_bytes);
    close(fd);

    PARCBufferComposer *composer = parcBufferComposer_Create();
    parcBufferComposer_PutArray(composer, scratch_buffer, read_bytes);
    PARCBuffer *bb_sig = parcBufferComposer_ProduceBuffer(composer);

    PARCSignature *signatureToVerify = parcSignature_Create(PARCSigningAlgorithm_RSA,
                                                            PARC_HASH_SHA256,
                                                            bb_sig);
    parcBuffer_Release(&bb_sig);
    parcBufferComposer_Release(&composer);

    bool success = _parcInMemoryVerifier_VerifyDigest(data->inMemoryInterface, parcKey_GetKeyId(key), localHash, PARCCryptoSuite_RSA_SHA256, signatureToVerify);

    parcSignature_Release(&signatureToVerify);
    parcCryptoHash_Release(&localHash);
    parcKey_Release(&key);

    assertFalse(success, "Signature verified even with wrong hash");
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_InMemoryVerifier);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
