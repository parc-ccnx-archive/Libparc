/*
 * Copyright (c) 2013, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @author Mosko, Marc
 * @copyright 2013, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

#include <config.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <LongBow/runtime.h>

#include <parc/security/parc_Security.h>
#include <parc/security/parc_Signer.h>
#include <parc/algol/parc_Memory.h>

#include <parc/security/parc_SelfSignedCertificate.h>
#include <parc/security/parc_CryptoHash.h>
#include <parc/security/parc_PublicKeySignerPkcs12Store.h>

#include <openssl/pkcs12.h>
#include <openssl/x509v3.h>
#include <openssl/err.h>

static void _Destroy(struct ccnx_signer_interface **interfaceContextPtr);
static PARCCryptoHash *_GetPublickKeyDigest(void *interfaceContext);
static PARCCryptoHash *_GetCertificateDigest(void *interfaceContext);
static PARCBuffer *_GetDEREncodedCertificate(void *interfaceContext);
static PARCBuffer *_GetDEREncodedPublicKey(void *interfaceContext);
static PARCSignature  *_SignDigest(void *interfaceContext, const PARCCryptoHash *cryptoHash);

static PARCSigningAlgorithm _GetSigningAlgorithm(void *interfaceContext);
static PARCCryptoHashType _GetCryptoHashType(void *interfaceContext);

static PARCCryptoHasher *_GetCryptoHasher(void *interfaceContext);

static PARCSigningInterface filekeystoreinterface = {
    .interfaceContext         = NULL,
    .Destroy                  = _Destroy,
    .GetVerifierKeyDigest     = _GetPublickKeyDigest,
    .GetCertificateDigest     = _GetCertificateDigest,
    .GetDEREncodedCertificate = _GetDEREncodedCertificate,
    .GetDEREncodedPublicKey   = _GetDEREncodedPublicKey,
    .GetCryptoHasher          = _GetCryptoHasher,
    .SignDigest               = _SignDigest,
    .GetSigningAlgorithm      = _GetSigningAlgorithm,
    .GetCryptoHashType        = _GetCryptoHashType
};

typedef struct parc_publickey_signer_pkcs12store {
    EVP_PKEY *private_key;
    EVP_PKEY *public_key;
    X509 *x509_cert;

    // These will be 0 length until asked for
    PARCBuffer *public_key_digest;
    PARCBuffer *certificate_digest;
    PARCBuffer *public_key_der;
    PARCBuffer *certificate_der;

    PARCCryptoHashType hashType;
    PARCCryptoHasher *hasher;
} PARCPublicKeySignerPkcs12Store;

static int _parcPublicKeySignerPkcs12Store_ParseFile(
    PARCPublicKeySignerPkcs12Store *keystore,
    const char *filename,
    const char *password);

#ifdef __APPLE__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

bool
parcPublicKeySignerPkcs12Store_CreateFile(
    const char *filename,
    const char *password,
    const char *subjectName,
    unsigned keyLength,
    unsigned validityDays)
{
    parcSecurity_AssertIsInitialized();

    bool result = false;

    RSA *rsa = RSA_new();
    assertNotNull(rsa, "RSA_new failed.");

    EVP_PKEY *privateKey = EVP_PKEY_new();
    assertNotNull(privateKey, "EVP_PKEY_new() failed.");

    X509 *cert = X509_new();
    assertNotNull(cert, "X509_new() failed.");

    if (parcSelfSignedCertificate_CreateRSACertificate(subjectName, keyLength, validityDays, cert, rsa, privateKey) == true) {
        // construct the full PKCS12 keystore to hold the certificate and private key
        PKCS12 *pkcs12 = PKCS12_create((char *) password,
                                       "ccnxuser",
                                       privateKey,
                                       cert,
                                       NULL,
                                       0,
                                       0,
                                       0 /*default iter*/,
                                       PKCS12_DEFAULT_ITER /*mac_iter*/,
                                       0);

        if (pkcs12 != NULL) {
            int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
            if (fd != -1) {
                FILE *fp = fdopen(fd, "wb");
                if (fp != NULL) {
                    i2d_PKCS12_fp(fp, pkcs12);
                    fclose(fp);
                    result = true;
                } else {
                    trapUnrecoverableState("Cannot fdopen(3) the file descriptor %d", fd);
                }
                close(fd);
            } else {
                trapUnrecoverableState("Cannot open(2) the file '%s': %s", filename, strerror(errno));
            }
            PKCS12_free(pkcs12);
        } else {
            unsigned long errcode;
            while ((errcode = ERR_get_error()) != 0) {
                fprintf(stderr, "openssl error: %s\n", ERR_error_string(errcode, NULL));
            }
            trapUnrecoverableState("PKCS12_create returned a NULL value.");
        }
    }

    EVP_PKEY_free(privateKey);
    RSA_free(rsa);
    X509_free(cert);

    return result;
}

