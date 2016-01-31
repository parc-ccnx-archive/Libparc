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
 * @file parc_Signature.h
 * @ingroup security
 * @brief A tuple of (SigningAlgorithm, SignatureBuffer, PublicKeyDigest)
 *
 * A PARCSignature wraps the tuple { SigningAlgorithm, SignatureBuffer, PublicKeyDigest },
 * where PublicKeyDigest is a PARCCryptoHash of the publisher public key digest.
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libparc_parc_Signature_h
#define libparc_parc_Signature_h

#include <parc/security/parc_CryptoHash.h>
#include <parc/security/parc_CryptoHashType.h>
#include <parc/security/parc_SigningAlgorithm.h>
#include <parc/algol/parc_Buffer.h>

struct parc_signature;
typedef struct parc_signature PARCSignature;

/**
 * Create a `PARCSignature` instance wrapping all the pieces needed to use it.
 *
 * @param [in] signingAlgorithm is the algorithm used to produce the signature
 * @param [in] hashType The PARCCryptoHashType of cryptographic hash digest computed from the input bits which is ultimately signed.
 * @param [in] signatureBits is the actual signature, as an array of bytes
 *
 * @return A pointer to a `PARCSignature` instance.
 *
 * Example:
 * @code
 * {
 *     PARCBuffer *sigbits = parcBuffer_Flip(parcBuffer_FromString("signature bits"));
 *
 *     PARCSignature *signature = parcSignature_Create(PARCSigningAlgorithm_RSA, PARC_HASH_SHA256, sigbits);
 *     parcBuffer_Release(&sigbits);
 * }
 * @endcode
 */
PARCSignature *parcSignature_Create(PARCSigningAlgorithm signingAlgorithm, PARCCryptoHashType hashType, PARCBuffer *signatureBits);

/**
 * Increase the number of references to an instance of this object.
 *
 * Note that new instance is not created,
 * only that the given instance's reference count is incremented.
 * Discard the reference by invoking `parcSignature_Release`.
 *
 * @param [in] signature A pointer to the instance of `PARCSignature` to acquire.
 * @return The value of the input parameter @p instance.
 *
 * Example:
 * @code
 * {
 *     ...
 *
 *     PARCSignature *signature = parcSignature_Acquire(instance);
 *
 *     parcSignature_Release(&signature);
 * }
 * @endcode
 *
 * @see `parcSignature_Release`
 */

PARCSignature *parcSignature_Acquire(const PARCSignature *signature);

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
 * @param [in,out] signaturePtr A pointer to a pointer to the instance to release.
 *
 *
 * Example:
 * @code
 * {
 *     PARCBuffer *sigbits = parcBuffer_Flip(parcBuffer_FromString("signature bits"));
 *
 *     PARCSignature *signature = parcSignature_Create(PARCSigningAlgorithm_RSA, PARC_HASH_SHA256, sigbits);
 *     parcBuffer_Release(&sigbits);
 *     parcSignature_Release(&signature);
 * }
 * @endcode
 *
 * @see `parcSignature_Acquire`
 */
void parcSignature_Release(PARCSignature **signaturePtr);

/**
 * Returns the signing algorithm.
 *
 * @param [in] signature The `PARCSignature` instance from which the signing algorithm is retrieved.
 *
 * @return A `PARCSigningAlgorithm` value corresponding to this signature.
 *
 * Example:
 * @code
 * {
 *     PARCBuffer *sigbits = parcBuffer_Flip(parcBuffer_FromString("signature bits"));
 *
 *     PARCSignature *signature = parcSignature_Create(PARCSigningAlgorithm_RSA, PARC_HASH_SHA256, sigbits);
 *     parcBuffer_Release(&sigbits);
 *
 *     PARCSigningAlgorithm algorithm = parcSignature_GetSigningAlgorithm(signature);
 * }
 * @endcode
 */
PARCSigningAlgorithm parcSignature_GetSigningAlgorithm(const PARCSignature *signature);

/**
 * Returns the digest algorithm used to compute the digest we signed.
 *
 * @param [in] signature The `PARCSignature` instance from which the hash type is retrieved.
 *
 * @return A `PARCCryptoHashType` value corresponding to this signature.
 *
 * Example:
 * @code
 * {
 *     PARCBuffer *sigbits = parcBuffer_Flip(parcBuffer_FromString("signature bits"));
 *
 *     PARCSignature *signature = parcSignature_Create(PARCSigningAlgorithm_RSA, PARC_HASH_SHA256, sigbits);
 *     parcBuffer_Release(&sigbits);
 *
 *     PARCCryptoHashType hashType = parcSignature_GetHashType(signature);
 * }
 * @endcode
 */
