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
 * @file parc_SymmetricKeyStore.h
 * @ingroup security
 * @brief A PARCKeyStore instance for symmetric keys that can be used to produce,
 *        for example, HMAC authenticator tags.
 *
 * The secret key is stored in a PARC proprietary file format.
 *
 * ---------------------------------------------------------------------------
 * From the Java implementation code comments:
 *
 * This is a specialized keystore for storing symmetric keys. We looked at PKCS #11 for this but decided
 * against it for now because industry doesn't seem to be standardizing around it - at least not yet, and
 * standard support for it is somewhat sketchy at this point.
 *
 * The keystore can be used for only one key at a time and is located by naming it with a suffix
 * created from the key's digest.
 *
 * Following is the formula for the KeyStore
 *
 * Let P=passphrase
 * Let PT = symmetric key to store
 * Let IV = random 16-bytes
 *
 * aesK = HMAC-SHA256(P, '\0')
 * macK = HMAC-SHA256(P, '\1')
 * AES256-CBC(IV, key, PT) - performs AES256 in CBC mode
 *
 * SK = IV || AES256-CBC(IV, aesK, PT) || HMAC-SHA256(macK, AES256-CBC(IV, aesK, PT))
 *
 * SK is the symmetric keystore ciphertext
 *
 * ASN1 encoded KeyStore = Version || Key algorithm OID || SK
 * ---------------------------------------------------------------------------
 *
 * @author Marc Mosko, Christopher A. Wood, Computing Science Laboratory, PARC
 * @copyright 2013-2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_SymmetricKeyStore_h
#define libparc_parc_SymmetricKeyStore_h

#include <parc/security/parc_Signer.h>
#include <parc/algol/parc_Buffer.h>

struct parc_symmetric_keystore;
typedef struct parc_symmetric_keystore PARCSymmetricKeyStore;

extern PARCKeyStoreInterface *PARCSymmetricKeyStoreAsKeyStore;

/**
 * Increase the number of references to a `PARCSymmetricKeyStore` instance.
 *
 * Note that new `PARCSymmetricKeyStore` is not created,
 * only that the given `PARCSymmetricKeyStore` reference count is incremented.
 * Discard the reference by invoking `parcSymmetricKeyStore_Release`.
 *
 * @param [in] instance A pointer to a valid PARCSymmetricKeyStore instance.
 *
 * @return The same value as @p instance.
 *
 * Example:
 * @code
 * {
 *     parcSymmetricKeyStore_CreateFile(...);
 *     PARCSymmetricKeyStore *a = parcSymmetricKeyStore_OpenFile(...)
 *
 *     PARCSymmetricKeyStore *b = parcSymmetricKeyStore_Acquire();

 *     parcSymmetricKeyStore_Release(&a);
 *     parcSymmetricKeyStore_Release(&b);
 * }
 * @endcode
 */
PARCSymmetricKeyStore *parcSymmetricKeyStore_Acquire(const PARCSymmetricKeyStore *instance);

/**
 * Release a previously acquired reference to the given `PARCSymmetricKeyStore` instance,
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
 *     PARCSymmetricKeyStore *a = parcSymmetricKeyStore_Open(...);
 *
 *     parcSymmetricKeyStore_Release(&a);
 * }
 * @endcode
 */
void parcSymmetricKeyStore_Release(PARCSymmetricKeyStore **instancePtr);

/**
 * Create a symmetric (secret) key of the given bit length (e.g. 256)
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCBuffer *parcSymmetricKeyStore_CreateKey(unsigned bits);

PARCBuffer *parcSymmetricKeyStore_GetKey(PARCSymmetricKeyStore *keyStore);

PARCCryptoHash *parcSymmetricKeyStore_GetVerifierKeyDigest(PARCSymmetricKeyStore *keyStore);

/**
 * Creates a PARC format symmetric keystore.  It only contains a single key.
 *
 * Return 0 on success, -1 on failure
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
bool parcSymmetricKeyStore_CreateFile(const char *filename, const char *password, PARCBuffer *secret_key);

/**
 * Create a PKCS12 signing context for use in ccnx_Signing by reading the PARC symmetric key
 * file given by filename.  It is destroyed
 * by parc_Signing when the signing context is destroyed.
 *
 * @param [in] filename The filename.
 * @param [in] password The password to use.
 * @param [in] hmacHashType is for the HMAC, e.g. PARC_HASH_SHA256
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCSymmetricKeyStore *parcSymmetricKeyStore_OpenFile(const char *filename, const char *password, PARCCryptoHashType hmacHashType);

/**
 * Create a PKCS12 signing context for use in ccnx_Signing from the provided key.
 * This is an in-memory only signer.
 * It is destroyed by parc_Signing when the signing context is destroyed.
 *
 * @param secret_key is the shared secret, we take ownership of the buffer.
 * @param hmacHashType is for the HMAC, e.g. PARC_HASH_SHA256
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCSymmetricKeyStore *parcSymmetricKeyStore_Create(PARCBuffer *secret_key);
#endif // libparc_parc_SymmetricKeyStore_h