PARCSigningInterface *
parcPublicKeySignerPkcs12Store_Open(const char *filename, const char *password, PARCCryptoHashType hashType)
{
    PARCSigningInterface *interface = parcMemory_AllocateAndClear(sizeof(PARCSigningInterface));
    assertNotNull(interface, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(PARCSigningInterface));

    PARCPublicKeySignerPkcs12Store *keystore = parcMemory_AllocateAndClear(sizeof(PARCPublicKeySignerPkcs12Store));
    assertNotNull(keystore, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(PARCPublicKeySignerPkcs12Store));

    int result = _parcPublicKeySignerPkcs12Store_ParseFile(keystore, filename, password);
    if (result != 0) {
        parcMemory_Deallocate((void **) &keystore);
        parcMemory_Deallocate((void **) &interface);
        return NULL;
    }

    *interface = filekeystoreinterface;
    interface->interfaceContext = keystore;

    keystore->public_key_digest = NULL;
    keystore->certificate_digest = NULL;
    keystore->public_key_der = NULL;
    keystore->certificate_der = NULL;
    keystore->hasher = parcCryptoHasher_Create(hashType);
    keystore->hashType = hashType;

    return interface;
}

// ===========================================
// Actual implementation

static int
_parcPublicKeySignerPkcs12Store_ParseFile(PARCPublicKeySignerPkcs12Store *keystore, const char *filename, const char *password)
{
    parcSecurity_AssertIsInitialized();

    FILE *fp = fopen(filename, "rb");

    assertNotNull(fp, "Error opening %s: %s", filename, strerror(errno));
    if (fp == NULL) {
        return -1;
    }

    PKCS12 *p12Keystore = NULL;
    d2i_PKCS12_fp(fp, &p12Keystore);
    fclose(fp);

    int success = PKCS12_parse(p12Keystore, password, &keystore->private_key, &keystore->x509_cert, NULL);
    PKCS12_free(p12Keystore);

    if (!success) {
        unsigned long errcode;
        while ((errcode = ERR_get_error()) != 0) {
            fprintf(stderr, "openssl error: %s\n", ERR_error_string(errcode, NULL));
        }
        return -1;
    }

    keystore->public_key = X509_get_pubkey(keystore->x509_cert);
    return 0;
}

