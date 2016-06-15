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
 * @file parc_CryptoHasher.h
 * @ingroup security
 * @brief Computes digests of bytes or PARCBuffers.
 *
 * The PARCCryptoHasher computes digests of bytes or PARCBuffers.
 * It produces a PARCCryptoHash (without the "er"), which contains the
 * digest and the algorithm used to compute the digest.
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */

#ifndef libparc_parc_CryptoHasher_h
#define libparc_parc_CryptoHasher_h

#include <parc/algol/parc_Buffer.h>
#include <parc/security/parc_CryptoHash.h>

struct parc_crypto_hasher;
typedef struct parc_crypto_hasher PARCCryptoHasher;

typedef struct parc_crypto_hasher_interface {
    void *functor_env;

    /**
     * Called with the environment and returns the setup context.
     *
     * @param [in] env The context environment (modified if needed).
     */
    void *(*hasher_setup)(void *env);

    /**
     * Setup the local context for the cryptographic hasher.
     *
     * These operate on the setup context, not the environment.
     *
     * @param [in] setup_ctx The local context that is initialized.
     *
     * @return 0 Successful initialization
     * @return -1 An error occurred
     */
    int (*hasher_init)(void *setup_ctx);

    /**
     * Updated the digest using raw bytes
     *
     * @param [in] setup_ctx The local context for the hash digester
     * @param [in] buffer Pointer to an array containing hte raw bytes used to update the digester
     * @param [in] length Length of the input byte array
     *
     * @return 0 Successul update
     * @return -1 An error occurred
     */
    int (*hasher_update)(void *setup_ctx, const void *buffer, size_t length);

    /**
     * Finalize the digest.  Appends the digest to the output buffer, which
     * the user must allocate.
     *
     * @param [in] setup_ctx The local context for the hash digester
     *
     * @return non-NULL A `PARCBuffer` containing the final hash digest.
     * @return NULL An error ocurred
     */
    PARCBuffer* (*hasher_finalize)(void *setup_ctx);

    /**
     * Destroy the digester, releasing internal references as needed.
     *
     * @param [in] setup_ctx A pointer to a local context to destroy.
     */
    void (*hasher_destroy)(void **setup_ctx);
} PARCCryptoHasherInterface;

/**
 * Create one of the pre-defined cryptographic hash "digesters" from the
 * available `PARCCryptoHashType` types.
 *
 * @param [in] type A `PARCCryptoHashType` value.
 *
 * @return A newly allocated `PARCCryptoHasher` instance that must be freed by `parcCryptoHasher_Release()`
 *
 * Example:
 * @code
 * {
 *     PARCCryptoHasher *digester = parcCryptoHasher_Create(PARCCryptoHashType_SHA256);
 *     // initialize if needed
 *     // update bytes or finalize as needed
 *     parcCryptoHasher_Release(&digester);
 * }
 * @endcode
 */
PARCCryptoHasher *parcCryptoHasher_Create(PARCCryptoHashType type);


/**
 * Creates a custom hasher using the provided functor.  Useful for
 * implementing HMAC [RFC 2104] without leaking the key outside the keystore.
 *
 * The functor may carry an environment (i.e. info from the keystore) that will
 * be echod back when CryptoHasher call the <code>functor->hasher_setup(functor->functor_env)</code>.
 * All subsequent calls are passed the setup context
 *
 * Example:
 * @code
 * {
 *      static PARCCryptoHasherInterface hash_crc32c_template = {
 *          .functor_env    = NULL,
 *          .hasher_setup   = crc32cHasher_setup,
 *          .hasher_init    = crc32cHasher_init,
 *          .hasher_update  = crc32cHasher_update,
 *          .hasher_finalize= crc32cHasher_finalize,
 *          .hasher_destroy = crc32cHasher_destroy
 *      };
 *
 *      CRC32CSigner *crc32Signer = parcMemory_AllocateAndClear(sizeof(CRC32CSigner));
 *      crc32Signer->hasher_functor = hash_crc32c_template;
 *      crc32Signer->hasher_functor.functor_env = crc32Signer;
 *      crc32Signer->hasher = parcCryptoHasher_CustomHasher(PARCCryptoHashType_CRC32C, crc32Signer->hasher_functor);
 *
 *      PARCSigningInterface *signer = parcMemory_AllocateAndClear(sizeof(PARCSigningInterface));
 *      *signer = crc32signerimpl_template;
 *      signer->interfaceContext crc32Signer;
 *      return signer;
 * }
 * @endcode
 */
PARCCryptoHasher *parcCryptoHasher_CustomHasher(PARCCryptoHashType type, PARCCryptoHasherInterface functor);

/**
 * Increase the number of references to a `PARCCryptoHasher` instance.
 *
 * Note that a new `PARCCryptoHasher` is not created,
 * only that the given `PARCCryptoHasher` reference count is incremented.
 * Discard the reference by invoking {@link parcCryptoHasher_Release}.
 *
 * @param [in] hasher A pointer to the original instance.
 * @return The value of the input parameter @p instance.
 *
 * Example:
 * @code
 * {
 *     PARCCryptoHasher *x = ...
 *     PARCCryptoHasher *x2 = parcCryptoHasher_Acquire(x);
 *
 *     parcCryptoHasher_Release(&x);
 *     parcCryptoHasher_Release(&x2);
 * }
 * @endcode
 *
 * @see {@link parcCryptoHasher_Release}
 */
