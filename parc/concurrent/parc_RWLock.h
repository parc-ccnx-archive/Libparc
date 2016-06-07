/*
 * Copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @file parc_RWLock.h
 * @brief <#Brief Description#>
 *
 * <#Detailed Description#>
 *
 * @author <#Greg Rutz <g.rutz@cablelabs.com>#>, Cable Television Laboratories, Inc. (CableLabs)
 * @copyright (c) 2016, Cable Television Laboratories, Inc.  All rights reserved.
 */
#ifndef PARCLibrary_parc_RWLock
#define PARCLibrary_parc_RWLock
#include <stdbool.h>

#include <parc/algol/parc_HashCode.h>
#include <parc/algol/parc_JSON.h>

struct PARCRWLock;
typedef struct PARCRWLock PARCRWLock;

/**
 * Increase the number of references to a `PARCRWLock` instance.
 *
 * Note that new `PARCRWLock` is not created,
 * only that the given `PARCRWLock` reference count is incremented.
 * Discard the reference by invoking `parcRWLock_Release`.
 *
 * @param [in] instance A pointer to a valid PARCRWLock instance.
 *
 * @return The same value as @p instance.
 *
 * Example:
 * @code
 * {
 *     PARCRWLock *a = parcRWLock_Create();
 *
 *     PARCRWLock *b = parcRWLock_Acquire();
 *
 *     parcRWLock_Release(&a);
 *     parcRWLock_Release(&b);
 * }
 * @endcode
 */
PARCRWLock *parcRWLock_Acquire(const PARCRWLock *instance);

#ifdef PARCLibrary_DISABLE_VALIDATION
#  define parcRWLock_OptionalAssertValid(_instance_)
#else
#  define parcRWLock_OptionalAssertValid(_instance_) parcRWLock_AssertValid(_instance_)
#endif

/**
 * Assert that the given `PARCRWLock` instance is valid.
 *
 * @param [in] instance A pointer to a valid PARCRWLock instance.
 *
 * Example:
 * @code
 * {
 *     PARCRWLock *a = parcRWLock_Create();
 *
 *     parcRWLock_AssertValid(a);
 *
 *     printf("Instance is valid.\n");
 *
 *     parcRWLock_Release(&b);
 * }
 * @endcode
 */
void parcRWLock_AssertValid(const PARCRWLock *instance);

/**
 * Create an instance of PARCRWLock
 *
 * <#Paragraphs Of Explanation#>
 *
 * @return non-NULL A pointer to a valid PARCRWLock instance.
 * @return NULL An error occurred.
 *
 * Example:
 * @code
 * {
 *     PARCRWLock *a = parcRWLock_Create();
 *
 *     parcRWLock_Release(&a);
 * }
 * @endcode
 */
PARCRWLock *parcRWLock_Create(void);

/**
 * Compares @p instance with @p other for order.
 *
 * Returns a negative integer, zero, or a positive integer as @p instance
 * is less than, equal to, or greater than @p other.
 *
 * @param [in] instance A pointer to a valid PARCRWLock instance.
 * @param [in] other A pointer to a valid PARCRWLock instance.
 *
 * @return <0 Instance is less than @p other.
 * @return 0 Instance a and instance b compare the same.
 * @return >0 Instance a is greater than instance b.
 *
 * Example:
 * @code
 * {
 *     PARCRWLock *a = parcRWLock_Create();
 *     PARCRWLock *b = parcRWLock_Create();
 *
 *     if (parcRWLock_Compare(a, b) == 0) {
 *         printf("Instances are equal.\n");
 *     }
 *
 *     parcRWLock_Release(&a);
 *     parcRWLock_Release(&b);
 * }
 * @endcode
 *
 * @see parcRWLock_Equals
 */
int parcRWLock_Compare(const PARCRWLock *instance, const PARCRWLock *other);

/**
 * Determine if an instance of `PARCRWLock` is valid.
 *
 * Valid means the internal state of the type is consistent with its required current or future behaviour.
 * This may include the validation of internal instances of types.
 *
 * @param [in] instance A pointer to a valid PARCRWLock instance.
 *
 * @return true The instance is valid.
 * @return false The instance is not valid.
 *
 * Example:
 * @code
 * {
 *     PARCRWLock *a = parcRWLock_Create();
 *
 *     if (parcRWLock_IsValid(a)) {
 *         printf("Instance is valid.\n");
 *     }
 *
 *     parcRWLock_Release(&a);
 * }
 * @endcode
 *
 */
