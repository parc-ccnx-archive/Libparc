/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @file parc_CertificateFactory.h
 * @ingroup security
 * @brief A factory to build certificates.
 *
 * @author Christopher Wood, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_CertificateFactory_h
#define libparc_parc_CertificateFactory_h

#include <parc/security/parc_Certificate.h>
#include <parc/security/parc_CertificateType.h>
#include <parc/security/parc_ContainerEncoding.h>

struct parc_certificate_factory;
typedef struct parc_certificate_factory PARCCertificateFactory;

/**
 * Create a `PARCCertificateFactory` to build certificates with the specified type and encoding.
 *
 * @param [in] type The `PARCCertificateType` of certificates to construct.
 * @param [in] encoding The `PARCContainerEncoding` of certificates to construct.
 *
 * @return NULL The memory could not be allocated.
 * @return non-NULL A newly allocated `PARCCertificateFactory`.
 *
 * Example:
 * @code
 * {
 *     PARCCertificateFactory *factory = parcCertificateFactory_Create(PARCCertificateType_X509, PARCContainerEncoding_PEM);
 * }
 * @endcode
 */
PARCCertificateFactory *parcCertificateFactory_Create(PARCCertificateType type, PARCContainerEncoding encoding);

/**
 * Create a `PARCCertificate` from the specified filename and password.
 *
 * @param [in] factory The `PARCCertificateFactory` instance used to build the certificate.
 * @param [in] filename A nul-terminated path to the certificate file.
 * @param [in] password A nul-terminated password.
 *
 * @return NULL The memory could not be allocated.
 * @return non-NULL A newly allocated `PARCCertificate`.
 *
 * Example:
 * @code
 * {
 *     char *pathToCertificate = "file.pem";
 *     PARCCertificateFactory *factory = parcCertificateFactory_Create(PARCCertificateType_X509, PARCContainerEncoding_PEM);
 *     PARCCertificate *certificate = parcCertificateFactory_CreateCertificateFromFile(factory, pathToCertificate, NULL);
 * }
 * @endcode
 */
PARCCertificate *parcCertificateFactory_CreateCertificateFromFile(PARCCertificateFactory *factory, char *filename, char *password);

/**
 * Create a `PARCCertificate` from the specified `PARCBuffer`.
 *
 * @param [in] factory The `PARCCertificateFactory` instance used to build the certificate.
 * @param [in] buffer A `PARCBuffer` encoding a `PARCCertificate` instance.
 *
 * @return NULL The memory could not be allocated.
 * @return non-NULL A newly allocated `PARCCertificate`.
 *
 * Example:
 * @code
 * {
 *     PARCBuffer *buffer = ...;
 *     PARCCertificateFactory *factory = parcCertificateFactory_Create(PARCCertificateType_X509, PARCContainerEncoding_DER);
 *     PARCCertificate *certificate = parcCertificateFactory_CreateCertificateFromBuffer(factory, buffer);
 * }
 * @endcode
 */
PARCCertificate *parcCertificateFactory_CreateCertificateFromBuffer(PARCCertificateFactory *factory, PARCBuffer *buffer);

/**
 * Create a self-signed `PARCCertificate` using the specified parameters and return
 * the corresponding private key.
 *
 * Note: this is equivalent to the following OpenSSL command:
 *   XXXXYYYYZZZZ
 *   TODO TODO TODO
 *
 * @param [in] factory The `PARCCertificateFactory` instance used to build the certificate.
 * @param [in, out] privateKey A pointer to a `PARCBuffer` pointer where the new certificate private key will be stored.
 * @param [in] subjectName The name of the certificate subject.
 * @param [in] keyLength The length of the public key to be derived.
 * @param [in] validityDays The validity period.
 *
 * @return NULL The memory could not be allocated.
 * @return non-NULL A newly allocated `PARCCertificate`.
 *
 * Example:
 * @code
 * {
 *     PARCBuffer *buffer = ...;
 *     PARCCertificateFactory *factory = parcCertificateFactory_Create(PARCCertificateType_X509, PARCContainerEncoding_DER);
 *     PARCCertificate *certificate = parcCertificateFactory_CreateCertificateFromBuffer(factory, buffer);
 * }
 * @endcode
 */
PARCCertificate *parcCertificateFactory_CreateSelfSignedCertificate(PARCCertificateFactory *factort, PARCBuffer **privateKey, char *subjectName, size_t keyLength, size_t valdityDays);

/**
 * Increase the number of references to a `PARCCertificateFactory` instance.
 *
 * Note that a new `PARCCertificateFactory` is not created,
 * only that the given `PARCCertificateFactory` reference count is incremented.
 * Discard the reference by invoking {@link parcCertificateFactory_Release}.
 *
 * @param [in] certificate A pointer to the original instance.
 * @return The value of the input parameter @p instance.
 *
 * Example:
 * @code
 * {
 *     PARCCertificateFactory *x = parcCertificateFactory_CreateFromFile(...);
 *     PARCCertificateFactory *x2 = parcCertificateFactory_Acquire(x);
 *
 *     parcCertificateFactory_Release(&x);
 *     parcCertificateFactory_Release(&x2);
 * }
 * @endcode
 *
 * @see {@link parcCertificateFactory_Release}
 */
PARCCertificateFactory *parcCertificateFactory_Acquire(const PARCCertificateFactory *factory);

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
 * @param [in,out] factoryP A pointer to a pointer to the instance to release.
 *
 * Example:
 * @code
 * {
 *     PARCCertificateFactory *x = parcCertificateFactory_CreateFromFile(...);
 *
 *     parcCertificateFactory_Release(&x);
 * }
 * @endcode
 */
void parcCertificateFactory_Release(PARCCertificateFactory **factoryP);
#endif // libparc_parc_CertificateFactory_h
