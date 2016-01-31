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
 * @file parc_CertificateType.h
 * @ingroup security
 * @brief A type specifying a certificate.
 *
 * @author Christopher Wood, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libparc_parc_CertificateType_h
#define libparc_parc_CertificateType_h

typedef enum {
    PARCCertificateType_X509,
    PARCCertificateType_Invalid
} PARCCertificateType;

/**
 * Convert the `PARCCertificateType` value to a human-readable string representation.
 *
 * @param [in] type A `PARCCertificateType` value
 *
 * @return A static, null-terminated string.
 *
 * Example:
 * @code
 * {
 *     PARCCertificateType type = PARCCertificateType_X509;
 *     const char *stringRep = parcCertificateType_ToString(type);
 *     // use stringRep as necessary, and then free
 * }
 * @endcode
 */
const char *parcCertificateType_ToString(PARCCertificateType type);

/**
 * Convert a string representation value of a `PARCCertificateType` to an actual value.
 *
 * @param [in] name A string representation of a `PARCCertificateType` value.
 *
 * @return A `PARCCertificateType` value.
 *
 * Example:
 * @code
 * {
 *     const char stringRep[17] = "PARCCertificateType_X509";
 *     PARCCertificateType type = parcCertificateType_FromString(stringRep);
 *     // use stringRep as necessary, and then free
 * }
 * @endcode
 */
PARCCertificateType parcCertificateType_FromString(const char *name);
#endif // libparc_parc_CertificateType_h
