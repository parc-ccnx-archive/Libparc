/*
 * Copyright (c) 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @file parc_ThreadPool.h
 * @brief <#Brief Description#>
 *
 * <#Detailed Description#>
 *
 * @author <#gscott#>, Computing Science Laboratory, PARC
 * @copyright 2015 Palo Alto Research Center, Inc. (PARC), A Xerox Company.  All Rights Reserved.
 */
#ifndef PARCLibrary_parc_ThreadPool
#define PARCLibrary_parc_ThreadPool
#include <stdbool.h>

#include <parc/algol/parc_JSON.h>
#include <parc/algol/parc_HashCode.h>

struct PARCThreadPool;
typedef struct PARCThreadPool PARCThreadPool;

/**
 * Increase the number of references to a `PARCThreadPool` instance.
 *
 * Note that new `PARCThreadPool` is not created,
 * only that the given `PARCThreadPool` reference count is incremented.
 * Discard the reference by invoking `parcThreadPool_Release`.
 *
 * @param [in] instance A pointer to a valid PARCThreadPool instance.
 *
 * @return The same value as @p instance.
 *
 * Example:
 * @code
 * {
 *     PARCThreadPool *a = parcThreadPool_Create();
 *
 *     PARCThreadPool *b = parcThreadPool_Acquire();

 *     parcThreadPool_Release(&a);
 *     parcThreadPool_Release(&b);
 * }
 * @endcode
 */
PARCThreadPool *parcThreadPool_Acquire(const PARCThreadPool *instance);

#ifdef PARCLibrary_DISABLE_VALIDATION
#  define parcThreadPool_OptionalAssertValid(_instance_)
#else
#  define parcThreadPool_OptionalAssertValid(_instance_) parcThreadPool_AssertValid(_instance_)
#endif

/**
 * Assert that the given `PARCThreadPool` instance is valid.
 *
 * @param [in] instance A pointer to a valid PARCThreadPool instance.
 *
 * Example:
 * @code
 * {
 *     PARCThreadPool *a = parcThreadPool_Create();
 *
 *     parcThreadPool_AssertValid(a);
 *
 *     printf("Instance is valid.\n");
 *
 *     parcThreadPool_Release(&b);
 * }
 * @endcode
 */
void parcThreadPool_AssertValid(const PARCThreadPool *instance);

/**
 * Create an instance of PARCThreadPool
 *
 * <#Paragraphs Of Explanation#>
 *
 * @return non-NULL A pointer to a valid PARCThreadPool instance.
 * @return NULL An error occurred.
 *
 * Example:
 * @code
 * {
 *     PARCThreadPool *a = parcThreadPool_Create();
 *
 *     parcThreadPool_Release(&a);
 * }
 * @endcode
 */
PARCThreadPool *parcThreadPool_Create(int poolSize);

/**
 * Compares @p instance with @p other for order.
 *
 * Returns a negative integer, zero, or a positive integer as @p instance
 * is less than, equal to, or greater than @p other.
 *
 * @param [in] instance A pointer to a valid PARCThreadPool instance.
 * @param [in] other A pointer to a valid PARCThreadPool instance.
 *
 * @return <0 Instance is less than @p other.
 * @return 0 Instance a and instance b compare the same.
 * @return >0 Instance a is greater than instance b.
 *
 * Example:
 * @code
 * {
 *     PARCThreadPool *a = parcThreadPool_Create();
 *     PARCThreadPool *b = parcThreadPool_Create();
 *
 *     if (parcThreadPool_Compare(a, b) == 0) {
 *         printf("Instances are equal.\n");
 *     }
 *
 *     parcThreadPool_Release(&a);
 *     parcThreadPool_Release(&b);
 * }
 * @endcode
 *
 * @see parcThreadPool_Equals
 */
int parcThreadPool_Compare(const PARCThreadPool *instance, const PARCThreadPool *other);

/**
 * Create an independent copy the given `PARCBuffer`
 *
 * A new buffer is created as a complete copy of the original.
 *
 * @param [in] original A pointer to a valid PARCThreadPool instance.
 *
 * @return NULL Memory could not be allocated.
 * @return non-NULL A pointer to a new `PARCThreadPool` instance.
 *
 * Example:
 * @code
 * {
 *     PARCThreadPool *a = parcThreadPool_Create();
 *
 *     PARCThreadPool *copy = parcThreadPool_Copy(&b);
 *
 *     parcThreadPool_Release(&b);
 *     parcThreadPool_Release(&copy);
 * }
 * @endcode
 */
PARCThreadPool *parcThreadPool_Copy(const PARCThreadPool *original);

/**
 * Print a human readable representation of the given `PARCThreadPool`.
 *
 * @param [in] instance A pointer to a valid PARCThreadPool instance.
 * @param [in] indentation The indentation level to use for printing.
 *
 * Example:
 * @code
 * {
 *     PARCThreadPool *a = parcThreadPool_Create();
 *
 *     parcThreadPool_Display(a, 0);
 *
 *     parcThreadPool_Release(&a);
 * }
 * @endcode
 */
void parcThreadPool_Display(const PARCThreadPool *instance, int indentation);

