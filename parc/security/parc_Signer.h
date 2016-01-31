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
 * @file parc_Signer.h
 * @ingroup security
 * @brief The API a crytography provider must interfaceement.
 *
 * One concrete implementation is the parc_PublicKeySignerPcks12Store, which uses a PKCS #12 wrapper.
 * Another is parc_AesSignerFileStore, which uses an AES-256 key and HMAC-SHA256.
 *
 * A signer IS NOT THREAD-SAFE.
 *
 * Here's some typical use cases of the signer.
 *
 * <code>
 * // create the file
 * parcPublicKeySignerPkcs12Store_CreateFile(filename, password, "alice", key_bits, valid_days);
 *
 * // open it as an RSA provider for the signer
 * PARCSigner *signer = parcSigner_Create( parcPublicKeySignerPkcs12Store_Open(filename, password, PARC_HASH_SHA256) );
 * PARCCryptoHasher *hasher = parcSigner_GetCryptoHasher(signer);
 * parcCryptoHasher_Init(hasher);
 * parcCryptoHasher_Update_Bytes(hasher, to_sign, sizeof(to_sign));
 * PARCCryptoHash *hash = parcCryptoHasher_Finalize(hasher);
 * PARCSignature *sig = parcSigner_SignDigest(signer, hash);
 *
 * char *s = parcSignature_ToString(sig);
 * printf("Signature: %s\n", s);
 * parcMemory_Deallocate((void **)&s);
 *
 * parcCryptoHash_Release(&hash);
 * parcSignature_Release(&sig);
 * parcSigner_Release(&signer);
 * </code>
 *
 *
 * If you want to use a HMAC, you could do something like this:
 *
 * <code>
 * // create the file
 * PARCBuffer *secret_key = parcAesSignerFileStore_CreateKey(256);
 * parcAesSignerFileStore_CreateFile(filename, password, secret_key);
 *
 * // open it as a symmetric key provider for the signer
 * PARCSigner *signer = parcSigner_Create( parcAesSignerFileStore_OpenFile(filename, password, PARC_HASH_SHA256) );
 * </code>
 *
 * and the rest of the code is the same.  Or you could skip the file part and have
 * just done this to use an in-memory hmac
 *
 * <code>
 * PARCSigner *signer = parcSigner_Create( parcAesSignerFileStore_Create(secret_key, PARC_HASH_SHA256) )
 * </code>
 *
 *
 * @author Marc Mosko, Christopher A. Wood, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libparc_parc_Signer_h
#define libparc_parc_Signer_h

#include <parc/algol/parc_Buffer.h>
#include <parc/security/parc_CryptoHasher.h>
#include <parc/security/parc_Signature.h>
#include <parc/security/parc_CryptoHashType.h>
#include <parc/security/parc_Key.h>

struct parc_signer;
/**
 * @typedef PARCSigner
 * @brief The structure for PARCSigner
 */

typedef struct parc_signer PARCSigner;

/**
 * @def parcSigner_OptionalAssertValid
 * Optional validation of the given instance.
 *
 * Define `PARCLibrary_DISABLE_VALIDATION` to disable validation.
 */
#ifdef PARCLibrary_DISABLE_VALIDATION
#  define parcSigner_OptionalAssertValid(_instance_)
#else
#  define parcSigner_OptionalAssertValid(_instance_) parcSigner_AssertValid(_instance_)
#endif

/**
 * @typedef PARCSigningInterface
 * @brief The CCN signing implementation structure.
 *
 * This defines the contract that any concrete implementation provides.
 *
 */
