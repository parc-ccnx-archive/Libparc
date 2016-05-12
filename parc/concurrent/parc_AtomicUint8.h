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
 * @file parc_AtomicUint8.h
 * @brief An atomically updated 8-bit unsigned integer.
 *
 * <#Detailed Description#>
 *
 * @author <#Glenn Scott <Glenn.Scott@parc.com>#>, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef PARCLibrary_parc_AtomicUint8
#define PARCLibrary_parc_AtomicUint8
#include <stdbool.h>
#include <stdint.h>

#include <parc/algol/parc_JSON.h>

#ifdef PARCLibrary_DISABLE_ATOMICS
#include <pthread.h>
struct PARCAtomicUint8;
typedef struct PARCAtomicUint8 PARCAtomicUint8;
#else
typedef uint8_t PARCAtomicUint8;
#endif


PARCAtomicUint8 *parcAtomicInteger_CreateUint8(uint8_t value);

uint8_t parcAtomicUint8_AddImpl(PARCAtomicUint8 *value, uint8_t addend);

uint8_t parcAtomicUint8_SubtractImpl(PARCAtomicUint8 *value, uint8_t subtrahend);

bool parcAtomicUint8_CompareAndSwapImpl(PARCAtomicUint8 *value, uint8_t predicate, uint8_t newValue);

#ifdef PARCLibrary_DISABLE_ATOMICS

#define parcAtomicUint8_Add                parcAtomicUint8_AddImpl

#define parcAtomicUint8_Subtract           parcAtomicUint8_SubtractImpl

#define parcAtomicUint8_CompareAndSwap     parcAtomicUint8_CompareAndSwapImpl

#else

#define parcAtomicUint8_Add(_atomic_uint8_, _addend_) \
    __sync_add_and_fetch(_atomic_uint8_, _addend_)

#define parcAtomicUint8_Subtract(_atomic_uint8_, _subtrahend_) \
    __sync_sub_and_fetch(_atomic_uint8_, _subtrahend_)

#define parcAtomicUint8_CompareAndSwap(_atomic_uint8_, _predicate_, _newValue_) \
    __sync_bool_compare_and_swap(_atomic_uint8_, _predicate_, _newValue_)
#endif

#define parcAtomicUint8_Increment(_atomic_uint8_) parcAtomicUint8_Add(_atomic_uint8_, 1)
#define parcAtomicUint8_Decrement(_atomic_uint8_) parcAtomicUint8_Subtract(_atomic_uint8_, 1)

/**
 * Increase the number of references to a `PARCAtomicUint8` instance.
 *
 * Note that new `PARCAtomicUint8` is not created,
 * only that the given `PARCAtomicUint8` reference count is incremented.
 * Discard the reference by invoking `parcAtomicUint8_Release`.
 *
 * @param [in] instance A pointer to a valid PARCAtomicUint8 instance.
 *
 * @return The same value as @p instance.
 *
 * Example:
 * @code
 * {
 *     PARCAtomicUint8 *a = parcAtomicUint8_Create();
 *
 *     PARCAtomicUint8 *b = parcAtomicUint8_Acquire();
 *
 *     parcAtomicUint8_Release(&a);
 *     parcAtomicUint8_Release(&b);
 * }
 * @endcode
 */
PARCAtomicUint8 *parcAtomicUint8_Acquire(const PARCAtomicUint8 *instance);

#ifdef PARCLibrary_DISABLE_VALIDATION
#  define parcAtomicUint8_OptionalAssertValid(_instance_)
#else
#  define parcAtomicUint8_OptionalAssertValid(_instance_) parcAtomicUint8_AssertValid(_instance_)
#endif

/**
 * Assert that the given `PARCAtomicUint8` instance is valid.
 *
 * @param [in] instance A pointer to a valid PARCAtomicUint8 instance.
 *
 * Example:
 * @code
 * {
 *     PARCAtomicUint8 *a = parcAtomicUint8_Create();
 *
 *     parcAtomicUint8_AssertValid(a);
 *
 *     printf("Instance is valid.\n");
 *
 *     parcAtomicUint8_Release(&b);
 * }
 * @endcode
 */