/**
 * Determine if two `PARCThreadPool` instances are equal.
 *
 * The following equivalence relations on non-null `PARCThreadPool` instances are maintained: *
 *   * It is reflexive: for any non-null reference value x, `parcThreadPool_Equals(x, x)` must return true.
 *
 *   * It is symmetric: for any non-null reference values x and y, `parcThreadPool_Equals(x, y)` must return true if and only if
 *        `parcThreadPool_Equals(y x)` returns true.
 *
 *   * It is transitive: for any non-null reference values x, y, and z, if
 *        `parcThreadPool_Equals(x, y)` returns true and
 *        `parcThreadPool_Equals(y, z)` returns true,
 *        then `parcThreadPool_Equals(x, z)` must return true.
 *
 *   * It is consistent: for any non-null reference values x and y, multiple invocations of `parcThreadPool_Equals(x, y)`
 *         consistently return true or consistently return false.
 *
 *   * For any non-null reference value x, `parcThreadPool_Equals(x, NULL)` must return false.
 *
 * @param [in] x A pointer to a valid PARCThreadPool instance.
 * @param [in] y A pointer to a valid PARCThreadPool instance.
 *
 * @return true The instances x and y are equal.
 *
 * Example:
 * @code
 * {
 *     PARCThreadPool *a = parcThreadPool_Create();
 *     PARCThreadPool *b = parcThreadPool_Create();
 *
 *     if (parcThreadPool_Equals(a, b)) {
 *         printf("Instances are equal.\n");
 *     }
 *
 *     parcThreadPool_Release(&a);
 *     parcThreadPool_Release(&b);
 * }
 * @endcode
 * @see parcThreadPool_HashCode
 */
bool parcThreadPool_Equals(const PARCThreadPool *x, const PARCThreadPool *y);

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
 * If two instances are equal according to the {@link parcThreadPool_Equals} method,
 * then calling the {@link parcThreadPool_HashCode} method on each of the two instances must produce the same integer result.
 *
 * It is not required that if two instances are unequal according to the
 * {@link parcThreadPool_Equals} function,
 * then calling the `parcThreadPool_HashCode`
 * method on each of the two objects must produce distinct integer results.
 *
 * @param [in] instance A pointer to a valid PARCThreadPool instance.
 *
 * @return The hashcode for the given instance.
 *
 * Example:
 * @code
 * {
 *     PARCThreadPool *a = parcThreadPool_Create();
 *
 *     PARCHashCode hashValue = parcThreadPool_HashCode(buffer);
 *     parcThreadPool_Release(&a);
 * }
 * @endcode
 */
PARCHashCode parcThreadPool_HashCode(const PARCThreadPool *instance);

/**
 * Determine if an instance of `PARCThreadPool` is valid.
 *
 * Valid means the internal state of the type is consistent with its required current or future behaviour.
 * This may include the validation of internal instances of types.
 *
 * @param [in] instance A pointer to a valid PARCThreadPool instance.
 *
 * @return true The instance is valid.
 * @return false The instance is not valid.
 *
 * Example:
 * @code
 * {
 *     PARCThreadPool *a = parcThreadPool_Create();
 *
 *     if (parcThreadPool_IsValid(a)) {
 *         printf("Instance is valid.\n");
 *     }
 *
 *     parcThreadPool_Release(&a);
 * }
 * @endcode
 *
 */
bool parcThreadPool_IsValid(const PARCThreadPool *instance);

/**
 * Release a previously acquired reference to the given `PARCThreadPool` instance,
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
 *     PARCThreadPool *a = parcThreadPool_Create();
 *
 *     parcThreadPool_Release(&a);
 * }
 * @endcode
 */
void parcThreadPool_Release(PARCThreadPool **instancePtr);

/**
 * Create a `PARCJSON` instance (representation) of the given object.
 *
 * @param [in] instance A pointer to a valid PARCThreadPool instance.
 *
 * @return NULL Memory could not be allocated to contain the `PARCJSON` instance.
 * @return non-NULL An allocated C string that must be deallocated via parcMemory_Deallocate().
 *
 * Example:
 * @code
 * {
 *     PARCThreadPool *a = parcThreadPool_Create();
 *
 *     PARCJSON *json = parcThreadPool_ToJSON(a);
 *
 *     printf("JSON representation: %s\n", parcJSON_ToString(json));
 *     parcJSON_Release(&json);
 *
 *     parcThreadPool_Release(&a);
 * }
 * @endcode
 */
PARCJSON *parcThreadPool_ToJSON(const PARCThreadPool *instance);

/**
 * Produce a null-terminated string representation of the specified `PARCThreadPool`.
 *
 * The result must be freed by the caller via {@link parcMemory_Deallocate}.
 *
 * @param [in] instance A pointer to a valid PARCThreadPool instance.
 *
 * @return NULL Cannot allocate memory.
 * @return non-NULL A pointer to an allocated, null-terminated C string that must be deallocated via {@link parcMemory_Deallocate}.
 *
 * Example:
 * @code
 * {
 *     PARCThreadPool *a = parcThreadPool_Create();
 *
 *     char *string = parcThreadPool_ToString(a);
 *
 *     parcThreadPool_Release(&a);
 *
 *     parcMemory_Deallocate(&string);
 * }
 * @endcode
 *
 * @see parcThreadPool_Display
 */
char *parcThreadPool_ToString(const PARCThreadPool *instance);
#endif