typedef struct ccnx_signer_interface {
    void *interfaceContext;

    /**
     * The hash of the signer's public key (or secret key for HMAC).
     *
     * Try using `parcSigner_CreateKeyId` for a sinterfaceer interface.
     * You must destroy the returned PARCCryptoHash.
     * For public key, its the SHA256 digest of the public key.
     * For HMAC, its the SHA256 digest of the secret key.
     *
     * Equivalent of (for rsa/sha256):
     *    openssl rsa -in test_rsa_key.pem -outform DER -pubout -out test_rsa_pub.der
     *    openssl sha256 -out test_rsa_pub_sha256.bin -sha256 -binary < test_rsa_pub.der
     *
     * @param [in] interfaceContext A pointer to a concrete PARCSigner instance.
     *
     * @return A PARCCryptoHash value.
     */
    PARCCryptoHash *(*GetVerifierKeyDigest)(void *interfaceContext);

    /**
     * Returns a copy of the the certificate digest.
     * Returns NULL for symmetric keystores.
     *
     * Equivalent of (for rsa/sha256):
     *    openssl x509 -outform DER -out test_rsa_crt.der -in test_rsa.crt
     *    openssl sha256 -out test_rsa_crt_sha256.bin -sha256 -binary < test_rsa_crt.der
     * Which is also the same as (but not in der format)
     *    openssl x509 -in test_rsa.crt -fingerprint -sha256
     *
     * @param [in] interfaceContext A pointer to a concrete PARCSigner instance.
     *
     * @return A `PARCCryptoHash` instance which internally contains a hash digest of the certificate used by the signer.
     */
    PARCCryptoHash *(*GetCertificateDigest)(void *interfaceContext);

    /**
     * Returns a copy of the DER encoded certificate.
     * Returns NULL for symmetric keystores.
     *
     * Equivalent of:
     *   openssl x509 -outform DER -out test_rsa_crt.der -in test_rsa.crt
     *
     * @param [in] interfaceContextPtr A pointer to a concrete PARCSigner instance.
     *
     * @return A pointer to a PARCBuffer containing the encoded certificate.
     */
    PARCBuffer *(*GetDEREncodedCertificate)(void *interfaceContext);

    /**
     * Returns a copy of the encoded public key in Distinguished Encoding Rules (DER) form.
     *
     * Equivalent of (for rsa/sha256):
     *   `openssl rsa -in test_rsa_key.pem -outform DER -pubout -out test_rsa_pub.der`
     *
     * @param [in] interfaceContextPtr A pointer to a concrete PARCSigner instance.
     *
     * @return A pointer to a PARCBuffer containing the encoded public key.
     */
    PARCBuffer *(*GetDEREncodedPublicKey)(void *interfaceContext);

    /**
     * Returns a the hasher to use for the signature.  This is important for
     * symmetric key HMAC to use this hasher, not one from PARCCryptoHasher.
     *
     * DO NOT DESTROY THIS HASHER!  Just call _Init, _Update, and _Finalize.
     *
     * The hash type will be based on how the underlying implementation was initialized
     *
     * Equivalent of (for rsa/sha256)
     *    openssl rsautl -sign -inkey test_rsa_key.pem -in infile_digest -out infile.sig
     *
     * @param [in] interfaceContextPtr A pointer to a concrete PARCSigner instance.
     */
    PARCCryptoHasher *(*GetCryptoHasher)(void *interfaceContext);

    /**
     * Compute the signature of the given PARCCryptoHash.
     *
     * Equivalent of (for rsa/sha256)
     *    openssl rsautl -sign -inkey test_rsa_key.pem -in infile_digest -out infile.sig
     *
     * @param [in] interfaceContextPtr A pointer to a concrete PARCSigner instance.
     * @param [in] hashToSign The output of the given digest to sign
     *
     * @return A pointer to a PARCSignature instance that must be released via parcSignature_Release()
     */
    PARCSignature *(*SignDigest)(void *interfaceContext, const PARCCryptoHash * parcDigest);

    /**
     * Release a previously acquired reference to the specified instance,
     * decrementing the reference count for the instance.
     *
     * On the last release, it destroys the signing context and the underlying implementation
     *
     * @param [in out] interfaceContextPtr A pointer to a concrete `PARCSigner` pointer instance to be destroyed
     */
    void (*Destroy)(struct ccnx_signer_interface **interfaceContextPtr);

    /**
     * Return the PARSigningAlgorithm used for signing with the given `PARCSigner`
     *
     * @param [in] interfaceContext A pointer to a concrete PARCSigner instance.
     *
     * @return A PARSigningAlgorithm value.
     */
    PARCSigningAlgorithm (*GetSigningAlgorithm)(void *interfaceContext);

    /**
     * Return the digest algorithm used by the Signer
     *
     * @param [in] interfaceContext A pointer to a concrete PARCSigner instance.
     *
     * @return A PARCCryptoHashType value.
     */
    PARCCryptoHashType (*GetCryptoHashType)(void *interfaceContext);
} PARCSigningInterface;

