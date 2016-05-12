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
#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_Memory.h>

#include <parc/security/parc_Signer.h>
#include <parc/security/parc_SymmetricKeySigner.h>
#include <parc/security/parc_KeyStore.h>

#include <openssl/asn1.h>
#include <openssl/asn1t.h>
#include <openssl/safestack.h>
#include <openssl/bn.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <openssl/aes.h>
#include <openssl/hmac.h>

#define AES_KEYSTORE_VERSION 1L
#define IV_SIZE 16
#define AES_MAX_DIGEST_SIZE 128
#define AES_DEFAULT_DIGEST_ALGORITHM "SHA256"

struct PARCSymmetricKeySigner {
    PARCSymmetricKeyStore *keyStore;
    PARCKeyStore *generalKeyStore;

    PARCCryptoHash *secretKeyHash;
    PARCCryptoHasher *hasher;
    PARCCryptoHasherInterface hasherFunctor;
    PARCCryptoHashType hashType;

    unsigned hashLength;
    const EVP_MD *opensslMd;
};

// ==================================================
// HMAC implementation

static void *
_hmacCreate(void *env)
{
    PARCSymmetricKeySigner *signer = (PARCSymmetricKeySigner *) env;

    // HMAC_Init_ex seems to overrun the size of HMAC_CTX, so make it bigger

    HMAC_CTX *ctx = parcMemory_Allocate(sizeof(HMAC_CTX) * 2);
    assertNotNull(ctx, "parcMemory_Allocate(%zu) returned NULL for HMAC_CTX", sizeof(HMAC_CTX) * 2);

    HMAC_CTX_init(ctx);

    // Now initialize it with our digest and key, so in hmac_init we can avoid using those
    PARCBuffer *secretKey = parcSymmetricKeyStore_GetKey(signer->keyStore);
    assertTrue(parcBuffer_Remaining(secretKey) < 512, "The keystore secret key cannot be longer than %d", 512);

    HMAC_Init_ex(ctx, parcByteArray_Array(parcBuffer_Array(secretKey)), (int) parcBuffer_Remaining(secretKey), signer->opensslMd, NULL);

    return ctx;
}

static int
_hmacInit(void *ctx)
{
    // reset the HMAC state with NULLs, so we'll re-use the values we had from setup.
    HMAC_Init_ex((HMAC_CTX *) ctx, NULL, 0, NULL, NULL);
    return 0;
}

static int
_hmacUpdate(void *ctx, const void *buffer, size_t length)
{
    HMAC_Update(ctx, buffer, length);
    return 0;
}

static PARCBuffer*
_hmacFinalize(void *ctx)
{
    uint8_t buffer[EVP_MAX_MD_SIZE];
    unsigned length;
    HMAC_Final(ctx, buffer, &length);

    PARCBuffer *output = parcBuffer_Allocate(length);
    parcBuffer_PutArray(output, length, buffer);

    return output;
}

static void
_hmacDestroy(void **ctxPtr)
{
    HMAC_CTX_cleanup(*ctxPtr);
    parcMemory_Deallocate((void **) ctxPtr);
    *ctxPtr = NULL;
}

static PARCCryptoHasherInterface functor_hmac = {
    .functor_env     = NULL,
    .hasher_setup    = _hmacCreate,
    .hasher_init     = _hmacInit,
    .hasher_update   = _hmacUpdate,
    .hasher_finalize = _hmacFinalize,
    .hasher_destroy  = _hmacDestroy
};

static bool
_parcSymmetricKeySigner_Finalize(PARCSymmetricKeySigner **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCSymmetricKeySigner pointer.");
    PARCSymmetricKeySigner *signer = (PARCSymmetricKeySigner *) *instancePtr;
    if (signer->secretKeyHash != NULL) {
        parcCryptoHash_Release(&signer->secretKeyHash);
    }
    if (signer->hasher != NULL) {
        parcCryptoHasher_Release(&signer->hasher);
    }

    if (signer->keyStore != NULL) {
        parcSymmetricKeyStore_Release(&signer->keyStore);
    }
    if (signer->generalKeyStore != NULL) {
        parcKeyStore_Release(&signer->generalKeyStore);
    }

    return true;
}

parcObject_ImplementAcquire(parcSymmetricKeySigner, PARCSymmetricKeySigner);
parcObject_ImplementRelease(parcSymmetricKeySigner, PARCSymmetricKeySigner);

