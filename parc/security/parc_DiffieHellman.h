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
 * @file parc_DiffieHellman.h
 * @ingroup security
 * @brief A factory for Diffie Hellman parameters.
 *
 * @author Christopher A. Wood, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_DiffieHellman_h
#define libparc_parc_DiffieHellman_h

#include <parc/security/parc_DiffieHellmanGroup.h>
#include <parc/security/parc_DiffieHellmanKeyShare.h>

struct parc_diffie_hellman;
typedef struct parc_diffie_hellman PARCDiffieHellman;

/**
 * Create an instance of `PARCDiffieHellman.` that generates Diffie Hellman shares
 * for the specified key exchange mechanisms.
 *
 * @param [in] groupType A type of PARCDiffieHellmanGroup
 *
 * @return NULL Memory could not be allocated.
 * @return non-NULL A pointer to a `PARCDiffieHellman` instance.
 *
 * Example:
 * @code
 * {
 *     PARCDiffieHellman *dh = parcDiffieHellman_Create(PARCDiffieHellmanGroup_Secp521r1);
 *
 *     parcDiffieHellman_Release(&dh);
 * }
 * @endcode
 */
PARCDiffieHellman *parcDiffieHellman_Create(PARCDiffieHellmanGroup groupType);

/**
 * Increase the number of references to an instance of this object.
 *
 * Note that new instance is not created, only that the given instance's reference count
 * is incremented. Discard the reference by invoking `parcDiffieHellman_Release()`.
 *
 * @param [in] dh A `PARCDiffieHellman` instance.
 *
 * @return The value of the input parameter @p instance.
 *
 * Example:
 * @code
 * {
 *     ...
 *
 *     PARCDiffieHellman *dh = parcDiffieHellman_Acquire(dhInstance);
 *
 *     parcDiffieHellman_Release(&dh);
 * }
 * @endcode
 *
 * @see parcDiffieHellman_Release
 */
PARCDiffieHellman *parcDiffieHellman_Acquire(const PARCDiffieHellman *dh);

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
 * @param [in,out] dhP A pointer to a pointer to the instance to release.
 *
 * Example:
 * @code
 * {
 *     PARCDiffieHellman *dh = parcDiffieHellman_Create(PARCDiffieHellmanGroup_Secp521r1);
 *
 *     parcDiffieHellman_Release(&dh);
 * }
 * @endcode
 */
void parcDiffieHellman_Release(PARCDiffieHellman **dhP);





PARCDiffieHellmanKeyShare *parcDiffieHellman_GenerateKeyShare(PARCDiffieHellman *dh);

#endif // libparc_parc_DiffieHellman_h
