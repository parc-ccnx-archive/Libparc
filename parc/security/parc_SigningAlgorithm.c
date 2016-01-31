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
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>

#include <stdio.h>
#include <string.h>

#include <LongBow/runtime.h>

#include <parc/security/parc_SigningAlgorithm.h>

static struct {
    PARCSigningAlgorithm alg;
    char *name;
} _signingAlgorithm_ToString[] = {
    { PARCSigningAlgortihm_NULL, "PARCSigningAlgortihm_NULL" },
    { PARCSigningAlgorithm_RSA,  "PARCSigningAlgorithm_RSA"  },
    { PARCSigningAlgorithm_DSA,  "PARCSigningAlgorithm_DSA"  },
    { PARCSigningAlgorithm_HMAC, "PARCSigningAlgorithm_HMAC" },
    { 0,                         NULL                        }
};

const char *
parcSigningAlgorithm_ToString(PARCSigningAlgorithm alg)
{
    for (int i = 0; _signingAlgorithm_ToString[i].name != NULL; i++) {
        if (_signingAlgorithm_ToString[i].alg == alg) {
            return _signingAlgorithm_ToString[i].name;
        }
    }
    return NULL;
}

PARCSigningAlgorithm
parcSigningAlgorithm_FromString(const char *name)
{
    for (int i = 0; _signingAlgorithm_ToString[i].name != NULL; i++) {
        if (strcmp(_signingAlgorithm_ToString[i].name, name) == 0) {
            return _signingAlgorithm_ToString[i].alg;
        }
    }
    return PARCSigningAlgorithm_UNKNOWN;
}

PARCSigningAlgorithm
parcSigningAlgorithm_GetSigningAlgorithm(PARCCryptoSuite suite)
{
    switch (suite) {
        case PARCCryptoSuite_DSA_SHA256:
            return PARCSigningAlgorithm_DSA;

        case PARCCryptoSuite_RSA_SHA256:      // fallthrough
        case PARCCryptoSuite_RSA_SHA512:
            return PARCSigningAlgorithm_RSA;

        case PARCCryptoSuite_HMAC_SHA256:     // fallthrough
        case PARCCryptoSuite_HMAC_SHA512:
            return PARCSigningAlgorithm_HMAC;

        case PARCCryptoSuite_NULL_CRC32C:
            return PARCSigningAlgortihm_NULL;

        default:
            trapIllegalValue(suit, "Unknown crypto suite: %d", suite);
    }
}
