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
 * @file parc_PublicKeySignerPkcs12Store.h
 * @ingroup security
 * @brief A concrete implementation of ccnx_Signer using a PCKS12 keystore with
 * an RSA key and self-signed certificate.
 * This represents a concrete implementation of ccnx_Signer using a PCKS12 keystore with
 * an RSA key and self-signed certificate.
 *
 * You can create such a keystore using the parcPublicKeySignerPkcs12Store_CreateFile() function.
 *
 * Otherwise, you use this inside a parc_Signer, such as:
 *
 * PARCSigner *signer = parcSigner_Create( parcPublicKeySignerPkcs12Store_Open("mykeystore.p12", "12345") );
 *
 * The signer then takes ownership of the memory and will destroy the keystore when the
 * signer's destroy is called (destroy does not affect the on-disk file).
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libparc_parc_PublicKeySignerPkcs12Store_h
#define libparc_parc_PublicKeySignerPkcs12Store_h

#include <parc/security/parc_Signer.h>

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
 *     bool result = parcPublicKeySignerPkcs12Store_CreateFile(filename, password, subject, 1024, 32);
 * }
 * @endcode
 */
extern
bool parcPublicKeySignerPkcs12Store_CreateFile(
    const char *filename,
    const char *password,
    const char *subjectName,
    unsigned keyLength,
    unsigned validityDays);

/**
 * Create a PKCS12 signing context for use in ccnx_Signing.
 * It is destroyed by `ccnxSigner_Destroy` when the signing context is destroyed.
 *
 * @param [in] filename The name of a file containing the PKCS12 keystore.
 * @param [in] password The password to decrypt/unlock the determines how the signer digests data. Supports PARC_HASH_SHA256 and PARC_HASH_SHA512.
 * @param [in] hashType Determines how the signer digests data. Possible values are PARC_HASH_SHA256 and PARC_HASH_SHA512.
 *
 * @return A `PARCSigningInterface` instance using the public/private key pair contained within the PKCS12 file.
 *
 * Example:
 * @code
 * {
 *     const char *filename = "/tmp/ccnxFileKeyStore_Pkcs12Open_CreateAndOpen.p12";
 *     const char *password = "12345";
 *     const char *subject  = "alice";
 *     bool result = parcPublicKeySignerPkcs12Store_CreateFile(filename, password, subject, 1024, 32);
 *
 *     ...
 *
 *     PARCSigningInterface *interface = parcPublicKeySignerPkcs12Store_Open(filename, password, PARC_HASH_SHA256);
 *
 *     ...
 * }
 * @endcode
 */
PARCSigningInterface *parcPublicKeySignerPkcs12Store_Open(const char *filename, const char *password, PARCCryptoHashType hashType);

#endif // libparc_parc_PublicKeySignerPkcs12Store_h
