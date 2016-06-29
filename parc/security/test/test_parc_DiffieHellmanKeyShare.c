/*
 * Copyright (c) 2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
#include <parc/testing/parc_ObjectTesting.h>

#include "../parc_DiffieHellmanKeyShare.c"
#include <parc/algol/parc_SafeMemory.h>
#include <parc/security/parc_CryptoHashType.h>
#include <parc/algol/parc_Buffer.h>

LONGBOW_TEST_RUNNER(parc_DiffieHellmanKeyShare)
{
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

LONGBOW_TEST_RUNNER_SETUP(parc_DiffieHellmanKeyShare)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_DiffieHellmanKeyShare)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcDiffieHellmanKeyShare_AcquireRelease);
    LONGBOW_RUN_TEST_CASE(Global, parcDiffieHellmanKeyShare_Create);
    LONGBOW_RUN_TEST_CASE(Global, parcDiffieHellmanKeyShare_SerializePublicKey);
    LONGBOW_RUN_TEST_CASE(Global, parcDiffieHellmanKeyShare_SerializeDeserializePublicKey);
    LONGBOW_RUN_TEST_CASE(Global, parcDiffieHellmanKeyShare_DeserializePublicKey_ErrorWrongGroup);
    LONGBOW_RUN_TEST_CASE(Global, parcDiffieHellmanKeyShare_DeserializePublicKey_ErrorInvalidEncoding);
    LONGBOW_RUN_TEST_CASE(Global, parcDiffieHellmanKeyShare_Combine);
    LONGBOW_RUN_TEST_CASE(Global, parcDiffieHellmanKeyShare_Combine_Error_PublicKeyDeserializationFail);
    LONGBOW_RUN_TEST_CASE(Global, _parcDiffieHellmanKeyShare_HashSharedSecret);
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

LONGBOW_TEST_CASE(Global, parcDiffieHellmanKeyShare_AcquireRelease)
{
    PARCDiffieHellmanKeyShare *dh = parcDiffieHellmanKeyShare_Create(PARCDiffieHellmanGroup_Secp521r1);
    parcObjectTesting_AssertAcquireReleaseContract(parcDiffieHellmanKeyShare_Acquire, dh);
    parcDiffieHellmanKeyShare_Release(&dh);
}

LONGBOW_TEST_CASE(Global, parcDiffieHellmanKeyShare_Create)
{
    PARCDiffieHellmanKeyShare *dh = parcDiffieHellmanKeyShare_Create(PARCDiffieHellmanGroup_Secp521r1);
    assertNotNull(dh, "Expected a non-NULL PARCDiffieHellmanKeyShare instance");
    parcDiffieHellmanKeyShare_Release(&dh);
}

LONGBOW_TEST_CASE(Global, parcDiffieHellmanKeyShare_SerializePublicKey)
{
    PARCDiffieHellmanKeyShare *keyShare = parcDiffieHellmanKeyShare_Create(PARCDiffieHellmanGroup_Secp521r1);
    assertNotNull(keyShare, "Expected a non-NULL PARCDiffieHellmanKeyShare instance");

    PARCBuffer *publicKey = parcDiffieHellmanKeyShare_SerializePublicKey(keyShare);
    assertNotNull(publicKey, "Expected the public key to be serialized to a non-NULL PARCBuffer");

    const size_t sec521r1KeySize = 266;
    assertTrue(parcBuffer_Remaining(publicKey) == sec521r1KeySize, "Expected the public key size to be %zu, got %zu", sec521r1KeySize, parcBuffer_Remaining(publicKey));

    parcBuffer_Release(&publicKey);
    parcDiffieHellmanKeyShare_Release(&keyShare);
}

LONGBOW_TEST_CASE(Global, parcDiffieHellmanKeyShare_SerializeDeserializePublicKey)
{
    PARCDiffieHellmanKeyShare *keyShare = parcDiffieHellmanKeyShare_Create(PARCDiffieHellmanGroup_Secp521r1);
    assertNotNull(keyShare, "Expected a non-NULL PARCDiffieHellmanKeyShare instance");

    PARCBuffer *publicKey = parcDiffieHellmanKeyShare_SerializePublicKey(keyShare);
    assertNotNull(publicKey, "Expected the public key to be serialized to a non-NULL PARCBuffer");

    const size_t sec521r1KeySize = 266;
    assertTrue(parcBuffer_Remaining(publicKey) == sec521r1KeySize, "Expected the public key size to be %zu, got %zu", sec521r1KeySize, parcBuffer_Remaining(publicKey));

    // Deserialize the public key to get the OpenSSL EVP_PKEY type
    EVP_PKEY *rawPublicKey = _parcDiffieHellman_DeserializePublicKeyShare(keyShare, publicKey);
    assertNotNull(rawPublicKey, "Expected the raw public key to be deserialized");

    // Extract the public portions of the private key share and public key share
    EC_KEY *publicEcKey = EVP_PKEY_get1_EC_KEY(rawPublicKey);
    const EC_POINT *publicPoint = EC_KEY_get0_public_key(publicEcKey);

    EC_KEY *privateEcKey = EVP_PKEY_get1_EC_KEY(keyShare->privateKey);
    const EC_POINT *privatePoint = EC_KEY_get0_public_key(privateEcKey);

    // Compare the public portions of the key shares
    const EC_GROUP *group = EC_KEY_get0_group(publicEcKey);
    BN_CTX *bigNumberContext = BN_CTX_new();
    int equalResult = EC_POINT_cmp(group, publicPoint, privatePoint, bigNumberContext);
    assertTrue(equalResult == 0, "Expected the two public points to be equal.");

    BN_CTX_free(bigNumberContext);
    EVP_PKEY_free(rawPublicKey);
    parcBuffer_Release(&publicKey);
    parcDiffieHellmanKeyShare_Release(&keyShare);
}

LONGBOW_TEST_CASE(Global, parcDiffieHellmanKeyShare_DeserializePublicKey_ErrorWrongGroup)
{
    PARCDiffieHellmanKeyShare *keyShare = parcDiffieHellmanKeyShare_Create(PARCDiffieHellmanGroup_Secp521r1);
    assertNotNull(keyShare, "Expected a non-NULL PARCDiffieHellmanKeyShare instance");

    PARCBuffer *publicKey = parcDiffieHellmanKeyShare_SerializePublicKey(keyShare);
    assertNotNull(publicKey, "Expected the public key to be serialized to a non-NULL PARCBuffer");

    PARCDiffieHellmanKeyShare *alternateKeyShare = parcDiffieHellmanKeyShare_Create(PARCDiffieHellmanGroup_Prime256v1);

    // Deserialize the public key with a different group and hit the failure
    EVP_PKEY *rawPublicKey = _parcDiffieHellman_DeserializePublicKeyShare(alternateKeyShare, publicKey);
    assertNull(rawPublicKey, "Expected the raw public key to not be deserialized");

    parcBuffer_Release(&publicKey);
    parcDiffieHellmanKeyShare_Release(&keyShare);
    parcDiffieHellmanKeyShare_Release(&alternateKeyShare);
}

LONGBOW_TEST_CASE(Global, parcDiffieHellmanKeyShare_DeserializePublicKey_ErrorInvalidEncoding)
{
    PARCDiffieHellmanKeyShare *keyShare = parcDiffieHellmanKeyShare_Create(PARCDiffieHellmanGroup_Secp521r1);
    assertNotNull(keyShare, "Expected a non-NULL PARCDiffieHellmanKeyShare instance");

    // Deserialize the public key with a different group
    PARCBuffer *publicKey = parcBuffer_Allocate(32);
    EVP_PKEY *rawPublicKey = _parcDiffieHellman_DeserializePublicKeyShare(keyShare, publicKey);
    assertNull(rawPublicKey, "Expected the raw public key to not be deserialized");

    parcBuffer_Release(&publicKey);
    parcDiffieHellmanKeyShare_Release(&keyShare);
}

LONGBOW_TEST_CASE(Global, parcDiffieHellmanKeyShare_Combine)
{
    PARCDiffieHellmanKeyShare *keyShare = parcDiffieHellmanKeyShare_Create(PARCDiffieHellmanGroup_Secp521r1);
    assertNotNull(keyShare, "Expected a non-NULL PARCDiffieHellmanKeyShare instance");

    PARCBuffer *publicKey = parcDiffieHellmanKeyShare_SerializePublicKey(keyShare);
    assertNotNull(publicKey, "Expected the public key to be serialized to a non-NULL PARCBuffer");

    PARCBuffer *sharedSecret = parcDiffieHellmanKeyShare_Combine(keyShare, publicKey);
    assertNotNull(sharedSecret, "Expected the shared secret to be non-NULL");

    const size_t secretSize = 32; // = 256 / 8 bytes
    assertTrue(parcBuffer_Remaining(sharedSecret) == secretSize, "invalid size");

    parcBuffer_Release(&sharedSecret);
    parcBuffer_Release(&publicKey);
    parcDiffieHellmanKeyShare_Release(&keyShare);
}

LONGBOW_TEST_CASE(Global, parcDiffieHellmanKeyShare_Combine_Error_PublicKeyDeserializationFail)
{
    PARCDiffieHellmanKeyShare *keyShare = parcDiffieHellmanKeyShare_Create(PARCDiffieHellmanGroup_Secp521r1);
    assertNotNull(keyShare, "Expected a non-NULL PARCDiffieHellmanKeyShare instance");

    PARCBuffer *publicKey = parcBuffer_Allocate(32);
    PARCBuffer *sharedSecret = parcDiffieHellmanKeyShare_Combine(keyShare, publicKey);
    assertNull(sharedSecret, "Expected the shared secret to be non-NULL");

    parcBuffer_Release(&publicKey);
    parcDiffieHellmanKeyShare_Release(&keyShare);
}

LONGBOW_TEST_CASE(Global, _parcDiffieHellmanKeyShare_HashSharedSecret)
{
    PARCBuffer *input = parcBuffer_Allocate(1024);
    PARCBuffer *digestValue = _parcDiffieHellmanKeyShare_HashSharedSecret(input);
    size_t digestLength = parcBuffer_Remaining(digestValue);
    size_t expectedLength = 32; // 256 bits for PARCCryptoHashType_SHA256
    assertTrue(digestLength == 32, "Expected a %zu byte digest, got %zu", expectedLength, digestLength);

    PARCCryptoHasher *hasher = parcCryptoHasher_Create(PARCCryptoHashType_SHA256);
    parcCryptoHasher_Init(hasher);
    parcCryptoHasher_UpdateBuffer(hasher, input);
    PARCCryptoHash *digest = parcCryptoHasher_Finalize(hasher);

    PARCBuffer *computedDigest = parcBuffer_Acquire(parcCryptoHash_GetDigest(digest));

    parcCryptoHash_Release(&digest);
    parcCryptoHasher_Release(&hasher);

    assertTrue(parcBuffer_Equals(digestValue, computedDigest), "Expected the secret input to be hashed correctly.");

    parcBuffer_Release(&input);
    parcBuffer_Release(&digestValue);
    parcBuffer_Release(&computedDigest);
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_DiffieHellmanKeyShare);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
