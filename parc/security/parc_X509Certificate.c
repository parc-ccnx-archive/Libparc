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
//  parc_X509Certificate.c
//  PARC Library
//
//  Created by Wood, Christopher <Christopher.Wood@parc.com> on 8/31/15.
//  Copyright (c) 2015 Computer Science Laboratory. All rights reserved.
//

#include <config.h>

#include <parc/security/parc_X509Certificate.h>
#include <parc/security/parc_Certificate.h>

#include <parc/security/parc_CryptoHash.h>
#include <parc/security/parc_Security.h>
#include <parc/algol/parc_Buffer.h>
#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_Memory.h>

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509v3.h>

static PARCCryptoHash *_getPublicKeyDigest(void *interfaceContext);
static PARCCryptoHash *_getCertificateDigest(void *interfaceContext);
static PARCBuffer *_getDEREncodedCertificate(void *interfaceContext);
static PARCBuffer *_getDEREncodedPublicKey(void *interfaceContext);
static PARCCertificateType _getCertificateType(const void *cert);
static PARCContainerEncoding _getContainerEncoding(const void *cert);

PARCCertificateInterface *PARCX509CertificateInterface = &(PARCCertificateInterface) {
    .GetPublicKeyDigest       = _getPublicKeyDigest,
    .GetCertificateDigest     = _getCertificateDigest,
    .GetDEREncodedCertificate = _getDEREncodedCertificate,
    .GetDEREncodedPublicKey   = _getDEREncodedPublicKey,
    .GetCertificateType       = _getCertificateType,
    .GetContainerEncoding     = _getContainerEncoding
};

struct parc_X509_certificate {
    PARCCertificateType type;
    PARCContainerEncoding encoding;

    // Cache of results
    PARCCryptoHash *keyDigest;
    PARCCryptoHash *certificateDigest;
    PARCBuffer *derEncodedCertificate;
    PARCBuffer *derEncodedKey;

    BIO *certificateBIO;
    X509 *certificate;
    EVP_PKEY *publicKey;
};

#ifdef __APPLE__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

static PARCCryptoHash *
_getPublicKeyDigest(void *interfaceContext)
{
    parcSecurity_AssertIsInitialized();

    assertNotNull(interfaceContext, "Parameter must be non-null PARCX509Certificate");

    PARCX509Certificate *certificate = (PARCX509Certificate *) interfaceContext;

    if (certificate->keyDigest == NULL) {
        PARCBuffer *derEncodedKey = _getDEREncodedPublicKey(certificate);
        PARCCryptoHasher *hasher = parcCryptoHasher_Create(PARC_HASH_SHA256);
        parcCryptoHasher_Init(hasher);
        parcCryptoHasher_UpdateBuffer(hasher, derEncodedKey);
        PARCCryptoHash *hash = parcCryptoHasher_Finalize(hasher);

        certificate->keyDigest = parcCryptoHash_Create(PARC_HASH_SHA256, parcCryptoHash_GetDigest(hash));
        parcCryptoHash_Release(&hash);
        parcCryptoHasher_Release(&hasher);
    }

    return certificate->keyDigest;
}

static PARCCryptoHash *
_getCertificateDigest(void *interfaceContext)
{
    parcSecurity_AssertIsInitialized();

    assertNotNull(interfaceContext, "Parameter must be non-null PARCX509Certificate");

    PARCX509Certificate *certificate = (PARCX509Certificate *) interfaceContext;

    if (certificate->certificateDigest == NULL) {
        uint8_t digestBuffer[SHA256_DIGEST_LENGTH];
        int result = X509_digest(certificate->certificate, EVP_sha256(), digestBuffer, NULL);
        if (result) {
            PARCBuffer *digest =
                parcBuffer_Flip(parcBuffer_PutArray(parcBuffer_Allocate(SHA256_DIGEST_LENGTH), SHA256_DIGEST_LENGTH, digestBuffer));
            certificate->certificateDigest = parcCryptoHash_Create(PARC_HASH_SHA256, digest);
            parcBuffer_Release(&digest);
        }
    }

    return certificate->certificateDigest;
}

static PARCBuffer *
_getDEREncodedCertificate(void *interfaceContext)
{
    parcSecurity_AssertIsInitialized();

    assertNotNull(interfaceContext, "Parameter must be non-null PARCX509Certificate");

    PARCX509Certificate *certificate = (PARCX509Certificate *) interfaceContext;

    if (certificate->derEncodedCertificate == NULL) {
        uint8_t *der = NULL;

        // this allocates memory for der
        int derLength = i2d_X509(certificate->certificate, &der);
        if (derLength > 0) {
            certificate->derEncodedCertificate =
                parcBuffer_Flip(parcBuffer_PutArray(parcBuffer_Allocate(derLength), derLength, der));
        }
        OPENSSL_free(der);
    }

    return certificate->derEncodedCertificate;
}