void parcAtomicUint8_AssertValid(const PARCAtomicUint8 *instance);

/**
 * Create an instance of PARCAtomicUint8
 *
 * <#Paragraphs Of Explanation#>
 *
 * @return non-NULL A pointer to a valid PARCAtomicUint8 instance.
 * @return NULL An error occurred.
 *
 * Example:
 * @code
 * {
 *     PARCAtomicUint8 *a = parcAtomicUint8_Create();
 *
 *     parcAtomicUint8_Release(&b);
 * }
 * @endcode
 */
PARCAtomicUint8 *parcAtomicUint8_Create(uint8_t);

/**
 * Compares @p instance with @p other for order.
 *
 * Returns a negative integer, zero, or a positive integer as @p instance
 * is less than, equal to, or greater than @p other.
 *
 * @param [in] instance A pointer to a valid PARCAtomicUint8 instance.
 * @param [in] other A pointer to a valid PARCAtomicUint8 instance.
 *
 * @return <0 Instance is less than @p other.
 * @return 0 Instance a and instance b compare the same.
 * @return >0 Instance a is greater than instance b.
 *
 * Example:
 * @code
 * {
 *     PARCAtomicUint8 *a = parcAtomicUint8_Create();
 *     PARCAtomicUint8 *b = parcAtomicUint8_Create();
 *
 *     if (parcAtomicUint8_Compare(a, b) == 0) {
 *         printf("Instances are equal.\n");
 *     }
 *
 *     parcAtomicUint8_Release(&a);
 *     parcAtomicUint8_Release(&b);
 * }
 * @endcode
 *
 * @see parcAtomicUint8_Equals
 */
int parcAtomicUint8_Compare(const PARCAtomicUint8 *instance, const PARCAtomicUint8 *other);

/**
 * Create an independent copy the given `PARCBuffer`
 *
 * A new buffer is created as a complete copy of the original.
 *
 * @param [in] original A pointer to a valid PARCAtomicUint8 instance.
 *
 * @return NULL Memory could not be allocated.
 * @return non-NULL A pointer to a new `PARCAtomicUint8` instance.
 *
 * Example:
 * @code
 * {
 *     PARCAtomicUint8 *a = parcAtomicUint8_Create();
 *
 *     PARCAtomicUint8 *copy = parcAtomicUint8_Copy(&b);
 *
 *     parcAtomicUint8_Release(&b);
 *     parcAtomicUint8_Release(&copy);
 * }
 * @endcode
 */
PARCAtomicUint8 *parcAtomicUint8_Copy(const PARCAtomicUint8 *original);

/**
 * Determine if two `PARCAtomicUint8` instances are equal.
 *
 * The following equivalence relations on non-null `PARCAtomicUint8` instances are maintained: *
 *   * It is reflexive: for any non-null reference value x, `parcAtomicUint8_Equals(x, x)` must return true.
 *
 *   * It is symmetric: for any non-null reference values x and y, `parcAtomicUint8_Equals(x, y)` must return true if and only if
 *        `parcAtomicUint8_Equals(y x)` returns true.
 *
 *   * It is transitive: for any non-null reference values x, y, and z, if
 *        `parcAtomicUint8_Equals(x, y)` returns true and
 *        `parcAtomicUint8_Equals(y, z)` returns true,
 *        then `parcAtomicUint8_Equals(x, z)` must return true.
 *
 *   * It is consistent: for any non-null reference values x and y, multiple invocations of `parcAtomicUint8_Equals(x, y)`
 *         consistently return true or consistently return false.
 *
 *   * For any non-null reference value x, `parcAtomicUint8_Equals(x, NULL)` must return false.
 *
 * @param [in] x A pointer to a valid PARCAtomicUint8 instance.
 * @param [in] y A pointer to a valid PARCAtomicUint8 instance.
 *
 * @return true The instances x and y are equal.
 *
 * Example:
 * @code
 * {
 *     PARCAtomicUint8 *a = parcAtomicUint8_Create();
 *     PARCAtomicUint8 *b = parcAtomicUint8_Create();
 *
 *     if (parcAtomicUint8_Equals(a, b)) {
 *         printf("Instances are equal.\n");
 *     }
 *
 *     parcAtomicUint8_Release(&a);
 *     parcAtomicUint8_Release(&b);
 * }
 * @endcode
 * @see parcAtomicUint8_HashCode
 */
