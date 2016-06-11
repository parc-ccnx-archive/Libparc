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
 * @author Christopher A. Wood
 * @copyright (c) 2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_Memory.h>

#include <parc/security/parc_DiffieHellmanKeyShare.h>
#include <parc/security/parc_CryptoHasher.h>
#include <parc/security/parc_CryptoHash.h>

#include <openssl/pem.h>
#include <openssl/rand.h>

struct parc_diffie_hellman_keyshare {
    PARCDiffieHellmanGroup groupType;
    EVP_PKEY *privateKey;
};

static bool
_parcDiffieHellmanKeyShare_Destructor(PARCDiffieHellmanKeyShare **pointer)
{
    PARCDiffieHellmanKeyShare *share = *pointer;

    if (share->privateKey != NULL) {
        EVP_PKEY_free(share->privateKey);
    }

    return true;
}

parcObject_Override(PARCDiffieHellmanKeyShare, PARCObject,
    .destructor = (PARCObjectDestructor *) _parcDiffieHellmanKeyShare_Destructor);

parcObject_ImplementAcquire(parcDiffieHellmanKeyShare, PARCDiffieHellmanKeyShare);
parcObject_ImplementRelease(parcDiffieHellmanKeyShare, PARCDiffieHellmanKeyShare);

static EVP_PKEY *
_parcDiffieHellmanKeyShare_CreateShare(int curveid)
{
    EVP_PKEY_CTX *pctx;
    EVP_PKEY_CTX *kctx;

    pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);
    if (pctx == NULL) {
        return NULL;
    }

    int result = EVP_PKEY_paramgen_init(pctx);
    if (result != 1) {
        EVP_PKEY_CTX_free(pctx);
        return NULL;
    }

    result = EVP_PKEY_CTX_set_ec_paramgen_curve_nid(pctx, curveid);
    if (result != 1) {
        EVP_PKEY_CTX_free(pctx);
        return NULL;
    }

    EVP_PKEY *params = NULL;
    result = EVP_PKEY_paramgen(pctx, &params);
    if (result != 1) {
        EVP_PKEY_CTX_free(pctx);
        return NULL;
    }

	kctx = EVP_PKEY_CTX_new(params, NULL);
    if (kctx == NULL) {
        EVP_PKEY_CTX_free(pctx);
        EVP_PKEY_free(params);
        return NULL;
    }

    result = EVP_PKEY_keygen_init(kctx);
    if (result != 1) {
        EVP_PKEY_CTX_free(pctx);
        EVP_PKEY_free(params);
        return NULL;
    }

    EVP_PKEY *pkey = NULL;
    result = EVP_PKEY_keygen(kctx, &pkey);
    if (result != 1) {
        EVP_PKEY_CTX_free(pctx);
        EVP_PKEY_free(params);
        EVP_PKEY_CTX_free(kctx);
        return NULL;
    }

    EVP_PKEY_CTX_free(pctx);
    EVP_PKEY_CTX_free(kctx);
    EVP_PKEY_free(params);

    return pkey;
}

PARCDiffieHellmanKeyShare *
parcDiffieHellmanKeyShare_Create(PARCDiffieHellmanGroup groupType)
{
    PARCDiffieHellmanKeyShare *keyShare = parcObject_CreateInstance(PARCDiffieHellmanKeyShare);

    if (keyShare != NULL) {
        keyShare->groupType = groupType;

        switch (groupType) {
            case PARCDiffieHellmanGroup_Prime256v1:
                keyShare->privateKey = _parcDiffieHellmanKeyShare_CreateShare(NID_X9_62_prime256v1);
                break;
            case PARCDiffieHellmanGroup_Secp521r1:
                keyShare->privateKey = _parcDiffieHellmanKeyShare_CreateShare(NID_secp521r1);
                break;
            case PARCDiffieHellmanGroup_Curve2559:
            default:
                break;
        }

        if (keyShare->privateKey == NULL) {
            assertTrue(false, "Unable to instantiate a private key.");
            parcDiffieHellmanKeyShare_Release(&keyShare);
        }
    }

    return keyShare;
}

PARCBuffer *
parcDiffieHellmanKeyShare_SerializePublicKey(PARCDiffieHellmanKeyShare *keyShare)
{
    EC_KEY *ecKey = EVP_PKEY_get1_EC_KEY(keyShare->privateKey);

    BN_CTX *bnctx = BN_CTX_new();
    point_conversion_form_t form = EC_KEY_get_conv_form(ecKey);
    const EC_POINT *point = EC_KEY_get0_public_key(ecKey);
    const EC_GROUP *group = EC_KEY_get0_group(ecKey);
    char *keyBuffer = EC_POINT_point2hex(group, point, form, bnctx);
    int length = strlen(keyBuffer);

    PARCBuffer *publicKey = parcBuffer_Allocate(length);
    parcBuffer_PutArray(publicKey, length, (uint8_t *) keyBuffer);
    parcBuffer_Flip(publicKey);

    free(keyBuffer);
    BN_CTX_free(bnctx);

    return publicKey;
}