/**
 * Assert that an instance of `PARCSigner` is valid.
 *
 * If the instance is not valid, terminate via {@link trapIllegalValue}
 *
 * Valid means the internal state of the type is consistent with its
 * required current or future behaviour.
 * This may include the validation of internal instances of types.
 *
 * @param [in] signer A pointer to a PARCSigner instance.
 *
 * Example
 * @code
 * {
 *      ...
 *
 *      PARCSigner *signer = parcSigner_Create(parcPublicKeySignerPkcs12Store_Open("filename", "password", PARC_HASH_SHA256));
 *      parcSigner_AssertValid(signer);
 *
 *      ...
 * }
 * @endcode
 */
void parcSigner_AssertValid(const PARCSigner *signer);

/**
 * Create a signing context based on a concrete implementation.
 *
 * @param [in] interfaceContext A concrete implementation of a `PARCSigner`
 *
 * @return NULL A `PARCSigner` could not be allocated
 * @return PARCSigner A new `PARCSigner` instance derived from the specified concrete signer context.
 *
 * Example:
 * @code
 * {
 *     PARCSigner *s = parcSigner_Create(parcPublicKeySignerPkcs12Store_Open("filename", "password", PARC_HASH_SHA256));
 * }
 * @endcode
 */
PARCSigner *parcSigner_Create(PARCSigningInterface *interfaceContext);

/**
 * Increase the number of references to the given `PARCSigner` instance.
 *
 * A new instance is not created,
 * only that the given instance's reference count is incremented.
 * Discard the acquired reference by invoking `parcSigner_Release()`.
 *
 * @param [in] signer A pointer to a `PARCSigner` instance.
 *
 * @return NULL An error occurred.
 * @return non-NULL A pointer to a PARCSigner instance.
 *
 * Example:
 * @code
 * {
 *      ...
 *
 *      PARCSigner *signer = parcSigner_Create(parcPublicKeySignerPkcs12Store_Open("filename", "password", PARC_HASH_SHA256));
 *      parcSigner_AssertValid(signer);
 *      PARCSigner *handle = parcSigner_Acquire(signer);
 *      // use the handle instance as needed
 *
 *      ...
 * }
 * @endcode
 */
PARCSigner *parcSigner_Acquire(const PARCSigner *signer);

/**
 * Release a previously acquired reference to the specified instance,
 * decrementing the reference count for the instance.
 *
 * The pointer to the instance is set to NULL as a side-effect of this function.
 *
 * If the invocation causes the last reference to the instance to be released,
 * the instance is deallocated and the instance's implementation will perform
 * additional cleanup and release other privately held references.
 *
 * The contents of the dealloced memory used for the PARC object are undefined.
 * Do not reference the object after the last release.
 *
 * @param [in,out] signerPtr A pointer to a pointer to the instance to release.
 *
 * Example:
 * @code
 * {
 *     PARCSigner *x = parcSigner_Create(interfaceContext);
 *
 *     parcSigner_Release(&x);
 * }
 * @endcode
 */
void parcSigner_Release(PARCSigner **signerPtr);

/**
 * Release a previously acquired reference to the specified instance,
 * decrementing the reference count for the instance.
 *
 * On the last release, it destroys the signing context and the underlying implementation
 *
 * @param [in, out] signingPtr A pointer to the `PARCSigner` pointer instance to be destroyed
 *
 * Example:
 * @code
 * @code
 * {
 *      ...
 *
 *      PARCSigner *signer = parcSigner_Create(parcPublicKeySignerPkcs12Store_Open("filename", "password", PARC_HASH_SHA256));
 *      parcSigner_Release(signer);
 *
 *      ...
 * }
 * @endcode
 */
void parcSigner_Release(PARCSigner **signingPtr);

/**
 * The hash of the signer's public key (or secret key for HMAC).
 *
 * Try using `parcSigner_CreateKeyId` for a sinterfaceer interface.
 * You must destroy the returned PARCCryptoHash.
 * For public key, its the SHA256 digest of the public key.
 * For HMAC, its the SHA256 digest of the secret key.
 *
 * Equivalent of (for rsa/sha256):
 *    openssl rsa -in test_rsa_key.pem -outform DER -pubout -out test_rsa_pub.der
 *    openssl sha256 -out test_rsa_pub_sha256.bin -sha256 -binary < test_rsa_pub.der
 *
 * @param [in] signer A pointer to a PARCSigner instance.
 * @return A PARCCryptoHash value.
 *
 * Example:
 * @code
 * {
 *      PARCSigner *signer = parcSigner_Create(parcPublicKeySignerPkcs12Store_Open("filename", "password", PARC_HASH_SHA256));
 *
 *      PARCCryptoHash *verifierKeyId = parcSigner_GetVerifierKeyDigest(signer);
 * }
 * @endcode
 */