PARCCryptoHasher *parcCryptoHasher_Acquire(const PARCCryptoHasher *hasher);

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
 * @param [in,out] hasherP A pointer to a pointer to the instance to release.
 *
 * Example:
 * @code
 * {
 *     PARCCryptoHasher *x = ...
 *
 *     parcCryptoHasher_Release(&x);
 * }
 * @endcode
 *
 * @see {@link parcCryptoHasher_Acquire}
 */
void parcCryptoHasher_Release(PARCCryptoHasher **hasherP);

/**
 * Reset the internal state of the digest to start a new session.
 *
 * @param [in] digester A `PARCCryptoHasher` instance.
 *
 * @return 0 Successful reset
 * @return -1 Some failure occurred
 *
 * Example:
 * @code
 * {
 *     PARCCryptoHasher *digester = parcCryptoHasher_Create(PARCCryptoHashType_SHA256);
 *     parcCryptoHasher_Init(digester);
 *     // update bytes or finalize as needed
 *     parcCryptoHasher_Release(&digester);
 * }
 * @endcode
 */
int parcCryptoHasher_Init(PARCCryptoHasher *digester);

/**
 * Add bytes to the digest.
 *
 * @param [in] hasher A `PARCCryptoHasher` instance.
 * @param [in] buffer A pointer to a raw buffer with bytes to update the hash digest.
 * @param [in] length Length of the input byte array.
 *
 * @return 0 Successfully added bytes to the digest internally.
 * @return -1 Some failure occurred
 *
 * Example:
 * @code
 * {
 *     PARCCryptoHasher *digester = parcCryptoHasher_Create(PARCCryptoHashType_SHA256);
 *     parcCryptoHasher_Init(digester);
 *     ...
 *     uint8_t *buffer = ...
 *     size_t bufferLen = 32;
 *     parcCryptoHasher_UpdateBytes(digester, buffer, bufferLen);
 *     // update bytes or finalize as needed
 *     parcCryptoHasher_Release(&digester);
 * }
 * @endcode
 */
int parcCryptoHasher_UpdateBytes(PARCCryptoHasher *hasher, const void *buffer, size_t length);

/**
 * Add bytes to the digest. The bytes used are those starting at the
 * specified buffer's "position" value.
 *
 * @param [in] hasher A `PARCCryptoHasher` instance.
 * @param [in] buffer A `PARCBuffer` instance containing the bytes to add to the digest.
 *
 * @return 0 Successfully added bytes to the digest internally.
 * @return -1 Some failure occurred
 *
 * Example:
 * @code
 * {
 *     PARCCryptoHasher *digester = parcCryptoHasher_Create(PARCCryptoHashType_SHA256);
 *     parcCryptoHasher_Init(digester);
 *     ...
 *     PARCBuffer *buffer = ...
 *     parcCryptoHasher_UpdateBuffer(digester, buffer);
 *     // update bytes or finalize as needed
 *     parcCryptoHasher_Release(&digester);
 * }
 * @endcode
 */
int parcCryptoHasher_UpdateBuffer(PARCCryptoHasher *hasher, const PARCBuffer *buffer);

/**
 * Finalize the digest.  Appends the digest to the output buffer, which
 * the user must allocate.
 *
 * @param [in] hasher A `PARCCryptoHasher` instance.
 *
 * @return The output buffer - the final digest from the hash function computation.
 *
 * Example:
 * @code
 * {
 *     PARCCryptoHasher *digester = parcCryptoHasher_Create(PARCCryptoHashType_SHA256);
 *     parcCryptoHasher_Init(digester);
 *     ...
 *     PARCBuffer *buffer = ...
 *     parcCryptoHasher_UpdateBuffer(digester, buffer);
 *     PARCBuffer *hashDigest = parcCryptoHasher_Finalize(digester);
 *     // use the hashDigest as needed
 *     parcBuffer_Release(&hashDigest);
 *     parcCryptoHasher_Release(&digester);
 * }
 * @endcode
 */
PARCCryptoHash *parcCryptoHasher_Finalize(PARCCryptoHasher *hasher);

/**
 * Destroy the digester, releasing internal references as needed.
 *
 * @param [in] hasherPtr A pointer to a `PARCCryptoHasher` instance.
 *
 * Example:
 * @code
 * {
 *     PARCCryptoHasher *digester = parcCryptoHasher_Create(PARCCryptoHashType_SHA256);
 *     parcCryptoHasher_Init(digester);
 *     ...
 *     parcCryptoHasher_Release(&digester);
 * }
 * @endcode
 */
void parcCryptoHasher_Release(PARCCryptoHasher **hasherPtr);
#endif // libparc_parc_CryptoHasher_h
