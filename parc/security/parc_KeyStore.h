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
 * @file parc_KeyStore.h
 * @ingroup security
 * @brief A container of Key Store information.
 *
 * A Key Store is a repository of key information typically accessable
 * through some authentication and authorisation system.
 * The PARCKeyStore type contains the necessary information to successfully
 * gain access to a Key Store.
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_KeyStore_h
#define libparc_parc_KeyStore_h

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_Buffer.h>
#include <parc/security/parc_CryptoHash.h>

struct parc_key_store;
typedef struct parc_key_store PARCKeyStore;

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
 * @param [in] interfaceContext A pointer to a concrete PARCKeyStore instance.
 *
 * @return A PARCCryptoHash value.
 */
typedef PARCCryptoHash *(PARCKeyStoreGetVerifierKeyDigest)(const void *interfaceContext);

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
 * @param [in] interfaceContext A pointer to a concrete PARCKeyStore instance.
 *
 * @return A `PARCCryptoHash` instance which internally contains a hash digest of the certificate used by the signer.
 */
typedef PARCCryptoHash *(PARCKeyStoreGetCertificateDigest)(const void *interfaceContext);

/**
 * Returns a copy of the DER encoded certificate.
 * Returns NULL for symmetric keystores.
 *
 * Equivalent of:
 *   openssl x509 -outform DER -out test_rsa_crt.der -in test_rsa.crt
 *
 * @param [in] interfaceContextPtr A pointer to a concrete PARCKeyStore instance.
 *
 * @return A pointer to a PARCBuffer containing the encoded certificate.
 */
typedef PARCBuffer *(PARCKeyStoreGetDEREncodedCertificate)(const void *interfaceContext);

/**
 * Returns a copy of the encoded public key in Distinguished Encoding Rules (DER) form.
 *
 * Equivalent of (for rsa/sha256):
 *   `openssl rsa -in test_rsa_key.pem -outform DER -pubout -out test_rsa_pub.der`
 *
 * @param [in] interfaceContextPtr A pointer to a concrete PARCKeyStore instance.
 *
 * @return A pointer to a PARCBuffer containing the encoded public key.
 */
typedef PARCBuffer *(PARCKeyStoreGetDEREncodedPublicKey)(const void *interfaceContext);

/**
 * Returns a copy of the encoded private key in Distinguished Encoding Rules (DER) form.
 *
 * Equivalent of (for rsa/sha256):
 *   `openssl rsa -in test_rsa_key.pem -outform DER -out test_rsa.der`
 *
 * @param [in] interfaceContextPtr A pointer to a concrete PARCKeyStore instance.
 *
 * @return A pointer to a PARCBuffer containing the encoded private key.
 */
typedef PARCBuffer *(PARCKeyStoreGetDEREncodedPrivateKey)(const void *interfaceContext);


typedef struct parc_keystore_interface {
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
     * @param [in] interfaceContext A pointer to a concrete PARCKeyStore instance.
     *
     * @return A PARCCryptoHash value.
     */
    PARCKeyStoreGetVerifierKeyDigest *getVerifierKeyDigest;

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
     * @param [in] interfaceContext A pointer to a concrete PARCKeyStore instance.
     *
     * @return A `PARCCryptoHash` instance which internally contains a hash digest of the certificate used by the signer.
     */
    PARCKeyStoreGetCertificateDigest *getCertificateDigest;

    /**
     * Returns a copy of the DER encoded certificate.
     * Returns NULL for symmetric keystores.
     *
     * Equivalent of:
     *   openssl x509 -outform DER -out test_rsa_crt.der -in test_rsa.crt
     *
     * @param [in] interfaceContextPtr A pointer to a concrete PARCKeyStore instance.
     *
     * @return A pointer to a PARCBuffer containing the encoded certificate.
     */
    PARCKeyStoreGetDEREncodedCertificate *getDEREncodedCertificate;

    /**
     * Returns a copy of the encoded public key in Distinguished Encoding Rules (DER) form.
     *
     * Equivalent of (for rsa/sha256):
     *   `openssl rsa -in test_rsa_key.pem -outform DER -pubout -out test_rsa_pub.der`
     *
     * @param [in] interfaceContextPtr A pointer to a concrete PARCKeyStore instance.
     *
     * @return A pointer to a PARCBuffer containing the encoded public key.
     */
    PARCKeyStoreGetDEREncodedPublicKey *getDEREncodedPublicKey;

    /**
     * Returns a copy of the encoded private key in Distinguished Encoding Rules (DER) form.
     *
     * Equivalent of (for rsa/sha256):
     *   `openssl rsa -in test_rsa_key.pem -outform DER -out test_rsa.der`
     *
     * @param [in] interfaceContextPtr A pointer to a concrete PARCKeyStore instance.
     *
     * @return A pointer to a PARCBuffer containing the encoded private key.
     */
    PARCKeyStoreGetDEREncodedPrivateKey *getDEREncodedPrivateKey;
} PARCKeyStoreInterface;