PARCCryptoHash *parcSigner_GetVerifierKeyDigest(const PARCSigner *signer);

/**
 * Create a PARCKeyId instance from the given pinter to a `PARCSigner` instance.
 *
 * @param [in] signer A pointer to a PARCSigner instance.
 *
 * @return A pointer to a created PARCKeyId instance that must be released via `parcKeyId_Release()`
 *
 * Example:
 * @code
 * {
 *      PARCSigner *signer = parcSigner_Create(parcPublicKeySignerPkcs12Store_Open("filename", "password", PARC_HASH_SHA256));
 *
 *      PARCKeyId *keyId = parcSigner_CreateKeyId(signer);
 *
 *      parcKeyId_Release(&keyId);
 * }
 * @endcode
 * @see parcKeyId_Release
 */
PARCKeyId *parcSigner_CreateKeyId(const PARCSigner *signer);

/**
 * Returns a copy of the the certificate digest.
 * Returns NULL for symmetric keystores.
 *
 * Equivalent of (for rsa/sha256):
 *    openssl x509 -outform DER -out test_rsa_crt.der -in test_rsa.crt
 *    openssl sha256 -out test_rsa_crt_sha256.bin -sha256 -binary < test_rsa_crt.der
 * Which is also the same as (but not in der format)
 *    openssl x509 -in test_rsa.crt -fingerprint -sha256
 *
 * @param [in] signer A pointer to a PARCSigner instance.
 *
 * @return A `PARCCryptoHash` instance which internally contains a hash digest of the certificate used by the signer.
 *
 * Example:
 * @code
 * {
 *      PARCSigner *signer = parcSigner_Create(parcPublicKeySignerPkcs12Store_Open("filename", "password", PARC_HASH_SHA256));
 *
 *      PARCCryptoHash *certificateHash = parcSigner_GetCertificateDigest(signer);
 * }
 * @endcode
 */
PARCCryptoHash *parcSigner_GetCertificateDigest(PARCSigner *signer);

/**
 * Returns a copy of the DER encoded certificate.
 * Returns NULL for symmetric keystores.
 *
 * Equivalent of:
 *   openssl x509 -outform DER -out test_rsa_crt.der -in test_rsa.crt
 *
 * @param [in] signer A pointer to a PARCSigner instance.
 *
 * @return A pointer to a PARCBuffer containing the encoded certificate.
 *
 * Example:
 * @code
 * {
 *      PARCSigner *signer = parcSigner_Create(parcPublicKeySignerPkcs12Store_Open("filename", "password", PARC_HASH_SHA256));
 *
 *      PARCBuffer *encodedCertificate = parcSigner_GetDEREncodedCertificate(signer);
 * }
 * @endcode
 */
PARCBuffer *parcSigner_GetDEREncodedCertificate(PARCSigner *signer);

/**
 * Returns a copy of the encoded public key in Distinguished Encoding Rules (DER) form.
 *
 * Equivalent of (for rsa/sha256):
 *   `openssl rsa -in test_rsa_key.pem -outform DER -pubout -out test_rsa_pub.der`
 *
 * @param [in] signer A pointer to a PARCSigner instance.
 *
 * @return A pointer to a PARCBuffer containing the encoded public key.
 *
 * Example:
 * @code
 * {
 *      PARCSigner *signer = parcSigner_Create(parcPublicKeySignerPkcs12Store_Open("filename", "password", PARC_HASH_SHA256));
 *
 *      PARCBuffer *encodedKey = parcSigner_GetDEREncodedPublicKey(signer);
 * }
 * @endcode
 */
PARCBuffer *parcSigner_GetDEREncodedPublicKey(const PARCSigner *signer);

/**
 * Get the DER encoded public key and keyid wrapped in a `PARCKey` instance.
 *
 * @param [in] signer A pointer to a PARCSigner instance.
 *
 * @return A newly allocated `PARCKey` instance containing the public key used to verify signatures computed by this signer.
 *
 * Example:
 * @code
 * {
 *      PARCSigner *signer = parcSigner_Create(parcPublicKeySignerPkcs12Store_Open("filename", "password", PARC_HASH_SHA256));
 *
 *      PARCKey *publicKey = parcSigner_CreatePublicKey(signer);
 * }
 * @endcode
 */
