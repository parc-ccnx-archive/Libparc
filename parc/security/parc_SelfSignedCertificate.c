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
 * @author Marc Mosko, Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

#include <config.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#include <LongBow/runtime.h>
#include <parc/algol/parc_Memory.h>

#include <parc/security/parc_Security.h>
#include <parc/security/parc_SelfSignedCertificate.h>
#include <parc/security/parc_Security.h>

#include <openssl/pkcs12.h>
#include <openssl/rand.h>
#include <openssl/err.h>

#ifdef __APPLE__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

/**
 * Returns true on success, false on failure
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
static bool
_addCertificateExtensionWithContext(X509 *cert, int nid, char *value)
{
    X509_EXTENSION *extension;
    X509V3_CTX context;

    X509V3_set_ctx_nodb(&context);
    X509V3_set_ctx(&context, cert, cert, NULL, NULL, 0);
    extension = X509V3_EXT_conf_nid(NULL, &context, nid, value);
    if (extension == NULL) {
        return false;
    }
    X509_add_ext(cert, extension, -1);
    X509_EXTENSION_free(extension);
    return true;
}

/**
 * Returns true on success, false on failure
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
static bool
_addCertificateExtension(X509 *cert, int nid, char *value)
{
    X509_EXTENSION *extension = X509V3_EXT_conf_nid(NULL, NULL, nid, value);
    if (extension == NULL) {
        return false;
    }
    X509_add_ext(cert, extension, -1);
    X509_EXTENSION_free(extension);
    return true;
}

/**
 * Add a subject_key_identifier and authority_key_dentifier based
 * on the SHA256 hash of the public key.
 *
 * Return true on success, false on error
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
static bool
_addKeyIdentifier(X509 *cert)
{
    unsigned char spkid[SHA256_DIGEST_LENGTH];
    char spkid_hex[1 + 2 * SHA256_DIGEST_LENGTH];

    /* Generate a KeyID which is the SHA256 digest of the DER encoding
     * of a SubjectPublicKeyInfo.  Note that this is slightly uncommon,
     * but it is more general and complete than digesting the BIT STRING
     * component of the SubjectPublicKeyInfo itself (and no standard dictates
     * how you must generate a key ID).  This code must produce the same result
     * as the Java version applied to the same SubjectPublicKeyInfo.
     */

    if (ASN1_item_digest(ASN1_ITEM_rptr(X509_PUBKEY), EVP_sha256(), X509_get_X509_PUBKEY(cert), spkid, NULL)) {
        for (int i = 0; i < 32; i++) {
            snprintf(&spkid_hex[2 * i], 3, "%02X", (unsigned) spkid[i]);
        }
        if (_addCertificateExtension(cert, NID_subject_key_identifier, spkid_hex) == true) {
            if (_addCertificateExtensionWithContext(cert, NID_authority_key_identifier, "keyid:always") == true) {
                return true;
            }
        }
    }
    return false;
}

/**
 * Add the specified name as the certificate subject name.
 * Return true on success, false on failure
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
static bool
_addSubjectName(X509 *cert, const char *subjectname)
{
    // Set up the simple subject name and issuer name for the certificate.
    X509_NAME *name = X509_get_subject_name(cert);
    assertNotNull(name, "Got null name from X509_get_subject_name");

    if (X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char *) subjectname, -1, -1, 0)) {
        if (X509_set_issuer_name(cert, name)) {
            return true;
        }
    }
    return false;
}

/**
 * Add a random serial number.
 * Returns true on success and false on error
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
static bool
_addRandomSerial(X509 *cert)
{
    unsigned long serial = 0;
    unsigned char serial_bytes[sizeof(serial)];

    // Construct random positive serial number.
    RAND_bytes(serial_bytes, sizeof(serial_bytes));
    serial_bytes[0] &= 0x7F;
    serial = 0;
    for (int i = 0; i < sizeof(serial_bytes); i++) {
        serial = (256 * serial) + serial_bytes[i];
    }
    ASN1_INTEGER_set(X509_get_serialNumber(cert), serial);
    return true;
}

static bool
_addValidityPeriod(X509 *cert, unsigned validityDays)
{
    // Set the validity from now for the specified number of days.
    X509_gmtime_adj(X509_get_notBefore(cert), (long) 0);
    X509_gmtime_adj(X509_get_notAfter(cert), (long) (60 * 60 * 24 * validityDays));
    return true;
}

static bool
_addExtensions(X509 *cert)
{
    // Add the necessary extensions.
    if (_addCertificateExtension(cert, NID_basic_constraints, "critical,CA:FALSE") == true) {
        if (_addCertificateExtension(cert, NID_key_usage, "digitalSignature,nonRepudiation,keyEncipherment,dataEncipherment,keyAgreement") == true) {
            if (_addCertificateExtension(cert, NID_ext_key_usage, "clientAuth") == true) {
                return true;
            }
        }
    }
    return false;
}

bool
parcSelfSignedCertificate_CreateRSACertificate(const char *subjectname, unsigned keylength, unsigned validityDays, X509 *cert, RSA *rsa, EVP_PKEY *private_key)
{
    int res;
    bool return_value = false;
    BIGNUM *pub_exp;

    pub_exp = BN_new();

    BN_set_word(pub_exp, RSA_F4);
    res = 1;
    if (RSA_generate_key_ex(rsa, keylength, pub_exp, NULL)) {
        if (EVP_PKEY_set1_RSA(private_key, rsa)) {
            if (X509_set_version(cert, 2)) { // 2 => X509v3
                return_value = true;
            }
        }
    }
    if (return_value) {
        // add serial number
        if (_addRandomSerial(cert) == true) {
            if (_addValidityPeriod(cert, validityDays) == true) {
                if (X509_set_pubkey(cert, private_key) == 1) {
                    if (_addSubjectName(cert, subjectname) == true) {
                        if (_addExtensions(cert) == true) {
                            if (_addKeyIdentifier(cert) == true) {
                                // The certificate is complete, sign it.
                                if (X509_sign(cert, private_key, EVP_sha256())) {
                                    return_value = true;
                                } else {
                                    printf("error: (%d) %s\n", res, ERR_lib_error_string(res));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    ERR_print_errors_fp(stdout);

    BN_free(pub_exp);

    return return_value;
}

#ifdef __APPLE__
#pragma clang diagnostic pop
#endif
