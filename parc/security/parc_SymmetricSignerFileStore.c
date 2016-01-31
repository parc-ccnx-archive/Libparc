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
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
/**
 * To compute an HMAC, we need to interfaceement our own CryptoHasher so we can
 * initialize it with the secret key
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */

#include <config.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

#include <openssl/asn1.h>
#include <openssl/asn1t.h>
#include <openssl/safestack.h>
#include <openssl/bn.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <openssl/aes.h>
#include <openssl/hmac.h>

#include <LongBow/runtime.h>

#include <parc/security/parc_Signer.h>
#include <parc/algol/parc_Memory.h>
#include <parc/security/parc_Security.h>

#include <parc/security/parc_SymmetricSignerFileStore.h>

#define AES_KEYSTORE_VERSION 1L
#define IV_SIZE 16
#define AES_MAX_DIGEST_SIZE 128
#define AES_DEFAULT_DIGEST_ALGORITHM "SHA256"

static PARCBuffer *_createDerivedKey(const char *key, size_t keylength, unsigned char *salt, unsigned int saltlen);

static void _destroy(struct ccnx_signer_interface **interfaceContextPtr);

static PARCCryptoHash *_getSecretKeyDigest(void *interfaceContext);

static PARCSigningAlgorithm _getSigningAlgorithm(void *interfaceContext);

/**
 * wrap the HMAC in digestToSign in a PARCSignature
 *
 * @param hashToSign is the HMAC computed by the our PARCCryptoHasher.
 */
static PARCSignature  *
_signDigest(void *interfaceContext, const PARCCryptoHash *hashToSign)
{
    // The digest computed via our hash function (hmac) is the actual signature.
    // just need to wrap it up with the right parameters.
    PARCBuffer *signatureBits = parcBuffer_Copy(parcCryptoHash_GetDigest(hashToSign));
    PARCSignature  *result = parcSignature_Create(_getSigningAlgorithm(interfaceContext), parcCryptoHash_GetDigestType(hashToSign), signatureBits);
    parcBuffer_Release(&signatureBits);
    return result;
}

static PARCCryptoHashType _getCryptoHashType(void *interfaceContext);

static PARCCryptoHasher *_getCryptoHasher(void *interfaceContent);

static PARCSigningInterface aeskeystoreinterface = {
    .interfaceContext         = NULL,
    .Destroy                  = _destroy,
    .GetVerifierKeyDigest     = _getSecretKeyDigest,
    .GetCertificateDigest     = NULL,
    .GetDEREncodedCertificate = NULL,
    .GetDEREncodedPublicKey   = NULL,
    .GetCryptoHasher          = _getCryptoHasher,
    .SignDigest               = _signDigest,
    .GetSigningAlgorithm      = _getSigningAlgorithm,
    .GetCryptoHashType        = _getCryptoHashType
};

static void *_hmacCreate(void *env);
static int _hmacInit(void *ctx);
static int _hmacUpdate(void *ctx, const void *buffer, size_t length);
static PARCBuffer*_hmacFinalize(void *ctx);
static void _hmacDestroy(void **ctx);

static PARCCryptoHasherInterface functor_hmac = {
    .functor_env     = NULL,
    .hasher_setup    = _hmacCreate,
    .hasher_init     = _hmacInit,
    .hasher_update   = _hmacUpdate,
    .hasher_finalize = _hmacFinalize,
    .hasher_destroy  = _hmacDestroy
};

typedef struct ccnx_aes_signer {
    PARCBuffer *secretKey;

    // These will be 0 length until asked for
    PARCCryptoHash *secretKeyHash;

    PARCCryptoHasher *hasher;
    PARCCryptoHasherInterface hasherFunctor;
    PARCCryptoHashType hashType;
    unsigned hashLength;
    const EVP_MD *opensslMd;
} _PARCAesSignerFileStore;

// =============================================================

#ifdef __APPLE__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

static int _createAESKeyStore(const char *filename, const char *password, PARCBuffer *key);

