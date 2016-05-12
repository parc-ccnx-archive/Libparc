/*
 * Copyright (c) 2013-2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @author Marc Mosko, Christopher A. Wood
 * @copyright (c) 2013-2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>
#include <stdio.h>

#include <LongBow/runtime.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_Object.h>

#include <parc/security/parc_Signer.h>
#include <parc/security/parc_KeyStore.h>

struct parc_signer {
    PARCObject *instance;
    PARCSigningInterface *interface;
};

static bool
_parcSigner_FinalRelease(PARCSigner **signerPtr)
{
    PARCSigner *signer = *signerPtr;
    if (signer->instance != NULL) {
        parcObject_Release(&(signer->instance));
    }
    return true;
}

void
parcSigner_AssertValid(const PARCSigner *signer)
{
    assertNotNull(signer, "Parameter must be non-null PARCSigner");
}

parcObject_ImplementAcquire(parcSigner, PARCSigner);
parcObject_ImplementRelease(parcSigner, PARCSigner);

parcObject_Override(PARCSigner, PARCObject,
    .destructor = (PARCObjectDestructor *) _parcSigner_FinalRelease);

PARCSigner *
parcSigner_Create(PARCObject *instance, PARCSigningInterface *interfaceContext)
{
    assertNotNull(interfaceContext, "Parameter must be non-null implementation pointer");

    PARCSigner *signer = parcObject_CreateInstance(PARCSigner);
    if (signer != NULL) {
        signer->instance = parcObject_Acquire(instance);
        signer->interface = interfaceContext;
    }
    return signer;
}

PARCKey *
parcSigner_CreatePublicKey(PARCSigner *signer)
{
    PARCKeyStore *keyStore = parcSigner_GetKeyStore(signer);

    PARCCryptoHash *hash = parcKeyStore_GetVerifierKeyDigest(keyStore);

    PARCKeyId *keyid = parcKeyId_Create(parcCryptoHash_GetDigest(hash));
    parcCryptoHash_Release(&hash);

    PARCBuffer *derEncodedKey = parcKeyStore_GetDEREncodedPublicKey(keyStore);

    PARCKey *key = parcKey_CreateFromDerEncodedPublicKey(keyid,
                                                         parcSigner_GetSigningAlgorithm(signer),
                                                         derEncodedKey);

    parcBuffer_Release(&derEncodedKey);
    parcKeyId_Release(&keyid);

    return key;
}

PARCKeyId *
parcSigner_CreateKeyId(const PARCSigner *signer)
{
    PARCCryptoHash *hash = parcKeyStore_GetVerifierKeyDigest(parcSigner_GetKeyStore(signer));
    PARCBuffer *keyidBytes = parcCryptoHash_GetDigest(hash);
    PARCKeyId *result = parcKeyId_Create(keyidBytes);

    parcCryptoHash_Release(&hash);
    return result;
}

PARCCryptoHasher *
parcSigner_GetCryptoHasher(const PARCSigner *signer)
{
    parcSigner_OptionalAssertValid(signer);

    return signer->interface->GetCryptoHasher(signer->instance);
}

PARCSignature *
parcSigner_SignDigest(const PARCSigner *signer, const PARCCryptoHash *parcDigest)
{
    parcSigner_OptionalAssertValid(signer);

    assertNotNull(parcDigest, "parcDigest to sign must not be null");
    return signer->interface->SignDigest(signer->instance, parcDigest);
}

PARCSignature *
parcSigner_SignBuffer(const PARCSigner *signer, const PARCBuffer *buffer)
{
    parcSigner_OptionalAssertValid(signer);
    assertNotNull(buffer, "buffer to sign must not be null");

    PARCCryptoHashType hashType = parcSigner_GetCryptoHashType(signer);
    PARCCryptoHasher *hasher = parcCryptoHasher_Create(hashType);
    parcCryptoHasher_Init(hasher);
    parcCryptoHasher_UpdateBuffer(hasher, buffer);
    PARCCryptoHash *hash = parcCryptoHasher_Finalize(hasher);
    parcCryptoHasher_Release(&hasher);

    PARCSignature *signature = parcSigner_SignDigest(signer, hash);
    parcCryptoHash_Release(&hash);

    return signature;
}

PARCSigningAlgorithm
parcSigner_GetSigningAlgorithm(PARCSigner *signer)
{
    parcSigner_OptionalAssertValid(signer);

    return signer->interface->GetSigningAlgorithm(signer->instance);
}

PARCCryptoHashType
parcSigner_GetCryptoHashType(const PARCSigner *signer)
{
    parcSigner_OptionalAssertValid(signer);

    return signer->interface->GetCryptoHashType(signer->instance);
}

PARCKeyStore *
parcSigner_GetKeyStore(const PARCSigner *signer)
{
    parcSigner_OptionalAssertValid(signer);

    return signer->interface->GetKeyStore(signer->instance);
}
