/*
 * Copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @file parc_Certificate.h
 * @ingroup security
 * @brief The API for a generic certificate.
 *
 * @author Christopher A. Wood, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */

#ifndef libparc_parc_Certificate_h
#define libparc_parc_Certificate_h

#include <parc/algol/parc_Buffer.h>
#include <parc/security/parc_CertificateType.h>
#include <parc/security/parc_ContainerEncoding.h>
#include <parc/security/parc_CryptoHasher.h>
#include <parc/security/parc_Signature.h>
#include <parc/security/parc_CryptoHashType.h>
#include <parc/security/parc_Key.h>

struct parc_certificate;
/**
 * @typedef PARCCertificate
 * @brief The structure for PARCCertificate
 */
typedef struct parc_certificate PARCCertificate;

typedef struct parc_certificate_interface {
    /**
     * The hash of the certificate's public key.
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
     * @param [in] certificate A pointer to a concrete `PARCCertificate` instance.
     *
     * @return A `PARCCryptoHash` instance.
     */
    PARCCryptoHash *(*GetPublicKeyDigest)(void *certificate);

    /**
     * Returns a copy of the the certificate digest.
     *
     * Returns NULL for symmetric keystores.
     *
     * Equivalent of (for rsa/sha256):
     *    openssl x509 -outform DER -out test_rsa_crt.der -in test_rsa.crt
     *    openssl sha256 -out test_rsa_crt_sha256.bin -sha256 -binary < test_rsa_crt.der
     * Which is also the same as (but not in der format)
     *    openssl x509 -in test_rsa.crt -fingerprint -sha256
     *
     * @param [in] certificate A pointer to a concrete `PARCCertificate` instance.
     *
     * @return A `PARCCryptoHash` instance which internally contains a hash digest of the certificate.
     */
    PARCCryptoHash *(*GetCertificateDigest)(void *certificate);

    /**
     * Returns a copy of the DER encoded certificate.
     *
     * Returns NULL for symmetric keystores.
     *
     * Equivalent of:
     *   openssl x509 -outform DER -out test_rsa_crt.der -in test_rsa.crt
     *
     * @param [in] certificate A pointer to a concrete `PARCCertificate` instance.
     *
     * @return A pointer to a `PARCBuffer` containing the encoded certificate.
     */
    PARCBuffer *(*GetDEREncodedCertificate)(void *certificate);

    /**
     * Returns a copy of the encoded public key in DER form.
     *
     * Equivalent of (for rsa/sha256):
     *   `openssl rsa -in test_rsa_key.pem -outform DER -pubout -out test_rsa_pub.der`
     *
     * @param [in] certificate A pointer to a concrete `PARCCertificate` instance.
     *
     * @return A pointer to a `PARCBuffer` containing the encoded public key.
     */
    PARCBuffer *(*GetDEREncodedPublicKey)(void *certificate);

    /**
     * Returns the `PARCCertificateType` of this certificate, i.e., PEM, DER, PKCS12.
     *
     * @param [in] certificate A pointer to a concrete `PARCCertificate` instance.
     *
     * @return The `PARCCertificateType` associated with this certificate.
     */
    PARCCertificateType (*GetCertificateType)(const void *certificate);

    /**
     * Returns the `PARCContainerEncoding` of this certificate, e.g., X509.
     *
     * @param [in] certificate A pointer to a concrete `PARCCertificate` instance.
     *
     * @return The `PARCContainerEncoding` associated with this certificate.
     */
    PARCContainerEncoding (*GetContainerEncoding)(const void *certificate);
} PARCCertificateInterface;

/**
 * Create a generic `PARCCertificate` instance from a concrete `PARCCertificate` instance.
 *
 * NOTE: This function should not be used directly. Construct certificates using the
 * `PARCCertificateFactory` instead.
 *
 * @param [in] impl A pointer to a concrete `PARCCertificate` interface implementation.
 * @param [in] instance A pointer to the instance that implements this interface.
 *
 * @return NULL Memory could not be allocated.
 * @return non-NULL A newly allocated `PARCCertificate` instance containing the concrete
 *                  `PARCCertificate` instance.
 *
 * Example:
 * @code
 * {
 *     PARCX509Certificate *certificate = ...;
 *     PARCCertificate *wrapper = parcCertificate_CreateFromInstance(PARCX509CertificateInterface, certificate);
 * }
 * @endcode
 */
