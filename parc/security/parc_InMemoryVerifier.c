/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @header parc_InMemoryVerifier.c
 * <#Abstract#>
 *
 *     <#Discussion#>
 *
 * @author Mosko, Marc
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
#include <config.h>
#include <stdio.h>

#include <LongBow/runtime.h>
#include <parc/security/parc_InMemoryVerifier.h>
#include <parc/security/parc_CryptoHasher.h>
#include <parc/security/parc_CryptoCache.h>
#include <parc/algol/parc_Memory.h>

#include <openssl/x509v3.h>

struct parc_inmemory_verifier {
    PARCCryptoHasher *hasher_sha256;
    PARCCryptoHasher *hasher_sha512;
    PARCCryptoCache *key_cache;
};

static bool
_parcInMemoryVerifier_Destructor(PARCInMemoryVerifier **verifierPtr)
{
    PARCInMemoryVerifier *verifier = *verifierPtr;

    parcCryptoHasher_Release(&(verifier->hasher_sha256));
    parcCryptoHasher_Release(&(verifier->hasher_sha512));
    parcCryptoCache_Destroy(&(verifier->key_cache));

    return true;
}

parcObject_ImplementAcquire(parcInMemoryVerifier, PARCInMemoryVerifier);
parcObject_ImplementRelease(parcInMemoryVerifier, PARCInMemoryVerifier);

parcObject_Override(PARCInMemoryVerifier, PARCObject,
    .destructor = (PARCObjectDestructor *) _parcInMemoryVerifier_Destructor);

PARCInMemoryVerifier *
parcInMemoryVerifier_Create()
{
    PARCInMemoryVerifier *verifier = parcObject_CreateInstance(PARCInMemoryVerifier);
    if (verifier != NULL) {
        // right now only support sha-256.  need to figure out how to make this flexible
        verifier->hasher_sha256 = parcCryptoHasher_Create(PARCCryptoHashType_SHA256);
        verifier->hasher_sha512 = parcCryptoHasher_Create(PARCCryptoHashType_SHA512);
        verifier->key_cache = parcCryptoCache_Create();
    }

    return verifier;
}


// ======================================

static PARCCryptoHasher *
_parcInMemoryVerifier_GetCryptoHasher(void *interfaceContext, PARCKeyId *keyid, PARCCryptoHashType hashType)
{
    PARCInMemoryVerifier *verifier = (PARCInMemoryVerifier *) interfaceContext;

    const PARCKey *key = parcCryptoCache_GetKey(verifier->key_cache, keyid);
    if (key == NULL) {
        return false;
    }

    assertFalse(parcKey_GetSigningAlgorithm(key) == PARCSigningAlgorithm_HMAC, "HMAC not supported yet");

    switch (hashType) {
        case PARCCryptoHashType_SHA256:
            return verifier->hasher_sha256;

        case PARCCryptoHashType_SHA512:
            return verifier->hasher_sha512;

        default:
            trapUnexpectedState("unsupported hash type: %d", hashType);
    }
}

static bool
_parcInMemoryVerifier_AllowedCryptoSuite(void *interfaceContext, PARCKeyId *keyid, PARCCryptoSuite suite)
{
    PARCInMemoryVerifier *verifier = (PARCInMemoryVerifier *) interfaceContext;

    const PARCKey *key = parcCryptoCache_GetKey(verifier->key_cache, keyid);
    if (key == NULL) {
        return false;
    }

    switch (parcKey_GetSigningAlgorithm(key)) {
        case PARCSigningAlgorithm_RSA:
            switch (suite) {
                case PARCCryptoSuite_RSA_SHA256:
                    return true;

                case PARCCryptoSuite_RSA_SHA512:
                    return true;

                default:
                    return false;
            }
            break;

        case PARCSigningAlgorithm_DSA:
            switch (suite) {
                default:
                    return false;
            }
            break;

        case PARCSigningAlgorithm_HMAC:
            switch (suite) {
                case PARCCryptoSuite_HMAC_SHA256:
                    return true;
                default:
                    return false;
            }
            break;

        default:
            trapUnexpectedState("Unknown signing algorithm: %s",
                                parcSigningAlgorithm_ToString(parcKey_GetSigningAlgorithm(key)));
            return false;
    }

    return false;
}

static bool _parcInMemoryVerifier_RSAKey_Verify(PARCInMemoryVerifier *verifier, PARCCryptoHash *localHash,
                                                PARCSignature *signatureToVerify, PARCBuffer *derEncodedKey);

