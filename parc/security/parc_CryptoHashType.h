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
 * @file parc_CryptoHashType.h
 * @ingroup security
 * @brief A type specifying a cryptographic hash (or CRC check) algorithm.
 *
 * This type is overloaded to support both cryptographic hash digest algorithms and cyclical-reduncancy
 * check (CRC) algorithms. See the available `PARCCryptoHashType` enum types for an exhaustive
 * list of the supported algorithms.
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libparc_parc_CryptoHashType_h
#define libparc_parc_CryptoHashType_h

typedef enum {
    PARC_HASH_SHA256,
    PARC_HASH_SHA512,
    PARC_HASH_CRC32C,
    PARC_HASH_NULL
} PARCCryptoHashType;

/**
 * Convert the `PARCCryptoHashType` value to a human-readable string representation.
 *
 * @param [in] type A `PARCCryptoHashType` value
 *
 * @return A static, null-terminated string.
 *
 * Example:
 * @code
 * {
 *     PARCCryptoHashType type = PARC_HASH_SHA256;
 *     const char *stringRep = parcCryptoHashType_ToString(type);
 *     // use stringRep as necessary, and then free
 * }
 * @endcode
 */
const char *parcCryptoHashType_ToString(PARCCryptoHashType type);

/**
 * Convert a string representation value of a `PARCCryptoHashType` to an actual value.
 *
 * @param [in] name A string representation of a `PARCCryptoHashType` value.
 *
 * @return A `PARCCryptoHashType` value.
 *
 * Example:
 * @code
 * {
 *     const char stringRep[17] = "PARC_HASH_SHA256";
 *     PARCCryptoHashType type = parcCryptoHashType_FromString(stringRep);
 *     // use stringRep as necessary, and then free
 * }
 * @endcode
 */
PARCCryptoHashType parcCryptoHashType_FromString(const char *name);
#endif // libparc_parc_CryptoHashType_h
