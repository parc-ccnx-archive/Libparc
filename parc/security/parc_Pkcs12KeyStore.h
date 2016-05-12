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
 * @file parc_Pkcs12KeyStore.h
 * @ingroup security
 * @brief A concrete implementation of PARCKeyStore based on a PCKS12 keystore.
 *
 * @author Marc Mosko, Christopher A. Wood, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_Pkcs12KeyStore_h
#define libparc_parc_Pkcs12KeyStore_h

#include <parc/security/parc_KeyStore.h>
#include <parc/security/parc_Signer.h>

struct parc_pkcs12_keystore;
typedef struct parc_pkcs12_keystore PARCPkcs12KeyStore;

extern PARCKeyStoreInterface *PARCPkcs12KeyStoreAsKeyStore;

/**
 * Increase the number of references to a `PARCPublicKeySigner` instance.
 *
 * Note that new `PARCPublicKeySigner` is not created,
 * only that the given `PARCPublicKeySigner` reference count is incremented.
 * Discard the reference by invoking `parcPublicKeySigner_Release`.
 *
 * @param [in] instance A pointer to a valid PARCPublicKeySigner instance.
 *
 * @return The same value as @p instance.
 *
 * Example:
 * @code
 * {
 *     parcPkcs12KeyStore_CreateFile(...);
 *     PARCPkcs12KeyStore *a = parcPkcs12Store_Open(...)
 *
 *     PARCPkcs12KeyStore *b = parcPkcs12KeyStore_Acquire();

 *     parcPkcs12KeyStore_Release(&a);
 *     parcPkcs12KeyStore_Release(&b);
 * }
 * @endcode
 */
PARCPkcs12KeyStore *parcPkcs12KeyStore_Acquire(const PARCPkcs12KeyStore *instance);

/**
 * Release a previously acquired reference to the given `PARCPkcs12KeyStore` instance,
 * decrementing the reference count for the instance.
 *
 * The pointer to the instance is set to NULL as a side-effect of this function.
 *
 * If the invocation causes the last reference to the instance to be released,
 * the instance is deallocated and the instance's implementation will perform
 * additional cleanup and release other privately held references.
 *
 * @param [in,out] instancePtr A pointer to a pointer to the instance to release.
 *
 * Example:
 * @code
 * {
 *     PARCPkcs12KeyStore *a = parcPkcs12Store_Open(...);
 *
 *     parcPkcs12KeyStore_Release(&a);
 * }
 * @endcode
 */
void parcPkcs12KeyStore_Release(PARCPkcs12KeyStore **instancePtr);

/**
 * Creates a PKCS12 keystore identity with a self-signed certifiate. Note that this call currently
 * aborts if keystore i/o access fails, behavior that may change in the future.
 *
 * @param [in] filename The name of the PKCS12 file.
 * @param [in] password The password to open the PKCS12 file.
 * @param [in] subjectName The certificate subject associated with the PKCS12 file.
 * @param [in] keyLength The length of the public key associated with the PKCS12 file.
 * @param [in] validityDays The validity (in days) of the certificate associated with the PKCS12 file.
 *
 * @return true on success, false if certificate creation fails, and will abort if keystore i/o fails.
 *
 * Example:
 * @code
 * {
 *     const char *filename = "/tmp/ccnxFileKeyStore_Pkcs12Open_CreateAndOpen.p12";
 *     const char *password = "12345";
 *     const char *subject  = "alice";
 *     bool result = parcPkcs12KeyStore_CreateFile(filename, password, subject, 1024, 32);
 * }
 * @endcode
 */
bool parcPkcs12KeyStore_CreateFile(const char *filename, const char *password, const char *subjectName,
                                   unsigned keyLength, unsigned validityDays);

/**
 * Create a `PARCPkcs12KeyStore` instance.
 *
 * @param [in] filename The name of a file containing the PKCS12 keystore.
 * @param [in] password The password to decrypt/unlock the determines how the signer digests data. Supports PARC_HASH_SHA256 and PARC_HASH_SHA512.
 * @param [in] hashType Determines how the signer digests data. Possible values are PARC_HASH_SHA256 and PARC_HASH_SHA512.
 *
 * @return A `PARCPkcs12KeyStore` instance using the public/private key pair contained within the PKCS12 file.
 *
 * Example:
 * @code
 * {
 *     const char *filename = "/tmp/ccnxFileKeyStore_Pkcs12Open_CreateAndOpen.p12";
 *     const char *password = "12345";
 *     const char *subject  = "alice";
 *     bool result = parcPkcs12KeyStore_CreateFile(filename, password, subject, 1024, 32);
 *
 *     ...
 *
 *     PARCSigningInterface *interface = parcPublicKeySignerPkcs12Store_Open(filename, password, PARC_HASH_SHA256);
 *
 *     ...
 * }
 * @endcode
 */
PARCPkcs12KeyStore *parcPkcs12KeyStore_Open(const char *filename, const char *password, PARCCryptoHashType hashType);

#endif // libparc_parc_PublicKeySignerPkcs12Store_h