PARCCertificate *parcCertificate_CreateFromInstance(PARCCertificateInterface *impl, void *instance);

/**
 * Increase the number of references to a `PARCCertificate` instance.
 *
 * Note that a new `PARCCertificate` is not created,
 * only that the given `PARCCertificate` reference count is incremented.
 * Discard the reference by invoking {@link parcCertificate_Release}.
 *
 * @param [in] certificate A pointer to the original instance.
 * @return The value of the input parameter @p instance.
 *
 * Example:
 * @code
 * {
 *     PARCCertificate *x = parcCertificate_CreateFromInstance(...);
 *     PARCCertificate *x2 = parcCertificate_Acquire(x);
 *
 *     parcCertificate_Release(&x);
 *     parcCertificate_Release(&x2);
 * }
 * @endcode
 *
 * @see {@link parcCertificate_Release}
 */
PARCCertificate *parcCertificate_Acquire(const PARCCertificate *certificate);

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
 * @param [in,out] certificateP A pointer to a pointer to the instance to release.
 *
 * Example:
 * @code
 * {
 *     PARCCertificate *x = parcCertificate_Acquire(...);
 *
 *     parcCertificate_Release(&x);
 * }
 * @endcode
 *
 * @see {@link parcCertificate_Acquire}
 */
void parcCertificate_Release(PARCCertificate **certificateP);

/**
 * Returns the `PARCCertificateType` of this certificate, i.e., X509.
 *
 * @param [in] certificate A pointer to a concrete `PARCCertificate` instance.
 *
 * @return The `PARCCertificateType` associated with this certificate.
 *
 * Example:
 * @code
 * {
 *     PARCX509Certificate *certificate = parcX509Certificate_CreateFromPEMFile(filename);
 *     PARCCertificate *wrapper = parcCertificate_CreateFromInstance(PARCX509CertificateInterface, certificate);
 *
 *     PARCCertificateType type = parcCertificate_GetCertificateType(wrapper);
 *     // type == PARCCertificateType_X509
 * }
 * @endcode
 */
PARCCertificateType parcCertificate_GetCertificateType(const PARCCertificate *certificate);

/**
 * Returns the `PARCContainerEncoding` of this certificate, e.g., PEM, DER.
 *
 * @param [in] certificate A pointer to a concrete `PARCCertificate` instance.
 *
 * @return The `PARCContainerEncoding` associated with this certificate.
 *
 * Example:
 * @code
 * {
 *     PARCX509Certificate *certificate = parcX509Certificate_CreateFromPEMFile(filename);
 *     PARCCertificate *wrapper = parcCertificate_CreateFromInstance(PARCX509CertificateInterface, certificate);
 *
 *     PARCContainerEncoding encoding = parcCertificate_GetCertificateType(wrapper);
 *     // encoding == PARCCertificateType_PEM
 * }
 * @endcode
 */
PARCContainerEncoding parcCertificate_GetContainerEncoding(const PARCCertificate *certificate);

/**
 * Retrieve the SHA-256 hash digest of the certificate's public key.
 *
 * You must release the returned `PARCCryptoHash` via {@link parcCryptoHash_Release}.
 *
 * Equivalent of (for rsa/sha256):
 *    openssl rsa -in test_rsa_key.pem -outform DER -pubout -out test_rsa_pub.der
 *    openssl sha256 -out test_rsa_pub_sha256.bin -sha256 -binary < test_rsa_pub.der
 *
 * @param [in] certificate A pointer to a `PARCCertificate` instance.
 *
 * @return NULL Memory could not be allocated.
 * @return non-NULL A `PARCCryptoHash` value which internally contains a hash digest of the certificate key.
 *
 * Example:
 * @code
 * {
 *     PARCX509Certificate *certificate = parcX509Certificate_CreateFromPEMFile(filename);
 *     PARCCertificate *wrapper = parcCertificate_CreateFromInstance(PARCX509CertificateInterface, certificate);
 *
 *     PARCCryptoHash *certificateKeyDigest = parcCertificate_GetPublicKeyDigest(wrapper);
 * }
 * @endcode
 */