static EVP_PKEY *
_parcDiffieHellman_DeserializePublicKeyShare(PARCDiffieHellmanKeyShare *keyShare, PARCBuffer *keyBuffer)
{
    EC_KEY *ecKey = EVP_PKEY_get1_EC_KEY(keyShare->privateKey);
    const EC_GROUP *myGroup = EC_KEY_get0_group(ecKey);

    EC_KEY *newKey = EC_KEY_new();
    BN_CTX *newCtx = BN_CTX_new();
    int result = EC_KEY_set_group(newKey, myGroup);
    if (result != 1) {
        BN_CTX_free(newCtx);
        EC_KEY_free(newKey);
        return NULL;
    }

    EC_POINT *newPoint = EC_POINT_new(myGroup);
    char *keyString = parcBuffer_ToString(keyBuffer);
    newPoint = EC_POINT_hex2point(myGroup, keyString, newPoint, newCtx);
    if (newPoint == NULL) {
        parcMemory_Deallocate(&keyString);
        BN_CTX_free(newCtx);
        EC_KEY_free(newKey);
        EC_POINT_free(newPoint);
        return NULL;
    }

    result = EC_KEY_set_public_key(newKey, newPoint);
    if (result != 1) {
        parcMemory_Deallocate(&keyString);
        BN_CTX_free(newCtx);
        EC_KEY_free(newKey);
        EC_POINT_free(newPoint);
        return NULL;
    }

    EVP_PKEY *peerkey = EVP_PKEY_new();
    result = EVP_PKEY_set1_EC_KEY(peerkey, newKey);
    if (result != 1) {
        parcMemory_Deallocate(&keyString);
        BN_CTX_free(newCtx);
        EC_KEY_free(newKey);
        EC_POINT_free(newPoint);
        EVP_PKEY_free(peerkey);
        return NULL;
    }

    BN_CTX_free(newCtx);
    EC_KEY_free(newKey);
    parcMemory_Deallocate((void **) &keyString);
    EC_POINT_free(newPoint);

    return peerkey;
}

static PARCBuffer *
_parcDiffieHellmanKeyShare_HashSharedSecret(PARCBuffer *secret)
{
    PARCCryptoHasher *hasher = parcCryptoHasher_Create(PARC_HASH_SHA256);
    parcCryptoHasher_Init(hasher);
    parcCryptoHasher_UpdateBuffer(hasher, secret);
    PARCCryptoHash *digest = parcCryptoHasher_Finalize(hasher);

    PARCBuffer *sharedSecret = parcBuffer_Acquire(parcCryptoHash_GetDigest(digest));

    parcCryptoHash_Release(&digest);
    parcCryptoHasher_Release(&hasher);

    return sharedSecret;
}

PARCBuffer *
parcDiffieHellmanKeyShare_Combine(PARCDiffieHellmanKeyShare *keyShare, PARCBuffer *theirs)
{
    EVP_PKEY *peerkey = _parcDiffieHellman_DeserializePublicKeyShare(keyShare, theirs);
    if (peerkey == NULL) {
        return NULL;
    }

    EVP_PKEY *privateKey = keyShare->privateKey;
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(privateKey, NULL);
    if (ctx == NULL) {
        EVP_PKEY_free(peerkey);
        return NULL;
    }

    int result = EVP_PKEY_derive_init(ctx);
	if (result != 1) {
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(peerkey);
        return NULL;
    }

    result = EVP_PKEY_derive_set_peer(ctx, peerkey) ;
    if (result != 1) {
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(peerkey);
        return NULL;
    }

    size_t secretLength = 0;
    result = EVP_PKEY_derive(ctx, NULL, &secretLength);
    if (result != 1) {
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(peerkey);
        return NULL;
    }

    unsigned char *secret = OPENSSL_malloc(secretLength);
	if (secret == NULL) {
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(peerkey);
        return NULL;
    }

    result = EVP_PKEY_derive(ctx, secret, &secretLength);
	if (result != 1) {
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(peerkey);
        OPENSSL_free(secret);
        return NULL;
    }

    PARCBuffer *secretBuffer = parcBuffer_Allocate(secretLength);
    parcBuffer_PutArray(secretBuffer, secretLength, secret);
    parcBuffer_Flip(secretBuffer);

    PARCBuffer *sharedSecret = _parcDiffieHellmanKeyShare_HashSharedSecret(secretBuffer);
    parcBuffer_Release(&secretBuffer);

    EVP_PKEY_CTX_free(ctx);
	EVP_PKEY_free(peerkey);
    OPENSSL_free(secret);

	return sharedSecret;
}