static PARCBuffer *_AESKeyStoreInit(const char *filename, const char *password);

/**
 * Create a symmetric (secret) key of the given bit length (e.g. 256)
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCBuffer *
parcSymmetricSignerFileStore_CreateKey(unsigned bits)
{
    assertTrue((bits & 0x07) == 0, "bits must be a multiple of 8");

    unsigned keylength = bits / 8;
    uint8_t buffer[keylength];
    RAND_bytes(buffer, keylength);
    return parcBuffer_Flip(parcBuffer_PutArray(parcBuffer_Allocate(keylength), keylength, buffer));
}

/**
 * Creates a PARC format symmetric keystore.  It only contains a single key.
 *
 * The final filename will be "file_prefix.
 *
 * Returns 0 on success, -1 on error.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
int
parcSymmetricSignerFileStore_CreateFile(const char *filename, const char *password, PARCBuffer *secret_key)
{
    assertTrue(parcBuffer_Remaining(secret_key) > 0, "The secret_key buffer is not flipped.  See parcBuffer_Flip()");
    return _createAESKeyStore(filename, password, secret_key);
}

/**
 * Create a PKCS12 signing context for use in ccnx_Signing.  It is destroyed
 * by ccnx_Signing when the signing context is destroyed.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCSigningInterface *
parcSymmetricSignerFileStore_OpenFile(const char *filename, const char *password, PARCCryptoHashType hmacHashType)
{
    PARCBuffer *secretKey = _AESKeyStoreInit(filename, password);
    assertNotNull(secretKey, "Could not read AES keystore %s", filename);

    PARCSigningInterface *interface = parcSymmetricSignerFileStore_Create(secretKey, hmacHashType);
    parcBuffer_Release(&secretKey);

    return interface;
}

/**
 * Create a PKCS12 signing context for use in ccnx_Signing from the provided key.  It is destroyed
 * by parc_Signing when the signing context is destroyed.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCSigningInterface *
parcSymmetricSignerFileStore_Create(PARCBuffer *secret_key, PARCCryptoHashType hmacHashType)
{
    _PARCAesSignerFileStore *keystore = parcMemory_AllocateAndClear(sizeof(_PARCAesSignerFileStore));
    assertNotNull(keystore, "parcMemory_AllocateAndClear(%zu) returned NULL, cannot allocate keystore", sizeof(_PARCAesSignerFileStore));

    keystore->hashType = hmacHashType;
    switch (hmacHashType) {
        case PARC_HASH_SHA256:
            keystore->hashLength = SHA256_DIGEST_LENGTH;
            keystore->opensslMd = EVP_sha256();
            break;

        case PARC_HASH_SHA512:
            keystore->hashLength = SHA512_DIGEST_LENGTH;
            keystore->opensslMd = EVP_sha512();
            break;

        default:
            parcBuffer_Release(&secret_key);
            parcMemory_Deallocate((void **) &keystore);
            trapIllegalValue(hmacHashType, "Unknown HMAC hash type: %d", hmacHashType);
    }

    keystore->secretKey = parcBuffer_Acquire(secret_key);

    // the signer key digest is SHA256, independent of the HMAC digest
    PARCCryptoHasher *hasher = parcCryptoHasher_Create(PARC_HASH_SHA256);
    parcCryptoHasher_Init(hasher);
    parcCryptoHasher_UpdateBuffer(hasher, secret_key);
    keystore->secretKeyHash = parcCryptoHasher_Finalize(hasher);
    parcCryptoHasher_Release(&hasher);

    // create the functor from the template then specialize it to this keystore.
    // This depends on keystore->secret_key being set.  It will cause a callback
    // into hmac_setup()
    keystore->hasherFunctor = functor_hmac;
    keystore->hasherFunctor.functor_env = keystore;
    keystore->hasher = parcCryptoHasher_CustomHasher(keystore->hashType, keystore->hasherFunctor);

    PARCSigningInterface *signer = parcMemory_AllocateAndClear(sizeof(PARCSigningInterface));
    assertNotNull(signer, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(PARCSigningInterface));
    *signer = aeskeystoreinterface;
    signer->interfaceContext = keystore;
    return signer;
}

static void
_destroy(struct ccnx_signer_interface **interfaceContextPtr)
{
    assertNotNull(interfaceContextPtr, "Got null double pointer");
    assertNotNull(*interfaceContextPtr, "Got null pointer dereference");

    PARCSigningInterface *interface = (PARCSigningInterface *) *interfaceContextPtr;
    _PARCAesSignerFileStore *keystore = (_PARCAesSignerFileStore *) interface->interfaceContext;

    parcBuffer_Release(&keystore->secretKey);
    parcCryptoHash_Release(&(keystore->secretKeyHash));

    parcCryptoHasher_Release(&(keystore->hasher));

    parcMemory_Deallocate((void **) &keystore);
    parcMemory_Deallocate((void **) &interface);
    *interfaceContextPtr = NULL;
}

static PARCSigningAlgorithm
_getSigningAlgorithm(void *interfaceContext)
{
    return PARCSigningAlgorithm_HMAC;
}

static PARCCryptoHashType
_getCryptoHashType(void *interfaceContext)
{
    _PARCAesSignerFileStore *keystore = (_PARCAesSignerFileStore *) interfaceContext;
    return keystore->hashType;
}

static PARCCryptoHasher *
_getCryptoHasher(void *interfaceContext)
{
    _PARCAesSignerFileStore *keystore = (_PARCAesSignerFileStore *) interfaceContext;
    return keystore->hasher;
}

// ==================================================
// HMAC implementation

static void *
_hmacCreate(void *env)
{
    _PARCAesSignerFileStore *keystore = (_PARCAesSignerFileStore *) env;

    // HMAC_Init_ex seems to overrun the size of HMAC_CTX, so make it bigger

    HMAC_CTX *ctx = parcMemory_Allocate(sizeof(HMAC_CTX) * 2);
    assertNotNull(ctx, "parcMemory_Allocate(%zu) returned NULL for HMAC_CTX", sizeof(HMAC_CTX) * 2);

    HMAC_CTX_init(ctx);

    // Now initialize it with our digest and key, so in hmac_init we can avoid using those
    assertTrue(parcBuffer_Remaining(keystore->secretKey) < INT_MAX, "The keystore secret key cannot be longer than %d", INT_MAX);
    HMAC_Init_ex(ctx, parcByteArray_Array(parcBuffer_Array(keystore->secretKey)), (int) parcBuffer_Remaining(keystore->secretKey), keystore->opensslMd, NULL);

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

// ==================================================
// implementation


static PARCCryptoHash *
_getSecretKeyDigest(void *interfaceContext)
{
    assertNotNull(interfaceContext, "Parameter must be non-null CCNxFileKeystore");
    _PARCAesSignerFileStore *keystore = (_PARCAesSignerFileStore *) interfaceContext;
    return keystore->secretKeyHash;
}

/**
 * The openssl ASN1 representation of the PARC symmetric key keystore.
 * It will be written to disk in DER format with an i2d call
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
typedef struct PARCAESKeystore_info_st {
    ASN1_INTEGER *version;
    ASN1_OBJECT *algorithm_oid;
    ASN1_OCTET_STRING *encrypted_key;
} _PARCSymmeticSignerFileStoreInfo;

// This generates a name that is not compliant with the PARC Naming conventions.
DECLARE_ASN1_FUNCTIONS(_PARCSymmeticSignerFileStoreInfo)

ASN1_SEQUENCE(_PARCSymmeticSignerFileStoreInfo) = {
    ASN1_SIMPLE(_PARCSymmeticSignerFileStoreInfo, version,       ASN1_INTEGER),
    ASN1_SIMPLE(_PARCSymmeticSignerFileStoreInfo, algorithm_oid, ASN1_OBJECT),
    ASN1_SIMPLE(_PARCSymmeticSignerFileStoreInfo, encrypted_key, ASN1_OCTET_STRING)
} ASN1_SEQUENCE_END(_PARCSymmeticSignerFileStoreInfo)

IMPLEMENT_ASN1_FUNCTIONS(_PARCSymmeticSignerFileStoreInfo)

static int
_i2d_AESKeystore_fp(FILE *fp, _PARCSymmeticSignerFileStoreInfo *aki)
{
    return ASN1_item_i2d_fp(ASN1_ITEM_rptr(_PARCSymmeticSignerFileStoreInfo), fp, aki);
}

static _PARCSymmeticSignerFileStoreInfo *
_d2iAESKeystoreFp(FILE *fp, _PARCSymmeticSignerFileStoreInfo *aki)
{
    return ASN1_item_d2i_fp(ASN1_ITEM_rptr(_PARCSymmeticSignerFileStoreInfo), fp, aki);
}

/**
 * returns 0 success, -1 error
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
static int
_createAESKeyStore(const char *filename, const char *password, PARCBuffer *key)
{
    FILE *fp = NULL;
    int fd = -1;
    int ans = -1;
    int nid;

    _PARCSymmeticSignerFileStoreInfo *keystore = NULL;

    EVP_CIPHER_CTX ctx;
    unsigned char *encrypted_key = NULL;

    int ekl = IV_SIZE + (int) parcBuffer_Remaining(key) + SHA256_DIGEST_LENGTH + AES_BLOCK_SIZE;
    int encrypt_length;

    fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);

    if (fd == -1) {
        goto Bail;
    }
    fp = fdopen(fd, "wb");
    if (fp == NULL) {
        goto Bail;
    }

    PARCBuffer *aes_key = _createDerivedKey(password, strlen(password), (unsigned char *) "\0", 1);
    PARCBuffer *mac_key = _createDerivedKey(password, strlen(password), (unsigned char *) "\1", 1);

    encrypted_key = malloc(ekl);
    if (!encrypted_key) {
        goto Bail;
    }
    RAND_bytes(encrypted_key, IV_SIZE);
    EVP_CIPHER_CTX_init(&ctx);
    if (!EVP_EncryptInit(&ctx, EVP_aes_256_cbc(), parcByteArray_Array(parcBuffer_Array(aes_key)), encrypted_key)) {
        goto Bail;
    }

    unsigned char *p;
    p = encrypted_key + IV_SIZE;
    if (!EVP_EncryptUpdate(&ctx, p, &encrypt_length, parcByteArray_Array(parcBuffer_Array(key)), (int) parcBuffer_Remaining(key))) {
        goto Bail;
    }
    p += encrypt_length;
    if (!EVP_EncryptFinal(&ctx, p, &encrypt_length)) {
        goto Bail;
    }
    p += encrypt_length;
    HMAC(EVP_sha256(), parcByteArray_Array(parcBuffer_Array(mac_key)), SHA256_DIGEST_LENGTH, encrypted_key, p - encrypted_key, p, NULL);

    if (!(keystore = _PARCSymmeticSignerFileStoreInfo_new())) {
        goto Bail;
    }
    if (!(keystore->version = ASN1_INTEGER_new())) {
        goto Bail;
    }
    if (!ASN1_INTEGER_set(keystore->version, AES_KEYSTORE_VERSION)) {
        goto Bail;
    }

    keystore->algorithm_oid = OBJ_txt2obj(AES_DEFAULT_DIGEST_ALGORITHM, 0);
    nid = OBJ_obj2nid(keystore->algorithm_oid);
    if (nid == NID_undef) {
        goto Bail;  // Shouldn't happen now but could later if we support more algorithms
    }
    if (!ASN1_OCTET_STRING_set(keystore->encrypted_key, encrypted_key, ekl)) {
        goto Bail;
    }
    _i2d_AESKeystore_fp(fp, keystore);
    ans = 0;
    goto cleanup;

 Bail:
    ans = -1;
 cleanup:

    parcBuffer_Release(&aes_key);
    parcBuffer_Release(&mac_key);

    if (fp != NULL) {
        fclose(fp);
    }
    if (encrypted_key) {
        free(encrypted_key);
    }
    if (keystore) {
        _PARCSymmeticSignerFileStoreInfo_free(keystore);
    }
    if (fd != -1) {
        close(fd);
    }
    return (ans);
}

static PARCBuffer *
_createDerivedKey(const char *key, size_t keylength, unsigned char *salt, unsigned int saltlen)
{
    unsigned char buffer[SHA256_DIGEST_LENGTH];
    HMAC(EVP_sha256(), key, (int) keylength, salt, saltlen, buffer, NULL);
    return parcBuffer_PutArray(parcBuffer_Allocate(SHA256_DIGEST_LENGTH), SHA256_DIGEST_LENGTH, buffer);
}

/**
 * Read the secret key out of the encrypted file.
 *
 * This function needs its name corrected and rewritten.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
static PARCBuffer *
_AESKeyStoreInit(const char *filename, const char *password)
{
    PARCBuffer *secret_key = NULL;

    FILE *fp = NULL;
    _PARCSymmeticSignerFileStoreInfo *ki = NULL;
    int version;
    char oidstr[80];

    PARCBuffer *aes_key = NULL;
    PARCBuffer *mac_key = NULL;

    unsigned char check[SHA256_DIGEST_LENGTH];
    unsigned char *keybuf = NULL;
    int check_start;
    EVP_CIPHER_CTX ctx;
    int length = 0;
    int final_length = 0;

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        goto Bail;
    }

    ki = _d2iAESKeystoreFp(fp, NULL);
    fclose(fp);
    if (ki == NULL) {
        goto Bail;
    }

    version = (int) ASN1_INTEGER_get(ki->version);
    if (version != AES_KEYSTORE_VERSION) {
        goto Bail;
    }

    OBJ_obj2txt(oidstr, sizeof(oidstr), ki->algorithm_oid, 0);
    if (strcasecmp(oidstr, AES_DEFAULT_DIGEST_ALGORITHM)) {
        goto Bail;
    }

    if (ki->encrypted_key->length < IV_SIZE + (SHA256_DIGEST_LENGTH * 2) + AES_BLOCK_SIZE) {
        goto Bail;
    }

    aes_key = _createDerivedKey(password, strlen(password), (unsigned char *) "\0", 1);
    mac_key = _createDerivedKey(password, strlen(password), (unsigned char *) "\1", 1);

    check_start = ki->encrypted_key->length - SHA256_DIGEST_LENGTH;
    HMAC(EVP_sha256(),
         parcByteArray_Array(parcBuffer_Array(mac_key)),
         SHA256_DIGEST_LENGTH,
         ki->encrypted_key->data,
         check_start,
         check,
         NULL);

    if (memcmp(&ki->encrypted_key->data[check_start], check, SHA256_DIGEST_LENGTH)) {
        goto Bail;
    }
    keybuf = malloc(SHA256_DIGEST_LENGTH + AES_BLOCK_SIZE);
    EVP_CIPHER_CTX_init(&ctx);
    if (!EVP_DecryptInit(&ctx, EVP_aes_256_cbc(), parcByteArray_Array(parcBuffer_Array(aes_key)), ki->encrypted_key->data)) {
        goto Bail;
    }
    if (!EVP_DecryptUpdate(&ctx, keybuf, &length, &ki->encrypted_key->data[IV_SIZE],
                           ki->encrypted_key->length - IV_SIZE - SHA256_DIGEST_LENGTH)) {
        goto Bail;
    }

    if (!EVP_DecryptFinal(&ctx, keybuf + length, &final_length)) {
        goto Bail;
    }

    secret_key = parcBuffer_CreateFromArray(keybuf, length);
    parcBuffer_Flip(secret_key);

    goto out;

 Bail:
    free(keybuf);

 out:
    if (aes_key) {
        parcBuffer_Release(&aes_key);
    }

    if (mac_key) {
        parcBuffer_Release(&mac_key);
    }

    return secret_key;
}

#ifdef __APPLE__
#pragma clang diagnostic pop
#endif
