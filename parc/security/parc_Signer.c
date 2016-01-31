/*
 * Copyright (c) 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
/**
 * @header <#Headline Name#>
 * <#Abstract#>
 *
 *     <#Discussion#>
 *
 * @author Marc Mosko
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
#include <config.h>
#include <stdio.h>

#include <LongBow/runtime.h>

#include <parc/security/parc_Signer.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_Object.h>

struct parc_signer {
    PARCSigningInterface *interface;
};

static void
_parcSigner_FinalRelease(PARCSigner **signerPtr)
{
    PARCSigner *signer = *signerPtr;

    signer->interface->Destroy(&signer->interface);
}


parcObject_ExtendPARCObject(PARCSigner, _parcSigner_FinalRelease, NULL, NULL, NULL, NULL, NULL, NULL);

void
parcSigner_AssertValid(const PARCSigner *signer)
{
    assertNotNull(signer, "Parameter must be non-null PARCSigner");
}

PARCSigner *
parcSigner_Create(PARCSigningInterface *interfaceContext)
{
    assertNotNull(interfaceContext, "Parameter must be non-null implementation pointer");

    PARCSigner *signer = parcObject_CreateInstance(PARCSigner);
    if (signer != NULL) {
        signer->interface = interfaceContext;
    }
    return signer;
}

parcObject_ImplementAcquire(parcSigner, PARCSigner);

parcObject_ImplementRelease(parcSigner, PARCSigner);

PARCCryptoHash *
parcSigner_GetVerifierKeyDigest(const PARCSigner *signer)
{
    parcSigner_OptionalAssertValid(signer);

    return signer->interface->GetVerifierKeyDigest(signer->interface->interfaceContext);
}

PARCCryptoHash *
parcSigner_GetCertificateDigest(PARCSigner *signer)
{
    parcSigner_OptionalAssertValid(signer);

    return signer->interface->GetCertificateDigest(signer->interface->interfaceContext);
}

PARCBuffer *
parcSigner_GetDEREncodedCertificate(PARCSigner *signer)
{
    parcSigner_OptionalAssertValid(signer);

    return signer->interface->GetDEREncodedCertificate(signer->interface->interfaceContext);
}

PARCBuffer *
parcSigner_GetDEREncodedPublicKey(const PARCSigner *signer)
{
    parcSigner_OptionalAssertValid(signer);

    return signer->interface->GetDEREncodedPublicKey(signer->interface->interfaceContext);
}

PARCKey *
parcSigner_CreatePublicKey(PARCSigner *signer)
{
    PARCCryptoHash *hash = parcSigner_GetVerifierKeyDigest(signer);

    PARCKeyId *keyid = parcKeyId_Create(parcCryptoHash_GetDigest(hash));

    PARCBuffer *derEncodedKey = parcSigner_GetDEREncodedPublicKey(signer);

    PARCKey *key = parcKey_CreateFromDerEncodedPublicKey(keyid,
                                                         parcSigner_GetSigningAlgorithm(signer),
                                                         derEncodedKey);

    parcBuffer_Release(&derEncodedKey);
    parcKeyId_Release(&keyid);

    parcCryptoHash_Release(&hash);
    return key;
}

PARCKeyId *
parcSigner_CreateKeyId(const PARCSigner *signer)
{
    PARCCryptoHash *hash = parcSigner_GetVerifierKeyDigest(signer);
    PARCBuffer *keyidBytes = parcCryptoHash_GetDigest(hash);
    PARCKeyId *result = parcKeyId_Create(keyidBytes);

    parcCryptoHash_Release(&hash);
    return result;
}

PARCCryptoHasher *
parcSigner_GetCryptoHasher(const PARCSigner *signer)
{
    parcSigner_OptionalAssertValid(signer);

    return signer->interface->GetCryptoHasher(signer->interface->interfaceContext);
}

PARCSignature *
parcSigner_SignDigest(const PARCSigner *signer, const PARCCryptoHash *parcDigest)
{
    parcSigner_OptionalAssertValid(signer);

    assertNotNull(parcDigest, "parcDigest to sign must not be null");
    return signer->interface->SignDigest(signer->interface->interfaceContext, parcDigest);
}

PARCSigningAlgorithm
parcSigner_GetSigningAlgorithm(PARCSigner *signer)
{
    parcSigner_OptionalAssertValid(signer);

    return signer->interface->GetSigningAlgorithm(signer->interface->interfaceContext);
}

PARCCryptoHashType
parcSigner_GetCryptoHashType(const PARCSigner *signer)
{
    parcSigner_OptionalAssertValid(signer);

    return signer->interface->GetCryptoHashType(signer->interface->interfaceContext);
}
