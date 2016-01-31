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
 * @file parc_Encoding.h
 * @ingroup security
 * @brief A Encoding specifying a certificate.
 *
 * @author Christopher Wood, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libparc_parc_Encoding_h
#define libparc_parc_Encoding_h

typedef enum {
    PARCContainerEncoding_PEM,
    PARCContainerEncoding_DER,
    PARCContainerEncoding_PKCS12,
    PARCContainerEncoding_Invalid
} PARCContainerEncoding;

/**
 * Convert the `PARCContainerEncoding` value to a human-readable string representation.
 *
 * @param [in] Encoding A `PARCContainerEncoding` value
 *
 * @return A static, null-terminated string.
 *
 * Example:
 * @code
 * {
 *     PARCContainerEncoding encoding = PARCContainerEncoding_X509;
 *     const char *stringRep = parcContainerEncoding_ToString(encoding);
 *     // use stringRep as necessary, and then free
 * }
 * @endcode
 */
const char *parcContainerEncoding_ToString(PARCContainerEncoding Encoding);

/**
 * Convert a string representation value of a `PARCContainerEncoding` to an actual value.
 *
 * @param [in] name A string representation of a `PARCContainerEncoding` value.
 *
 * @return A `PARCContainerEncoding` value.
 *
 * Example:
 * @code
 * {
 *     const char stringRep[17] = "PARCContainerEncoding_PEM";
 *     PARCContainerEncoding encoding = parcContainerEncoding_FromString(stringRep);
 *     // use stringRep as necessary, and then free
 * }
 * @endcode
 */
PARCContainerEncoding parcContainerEncoding_FromString(const char *name);
#endif // libparc_parc_Encoding_h
