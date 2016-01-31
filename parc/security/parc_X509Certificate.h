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
 * @file parc_X509Certificate.h
 * @ingroup security
 * @brief The API for a generic  certificate.
 *
 * @author Christopher A. Wood, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

#ifndef libparc_parc_X509Certificate_h
#define libparc_parc_X509Certificate_h

#include <parc/security//parc_Certificate.h>

struct parc_X509_certificate;
typedef struct parc_X509_certificate PARCX509Certificate;

extern PARCCertificateInterface *PARCX509CertificateInterface;

/**
 * Create a `PARCX509Certificate` from a PEM-encoded file.
 *
 * @param [in] filename A nul-terminated path to a certificate file.
 *
 * @return NULL The memory could not be allocated.
 * @return non-NULL A newly allocated `PARCX509Certificate`.
 *
 * Example:
 * @code
 * {
 *     char *pathToCertificate = "file.pem";
 *     PARCX509Certificate *certificate = parcX509Certificate_CreateFromPEMFile(pathToCertificate);
 * }
 * @endcode
 */
PARCX509Certificate *parcX509Certificate_CreateFromPEMFile(const char *filename);

/**
 * Create a `PARCX509Certificate` from a DER-encoded buffer.
 *
 * @param [in] buffer A `PARCBuffer` instance containing the DER-encoded certificate.
 *
 * @return NULL The memory could not be allocated.
 * @return non-NULL A newly allocated `PARCX509Certificate`.
 *
 * Example:
 * @code
 * {
 *     PARCBuffer *certificateBuffer = ...
 *     PARCX509Certificate *certificate = parcX509Certificate_CreateFromDERBuffer(certificateBuffer);
 * }
 * @endcode
 */
PARCX509Certificate *parcX509Certificate_CreateFromDERBuffer(const PARCBuffer *buffer);

/**
 * Increase the number of references to a `PARCX509Certificate` instance.
 *
 * Note that a new `PARCX509Certificate` is not created,
 * only that the given `PARCX509Certificate` reference count is incremented.
 * Discard the reference by invoking {@link parcX509Certificate_Release}.
 *
 * @param [in] certificate A pointer to the original instance.
 * @return The value of the input parameter @p instance.
 *
 * Example:
 * @code
 * {
 *     PARCX509Certificate *x = parcX509Certificate_CreateFromPEMFile(...);
 *     PARCX509Certificate *x2 = parcX509Certificate_Acquire(x);
 *
 *     parcX509Certificate_Release(&x);
 *     parcX509Certificate_Release(&x2);
 * }
 * @endcode
 *
 * @see {@link parcX509Certificate_Release}
 */
PARCX509Certificate *parcX509Certificate_Acquire(const PARCX509Certificate *certificate);

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
 *     PARCX509Certificate *x = parcX509Certificate_CreateFromPEMFile(...);
 *
 *     parcX509Certificate_Release(&x);
 * }
 * @endcode
 */
void parcX509Certificate_Release(PARCX509Certificate **certificateP);

/**
 * Retrieve the SHA-256 digest of the DER-encoded certificate.
 *
 * @param [in] certificate A pointer to `PARCX509Certificate` instance.
 *
 * @return The SHA-256 digest of the @p instance.
 *
 * Example:
 * @code
 * {
 *     PARCX509Certificate *x = parcX509Certificate_CreateFromPEMFile(...);
 *
 *     PARCCryptoHash *digest = parcX509Certificate_GetCertificateDigest(x);
 *     // use the digest
 *
 *     parcX509Certificate_Release(&x);
 * }
 * @endcode
 */
PARCCryptoHash *parcX509Certificate_GetCertificateDigest(PARCX509Certificate *certificate);

/**
 * Retrieve the SHA-256 digest of the DER-encoded public key that
 * is contained in the specified `PARCX509Certificate`.
 *
 * @param [in] certificate A pointer to `PARCX509Certificate` instance.
 *
 * @return The SHA-256 digest of the @p instance's public key.
 *
 * Example:
 * @code
 * {
 *     PARCX509Certificate *x = parcX509Certificate_CreateFromPEMFile(...);
 *
 *     PARCCryptoHash *digest = parcX509Certificate_GetCertificateKeyDigest(x);
 *     // use the digest
 *
 *     parcX509Certificate_Release(&x);
 * }
 * @endcode
 */
PARCCryptoHash *parcX509Certificate_GetCertificateKeyDigest(PARCX509Certificate *certificate);

/**
 * Retrieve the DER-encoded representation of the specified `PARCX509Certificate`.
 *
 * @param [in] certificate A pointer to `PARCX509Certificate` instance.
 *
 * @return A newly allocated `PARCBuffer` instance containing the DER-encoded form
 *         of the specified `PARCX509Certificate`.
 *
 * Example:
 * @code
 * {
 *     PARCX509Certificate *x = parcX509Certificate_CreateFromPEMFile(...);
 *
 *     PARCBuffer *certificateDER = parcX509Certificate_GetDEREncodedCertificate(x);
 *     // use the digest
 *
 *     parcX509Certificate_Release(&x);
 * }
 * @endcode
 */
PARCBuffer *parcX509Certificate_GetDEREncodedCertificate(PARCX509Certificate *certificate);

/**
 * Retrieve the DER-encoded representation of the public key contained within
 * the specified `PARCX509Certificate`.
 *
 * @param [in] certificate A pointer to `PARCX509Certificate` instance.
 *
 * @return A newly allocated `PARCBuffer` instance containing the DER-encoded form
 *         of the public key in the `PARCX509Certificate` instance.
 *
 * Example:
 * @code
 * {
 *     PARCX509Certificate *x = parcX509Certificate_CreateFromPEMFile(...);
 *
 *     PARCBuffer *publicKeyDER = parcX509Certificate_GetDEREncodedPublicKey(x);
 *     // use the digest
 *
 *     parcX509Certificate_Release(&x);
 * }
 * @endcode
 */
PARCBuffer *parcX509Certificate_GetDEREncodedPublicKey(PARCX509Certificate *certificate);
#endif // libparc_parc_X509Certificate_h
