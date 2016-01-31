/*
 * Copyright (c) 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>
#include <stdio.h>
#include <string.h>

#include <LongBow/runtime.h>

#include <parc/algol/parc_Object.h>
#include <parc/security/parc_Signature.h>
#include <parc/algol/parc_Memory.h>

struct parc_signature {
    PARCSigningAlgorithm signingAlgorithm;
    PARCCryptoHashType hashType;
    PARCBuffer *signatureBits;
};

static void
_parcSignature_FinalRelease(PARCSignature **signatureP)
{
    parcBuffer_Release(&(*signatureP)->signatureBits);
}

parcObject_ExtendPARCObject(PARCSignature, _parcSignature_FinalRelease, NULL, NULL, NULL, NULL, NULL, NULL);

PARCSignature *
parcSignature_Create(PARCSigningAlgorithm signingAlgorithm, PARCCryptoHashType hashType, PARCBuffer *signatureBits)
{
    assertNotNull(signatureBits, "SignatureBits Parameter cannot be null");

    PARCSignature *signature = parcObject_CreateInstance(PARCSignature);
    assertNotNull(signature, "parcObject_CreateInstance(%zu) returned NULL", sizeof(PARCSignature));

    signature->signingAlgorithm = signingAlgorithm;
    signature->hashType = hashType;
    signature->signatureBits = parcBuffer_Acquire(signatureBits);

    return signature;
}

parcObject_ImplementAcquire(parcSignature, PARCSignature);

parcObject_ImplementRelease(parcSignature, PARCSignature);


PARCSigningAlgorithm
parcSignature_GetSigningAlgorithm(const PARCSignature *signature)
{
    assertNotNull(signature, "Parameter must be non-null");
    return signature->signingAlgorithm;
}

PARCCryptoHashType
parcSignature_GetHashType(const PARCSignature *signature)
{
    assertNotNull(signature, "Parameter must be non-null");
    return signature->hashType;
}

PARCBuffer *
parcSignature_GetSignature(const PARCSignature *signature)
{
    assertNotNull(signature, "Parameter must be non-null");
    return signature->signatureBits;
}

char *
parcSignature_ToString(const PARCSignature *signature)
{
    assertNotNull(signature, "Parameter must be a non-null CCNxSignature pointer");

    char *bits = parcBuffer_ToString(signature->signatureBits);

    char *string;
    int nwritten = asprintf(&string, "CCNxSignedInfo { .signingAlg=%d, .digestAlg=%d, .signature=%s }",
                            signature->signingAlgorithm,
                            signature->hashType,
                            bits);
    assertTrue(nwritten >= 0, "Error calling asprintf");

    parcMemory_Deallocate((void **) &bits);

    char *result = parcMemory_StringDuplicate(string, strlen(string));
    free(string);
    return result;
}

bool
parcSignature_Equals(const PARCSignature *x, const PARCSignature *y)
{
    if (x == y) {
        return true;
    }
    if (x == NULL || y == NULL) {
        return false;
    }

    if (x->signingAlgorithm == y->signingAlgorithm) {
        if (x->hashType == y->hashType) {
            if (parcBuffer_Equals(x->signatureBits, y->signatureBits)) {
                return true;
            }
        }
    }

    return false;
}
