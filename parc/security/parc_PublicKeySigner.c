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

/**
 * @author Christopher A. Wood, Computing Science Laboratory, PARC
 * @copyright (c) 2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_Memory.h>

#include <parc/security/parc_PublicKeySigner.h>
#include <parc/security/parc_Pkcs12KeyStore.h>
#include <parc/security/parc_CryptoHash.h>
#include <parc/security/parc_Security.h>

#include <openssl/pkcs12.h>
#include <openssl/x509v3.h>
#include <openssl/err.h>

struct PARCPublicKeySigner {
    PARCKeyStore *keyStore;
    PARCSigningAlgorithm signingAlgorithm;
    PARCCryptoHashType hashType;
    PARCCryptoHasher *hasher;
};

static bool
_parcPublicKeySigner_Finalize(PARCPublicKeySigner **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCPublicKeySigner pointer.");

    PARCPublicKeySigner *instance = *instancePtr;

    if (instance->keyStore != NULL) {
        parcKeyStore_Release(&(instance->keyStore));
    }
    if (instance->hasher != NULL) {
        parcCryptoHasher_Release(&(instance->hasher));
    }

    return true;
}

void
parcPublicKeySigner_AssertValid(const PARCPublicKeySigner *instance)
{
    assertTrue(parcPublicKeySigner_IsValid(instance), "PARCPublicKeySigner is not valid.");
}

bool
parcPublicKeySigner_Equals(const PARCPublicKeySigner *x, const PARCPublicKeySigner *y)
{
    bool result = false;

    if (x == y) {
        result = true;
    } else if (x == NULL || y == NULL) {
        result = false;
    } else {
        if (x->signingAlgorithm == y->signingAlgorithm) {
            if (x->hashType == y->hashType) {
                return true;
            }
        }
    }

    return result;
}

PARCHashCode
parcPublicKeySigner_HashCode(const PARCPublicKeySigner *instance)
{
    PARCHashCode result = 0;

    return result;
}

bool
parcPublicKeySigner_IsValid(const PARCPublicKeySigner *instance)
{
    bool result = false;

    if (instance != NULL) {
        result = true;
    }

    return result;
}

char *
parcPublicKeySigner_ToString(const PARCPublicKeySigner *instance)
{
    char *result = parcMemory_Format("PARCPublicKeySigner@%p\n", instance);

    return result;
}

parcObject_ImplementAcquire(parcPublicKeySigner, PARCPublicKeySigner);
parcObject_ImplementRelease(parcPublicKeySigner, PARCPublicKeySigner);

parcObject_Override(PARCPublicKeySigner, PARCObject,
    .destructor = (PARCObjectDestructor *) _parcPublicKeySigner_Finalize,
    .toString = (PARCObjectToString *) parcPublicKeySigner_ToString,
    .equals = (PARCObjectEquals *) parcPublicKeySigner_Equals,
    .hashCode = (PARCObjectHashCode *) parcPublicKeySigner_HashCode);

PARCPublicKeySigner *
parcPublicKeySigner_Create(PARCKeyStore *keyStore, PARCSigningAlgorithm signingAlgorithm, PARCCryptoHashType hashType)
{
    PARCPublicKeySigner *result = parcObject_CreateInstance(PARCPublicKeySigner);

    if (result != NULL) {
        result->keyStore = parcKeyStore_Acquire(keyStore);
        result->signingAlgorithm = signingAlgorithm;
        result->hashType = hashType;
        result->hasher = parcCryptoHasher_Create(hashType);
    }

    return result;
}

static PARCSigningAlgorithm
_GetSigningAlgorithm(PARCPublicKeySigner *interfaceContext)
{
    return PARCSigningAlgorithm_RSA;
}

static PARCCryptoHashType
_GetCryptoHashType(PARCPublicKeySigner *signer)
{
    assertNotNull(signer, "Parameter must be non-null PARCCryptoHasher");
    return signer->hashType;
}

static PARCCryptoHasher *
_GetCryptoHasher(PARCPublicKeySigner *signer)
{
    assertNotNull(signer, "Parameter must be non-null PARCCryptoHasher");
    return signer->hasher;
}

static PARCKeyStore *
_GetKeyStore(PARCPublicKeySigner *signer)
{
    assertNotNull(signer, "Parameter must be non-null PARCCryptoHasher");
    return signer->keyStore;
}

static PARCSignature *
_SignDigest(PARCPublicKeySigner *signer, const PARCCryptoHash *digestToSign)
{
    parcSecurity_AssertIsInitialized();

    assertNotNull(signer, "Parameter must be non-null CCNxFileKeystore");
    assertNotNull(digestToSign, "Buffer to sign must not be null");

    // TODO: what is the best way to expose this?
    PARCKeyStore *keyStore = signer->keyStore;
    PARCBuffer *privateKeyBuffer = parcKeyStore_GetDEREncodedPrivateKey(keyStore);
    EVP_PKEY *privateKey = NULL;
    size_t keySize = parcBuffer_Remaining(privateKeyBuffer);
    uint8_t *bytes = parcBuffer_Overlay(privateKeyBuffer, keySize);
    privateKey = d2i_PrivateKey(EVP_PKEY_RSA, &privateKey, (const unsigned char **) &bytes, keySize);
    parcBuffer_Release(&privateKeyBuffer);

    RSA *rsa = EVP_PKEY_get1_RSA(privateKey);

    int opensslDigestType;

    switch (parcCryptoHash_GetDigestType(digestToSign)) {
        case PARCCryptoHashType_SHA256:
            opensslDigestType = NID_sha256;
            break;
        case PARCCryptoHashType_SHA512:
            opensslDigestType = NID_sha512;
            break;
        default:
            trapUnexpectedState("Unknown digest type: %s",
                                parcCryptoHashType_ToString(parcCryptoHash_GetDigestType(digestToSign)));
    }

    uint8_t *sig = parcMemory_Allocate(RSA_size(rsa));
    assertNotNull(sig, "parcMemory_Allocate(%u) returned NULL", RSA_size(rsa));

    unsigned sigLength = 0;
    PARCBuffer *bb_digest = parcCryptoHash_GetDigest(digestToSign);
    int result = RSA_sign(opensslDigestType,
                          (unsigned char *) parcByteArray_Array(parcBuffer_Array(bb_digest)),
                          (int) parcBuffer_Remaining(bb_digest),
                          sig,
                          &sigLength,
                          rsa);
    assertTrue(result == 1, "Got error from RSA_sign: %d", result);
    RSA_free(rsa);

    PARCBuffer *bbSign = parcBuffer_Allocate(sigLength);
    parcBuffer_Flip(parcBuffer_PutArray(bbSign, sigLength, sig));
    parcMemory_Deallocate((void **) &sig);

    PARCSignature *signature =
    parcSignature_Create(_GetSigningAlgorithm(signer),
                         parcCryptoHash_GetDigestType(digestToSign),
                         bbSign
                         );
    parcBuffer_Release(&bbSign);
    return signature;
}

PARCSigningInterface *PARCPublicKeySignerAsSigner = &(PARCSigningInterface) {
    .GetCryptoHasher = (PARCCryptoHasher *(*)(void *)) _GetCryptoHasher,
    .SignDigest = (PARCSignature *(*)(void *, const PARCCryptoHash *)) _SignDigest,
    .GetSigningAlgorithm = (PARCSigningAlgorithm (*)(void *)) _GetSigningAlgorithm,
    .GetCryptoHashType = (PARCCryptoHashType (*)(void *)) _GetCryptoHashType,
    .GetKeyStore = (PARCKeyStore *(*)(void *)) _GetKeyStore,
};
