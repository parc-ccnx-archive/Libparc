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
 * @file parc_Verifier.h
 * @ingroup security
 * @brief  Structures and functions to support verification.
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libparc_parc_Verifier_h
#define libparc_parc_Verifier_h

#include <parc/security/parc_CryptoHasher.h>
#include <parc/security/parc_Signature.h>
#include <parc/security/parc_CryptoHashType.h>
#include <parc/security/parc_Key.h>
#include <parc/algol/parc_ArrayList.h>

struct parc_verifier;
/**
 * @typedef PARCVerifier
 * @brief The structure for PARCVerifier
 */

typedef struct parc_verifier PARCVerifier;

/**
 * @typedef PARCVerifierInterface
 * @brief The interface for `PARCVerifier`
 */

typedef struct parc_verifier_interface {
    void *interfaceContext;

    PARCCryptoHasher*(*GetCryptoHasher)(void *interfaceContext, PARCKeyId * keyid, PARCCryptoHashType hashType);

    bool (*VerifyDigest)(void *interfaceContext, PARCKeyId *keyid, PARCCryptoHash *locallyComputedHash,
                         PARCCryptoSuite suite, PARCSignature *signatureToVerify);

    void (*AddKey)(void *interfaceContext, PARCKey *key);
    void (*RemoveKeyId)(void *interfaceContext, PARCKeyId *keyid);

    bool (*AllowedCryptoSuite)(void *interfaceContext, PARCKeyId *keyid, PARCCryptoSuite suite);

    void (*Destroy)(struct parc_verifier_interface **interfaceContextPtr);
} PARCVerifierInterface;

/**
 *
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCVerifier *parcVerifier_Create(PARCVerifierInterface *interfaceContext);

/**
 * Destroys the signing context and the underlying interface
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void parcVerifier_Destroy(PARCVerifier **verifierPtr);

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
                             PARCCryptoSuite suite, PARCSignature *signatureToVerify);

/**
 * Is the given crypto suite compatible with the key?
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
bool
parcVerifier_AllowedCryptoSuite(PARCVerifier *verifier, PARCKeyId *keyid, PARCCryptoSuite suite);

/**
 * Returns a CyrptoHasher for use with the keyid.  The caller should have already
 * verified that the specified hashType is compatible with the keyid by checking the
 * AllowedCryptoSuite
 *
 * Returns NULL if the hashType is not compatible with the key.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCCryptoHasher*
parcVerifier_GetCryptoHasher(PARCVerifier *verifier, PARCKeyId *keyid, PARCCryptoHashType hashType);

void
parcVerifier_AddKey(PARCVerifier *verifier, PARCKey *key);

void
parcVerifier_RemoveKeyId(PARCVerifier *verifier, PARCKeyId *keyid);
#endif // libparc_parc_Verifier_h
