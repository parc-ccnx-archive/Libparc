/*
 * Copyright (c) 2013-2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @file parc_Verifier.h
 * @ingroup security
 * @brief  Structures and functions to support verification.
 *
 * @author Marc Mosko, Christopher A. Wood, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_Verifier_h
#define libparc_parc_Verifier_h

#include <parc/algol/parc_Object.h>

#include <parc/security/parc_CryptoHasher.h>
#include <parc/security/parc_Signature.h>
#include <parc/security/parc_CryptoHashType.h>
#include <parc/security/parc_Key.h>

struct parc_verifier;
typedef struct parc_verifier PARCVerifier;

/**
 * @typedef PARCVerifierInterface
 * @brief The interface for `PARCVerifier`
 */
typedef struct parc_verifier_interface {
    /** @see parcVerifier_GetCryptoHasher */
    PARCCryptoHasher *(*GetCryptoHasher)(PARCObject *interfaceContext, PARCKeyId *keyid, PARCCryptoHashType hashType);

    /** @see parcVerifier_VerifyDigest */
    bool (*VerifyDigest)(PARCObject *interfaceContext, PARCKeyId *keyid, PARCCryptoHash *locallyComputedHash,
                         PARCCryptoSuite suite, PARCSignature *signatureToVerify);

    /** @see parcVerifier_AddKey */
    void (*AddKey)(PARCObject *interfaceContext, PARCKey *key);

    /** @see parcVerifier_RemoveKeyId */
    void (*RemoveKeyId)(PARCObject *interfaceContext, PARCKeyId *keyid);

    /** @see parcVerifier_AllowedCryptoSuite */
    bool (*AllowedCryptoSuite)(PARCObject *interfaceContext, PARCKeyId *keyid, PARCCryptoSuite suite);
} PARCVerifierInterface;

/**
 * Create a verifier context based on a concrete implementation.
 *
 * @param [in] instance A concrete implementation of a `PARCVerifier`
 * @param [in] interfaceContext The interface of a concrete implementation of a `PARCVerifier`
 *
 * @return NULL A `PARCVerifier` could not be allocated
 * @return PARCSigner A new `PARCVerifier` instance derived from the specified concrete signer context.
 *
 * Example:
 * @code
 * {
 *     PARCVerifier *verifier = parcVerifier_Create(verifierInstance, PARCInMemoryVerifierAsVerifier);
 * }
 * @endcode
 */
PARCVerifier *parcVerifier_Create(PARCObject *instance, PARCVerifierInterface *interfaceContext);

/**
 * Assert that an instance of `PARCVerifier` is valid.
 *
 * If the instance is not valid, terminate via {@link trapIllegalValue}
 *
 * Valid means the internal state of the type is consistent with its
 * required current or future behaviour.
 * This may include the validation of internal instances of types.
 *
 * @param [in] verifier A pointer to a PARCVerifier instance.
 *
 * Example
 * @code
 * {
 *     PARCVerifier *verifier = parcVerifier_Create(verifierInstance, PARCInMemoryVerifierAsVerifier);
 *
 *     parcVerifier_AssertValid(signer);
 * }
 * @endcode
 */
void parcVerifier_AssertValid(const PARCVerifier *verifier);

/**
 * Increase the number of references to the given `PARCVerifier` instance.
 *
 * A new instance is not created,
 * only that the given instance's reference count is incremented.
 * Discard the acquired reference by invoking `parcVerifier_Release()`.
 *
 * @param [in] signer A pointer to a `PARCVerifier` instance.
 *
 * @return NULL An error occurred.
 * @return non-NULL A pointer to a PARCVerifier instance.
 *
 * Example:
 * @code
 * {
 *      PARCVerifier *verifier = parcVerifier_Create(verifierInstance, PARCInMemoryVerifierAsVerifier);
 *      PARCVerifier *handle = parcVerifier_Acquire(signer);
 *      // use the handle instance as needed
 * }
 * @endcode
 */
PARCVerifier *parcVerifier_Acquire(const PARCVerifier *verifier);

/**
 * Release a previously acquired reference to the specified instance,
 * decrementing the reference count for the instance.
 *
 * The pointer to the instance is set to NULL as a side-effect of this function.
 *
 * If the invocation causes the last reference to the instance to be released,
 * the instance is deallocated and the instance's implementation will perform
 * additional cleanup and release other privately held references.
 *
 * The contents of the dealloced memory used for the PARC object are undefined.
 * Do not reference the object after the last release.
 *
 * @param [in,out] verifierPtr A pointer to a pointer to the instance to release.
 *
 * Example:
 * @code
 * {
 *     PARCVerifier *verifier = parcVerifier_Create(verifierInstance, PARCInMemoryVerifierAsVerifier);
 *
 *     parcVerifier_Release(&verifier);
 * }
 * @endcode
 */
void parcVerifier_Release(PARCVerifier **verifierPtr);