/**
 * Destroys the signing context and the underlying implementation
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
static void
_Destroy(struct ccnx_signer_interface **interfaceContextPtr)
{
    parcSecurity_AssertIsInitialized();

    assertNotNull(interfaceContextPtr, "Got null double pointer");
    assertNotNull(*interfaceContextPtr, "Got null pointer dereference");

    PARCSigningInterface *interface = (PARCSigningInterface *) *interfaceContextPtr;

    PARCPublicKeySignerPkcs12Store *keystore = (PARCPublicKeySignerPkcs12Store *) interface->interfaceContext;

    EVP_PKEY_free(keystore->private_key);
    EVP_PKEY_free(keystore->public_key);
    X509_free(keystore->x509_cert);

    if (keystore->public_key_digest != NULL) {
        parcBuffer_Release(&keystore->public_key_digest);
    }
    if (keystore->certificate_digest != NULL) {
        parcBuffer_Release(&keystore->certificate_digest);
    }
    if (keystore->public_key_der != NULL) {
        parcBuffer_Release(&keystore->public_key_der);
    }
    if (keystore->certificate_der != NULL) {
        parcBuffer_Release(&keystore->certificate_der);
    }

    parcCryptoHasher_Release(&(keystore->hasher));

    parcMemory_Deallocate((void **) &keystore);
    parcMemory_Deallocate((void **) &interface);
    *interfaceContextPtr = NULL;
}

static PARCCryptoHash *
_GetPublickKeyDigest(void *interfaceContext)
{
    parcSecurity_AssertIsInitialized();

    assertNotNull(interfaceContext, "Parameter must be non-null CCNxFileKeystore");

    PARCPublicKeySignerPkcs12Store *keystore = (PARCPublicKeySignerPkcs12Store *) interfaceContext;

    if (keystore->public_key_digest == NULL) {
        AUTHORITY_KEYID  *akid = X509_get_ext_d2i(keystore->x509_cert, NID_authority_key_identifier, NULL, NULL);
        if (akid != NULL) {
            ASN1_OCTET_STRING *skid = X509_get_ext_d2i(keystore->x509_cert, NID_subject_key_identifier, NULL, NULL);
            if (skid != NULL) {
                keystore->public_key_digest =
                    parcBuffer_PutArray(parcBuffer_Allocate(skid->length), skid->length, skid->data);
                parcBuffer_Flip(keystore->public_key_digest);
                AUTHORITY_KEYID_free(akid);
                ASN1_OCTET_STRING_free(skid);
            }
        }
    }

    if (keystore->public_key_digest == NULL) {
        // could not load from certificate, so calculate it on the public key
        uint8_t digestBuffer[SHA256_DIGEST_LENGTH];

        int result = ASN1_item_digest(ASN1_ITEM_rptr(X509_PUBKEY),
                                      EVP_sha256(),
                                      X509_get_X509_PUBKEY(keystore->x509_cert),
                                      digestBuffer,
                                      NULL);
        if (result != 1) {
            assertTrue(0, "Could not compute digest over certificate public key");
        } else {
            keystore->public_key_digest =
                parcBuffer_PutArray(parcBuffer_Allocate(SHA256_DIGEST_LENGTH), SHA256_DIGEST_LENGTH, digestBuffer);
            parcBuffer_Flip(keystore->public_key_digest);
        }
    }

    // This stores a reference, so keystore->public_key_digest will remain valid
    // even if the cryptoHasher is destroyed
    return parcCryptoHash_Create(PARC_HASH_SHA256, keystore->public_key_digest);
}

static PARCCryptoHash *
_GetCertificateDigest(void *interfaceContext)
{
    parcSecurity_AssertIsInitialized();

    assertNotNull(interfaceContext, "Parameter must be non-null CCNxFileKeystore");

    PARCPublicKeySignerPkcs12Store *keystore = (PARCPublicKeySignerPkcs12Store *) interfaceContext;

    if (keystore->certificate_digest == NULL) {
        uint8_t digestBuffer[SHA256_DIGEST_LENGTH];
        int result = X509_digest(keystore->x509_cert, EVP_sha256(), digestBuffer, NULL);
        if (result) {
            keystore->certificate_digest =
                parcBuffer_Flip(parcBuffer_PutArray(parcBuffer_Allocate(SHA256_DIGEST_LENGTH), SHA256_DIGEST_LENGTH, digestBuffer));
        }
    }

    // parcCryptoHash uses _Share on the buffer
    return parcCryptoHash_Create(PARC_HASH_SHA256, keystore->certificate_digest);
}

static PARCBuffer *
_GetDEREncodedCertificate(void *interfaceContext)
{
    parcSecurity_AssertIsInitialized();

    assertNotNull(interfaceContext, "Parameter must be non-null CCNxFileKeystore");

    PARCPublicKeySignerPkcs12Store *keystore = (PARCPublicKeySignerPkcs12Store *) interfaceContext;

    if (keystore->certificate_der == NULL) {
        uint8_t *der = NULL;

        // this allocates memory for der
        int derLength = i2d_X509(keystore->x509_cert, &der);
        if (derLength > 0) {
            keystore->certificate_der =
                parcBuffer_Flip(parcBuffer_PutArray(parcBuffer_Allocate(derLength), derLength, der));
        }
        OPENSSL_free(der);
    }

    return parcBuffer_Copy(keystore->certificate_der);
}

static PARCBuffer *
_GetDEREncodedPublicKey(void *interfaceContext)
{
    parcSecurity_AssertIsInitialized();

    assertNotNull(interfaceContext, "Parameter must be non-null CCNxFileKeystore");

    PARCPublicKeySignerPkcs12Store *keystore = (PARCPublicKeySignerPkcs12Store *) interfaceContext;

    if (keystore->public_key_der == NULL) {
        uint8_t *der = NULL;

        // this allocates memory for der
        int derLength = i2d_PUBKEY(keystore->public_key, &der);
        if (derLength > 0) {
            keystore->public_key_der =
                parcBuffer_Flip(parcBuffer_PutArray(parcBuffer_Allocate(derLength), derLength, der));
        }
        OPENSSL_free(der);
    }

    return parcBuffer_Copy(keystore->public_key_der);
}

static PARCSignature *
_SignDigest(void *interfaceContext, const PARCCryptoHash *digestToSign)
{
    parcSecurity_AssertIsInitialized();

    assertNotNull(interfaceContext, "Parameter must be non-null CCNxFileKeystore");
    assertNotNull(digestToSign, "Buffer to sign must not be null");

    PARCPublicKeySignerPkcs12Store *keystore = (PARCPublicKeySignerPkcs12Store *) interfaceContext;

    RSA *rsa = EVP_PKEY_get1_RSA(keystore->private_key);

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
        parcSignature_Create(_GetSigningAlgorithm(interfaceContext),
                             parcCryptoHash_GetDigestType(digestToSign),
                             bbSign
                             );
    parcBuffer_Release(&bbSign);
    return signature;
}

static PARCSigningAlgorithm
_GetSigningAlgorithm(void *interfaceContext)
{
    return PARCSigningAlgorithm_RSA;
}

static PARCCryptoHashType
_GetCryptoHashType(void *interfaceContext)
{
    PARCPublicKeySignerPkcs12Store *keystore = (PARCPublicKeySignerPkcs12Store *) interfaceContext;
    return keystore->hashType;
}


static PARCCryptoHasher *
_GetCryptoHasher(void *interfaceContext)
{
    assertNotNull(interfaceContext, "Parameter must be non-null CCNxFileKeystore");
    PARCPublicKeySignerPkcs12Store *keystore = (PARCPublicKeySignerPkcs12Store *) interfaceContext;

    return keystore->hasher;
}

#ifdef __APPLE__
#pragma clang diagnostic pop
#endif
