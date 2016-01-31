/*
 * Copyright (c) 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
//  parc_Certificate.c
//  PARC Library
//
//  Created by Wood, Christopher <Christopher.Wood@parc.com> on 8/31/15.
//  Copyright (c) 2015 Computer Science Laboratory. All rights reserved.
//

#include <config.h>
#include <stdio.h>

#include <parc/security/parc_Certificate.h>
#include <parc/security/parc_CertificateType.h>
#include <parc/security/parc_ContainerEncoding.h>

#include <parc/algol/parc_Object.h>

struct parc_certificate {
    PARCCertificateInterface *interface;
    void *instance;
};

static void
_parcCertificate_FinalRelease(PARCCertificate **certP)
{
    PARCCertificate *certificate = (PARCCertificate *) *certP;

    if (certificate->instance != NULL) {
        parcObject_Release(&certificate->instance);
    }
}

parcObject_ExtendPARCObject(PARCCertificate, _parcCertificate_FinalRelease, NULL, NULL, NULL, NULL, NULL, NULL);

static PARCCertificate *
_parcCertificate_Create(PARCCertificateInterface *impl, void *instance)
{
    PARCCertificate *cert = parcObject_CreateInstance(PARCCertificate);
    cert->interface = impl;
    cert->instance = instance;
    return cert;
}

parcObject_ImplementAcquire(parcCertificate, PARCCertificate);

parcObject_ImplementRelease(parcCertificate, PARCCertificate);

PARCCertificate *
parcCertificate_CreateFromInstance(PARCCertificateInterface *impl, void *instance)
{
    return _parcCertificate_Create(impl, instance);
}

PARCCertificateType
parcCertificate_GetCertificateType(const PARCCertificate *cert)
{
    if (cert->interface->GetCertificateType != NULL) {
        return cert->interface->GetCertificateType(cert->instance);
    }
    return PARCCertificateType_Invalid;
}

PARCContainerEncoding
parcCertificate_GetContainerEncoding(const PARCCertificate *cert)
{
    if (cert->interface->GetContainerEncoding != NULL) {
        return cert->interface->GetContainerEncoding(cert->instance);
    }
    return PARCContainerEncoding_Invalid;
}

PARCCryptoHash *
parcCertificate_GetPublicKeyDigest(const PARCCertificate *certificate)
{
    if (certificate->interface->GetPublicKeyDigest != NULL) {
        return certificate->interface->GetPublicKeyDigest(certificate->instance);
    }
    return NULL;
}

PARCCryptoHash *
parcCertificate_GetCertificateDigest(const PARCCertificate *certificate)
{
    if (certificate->interface->GetCertificateDigest != NULL) {
        return certificate->interface->GetCertificateDigest(certificate->instance);
    }
    return NULL;
}

PARCBuffer *
parcCertificate_GetDEREncodedCertificate(const PARCCertificate *certificate)
{
    if (certificate->interface->GetDEREncodedCertificate != NULL) {
        return certificate->interface->GetDEREncodedCertificate(certificate->instance);
    }
    return NULL;
}

PARCBuffer *
parcCertificate_GetDEREncodedPublicKey(const PARCCertificate *certificate)
{
    if (certificate->interface->GetDEREncodedPublicKey != NULL) {
        return certificate->interface->GetDEREncodedPublicKey(certificate->instance);
    }
    return NULL;
}

PARCKey *
parcCertificate_GetPublicKey(const PARCCertificate *certificate)
{
    PARCBuffer *derEncodedVersion = parcCertificate_GetDEREncodedPublicKey(certificate);
    PARCCryptoHash *keyDigest = parcCertificate_GetPublicKeyDigest(certificate);
    PARCKeyId *keyId = parcKeyId_Create(parcCryptoHash_GetDigest(keyDigest));

    PARCKey *key = parcKey_CreateFromDerEncodedPublicKey(keyId, PARCSigningAlgorithm_RSA, derEncodedVersion);

    parcBuffer_Release(&derEncodedVersion);
    parcCryptoHash_Release(&keyDigest);
    parcKeyId_Release(&keyId);

    return key;
}
