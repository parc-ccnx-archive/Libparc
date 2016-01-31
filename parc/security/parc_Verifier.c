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
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>
#include <stdio.h>

#include <LongBow/runtime.h>

#include <parc/security/parc_Verifier.h>
#include <parc/algol/parc_Memory.h>

struct parc_verifier {
    PARCVerifierInterface *interface;
};

/**
 *
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCVerifier *
parcVerifier_Create(PARCVerifierInterface *interfaceContext)
{
    assertNotNull(interfaceContext, "Parameter must be non-null interface pointer");

    PARCVerifier *verifier = parcMemory_AllocateAndClear(sizeof(PARCVerifier));
    assertNotNull(verifier, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(PARCVerifier));
    verifier->interface = interfaceContext;
    return verifier;
}

/**
 * Destroys the signing context and the underlying interface
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void
parcVerifier_Destroy(PARCVerifier **verifierPtr)
{
    assertNotNull(verifierPtr, "Got null double pointer");
    assertNotNull(*verifierPtr, "Got null pointer dereference");

    PARCVerifier *verifier = *verifierPtr;

    verifier->interface->Destroy(&verifier->interface);
    parcMemory_Deallocate((void **) &verifier);
    *verifierPtr = NULL;
}

/**
 * Verify the signature against the provided digest with the specified key.
 * If we do not trust the key, the signature will be rejected.
 *
 * Returns true if the signature is accepted,false if it is rejected
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
bool
parcVerifier_VerifySignature(PARCVerifier *verifier, PARCKeyId *keyid, PARCCryptoHash *locallyComputedHash,
                             PARCCryptoSuite suite, PARCSignature *signatureToVerify)
{
    assertNotNull(verifier, "Parameter must be non-null PARCVerifier");
    assertNotNull(locallyComputedHash, "cryptoHash to verify must not be null");
    assertNotNull(signatureToVerify, "Signature to verify must not be null");

    // null keyid is allowed now that we support CRCs, etc.

    return verifier->interface->VerifyDigest(verifier->interface->interfaceContext, keyid, locallyComputedHash, suite, signatureToVerify);
}

/**
 * Is the given crypto suite compatible with the key?
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
bool
parcVerifier_AllowedCryptoSuite(PARCVerifier *verifier, PARCKeyId *keyid, PARCCryptoSuite suite)
{
    assertNotNull(verifier, "Parameter must be non-null PARCVerifier");
    return verifier->interface->AllowedCryptoSuite(verifier->interface->interfaceContext, keyid, suite);
}

PARCCryptoHasher*
parcVerifier_GetCryptoHasher(PARCVerifier *verifier, PARCKeyId *keyid, PARCCryptoHashType hashType)
{
    assertNotNull(verifier, "Parameter must be non-null PARCVerifier");
    return verifier->interface->GetCryptoHasher(verifier->interface->interfaceContext, keyid, hashType);
}

void
parcVerifier_AddKey(PARCVerifier *verifier, PARCKey *key)
{
    assertNotNull(verifier, "Parameter must be non-null PARCVerifier");
    verifier->interface->AddKey(verifier->interface->interfaceContext, key);
}

void
parcVerifier_RemoveKeyId(PARCVerifier *verifier, PARCKeyId *keyid)
{
    assertNotNull(verifier, "Parameter must be non-null PARCVerifier");
    verifier->interface->RemoveKeyId(verifier->interface->interfaceContext, keyid);
}
