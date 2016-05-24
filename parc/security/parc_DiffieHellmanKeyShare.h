/*
 * Copyright (c) 2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @file parc_DiffieHellmanKeyShare.h
 * @ingroup security
 * @brief A Diffie Hellman key share.
 *
 * @author Christopher A. Wood, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_DiffieHellmanKeyShare_h
#define libparc_parc_DiffieHellmanKeyShare_h

#include <parc/security/parc_DiffieHellmanGroup.h>

struct parc_diffie_hellman_keyshare;
typedef struct parc_diffie_hellman_keyshare PARCDiffieHellmanKeyShare;

/**
 * Create a `PARCDiffieHellmanKeyShare` instance to hold one public and private
 * Diffie Hellman key share for the specified group.
 *
 * @param [in] groupType A type of PARCDiffieHellmanGroup
 *
 * @return NULL Memory could not be allocated.
 * @return non-NULL A pointer to a `PARCDiffieHellmanKeyShare` instance.
 *
 * Example:
 * @code
 * {
 *     PARCDiffieHellmanKeyShare *keyShare = parcDiffieHellmanKeyShare_Create(PARCDiffieHellmanGroup_Secp521r1);
 *
 *     parcDiffieHellmanKeyShare_Release(&keyShare);
 * }
 * @endcode
 */
PARCDiffieHellmanKeyShare *parcDiffieHellmanKeyShare_Create(PARCDiffieHellmanGroup groupType);

/**
 * Increase the number of references to an instance of this object.
 *
 * Note that new instance is not created, only that the given instance's reference count
 * is incremented. Discard the reference by invoking `parcDiffieHellman_Release()`.
 *
 * @param [in] keyShare A `PARCDiffieHellmanKeyShare` instance.
 *
 * @return The value of the input parameter @p instance.
 *
 * Example:
 * @code
 * {
 *     ...
 *
 *     PARCDiffieHellmanKeyShare *keyShare = parcDiffieHellmanKeyShare_Acquire(keyShareInstance);
 *
 *     parcDiffieHellmanKeyShare_Release(&keyShare);
 * }
 * @endcode
 *
 * @see parcDiffieHellmanKeyShare_Release
 */
PARCDiffieHellmanKeyShare *parcDiffieHellmanKeyShare_Acquire(const PARCDiffieHellmanKeyShare *keyShare);

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
 * @param [in,out] keyShareP A pointer to a pointer to the instance to release.
 *
 * Example:
 * @code
 * {
 *     ...
 *
 *     PARCDiffieHellmanKeyShare *keyShare = parcDiffieHellmanKeyShare_Acquire(keyShareInstance);
 *
 *     parcDiffieHellmanKeyShare_Release(&keyShare);
 * }
 * @endcode
 */
void parcDiffieHellmanKeyShare_Release(PARCDiffieHellmanKeyShare **keyShareP);

/**
 * Serialize the public key part of a `PARCDiffieHellmanKeyShare.`
 *
 * The public key is saved to a `PARCBuffer` and can be used for transport if needed.
 *
 * @param [in] keyShare A `PARCDiffieHellmanKeyShare` instance.
 *
 * @return A `PARCBuffer` containing the public key of this key share.
 *
 * Example:
 * @code
 * {
 *     PARCDiffieHellmanKeyShare *keyShare = parcDiffieHellmanKeyShare_Create(PARCDiffieHellmanGroup_Secp521r1);
 *
 *     PARCBuffer *publicKey = parcDiffieHellmanKeyShare_SerializePublicKey(keyShare);
 *     // use the public key
 *
 *     parcBuffer_Release(&publicKey);
 *     parcDiffieHellmanKeyShare_Release(&keyShare);
 * }
 * @endcode
 */ 
PARCBuffer *parcDiffieHellmanKeyShare_SerializePublicKey(PARCDiffieHellmanKeyShare *keyShare);

/**
 * Combine a `PARCDiffieHellmanKeyShare` with an encoded public key to create a shared secret.
 *
 * @param [in] keyShare A `PARCDiffieHellmanKeyShare` instance.
 * @param [in] publicShare The public key share to use to derive the shared secrect.
 *
 * @return A `PARCBuffer` containing the shared secret from the Diffie Hellman exchange.
 *
 * Example:
 * @code
 * {
 *     PARCDiffieHellmanKeyShare *keyShare = parcDiffieHellmanKeyShare_Create(PARCDiffieHellmanGroup_Secp521r1);
 *
 *     PARCBuffer *publicKey = parcDiffieHellmanKeyShare_SerializePublicKey(keyShare);
 *     
 *     ...
 *
 *     PARCBuffer *sharedSecret = parcDiffieHellmanKeyShare_Combine(keyShare, publicKey);
 *     // use the shared secret to derive other cryptographic secrets.
 *
 *     parcBuffer_Release(&sharedSecret);
 *     parcBuffer_Release(&publicKey);
 *     parcDiffieHellmanKeyShare_Release(&keyShare);
 * }
 * @endcode
 */ 
PARCBuffer *parcDiffieHellmanKeyShare_Combine(PARCDiffieHellmanKeyShare *keyShare, PARCBuffer *publicShare);
#endif // libparc_parc_DiffieHellmanKeyShare_h