parcObject_Override(PARCSymmetricKeySigner, PARCObject,
    .destructor = (PARCObjectDestructor *) _parcSymmetricKeySigner_Finalize);

void
parcSymmetricKeySigner_AssertValid(const PARCSymmetricKeySigner *instance)
{
    assertTrue(parcSymmetricKeySigner_IsValid(instance),
               "PARCSymmetricKeySigner is not valid.");
}

PARCSymmetricKeySigner *
parcSymmetricKeySigner_Create(PARCSymmetricKeyStore *keyStore, PARCCryptoHashType hmacHashType)
{
    PARCSymmetricKeySigner *result = parcObject_CreateInstance(PARCSymmetricKeySigner);

    if (result != NULL) {
        result->hashType = hmacHashType;
        switch (hmacHashType) {
            case PARC_HASH_SHA256:
                result->hashLength = SHA256_DIGEST_LENGTH;
                result->opensslMd = EVP_sha256();
                break;

            case PARC_HASH_SHA512:
                result->hashLength = SHA512_DIGEST_LENGTH;
                result->opensslMd = EVP_sha512();
                break;

            default:
                parcObject_Release((void **) &result);
                trapIllegalValue(hmacHashType, "Unknown HMAC hash type: %d", hmacHashType);
        }

        // the signer key digest is SHA256, independent of the HMAC digest
        result->secretKeyHash = parcSymmetricKeyStore_GetVerifierKeyDigest(keyStore);
        result->keyStore = parcSymmetricKeyStore_Acquire(keyStore);
        result->generalKeyStore = parcKeyStore_Create(result->keyStore, PARCSymmetricKeyStoreAsKeyStore);

        // create the functor from the template then specialize it to this keystore.
        // This depends on keystore->secret_key being set.  It will cause a callback
        // into hmac_setup()
        result->hasherFunctor = functor_hmac;
        result->hasherFunctor.functor_env = result;
        result->hasher = parcCryptoHasher_CustomHasher(hmacHashType, result->hasherFunctor);
    }

    return result;
}

bool
parcSymmetricKeySigner_IsValid(const PARCSymmetricKeySigner *instance)
{
    bool result = false;

    if (instance != NULL) {
        result = true;
    }

    return result;
}

static PARCSigningAlgorithm
_getSigningAlgorithm(PARCSymmetricKeySigner *signer)
{
    return PARCSigningAlgorithm_HMAC;
}

static PARCCryptoHashType
_getCryptoHashType(PARCSymmetricKeySigner *signer)
{
    return signer->hashType;
}

static PARCCryptoHasher *
_getCryptoHasher(PARCSymmetricKeySigner *signer)
{
    return signer->hasher;
}

static PARCKeyStore *
_getKeyStore(PARCSymmetricKeySigner *signer)
{
    return signer->generalKeyStore;
}

// ==================================================
// implementation

/**
 * wrap the HMAC in digestToSign in a PARCSignature
 *
 * @param hashToSign is the HMAC computed by the our PARCCryptoHasher.
 */
static PARCSignature  *
_signDigest(PARCSymmetricKeySigner *interfaceContext, const PARCCryptoHash *hashToSign)
{
    // The digest computed via our hash function (hmac) is the actual signature.
    // just need to wrap it up with the right parameters.
    PARCBuffer *signatureBits = parcBuffer_Copy(parcCryptoHash_GetDigest(hashToSign));
    PARCSignature  *result = parcSignature_Create(_getSigningAlgorithm(interfaceContext), parcCryptoHash_GetDigestType(hashToSign), signatureBits);
    parcBuffer_Release(&signatureBits);
    return result;
}

PARCSigningInterface *PARCSymmetricKeySignerAsSigner = &(PARCSigningInterface) {
    .GetCryptoHashType        = (PARCCryptoHashType (*)(void *)) _getCryptoHashType,
    .GetCryptoHasher          = (PARCCryptoHasher *(*)(void *)) _getCryptoHasher,
    .SignDigest               = (PARCSignature *(*)(void *, const PARCCryptoHash *)) _signDigest,
    .GetSigningAlgorithm      = (PARCSigningAlgorithm (*)(void *)) _getSigningAlgorithm,
    .GetKeyStore              = (PARCKeyStore *(*)(void *)) _getKeyStore,
};