PARCCryptoHash *parcCertificate_GetPublicKeyDigest(const PARCCertificate *certificate);

/**
 * Get the SHA-256 digest of the certificate.
 *
 * Equivalent of (for rsa/sha256):
 *    openssl x509 -outform DER -out test_rsa_crt.der -in test_rsa.crt
 *    openssl sha256 -out test_rsa_crt_sha256.bin -sha256 -binary < test_rsa_crt.der
 * Which is also the same as (but not in der format)
 *    openssl x509 -in test_rsa.crt -fingerprint -sha256
 *
 * @param [in] certificate A pointer to a `PARCCertificate` instance.
 *
 * @return NULL Memory could not be allocated.
 * @return A `PARCCryptoHash` instance which internally contains a hash digest of the certificate.
 *
 * Example:
 * @code
 * {
 *     PARCX509Certificate *certificate = parcX509Certificate_CreateFromPEMFile(filename);
 *     PARCCertificate *wrapper = parcCertificate_CreateFromInstance(PARCX509CertificateInterface, certificate);
 *
 *     PARCCryptoHash *certificateKeyDigest = parcCertificate_GetPublicKeyDigest(wrapper);
 * }
 * @endcode
 */
PARCCryptoHash *parcCertificate_GetCertificateDigest(const PARCCertificate *certificate);

/**
 * Get a `PARCBuffer` containing the DER encoded representation of the certificate.
 *
 * Equivalent of:
 *   openssl x509 -outform DER -out test_rsa_crt.der -in test_rsa.crt
 *
 * @param [in] certificate A pointer to a `PARCCertificate` instance.
 *
 * @return NULL Memory could not be allocated.
 * @return non-NULL A pointer to a `PARCBuffer` containing the encoded certificate.
 *
 * Example:
 * @code
 * {
 *     PARCX509Certificate *certificate = parcX509Certificate_CreateFromPEMFile(filename);
 *     PARCCertificate *wrapper = parcCertificate_CreateFromInstance(PARCX509CertificateInterface, certificate);
 *
 *     PARCBuffer *certificateDER = parcCertificate_GetDEREncodedCertificate(wrapper);
 * }
 * @endcode
 */
PARCBuffer *parcCertificate_GetDEREncodedCertificate(const PARCCertificate *certificate);

/**
 * Get the certificate's public key in DER encoding in a `PARCBuffer`.
 *
 * Equivalent of (for rsa/sha256):
 *   `openssl rsa -in test_rsa_key.pem -outform DER -pubout -out test_rsa_pub.der`
 *
 * @param [in] certificate A pointer to a `PARCCertificate` instance.
 *
 * @return NULL Memory could not be allocated.
 * @return non-NULL A pointer to a `PARCBuffer` containing the encoded certificate's public key.
 *
 * Example:
 * @code
 * {
 *     PARCX509Certificate *certificate = parcX509Certificate_CreateFromPEMFile(filename);
 *     PARCCertificate *wrapper = parcCertificate_CreateFromInstance(PARCX509CertificateInterface, certificate);
 *
 *     PARCBuffer *certificateDER = parcCertificate_GetDEREncodedPublicKey(wrapper);
 * }
 * @endcode
 */
PARCBuffer *parcCertificate_GetDEREncodedPublicKey(const PARCCertificate *certificate);

/**
 * Get the `PARCKey` public key associated with this certificate.
 *
 * @param [in] certificate A pointer to a `PARCCertificate` instance.
 *
 * @return NULL Memory could not be allocated.
 * @return non-NULL A pointer to a `PARCKey` instance.
 *
 * Example:
 * @code
 * {
 *     PARCX509Certificate *certificate = parcX509Certificate_CreateFromPEMFile(filename);
 *     PARCCertificate *wrapper = parcCertificate_CreateFromInstance(PARCX509CertificateInterface, certificate);
 *
 *     PARCKey *publicKey = parcCertificate_GetPublicKey(wrapper);
 * }
 * @endcode
 */
PARCKey *parcCertificate_GetPublicKey(const PARCCertificate *certificate);
#endif // libparc_parc_Certificate_h
