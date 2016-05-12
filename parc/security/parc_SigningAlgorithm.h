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
 * @file parc_SigningAlgorithm.h
 * @ingroup security
 * @brief This module encapsulates information about the types of available signing algorithms.
 *
 * Both asymmetric digital signature algorithms, e.g., RSA and DSA, and symmetric Message Authentication
 * Codes (MACS), e.g., HMAC, are supported. This module exposes the functionality necessary to map between
 * enum and human-readable string representations of these algorithms.
 *
 * @author Glenn Scott, Christopher A. Wood, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_SigningAlgorithm_h
#define libparc_parc_SigningAlgorithm_h

#include <parc/security/parc_CryptoHashType.h>
#include <parc/security/parc_CryptoSuite.h>

typedef enum {
    PARCSigningAlgorithm_UNKNOWN = -1,
    PARCSigningAlgorithm_RSA     =  1,
    PARCSigningAlgorithm_DSA     =  2,
    PARCSigningAlgorithm_HMAC    =  3,
    PARCSigningAlgortihm_NULL    =  4,
} PARCSigningAlgorithm;

/**
 * Return a human readable string representation of the specified signing algorithm.
 *
 * @param [in] algorithm A pointer to a PARCSigningAlgorithm as the target signing algorithm.
 * @return A static, null-terminated string.
 *
 * Example:
 * @code
 * {
 *     PARCSigningAlgorithm alg = PARCSigningAlgorithm_RSA;
 *     const char *stringRep = parcSigningAlgorithm_ToString(alg);
 *     // do something with stringRep
 * }
 * @endcode
 *
 * @see parcSigningAlgorithm_FromString
 */
const char *parcSigningAlgorithm_ToString(PARCSigningAlgorithm algorithm);

/**
 * Get the `PARCSigningAlgorithm` enum from a corresponding human-readable string representation
 * of a signing algorithm.
 *
 * @param [in] name A nul-terminate C-string representation of signing algorithm.
 * @return A valid `PARCSigningAlgorithm` enum.
 *
 * Example:
 * @code
 * {
 *     const char *stringRep = "PARCSigningAlgortihm_NULL";
 *     PARCSigningAlgorithm alg = parcSigningAlgorithm_FromString(stringRep);
 *     // do something with alg
 * }
 * @endcode
 *
 * @see parcSigningAlgorithm_ToString
 */
PARCSigningAlgorithm parcSigningAlgorithm_FromString(const char *name);

/**
 * Get the `PARCSigningAlgorithm` type associated with the specified `PARCCryptoSuite` type.
 *
 * PARCCryptoSuite types combine hash and signing algorithms to be used to signature and/or MAC generation.
 * Therefore, a PARCCryptoSuite type of PARCCryptoSuite_DSA_SHA256, for example, uses the
 * PARCSigningAlgorithm_DSA type of signing algorithm. This function serves to determine the
 * signing algorithm type from the suite.
 *
 * @param [in] suite The type of cryptographic suite used for signature and/or MAC generation.
 * @return A valid `PARCSigningAlgorithm` enum associated with the specified `PARCCryptoSuite` type.
 *
 * Example:
 * @code
 * {
 *     PARCCryptoSuite suite = PARCCryptoSuite_RSA_SHA256;
 *     PARCSigningAlgorithm alg = parcSigningAlgorithm_GetSigningAlgorithm(suite);
 *     // do something with alg
 * }
 * @endcode
 */
PARCSigningAlgorithm parcSigningAlgorithm_GetSigningAlgorithm(PARCCryptoSuite suite);
#endif // libparc_parc_SigningAlgorithm_h