static PARCBuffer *
_getDEREncodedPublicKey(void *interfaceContext)
{
    parcSecurity_AssertIsInitialized();

    assertNotNull(interfaceContext, "Parameter must be non-null PARCX509Certificate");

    PARCX509Certificate *certificate = (PARCX509Certificate *) interfaceContext;

    if (certificate->derEncodedKey == NULL) {
        uint8_t *der = NULL;

        // this allocates memory for der
        int derLength = i2d_PUBKEY(certificate->publicKey, &der);
        if (derLength > 0) {
            certificate->derEncodedKey = parcBuffer_Flip(parcBuffer_PutArray(parcBuffer_Allocate(derLength), derLength, der));
        }
        OPENSSL_free(der);
    }

    return certificate->derEncodedKey;
}

static PARCCertificateType
_getCertificateType(const void *instance)
{
    PARCX509Certificate *certificate = (PARCX509Certificate *) instance;
    return certificate->type;
}

static PARCContainerEncoding
_getContainerEncoding(const void *instance)
{
    PARCX509Certificate *certificate = (PARCX509Certificate *) instance;
    return certificate->encoding;
}

static void
_parcX509Certificate_FinalRelease(PARCX509Certificate **certP)
{
    PARCX509Certificate *cert = (PARCX509Certificate *) *certP;
    if (cert->certificateBIO != NULL) {
        BIO_free_all(cert->certificateBIO);
    }
    if (cert->publicKey != NULL) {
        EVP_PKEY_free(cert->publicKey);
    }
    if (cert->certificate != NULL) {
        X509_free(cert->certificate);
    }
    if (cert->keyDigest != NULL) {
        parcCryptoHash_Release(&cert->keyDigest);
    }
    if (cert->certificateDigest != NULL) {
        parcCryptoHash_Release(&cert->certificateDigest);
    }
    if (cert->derEncodedCertificate != NULL) {
        parcBuffer_Release(&cert->derEncodedCertificate);
    }
    if (cert->derEncodedKey != NULL) {
        parcBuffer_Release(&cert->derEncodedKey);
    }
}

parcObject_ExtendPARCObject(PARCX509Certificate, _parcX509Certificate_FinalRelease, NULL, NULL, NULL, NULL, NULL, NULL);

static PARCX509Certificate *
_createEmptyCertificate()
{
    PARCX509Certificate *cert = parcObject_CreateInstance(PARCX509Certificate);
    cert->certificateBIO = NULL;
    cert->certificate = NULL;
    cert->publicKey = NULL;
    cert->keyDigest = NULL;
    cert->certificateDigest = NULL;
    cert->derEncodedCertificate = NULL;
    cert->derEncodedKey = NULL;
    assertNotNull(cert, "Failure allocating memory for a new PARCX509Certificate instance");

    return cert;
}

static PARCX509Certificate *
_parcX509Certificate_CreateFromPEMFile(const char *filename)
{
    parcSecurity_AssertIsInitialized();

    PARCX509Certificate *cert = _createEmptyCertificate();

    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        parcX509Certificate_Release(&cert);
        return NULL;
    }

    cert->certificateBIO = BIO_new(BIO_s_file());
    size_t result = BIO_read_filename(cert->certificateBIO, filename);
    assertTrue(result == 1, "Unable to open the specified file");

    cert->certificate = PEM_read_bio_X509(cert->certificateBIO, NULL, 0, NULL);
    cert->publicKey = X509_get_pubkey(cert->certificate);

    return cert;
}

parcObject_ImplementAcquire(parcX509Certificate, PARCX509Certificate);

parcObject_ImplementRelease(parcX509Certificate, PARCX509Certificate);

PARCX509Certificate *
parcX509Certificate_CreateFromPEMFile(const char *filename)
{
    PARCX509Certificate *certificate = _parcX509Certificate_CreateFromPEMFile(filename);
    if (certificate) {
        certificate->type = PARCCertificateType_X509;
        certificate->encoding = PARCContainerEncoding_PEM;
        return certificate;
    }
    return NULL;
}

PARCX509Certificate *
parcX509Certificate_CreateFromDERBuffer(const PARCBuffer *buffer)
{
    parcSecurity_AssertIsInitialized();

    PARCX509Certificate *cert = _createEmptyCertificate();
    cert->type = PARCCertificateType_X509;
    cert->encoding = PARCContainerEncoding_DER;

    PARCByteArray *array = parcBuffer_Array(buffer);
    uint8_t *arrayIn = parcByteArray_Array(array);

    cert->certificate = d2i_X509(&cert->certificate, (const unsigned char **) &arrayIn, parcBuffer_Remaining(buffer));
    if (cert->certificate == NULL) {
        parcX509Certificate_Release(&cert);
        return NULL;
    }
    cert->publicKey = X509_get_pubkey(cert->certificate);

    return cert;
}

PARCCryptoHash *
parcX509Certificate_GetCertificateDigest(PARCX509Certificate *certificate)
{
    return _getCertificateDigest(certificate);
}

PARCCryptoHash *
parcX509Certificate_getPublicKeyDigest(PARCX509Certificate *certificate)
{
    return _getPublicKeyDigest(certificate);
}

PARCBuffer *
parcX509Certificate_GetDEREncodedCertificate(PARCX509Certificate *certificate)
{
    return _getDEREncodedCertificate(certificate);
}

PARCBuffer *
parcX509Certificate_GetDEREncodedPublicKey(PARCX509Certificate *certificate)
{
    return _getDEREncodedPublicKey(certificate);
}