bool parcAtomicUint8_Equals(const PARCAtomicUint8 *x, const PARCAtomicUint8 *y);

/**
 * Returns a hash code value for the given instance.
 *
 * The general contract of `HashCode` is:
 *
 * Whenever it is invoked on the same instance more than once during an execution of an application,
 * the `HashCode` function must consistently return the same value,
 * provided no information used in a corresponding comparisons on the instance is modified.
 *
 * This value need not remain consistent from one execution of an application to another execution of the same application.
 * If two instances are equal according to the {@link parcAtomicUint8_Equals} method,
 * then calling the {@link parcAtomicUint8_HashCode} method on each of the two instances must produce the same integer result.
 *
 * It is not required that if two instances are unequal according to the
 * {@link parcAtomicUint8_Equals} function,
 * then calling the `parcAtomicUint8_HashCode`
 * method on each of the two objects must produce distinct integer results.
 *
 * @param [in] instance A pointer to a valid PARCAtomicUint8 instance.
 *
 * @return The hashcode for the given instance.
 *
 * Example:
 * @code
 * {
 *     PARCAtomicUint8 *a = parcAtomicUint8_Create();
 *
 *     PARCHashCode hashValue = parcAtomicUint8_HashCode(buffer);
 *     parcAtomicUint8_Release(&a);
 * }
 * @endcode
 */
PARCHashCode parcAtomicUint8_HashCode(const PARCAtomicUint8 *instance);

/**
 * Determine if an instance of `PARCAtomicUint8` is valid.
 *
 * Valid means the internal state of the type is consistent with its required current or future behaviour.
 * This may include the validation of internal instances of types.
 *
 * @param [in] instance A pointer to a valid PARCAtomicUint8 instance.
 *
 * @return true The instance is valid.
 * @return false The instance is not valid.
 *
 * Example:
 * @code
 * {
 *     PARCAtomicUint8 *a = parcAtomicUint8_Create();
 *
 *     if (parcAtomicUint8_IsValid(a)) {
 *         printf("Instance is valid.\n");
 *     }
 *
 *     parcAtomicUint8_Release(&b);
 * }
 * @endcode
 */
bool parcAtomicUint8_IsValid(const PARCAtomicUint8 *instance);

/**
 * Release a previously acquired reference to the given `PARCAtomicUint8` instance,
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
 *     PARCAtomicUint8 *a = parcAtomicUint8_Create();
 *
 *     parcAtomicUint8_Release(&a);
 * }
 * @endcode
 */
void parcAtomicUint8_Release(PARCAtomicUint8 **instancePtr);

/**
 * Get the current value of the given `PARCAtomicUint8` instance.
 *
 * @param [in] instance A pointer to a valid `PARCAtomicUint8` instance.
 *
 * @return the current value of the given `PARCAtomicUint8` instance.
 *
 * Example:
 * @code
 * {
 *     PARCAtomicUint8 *instance = parcAtomicUint8_Create(7);
 *
 *     uint8_t value = parcAtomicUint8_GetValue(instance);
 *
 *     parcAtomicUint8_Release(&instance);
 * }
 * @endcode
 */
uint8_t parcAtomicUint8_GetValue(const PARCAtomicUint8 *instance);
#endif
