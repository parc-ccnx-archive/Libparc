/*
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * Copyright 2016 Palo Alto Research Center, Inc. (PARC), a Xerox company.  All Rights Reserved.
 * The content of this file, whole or in part, is subject to licensing terms.
 * If distributing this software, include this License Header Notice in each
 * file and provide the accompanying LICENSE file.
 */
/**
 * @author Christopher A. Wood, Computing Science Laboratory, PARC
 * @copyright 2016 Palo Alto Research Center, Inc. (PARC), A Xerox Company.  All Rights Reserved.
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
        case PARC_HASH_SHA256:
            opensslDigestType = NID_sha256;
            break;
        case PARC_HASH_SHA512:
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
    .Release = (void (*)(void **)) parcPublicKeySigner_Release
};
