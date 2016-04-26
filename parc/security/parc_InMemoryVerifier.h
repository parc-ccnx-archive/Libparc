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
 * @file parc_InMemoryVerifier.h
 * @ingroup security
 * @brief In memory verifier
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libparc_parc_InMemoryVerifier_h
#define libparc_parc_InMemoryVerifier_h

#include <parc/security/parc_Verifier.h>

struct parc_inmemory_verifier;
typedef struct parc_inmemory_verifier PARCInMemoryVerifier;

/**
 * Create an empty verifier.   It's destroyed via the PARCVerifierInterface->Destroy call.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCInMemoryVerifier *parcInMemoryVerifier_Create(void);

/**
 * Increase the number of references to the given `PARCInMemoryVerifier` instance.
 *
 * A new instance is not created,
 * only that the given instance's reference count is incremented.
 * Discard the acquired reference by invoking `parcInMemoryVerifier_Release()`.
 *
 * @param [in] signer A pointer to a `PARCInMemoryVerifier` instance.
 *
 * @return NULL An error occurred.
 * @return non-NULL A pointer to a PARCInMemoryVerifier instance.
 *
 * Example:
 * @code
 * {
 *      PARCInMemoryVerifier *verifier = parcInMemoryVerifier_Create();
 *      PARCInMemoryVerifier *handle = parcInMemoryVerifier_Acquire(signer);
 *      // use the handle instance as needed
 * }
 * @endcode
 */
PARCInMemoryVerifier *parcInMemoryVerifier_Acquire(const PARCInMemoryVerifier *verifier);

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
 *     PARCInMemoryVerifier *verifier = parcInMemoryVerifier_Create();
 *
 *     parcInMemoryVerifier_Release(&verifier);
 * }
 * @endcode
 */
void parcInMemoryVerifier_Release(PARCInMemoryVerifier **verifierPtr);
#endif // libparc_parc_InMemoryVerifier_h