PARCCryptoHashType parcSignature_GetHashType(const PARCSignature *signature);

/**
 * Gets the signature as a buffer of bits.
 *
 * @param [in] signature The `PARCSignature` instance from which the signature payload is retrieved.
 *
 * @return A `PARCBuffer` instanace containing the raw signature bytes.
 *
 * Example:
 * @code
 * {
 *     PARCBuffer *sigbits = parcBuffer_Flip(parcBuffer_FromString("signature bits"));
 *
 *     PARCSignature *signature = parcSignature_Create(PARCSigningAlgorithm_RSA, PARC_HASH_SHA256, sigbits);
 *     parcBuffer_Release(&sigbits);
 *
 *     PARCBuffer *payload = parcSignature_GetSignature(signature);
 * }
 * @endcode
 */
PARCBuffer *parcSignature_GetSignature(const PARCSignature *signature);

/**
 * Produce a nul-terminated string representation of the specified instance.
 *
 * The non-null result must be freed by the caller via `parcMemory_Deallocate()`.
 *
 * @param [in] signature A pointer to a PARCSignature instance.
 *
 * @return NULL An error occurred.
 * @return non-NULL A pointer to a nul-terminated C-string the must be dellocated via `parcMemory_Deallocate()`
 *
 * Example:
 * @code
 * {
 *     PARCBuffer *sigbits = parcBuffer_Flip(parcBuffer_FromString("signature bits"));
 *
 *     PARCSignature *signature = parcSignature_Create(PARCSigningAlgorithm_RSA, PARC_HASH_SHA256, sigbits);
 *     parcBuffer_Release(&sigbits);
 *
 *     char *string = parcSignature_ToString(signature);
 *
 *     parcMemory_Deallocate(&signature);
 * }
 * @endcode
 */
char *parcSignature_ToString(const PARCSignature *signature);

/**
 * Determine if two PARCSignature instances are equal.
 *
 * The following equivalence relations on non-null `PARCSignature` instances are maintained:
 *
 *  * It is reflexive: for any non-null reference value x, `parcSignature_Equals(x, x)`
 *      must return true.
 *
 *  * It is symmetric: for any non-null reference values x and y,
 *    `parcSignature_Equals(x, y)` must return true if and only if
 *        `parcSignature_Equals(y, x)` returns true.
 *
 *  * It is transitive: for any non-null reference values x, y, and z, if
 *        `parcSignature_Equals(x, y)` returns true and
 *        `parcSignature_Equals(y, z)` returns true,
 *        then  `parcSignature_Equals(x, z)` must return true.
 *
 *  * It is consistent: for any non-null reference values x and y, multiple
 *      invocations of `parcSignature_Equals(x, y)` consistently return true or
 *      consistently return false.
 *
 *  * For any non-null reference value x, `parcSignature_Equals(x, NULL)` must
 *      return false.
 *
 * @param [in] a A pointer to a PARCSignature instance.
 * @param [in] b A pointer to a PARCSignature instance.
 *
 * @return true if the two instances are equal
 * @return false if the two instancea are no equal.
 *
 * Example:
 * @code
 * {
 *     PARCBuffer *sigbits = parcBuffer_Flip(parcBuffer_FromString("signature bits"));
 *
 *     PARCSignature *signatureA = parcSignature_Create(PARCSigningAlgorithm_RSA, PARC_HASH_SHA256, sigbits);
 *     PARCSignature *signatureB = parcSignature_Create(PARCSigningAlgorithm_RSA, PARC_HASH_SHA256, sigbits);
 *     parcBuffer_Release(&sigbits);
 *
 *     if (parcSignature_Equals(signatureA, signatureB)) {
 *        // true
 *     } else {
 *        // false
 *     }
 *     parcMemory_Deallocate(&signatureA);
 *     parcMemory_Deallocate(&signatureB);
 * }
 * @endcode
 */
bool parcSignature_Equals(const PARCSignature *a, const PARCSignature *b);
#endif // libparc_parc_Signature_h