/**
 * The signature verifies if:
 * 0) we know the key for keyid
 * 1) the signing algorithm of the key corresponding to keyid is same as CCNxSignature
 * 2) The hash of the locallyComputedHash is the same type as the content object's ciphersuite
 * 3) the signature verifies
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
static bool
_parcInMemoryVerifier_VerifyDigest(void *interfaceContext, PARCKeyId *keyid, PARCCryptoHash *locallyComputedHash,
                                   PARCCryptoSuite suite, PARCSignature *objectSignature)
{
    PARCInMemoryVerifier *verifier = (PARCInMemoryVerifier *) interfaceContext;

    const PARCKey *key = parcCryptoCache_GetKey(verifier->key_cache, keyid);
    if (key == NULL) {
        return false;
    }

    assertTrue(_parcInMemoryVerifier_AllowedCryptoSuite(interfaceContext, keyid, suite), "Invalid crypto suite for keyid");

    if (parcKey_GetSigningAlgorithm(key) != parcSignature_GetSigningAlgorithm(objectSignature)) {
        fprintf(stdout, "Signatured failed, signing algorithms do not match: key %s sig %s\n",
                parcSigningAlgorithm_ToString(parcKey_GetSigningAlgorithm(key)),
                parcSigningAlgorithm_ToString(parcSignature_GetSigningAlgorithm(objectSignature)));
        return false;
    }

    if (parcCryptoHash_GetDigestType(locallyComputedHash) != parcCryptoSuite_GetCryptoHash(suite)) {
        fprintf(stdout, "Signatured failed, digest algorithms do not match: digest %s suite %s\n",
                parcCryptoHashType_ToString(parcCryptoHash_GetDigestType(locallyComputedHash)),
                parcCryptoHashType_ToString(parcCryptoSuite_GetCryptoHash(suite)));
        return false;
    }

    switch (parcSignature_GetSigningAlgorithm(objectSignature)) {
        case PARCSigningAlgorithm_RSA:
            return _parcInMemoryVerifier_RSAKey_Verify(verifier, locallyComputedHash, objectSignature, parcKey_GetKey(key));

        case PARCSigningAlgorithm_DSA:
            trapNotImplemented("DSA not supported");
            break;

        case PARCSigningAlgorithm_HMAC:
            trapNotImplemented("HMAC not supported");
            break;

        default:
            trapUnexpectedState("Unknown signing algorithm: %d", parcSignature_GetSigningAlgorithm(objectSignature));
    }


    return false;
}

static void
_parcInMemoryVerifier_AddKey(void *interfaceContext, PARCKey *key)
{
    assertNotNull(interfaceContext, "interfaceContext must be non-null");
    assertNotNull(key, "key must be non-null");

    PARCInMemoryVerifier *verifier = (PARCInMemoryVerifier *) interfaceContext;
    bool success = parcCryptoCache_AddKey(verifier->key_cache, key);
    assertTrue(success, "could not add key, it must be a duplicate");
}

static void
_parcInMemoryVerifier_RemoveKeyId(void *interfaceContext, PARCKeyId *keyid)
{
    assertNotNull(interfaceContext, "interfaceContent must be non-null");
    assertNotNull(keyid, "key must be non-null");

    PARCInMemoryVerifier *verifier = (PARCInMemoryVerifier *) interfaceContext;
    parcCryptoCache_RemoveKey(verifier->key_cache, keyid);
}

// ==============================================================
// Openssl specific parts

#ifdef __APPLE__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

/**
 * Return if the signature and key verify with the local hash.
 *
 * PRECONDITION:
 *  - You know the signature and key are RSA.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
static bool
_parcInMemoryVerifier_RSAKey_Verify(PARCInMemoryVerifier *verifier, PARCCryptoHash *localHash,
                                    PARCSignature *signatureToVerify, PARCBuffer *derEncodedKey)
{
    const uint8_t *der_bytes = parcByteArray_Array(parcBuffer_Array(derEncodedKey));

    long der_length = parcBuffer_Remaining(derEncodedKey);
    EVP_PKEY *unwrapped_key = d2i_PUBKEY(NULL, &der_bytes, der_length);

    if (unwrapped_key != NULL) {
        int success = 0;
        RSA *rsa = EVP_PKEY_get1_RSA(unwrapped_key);

        if (rsa != NULL) {
            int openssl_digest_type;

            switch (parcCryptoHash_GetDigestType(localHash)) {
                case PARCCryptoHashType_SHA256:
                    openssl_digest_type = NID_sha256;
                    break;
                case PARCCryptoHashType_SHA512:
                    openssl_digest_type = NID_sha512;
                    break;
                default:
                    trapUnexpectedState("Unknown digest type: %s",
                                        parcCryptoHashType_ToString(parcCryptoHash_GetDigestType(localHash)));
            }

            PARCBuffer *sigbits = parcSignature_GetSignature(signatureToVerify);
            PARCByteArray *bytearray = parcBuffer_Array(sigbits);
            unsigned signatureLength = (unsigned) parcBuffer_Remaining(sigbits);
            uint8_t *sigbuffer = parcByteArray_Array(bytearray);
            size_t signatureOffset = parcBuffer_ArrayOffset(sigbits);

            success = RSA_verify(openssl_digest_type,
                                 (unsigned char *) parcByteArray_Array(parcBuffer_Array(parcCryptoHash_GetDigest(localHash))),
                                 (unsigned) parcBuffer_Remaining(parcCryptoHash_GetDigest(localHash)),
                                 sigbuffer + signatureOffset,
                                 signatureLength,
                                 rsa);
            RSA_free(rsa);
        }
        EVP_PKEY_free(unwrapped_key);

        if (success) {
            return true;
        }
    }
    return false;
}

PARCVerifierInterface *PARCInMemoryVerifierAsVerifier = &(PARCVerifierInterface) {
    .GetCryptoHasher    = _parcInMemoryVerifier_GetCryptoHasher,
    .VerifyDigest       = _parcInMemoryVerifier_VerifyDigest,
    .AddKey             = _parcInMemoryVerifier_AddKey,
    .RemoveKeyId        = _parcInMemoryVerifier_RemoveKeyId,
    .AllowedCryptoSuite = _parcInMemoryVerifier_AllowedCryptoSuite,
};

#ifdef __APPLE__
#pragma clang diagnostic pop
#endif
