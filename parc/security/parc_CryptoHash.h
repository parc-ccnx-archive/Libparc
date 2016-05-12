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
 * @file parc_CryptoHash.h
 * @ingroup security
 *
 * @brief PARCDigest holds a cryptographic digest, which is comprised
 * the bytes of the digest and the algorithm used for the digest.
 *
 * @author Marc Mosko, Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_CryptoHash_h
#define libparc_parc_CryptoHash_h

#include <parc/algol/parc_Buffer.h>
#include <parc/security/parc_CryptoHashType.h>

struct parc_crypto_hash;
typedef struct parc_crypto_hash PARCCryptoHash;

/**
 * Create a Digest holding the type and digest buffer.
 *
 * Creates a new reference to the given PARCBuffer `digest`.
 *
 * @param [in] digestType The type of hash digest algorithm used to compute this digest.
 * @param [in] digest The actual hash digest instance.
 *
 * @return A newly allocated `PARCCryptoHash` instance that must be freed via `parcCryptoHash_Release()`
 *
 * Example:
 * @code
 * {
 *     PARCBuffer *buffer = ...
 *     PARCCryptoHash *digest = parcCryptoHash_Create(PARC_HASH_SHA256, buffer);
 *     ...
 *     parcCryptoHash_Release(&digest);
 * }
 * @endcode
 */
PARCCryptoHash *parcCryptoHash_Create(PARCCryptoHashType digestType, PARCBuffer *digest);

/**
 * Increase the number of references to a `PARCCryptoHash` instance.
 *
 * Note that a new `PARCCryptoHash` is not created,
 * only that the given `PARCCryptoHash` reference count is incremented.
 * Discard the reference by invoking {@link parcCryptoHash_Release}.
 *
 * @param [in] hash A pointer to the original instance.
 * @return The value of the input parameter @p instance.
 *
 * Example:
 * @code
 * {
 *     PARCCryptoHash *x = ...
 *     PARCCryptoHash *x2 = parcCryptoHash_Acquire(x);
 *
 *     parcCryptoHash_Release(&x);
 *     parcCryptoHash_Release(&x2);
 * }
 * @endcode
 *
 * @see {@link parcCryptoHash_Release}
 */
PARCCryptoHash *parcCryptoHash_Acquire(const PARCCryptoHash *hash);

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
 * @param [in,out] hashP A pointer to a pointer to the instance to release.
 *
 * Example:
 * @code
 * {
 *     PARCCryptoHash *x = ...
 *
 *     parcCryptoHash_Release(&x);
 * }
 * @endcode
 *
 * @see {@link parcCryptoHash_Acquire}
 */
void parcCryptoHash_Release(PARCCryptoHash **hashP);

/**
 * Create a digest, copying the buffer.
 *
 * @param [in] digestType The type of hash digest algorithm used to compute this digest.
 * @param [in] buffer Pointer to array containing the raw digest bytes.
 * @param [in] length Length of the digest byte array.
 *
 * @return A newly allocated `PARCCryptoHash` instance that must be freed via `parcCryptoHash_Release()`
 *
 * Example:
 * @code
 * {
 *     size_t bufferLen = 32;
 *     uint8_t *buffer = ...
 *     PARCCryptoHash *digest = parcCryptoHash_Create(PARC_HASH_SHA256, buffer, bufferLen);
 *     ...
 *     parcCryptoHash_Release(&digest);
 *     // free the raw buffer as needed
 * }
 * @endcode
 */
PARCCryptoHash *parcCryptoHash_CreateFromArray(PARCCryptoHashType digestType, const void *buffer, size_t length);

/**
 * Destroy the specified `PARCCryptoHash` instance.
 *
 * @param [in,out] parcDigestPtr Pointer to the instance to be destroyed.
 *
 * Example:
 * @code
 * {
 *     PARCBuffer *buffer = ...
 *     PARCCryptoHash *digest = parcCryptoHash_Create(PARC_HASH_SHA256, buffer);
 *     ...
 *     parcCryptoHash_Release(&digest);
 * }
 * @endcode
 */
