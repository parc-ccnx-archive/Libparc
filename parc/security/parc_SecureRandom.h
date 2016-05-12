/*
 * Copyright (c) 2015-2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @file parc_SecureRandom.h
 * @brief A cryptographically secure pseudorandom number generator that reads
 * from a secure randomness source on the system, e.g., /dev/urandom.
 *
 * @author Christopher A. Wood, Computing Science Laboratory, PARC
 * @copyright (c) 2015-2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef Libparc_parc_SecureRandom
#define Libparc_parc_SecureRandom

#include <stdbool.h>

#include <parc/algol/parc_Object.h>

struct parc_securerandom;
typedef struct parc_securerandom PARCSecureRandom;

/**
 * Increase the number of references to a `PARCSecureRandom` instance.
 *
 * Note that new `PARCSecureRandom` is not created,
 * only that the given `PARCSecureRandom` reference count is incremented.
 * Discard the reference by invoking `parcSecureRandom_Release`.
 *
 * @param [in] instance A pointer to a valid PARCSecureRandom instance.
 *
 * @return The same value as @p instance.
 *
 * Example:
 * @code
 * {
 *     PARCSecureRandom *a = parcSecureRandom_Create();
 *
 *     PARCSecureRandom *b = parcSecureRandom_Acquire();

 *     parcSecureRandom_Release(&a);
 *     parcSecureRandom_Release(&b);
 * }
 * @endcode
 */
PARCSecureRandom *parcSecureRandom_Acquire(const PARCSecureRandom *instance);

/**
 * Create an instance of PARCSecureRandom
 *
 * @return non-NULL A pointer to a valid PARCSecureRandom instance.
 * @return NULL An error occurred.
 *
 * Example:
 * @code
 * {
 *     PARCSecureRandom *rng = parcSecureRandom_Create();
 *
 *     parcSecureRandom_Release(&rng);
 * }
 * @endcode
 */
PARCSecureRandom *parcSecureRandom_Create(void);

/**
 * Create an instance of PARCSecureRandom with a specific seed stored in
 * a `PARCBuffer` instance.
 *
 * @param [in] seed A `PARCBuffer` instance.
 *
 * @return non-NULL A pointer to a valid PARCSecureRandom instance.
 * @return NULL An error occurred.
 *
 * Example:
 * @code
 * {
 *     PARCBuffer *seed = ...
 *     PARCSecureRandom *rng = parcSecureRandom_CreateWithSeed(seed);
 *
 *     parcSecureRandom_Release(&rng);
 * }
 * @endcode
 */
PARCSecureRandom *parcSecureRandom_CreateWithSeed(PARCBuffer *seed);

/**
 * Generate a 32-bit unsigned integer from a `PARCSecureRandom` instance.
 *
 * @param [in] rng A `PARCSecureRandom` instance.
 *
 * @return non-NULL A pointer to a valid PARCSecureRandom instance.
 * @return NULL An error occurred.
 *
 * Example:
 * @code
 * {
 *     PARCSecureRandom *rng = parcSecureRandom_Create();
 *
 *     uint32_t nextWord = parcSecureRandom_Next(rng);
 *     // use the word
 * }
 * @endcode
 */
uint32_t parcSecureRandom_Next(PARCSecureRandom *rng);

/**
 * Fill a `PARCBuffer` instance with random bytes from a `PARCSecureRandom` instance.
 * The resultant `PARCBuffer` will be ready for reading, i.e., one does not need
 * to call `parcBuffer_Flip()` on the result.
 *
 * @param [in] rng A `PARCSecureRandom` instance.
 * @param [in] buffer A `PARCBuffer` instance to fill.
 *
 * @return non-NULL A pointer to a valid PARCSecureRandom instance.
 * @return NULL An error occurred.
 *
 * Example:
 * @code
 * {
 *     PARCSecureRandom *rng = parcSecureRandom_Create();
 *
 *     PARCBuffer *buffer = parcBuffer_Allocate(32);
 *     ssize_t numBytes = parcSecureRandom_NextBytes(rng, buffer);
 *     // use the buffer
 * }
 * @endcode
 */
ssize_t parcSecureRandom_NextBytes(PARCSecureRandom *rng, PARCBuffer *buffer);

/**
 * Determine if an instance of `PARCSecureRandom` is valid.
 *
 * Valid means the internal state of the type is consistent with its required current or future behaviour.
 * This may include the validation of internal instances of types.
 *
 * @param [in] instance A pointer to a valid PARCSecureRandom instance.
 *
 * @return true The instance is valid.
 * @return false The instance is not valid.
 *
 * Example:
 * @code
 * {
 *     PARCSecureRandom *rng = parcSecureRandom_Create();
 *
 *     if (parcSecureRandom_IsValid(rng)) {
 *         printf("Instance is valid.\n");
 *     }
 *
 *     parcSecureRandom_Release(&rng);
 * }
 * @endcode
 *
 */
bool parcSecureRandom_IsValid(const PARCSecureRandom *instance);

/**
 * Release a previously acquired reference to the given `PARCSecureRandom` instance,
 * decrementing the reference count for the instance.
 *
 * The pointer to the instance is set to NULL as a side-effect of this function.
 *
 * If the invocation causes the last reference to the instance to be released,
 * the instance is deallocated and the instance's implementation will perform
 * additional cleanup and release other privately held references.
 *
 * @param [in,out] instancePtr A pointer to a pointer to the instance to release.
 *
 * Example:
 * @code
 * {
 *     PARCSecureRandom *rng = parcSecureRandom_Create();
 *
 *     parcSecureRandom_Release(&rng);
 * }
 * @endcode
 */
void parcSecureRandom_Release(PARCSecureRandom **instancePtr);
#endif
