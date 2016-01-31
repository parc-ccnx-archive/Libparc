/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @file parc_SymmetricSignerFileStore.h
 * @ingroup security
 * @brief A Symmetric key based Signer.
 *
 *
 * An Symmetric key based Signer.  It uses an HMAC-AES-256 message authenticator.
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
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libparc_parc_SymmetricSignerFileStore_h
#define libparc_parc_SymmetricSignerFileStore_h

#include <parc/security/parc_Signer.h>
#include <parc/algol/parc_Buffer.h>

/**
 * Create a symmetric (secret) key of the given bit length (e.g. 256)
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCBuffer *
parcSymmetricSignerFileStore_CreateKey(unsigned bits);

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
extern int
parcSymmetricSignerFileStore_CreateFile(const char *filename, const char *password, PARCBuffer *secret_key);

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
extern PARCSigningInterface *parcSymmetricSignerFileStore_OpenFile(const char *filename, const char *password, PARCCryptoHashType hmacHashType);

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
extern PARCSigningInterface *parcSymmetricSignerFileStore_Create(PARCBuffer *secret_key, PARCCryptoHashType hmacHashType);
#endif // libparc_parc_SymmetricSignerFileStore_h