void parcCryptoHash_Release(PARCCryptoHash **parcDigestPtr);

/**
 * Returns the digest algorithm, of type `PARCCryptoHashType`.
 *
 * @param [in] parcDigest The `PARCCryptoHash` instance being examined.
 *
 * @return A `PARCCryptoHashType` value.
 *
 * Example:
 * @code
 * {
 *     PARCBuffer *buffer = ...
 *     PARCCryptoHash *digest = parcCryptoHash_Create(PARC_HASH_SHA256, buffer);
 *     PARCCryptoHashType type = parcCryptoHash_GetDigestType(digest);
 *     // type will be PARC_HASH_SHA256
 *     ...
 *     parcCryptoHash_Release(&digest);
 * }
 * @endcode
 */
PARCCryptoHashType parcCryptoHash_GetDigestType(const PARCCryptoHash *parcDigest);

/**
 * Returnes the digest buffer, copy if you will destroy
 *
 *   Returns the inner digest buffer. You must copy it if you will make
 *   changes or destroy it.
 *
 * @param [in] cryptoHash The `PARCCryptoHash` instance being examined.
 *
 * @return A `PARCBuffer` instance containing the raw hash digest.
 *
 * Example:
 * @code
 * {
 *     PARCBuffer *buffer = ...
 *     PARCCryptoHash *digest = parcCryptoHash_Create(PARC_HASH_SHA256, buffer);
 *     PARCBuffer *rawDigest = parcCryptoHash_GetDigest(digest);
 *     // use the raw digest as necessary
 *     ...
 *     parcCryptoHash_Release(&digest);
 * }
 * @endcode
 */
PARCBuffer *parcCryptoHash_GetDigest(const PARCCryptoHash *cryptoHash);

/**
 * Determine if two PARCCryptoHash instances are equal.
 *
 * The following equivalence relations on non-null `PARCCryptoHash` instances are maintained:
 *
 *   * It is reflexive: for any non-null reference value x, `parcCryptoHash_Equals(x, x)` must return true.
 *
 *   * It is symmetric: for any non-null reference values x and y, `parcCryptoHash_Equals(x, y)` must return true if and only if
 *        parcCryptoHash_Equals(y x) returns true.
 *
 *   * It is transitive: for any non-null reference values x, y, and z, if
 *        `parcCryptoHash_Equals(x, y)` returns true and
 *        `parcCryptoHash_Equals(y, z)` returns true,
 *        then  `parcCryptoHash_Equals(x, z)` must return true.
 *
 *   * It is consistent: for any non-null reference values x and y, multiple invocations of `parcCryptoHash_Equals(x, y)`
 *         consistently return true or consistently return false.
 *
 *   * For any non-null reference value x, `parcCryptoHash_Equals(x, NULL)` must return false.
 *
 * @param [in] a A pointer to a PARCCryptoHash instance.
 * @param [in] b A pointer to a PARCCryptoHash instance.
 *
 * @return True if the given PARCCryptoHash instances are equal, false otherwise.
 *
 * Example:
 * @code
 * {
 *     PARCBuffer *buffer = ...
 *     PARCCryptoHash *orig = parcCryptoHash_Create(PARC_HASH_SHA256, buffer);
 *     PARCCryptoHash *copy = parcCryptoHash_Create(PARC_HASH_SHA256, buffer);
 *
 *     if (parcCryptoHash_Equals(orig, copy)) {
 *         // true
 *     } else {
 *         // false
 *     }
 *
 *     parcCryptoHash_Release(&orig);
 *     parcCryptoHash_Release(&copy);
 * }
 * @endcode
 */
bool parcCryptoHash_Equals(const PARCCryptoHash *a, const PARCCryptoHash *b);
#endif // libparc_parc_CryptoHash_h