/**
 * Verify the signature against the provided digest with the specified key.
 * If we do not trust the key, the signature will be rejected. In this context,
 * trusting a key means that it was previously added to this verifiers "store".
 *
 * Returns true if the signature is accepted,false if it is rejected.
 *
 * @param [in] verifier A `PARCVerifier` instance.
 * @param [in] keyId A `PARCKeyId` which identifies the verification key.
 * @param [in] hashDigest A `PARCCryptoHash` which stores the locally computed digest.
 * @param [in] suite The `PARCCryptoSuite` in which verification is performed.
 * @param [in] signature The `PARCSignature` which is to be verified.
 *
 * @retval true If the signature is valid
 * @retval false Otherwise
 *
 * Example:
 * @code
 * {
 *     PARCVerifier *verifier = parcVerifier_Create(verifierInstance, PARCInMemoryVerifierAsVerifier);
 *
 *     PARCKeyId *keyId = ...
 *     PARCCryptoHash *hash = ...
 *     PARCCryptoSuite suite = PARCCryptoSuite_RSA_SHA256;
 *     PARCSignature *signature = ...
 *
 *     bool valid = parcVerifier_VerifyDigestSignature(verifier, keyId, hash, suite, signature);
 *     if (valid) {
 *         // proceed
 *     }
 * }
 * @endcode
 */
bool
parcVerifier_VerifyDigestSignature(PARCVerifier *verifier, PARCKeyId *keyid, PARCCryptoHash *hashDigest,
                                   PARCCryptoSuite suite, PARCSignature *signatureToVerify);

/**
 * Check to see if the specified `PARCKeyId` is allowed with the given `PARCCryptoSuite`.
 *
 * A`PARCKey` identified by the given `PARCKeyId` can only be used for a particular algorithm.
 *
 * @param [in] verifier A `PARCVerifier` instance with a store of trusted `PARCKey` instances.
 * @param [in] keyId A `PARCKeyId` referring to the key we will check against (for this verifier).
 * @param [in] suite A `PARCCryptoSuite` to check against.
 *
 * @retval true If allowed
 * @retval false Otherwise
 *
 * Example:
 * @code
 * {
 *     PARCVerifier *verifeir = ...
 *     PARCKeyId *keyId = ...
 *     bool isAllowed = parcVerifier_AllowedCryptoSuite(verifier, keyId, PARCCryptoSuite_RSA_SHA256);
 *     // act accordingly
 * }
 * @endcode
 */
bool parcVerifier_AllowedCryptoSuite(PARCVerifier *verifier, PARCKeyId *keyId, PARCCryptoSuite suite);

/**
 * Returns a `PARCCryptoHasher` for use with the `PARCKeyId`. The caller should have already
 * verified that the specified `PARCCryptoHashType` is compatible with the key ID by
 * checking the AllowedCryptoSuite.
 *
 * @param [in] verifier A `PARCVerifier` instance with a store of trusted `PARCKey` instances.
 * @param [in] keyId A `PARCKeyId` referring to the key we will check against (for this verifier).
 * @param [in] suite A `PARCCryptoSuite` to check against.
 *
 * @retval non-NULL A `PARCCryptoHasher` instance.
 * @retval NULL If the PARCCryptoHashType is not compatible with the key.
 *
 * Example:
 * @code
 * {
 *     PARCVerifier *verifeir = ...
 *     PARCKeyId *keyId = ...
 *     bool isAllowed = parcVerifier_AllowedCryptoSuite(verifier, keyId, PARCCryptoHashType_SHA256);
 *     // act accordingly
 * }
 * @endcode
 */
PARCCryptoHasher *parcVerifier_GetCryptoHasher(PARCVerifier *verifier, PARCKeyId *keyid, PARCCryptoHashType hashType);

/**
 * Add the specified `PARCKey` to the trusted key store.
 *
 * @param [in] verifier A `PARCVerifier` instance with a store of trusted `PARCKey` instances.
 * @param [in] key A `PARCKey` containing the new trusted key.
 *
 * Example:
 * @code
 * {
 *     PARCVerifier *verifeir = ...
 *     PARCKey *key = ...
 *     parcVerifier_AddKey(verifier, key);
 * }
 * @endcode
 */
void parcVerifier_AddKey(PARCVerifier *verifier, PARCKey *key);

/**
 * Remove the key associated with the given `PARCKeyId` from the trusted key store.
 *
 * @param [in] verifier A `PARCVerifier` instance with a store of trusted `PARCKey` instances.
 * @param [in] keyId A `PARCKeyId` referencing the `PARCKey` to remove from the keystore.
 *
 * Example:
 * @code
 * {
 *     PARCVerifier *verifeir = ...
 *     PARCKey *key = ...
 *     parcVerifier_AddKey(verifier, key);
 *
 *     // Use the verifier with the key...
 *     ...
 *
 *     // Now remove it because we no longer need or trust it.
 *     PARCKeyId *keyId = parcKey_GetKeyId(key);
 *     parcVerifier_RemoveKeyId(verifier, keyId);
 * }
 * @endcode
 */
void parcVerifier_RemoveKeyId(PARCVerifier *verifier, PARCKeyId *keyid);
#endif // libparc_parc_Verifier_h
