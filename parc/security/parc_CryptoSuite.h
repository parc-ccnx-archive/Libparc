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
 * @file parc_CryptoSuite.h
 * @ingroup security
 * @brief Represent a cryptographic suite, a set of corresponding hash and signing/MAC/CRC algorithms.
 *
 * A cryptographic suite encapsulates the method by which (public key) digital signatures and
 * (private key) MACs. For example, a digital signature suite might combine SHA-256 as the hash
 * digest algorithm and RSA as the signature generation/verification algorithm. Such a suite
 * would would have the PARCCryptoSuite value PARCCryptoSuite_RSA_SHA256.
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libparc_parc_CryptoSuite_h
#define libparc_parc_CryptoSuite_h

#include <parc/security/parc_CryptoHashType.h>

typedef enum {
    PARCCryptoSuite_RSA_SHA256,
    PARCCryptoSuite_DSA_SHA256,
    PARCCryptoSuite_RSA_SHA512,
    PARCCryptoSuite_HMAC_SHA256,
    PARCCryptoSuite_HMAC_SHA512,
    PARCCryptoSuite_NULL_CRC32C,
    PARCCryptoSuite_EC_SECP_256K1,
    PARCCryptoSuite_UNKNOWN
} PARCCryptoSuite;

/**
 * Given a PARCCryptoSuite value, return the corresponding cryptographic hash as a `PARCCryptoHashType`.
 *
 * @param [in] suite A PARCCryptoSuite value.
 *
 * @return A PARCCryptoHashType value
 *
 * Example:
 * @code
 * {
 *     PARCryptoHashType hash = parcCryptoSuite_GetCryptoHash(PARCCryptoSuite_RSA_SHA256);
 * }
 * @endcode
 */
PARCCryptoHashType parcCryptoSuite_GetCryptoHash(PARCCryptoSuite suite);
#endif // libparc_parc_CryptoSuite_h