bool parcRWLock_IsValid(const PARCRWLock *instance);

/**
 * Release a previously acquired reference to the given `PARCRWLock` instance,
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
 *     PARCRWLock *a = parcRWLock_Create();
 *
 *     parcRWLock_Release(&a);
 * }
 * @endcode
 */
void parcRWLock_Release(PARCRWLock **instancePtr);

/**
 * Obtain the READ lock on the given `PARCRWLock` instance.  
 *
 * If the WRITE lock is already held by another thread, this function will block.
 * If the READ lock is already held by another thread, this function will increment the READ
 * lock counter and continue.
 * If the READ lock is already held by the current thread, this function will increment the
 * read lock counter and continue
 * 
 * Implementors should avoid deadlock by not attempting to acquire the READ lock with a
 * thread that already owns the WRITE lock.
 *
 * @param [in] lock A pointer to a valid `PARCRWLock` instance.
 *
 * @return true The READ lock was obtained successfully.
 * @return false The WRITE lock is already held by the current thread, the maximum READ lock counter has
 * been reached, or the `PARCRWLock` is invalid.
 *
 * Example:
 * @code
 * {
 *     if (parcRWLock_ReadLock(lock)) {
 *
 *     }
 * }
 * @endcode
 */
bool parcRWLock_ReadLock(PARCRWLock *lock);

/**
 * Obtain the lock on the given `PARCRWLock` instance.
 *
 * If the WRITE lock or READ lock is already held by another thread, this function will block.
 * If the lock is already held by the current thread, this function will return `false`.
 *
 * @param [in] lock A pointer to a valid `PARCRWLock` instance.
 *
 * @return true The lock was obtained successfully.
 * @return false The READ lock or WRITE lock is already held by the current thread, or the
 * `PARCRWLock` is invalid.
 *
 * Example:
 * @code
 * {
 *     if (parcRWLock_WriteLock(lock)) {
 *
 *     }
 * }
 * @endcode
 */
bool parcRWLock_WriteLock(PARCRWLock *lock);

/**
 * Try to obtain the READ lock on the given `PARCRWLock` instance.
 *
 * Once the lock is obtained, the caller must release the lock via `parcRWLock_Unlock`.
 *
 * @param [in] lock A pointer to a valid `PARCRWLock` instance.
 *
 * @return true A `PARCRWLock` READ lock was obtained
 * @return false A `PARCRWLock` READ lock was not unlocked.
 *
 * Example:
 * @code
 * {
 *     while (parcRWLock_TryReadLock(object))
 *         ;
 * }
 * @endcode
 */
bool parcRWLock_TryReadLock(PARCRWLock *lock);

/**
 * Try to obtain the WRITE lock on the given `PARCObject` instance.
 *
 * Once the lock is obtained, the caller must release the lock via `parcRWLock_Unlock`.
 *
 * @param [in] lock A pointer to a valid `PARCRWLock` instance.
 *
 * @return true The `PARCRWLock` WRITE lock was obtained.
 * @return false The `PARCRWLock` WRITE lock was not obtained
 *
 * Example:
 * @code
 * {
 *     while (parcRWLock_TryWriteLock(object))
 *         ;
 * }
 * @endcode
 */
bool parcRWLock_TryWriteLock(PARCRWLock *lock);

/**
 * Try to unlock the READ or WRITE lock on the given PARCObject instance.
 *
 * @param [in] lock A pointer to a valid PARCRWLock instance.
 *
 * @return true The PARCRWLock READ or WRITE lock was released by this thread.  Note this may
 * not leave the PARCRWLock unlocked, since other threads may also hold a READ lock.
 * @return false The PARCRWLock READ or WRITE lock was not released, either because the
 * PARCRWLock object was not initialized or because the calling thread did not hold a lock on
 * this object.
 *
 * Example:
 * @code
 * {
 *     parcRWLock_Unlock(object);
 * }
 * @endcode
 */
bool parcRWLock_Unlock(PARCRWLock *lock);

#endif