PARCKey *parcSigner_CreatePublicKey(PARCSigner *signer);

/**
 * Returns a the hasher to use for the signature.  This is important for
 * symmetric key HMAC to use this hasher, not one from PARCCryptoHasher.
 *
 * DO NOT DESTROY THIS HASHER!  Just call _Init, _Update, and _Finalize.
 *
 * The hash type will be based on how the underlying implementation was initialized
 *
 * Equivalent of (for rsa/sha256)
 *    openssl rsautl -sign -inkey test_rsa_key.pem -in infile_digest -out infile.sig
 *
 * @param [in] signer A pointer to a PARCSigner instance.
 *
 * Example:
 * @code
 * {
 *      PARCSigner *signer = parcSigner_Create(parcPublicKeySignerPkcs12Store_Open("filename", "password", PARC_HASH_SHA256));
 *
 *      PARCCryptoHasher hasher = parcSigner_GetCryptoHasher(signer);
 * }
 * @endcode
 */
PARCCryptoHasher *parcSigner_GetCryptoHasher(const PARCSigner *signer);

/**
 * Compute the signature of the given PARCCryptoHash.
 *
 * Equivalent of (for rsa/sha256)
 *    openssl rsautl -sign -inkey test_rsa_key.pem -in infile_digest -out infile.sig
 *
 * @param [in] signer A pointer to a PARCSigner instance.
 * @param [in] hashToSign The output of the given digest
 *
 * @return A pointer to a PARCSignature instance that must be released via parcSignature_Release()
 *
 * Example:
 * @code
 * {
 *     PARCSigner *signer = parcSigner_Create(parcPublicKeySignerPkcs12Store_Open("filename", "password", PARC_HASH_SHA256));
 *
 *     PARCCryptoHasher *hasher = parcSigner_GetCryptoHasher(signer);
 *     parcCryptoHasher_Init(hasher);
 *     parcCryptoHasher_Update_Bytes(hasher, &block->memory[relativePosition], length);
 *     PARCCryptoHash *hashToSign = parcCryptoHasher_Finalize(hasher);
 *
 *     PARCSignature signature = parcSigner_SignDigest(signer, hashToSign);
 * }
 * @endcode
 */
PARCSignature *parcSigner_SignDigest(const PARCSigner *signer, const PARCCryptoHash *hashToSign);

/**
 * Return the PARSigningAlgorithm used for signing with the given `PARCSigner`
 *
 * @param [in] signer A pointer to a PARCSigner instance.
 *
 * @return A PARSigningAlgorithm value.
 *
 * Example:
 * @code
 * {
 *      PARCSigner *signer = parcSigner_Create(parcPublicKeySignerPkcs12Store_Open("filename", "password", PARC_HASH_SHA256));
 *
 *      PARCSigningAlgorithm suite = parcSigner_GetSigningAlgorithm(signer);
 * }
 * @endcode
 */
PARCSigningAlgorithm parcSigner_GetSigningAlgorithm(PARCSigner *signer);

/**
 * Return the digest algorithm used by the Signer
 *
 * @param [in] signer A pointer to a PARCSigner instance.
 *
 * @return A PARCCryptoHashType value.
 *
 * Example:
 * @code
 * {
 *      PARCSigner *signer = parcSigner_Create(parcPublicKeySignerPkcs12Store_Open("filename", "password", PARC_HASH_SHA256));
 *
 *      PARCCryptoHashType suite = parcSigner_GetCryptoHashType(signer);
 * }
 * @endcode
 */
PARCCryptoHashType parcSigner_GetCryptoHashType(const PARCSigner *signer);

/**
 * Given a PARCSigner instance, return the corresponding PARCCryptoSuite value.
 *
 * @param [in] signer A pointer to a `PARCSigner` instance.
 *
 * @return A PARCCryptoSuite value.
 *
 * Example:
 * @code
 * {
 *      PARCSigner *signer = parcSigner_Create(parcPublicKeySignerPkcs12Store_Open("filename", "password", PARC_HASH_SHA256));
 *
 *      PARCCryptoSuite suite = parcSigner_GetCryptoSuite(signer);
 * }
 * @endcode
 */
PARCCryptoSuite parcSigner_GetCryptoSuite(const PARCSigner *signer);
#endif // libparc_parc_Signer_h