/**
 * Create a `PARCKeyStore` from a filename.
 *
 * @param [in] instance A concrete instance of a `PARCKeyStore.`
 * @param [in] interface The interface for the `PARCKeyStore.`
 *
 * @return A pointer to the new `PARCKeyStore`
 *
 * Example:
 * @code
 * {
 * }
 * @endcode
 */
PARCKeyStore *parcKeyStore_Create(PARCObject *instance, const PARCKeyStoreInterface *interface);

/**
 * Increase the number of references to an instance of this object.
 *
 * Note that new instance is not created, only that the given instance's reference count
 * is incremented. Discard the reference by invoking `parcKeyStore_Release()`.
 *
 * @param [in] keyStore A pointer to the original instance.
 *
 * @return The value of the input parameter @p instance.
 *
 * Example:
 * @code
 * {
 *     ...
 *
 *     PARCKeyStore *keyStore = parcKeyStore_Acquire(keyStoreInstance);
 *
 *     parcKeyStore_Release(&keyStore);
 * }
 * @endcode
 *
 * @see parcKey_Release
 */
PARCKeyStore *parcKeyStore_Acquire(const PARCKeyStore *keyStore);

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
 * @param [in] keyStorePtr A pointer to a pointer to the instance to release.
 *
 * Example:
 * @code
 * {
 *     ...
 *
 *     PARCKeyStore *keyStore = parcKeyStore_Acquire(keyStoreInstance);
 *
 *     parcKeyStore_Release(&keyStore);
 * }
 * @endcode
 */
void parcKeyStore_Release(PARCKeyStore **keyStorePtr);

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
 * @param [in] interfaceContext A pointer to a concrete PARCKeyStore instance.
 *
 * @return A PARCCryptoHash value.
 *
 * Example:
 * @code
 * {
 * }
 * @endcode
 */
PARCCryptoHash *parcKeyStore_GetVerifierKeyDigest(const PARCKeyStore *interfaceContext);

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
 * @param [in] interfaceContext A pointer to a concrete PARCKeyStore instance.
 *
 * @return A `PARCCryptoHash` instance which internally contains a hash digest of the certificate used by the signer.
 *
 * Example:
 * @code
 * {
 * }
 * @endcode
 */
PARCCryptoHash *parcKeyStore_GetCertificateDigest(const PARCKeyStore *interfaceContext);

/**
 * Returns a copy of the DER encoded certificate.
 * Returns NULL for symmetric keystores.
 *
 * Equivalent of:
 *   openssl x509 -outform DER -out test_rsa_crt.der -in test_rsa.crt
 *
 * @param [in] interfaceContext A pointer to a concrete PARCKeyStore instance.
 *
 * @return A pointer to a PARCBuffer containing the encoded certificate.
 *
 * Example:
 * @code
 * {
 * }
 * @endcode
 */
PARCBuffer *parcKeyStore_GetDEREncodedCertificate(const PARCKeyStore *interfaceContext);

/**
 * Returns a copy of the encoded public key in Distinguished Encoding Rules (DER) form.
 *
 * Equivalent of (for rsa/sha256):
 *   `openssl rsa -in test_rsa_key.pem -outform DER -pubout -out test_rsa_pub.der`
 *
 * @param [in] interfaceContext A pointer to a concrete PARCKeyStore instance.
 *
 * @return A pointer to a PARCBuffer containing the encoded public key.
 *
 * Example:
 * @code
 * {
 * }
 * @endcode
 */
PARCBuffer *parcKeyStore_GetDEREncodedPublicKey(const PARCKeyStore *interfaceContext);

/**
 * Returns a copy of the encoded private key in Distinguished Encoding Rules (DER) form.
 *
 * Equivalent of (for rsa/sha256):
 *   `openssl rsa -in test_rsa_key.pem -outform DER -out test_rsa.der`
 *
 * @param [in] interfaceContext A pointer to a concrete PARCKeyStore instance.
 *
 * @return A pointer to a PARCBuffer containing the encoded private key.
 *
 * Example:
 * @code
 * {
 * }
 * @endcode
 */
PARCBuffer *parcKeyStore_GetDEREncodedPrivateKey(const PARCKeyStore *interfaceContext);
#endif // libparc_parc_KeyStore_h
