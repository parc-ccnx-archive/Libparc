/*
 * Copyright (c) 2016, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @file parc_BufferPool.h
 * @brief <#Brief Description#>
 *
 * <#Detailed Description#>
 *
 * @author <#gscott#>, Palo Alto Research Center (PARC)
 * @copyright 2016, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef PARCLibrary_parc_BufferPool
#define PARCLibrary_parc_BufferPool
#include <stdbool.h>

#include <parc/algol/parc_JSON.h>
#include <parc/algol/parc_HashCode.h>

struct PARCBufferPool;
typedef struct PARCBufferPool PARCBufferPool;

/**
 * Increase the number of references to a `PARCBufferPool` instance.
 *
 * Note that new `PARCBufferPool` is not created,
 * only that the given `PARCBufferPool` reference count is incremented.
 * Discard the reference by invoking `parcBufferPool_Release`.
 *
 * @param [in] instance A pointer to a valid PARCBufferPool instance.
 *
 * @return The same value as @p instance.
 *
 * Example:
 * @code
 * {
 *     PARCBufferPool *a = parcBufferPool_Create();
 *
 *     PARCBufferPool *b = parcBufferPool_Acquire();

 *     parcBufferPool_Release(&a);
 *     parcBufferPool_Release(&b);
 * }
 * @endcode
 */
PARCBufferPool *parcBufferPool_Acquire(const PARCBufferPool *instance);

#ifdef PARCLibrary_DISABLE_VALIDATION
#  define parcBufferPool_OptionalAssertValid(_instance_)
#else
#  define parcBufferPool_OptionalAssertValid(_instance_) parcBufferPool_AssertValid(_instance_)
#endif

/**
 * Assert that the given `PARCBufferPool` instance is valid.
 *
 * @param [in] instance A pointer to a valid PARCBufferPool instance.
 *
 * Example:
 * @code
 * {
 *     PARCBufferPool *a = parcBufferPool_Create();
 *
 *     parcBufferPool_AssertValid(a);
 *
 *     printf("Instance is valid.\n");
 *
 *     parcBufferPool_Release(&b);
 * }
 * @endcode
 */
void parcBufferPool_AssertValid(const PARCBufferPool *instance);

/**
 * Create an instance of PARCBufferPool
 *
 * <#Paragraphs Of Explanation#>
 *
 * @return non-NULL A pointer to a valid PARCBufferPool instance.
 * @return NULL An error occurred.
 *
 * Example:
 * @code
 * {
 *     PARCBufferPool *a = parcBufferPool_Create();
 *
 *     parcBufferPool_Release(&a);
 * }
 * @endcode
 */
PARCBufferPool *parcBufferPool_Create(size_t limit, size_t bufferSize);

/**
 * Compares @p instance with @p other for order.
 *
 * Returns a negative integer, zero, or a positive integer as @p instance
 * is less than, equal to, or greater than @p other.
 *
 * @param [in] instance A pointer to a valid PARCBufferPool instance.
 * @param [in] other A pointer to a valid PARCBufferPool instance.
 *
 * @return <0 Instance is less than @p other.
 * @return 0 Instance a and instance b compare the same.
 * @return >0 Instance a is greater than instance b.
 *
 * Example:
 * @code
 * {
 *     PARCBufferPool *a = parcBufferPool_Create();
 *     PARCBufferPool *b = parcBufferPool_Create();
 *
 *     if (parcBufferPool_Compare(a, b) == 0) {
 *         printf("Instances are equal.\n");
 *     }
 *
 *     parcBufferPool_Release(&a);
 *     parcBufferPool_Release(&b);
 * }
 * @endcode
 *
 * @see parcBufferPool_Equals
 */
int parcBufferPool_Compare(const PARCBufferPool *instance, const PARCBufferPool *other);

/**
 * Create an independent copy the given `PARCBuffer`
 *
 * A new buffer is created as a complete copy of the original.
 *
 * @param [in] original A pointer to a valid PARCBufferPool instance.
 *
 * @return NULL Memory could not be allocated.
 * @return non-NULL A pointer to a new `PARCBufferPool` instance.
 *
 * Example:
 * @code
 * {
 *     PARCBufferPool *a = parcBufferPool_Create();
 *
 *     PARCBufferPool *copy = parcBufferPool_Copy(&b);
 *
 *     parcBufferPool_Release(&b);
 *     parcBufferPool_Release(&copy);
 * }
 * @endcode
 */
PARCBufferPool *parcBufferPool_Copy(const PARCBufferPool *original);

/**
 * Print a human readable representation of the given `PARCBufferPool`.
 *
 * @param [in] instance A pointer to a valid PARCBufferPool instance.
 * @param [in] indentation The indentation level to use for printing.
 *
 * Example:
 * @code
 * {
 *     PARCBufferPool *a = parcBufferPool_Create();
 *
 *     parcBufferPool_Display(a, 0);
 *
 *     parcBufferPool_Release(&a);
 * }
 * @endcode
 */
void parcBufferPool_Display(const PARCBufferPool *instance, int indentation);

/**
 * Determine if two `PARCBufferPool` instances are equal.
 *
 * The following equivalence relations on non-null `PARCBufferPool` instances are maintained: *
 *   * It is reflexive: for any non-null reference value x, `parcBufferPool_Equals(x, x)` must return true.
 *
 *   * It is symmetric: for any non-null reference values x and y, `parcBufferPool_Equals(x, y)` must return true if and only if
 *        `parcBufferPool_Equals(y x)` returns true.
 *
 *   * It is transitive: for any non-null reference values x, y, and z, if
 *        `parcBufferPool_Equals(x, y)` returns true and
 *        `parcBufferPool_Equals(y, z)` returns true,
 *        then `parcBufferPool_Equals(x, z)` must return true.
 *
 *   * It is consistent: for any non-null reference values x and y, multiple invocations of `parcBufferPool_Equals(x, y)`
 *         consistently return true or consistently return false.
 *
 *   * For any non-null reference value x, `parcBufferPool_Equals(x, NULL)` must return false.
 *
 * @param [in] x A pointer to a valid PARCBufferPool instance.
 * @param [in] y A pointer to a valid PARCBufferPool instance.
 *
 * @return true The instances x and y are equal.
 *
 * Example:
 * @code
 * {
 *     PARCBufferPool *a = parcBufferPool_Create();
 *     PARCBufferPool *b = parcBufferPool_Create();
 *
 *     if (parcBufferPool_Equals(a, b)) {
 *         printf("Instances are equal.\n");
 *     }
 *
 *     parcBufferPool_Release(&a);
 *     parcBufferPool_Release(&b);
 * }
 * @endcode
 * @see parcBufferPool_HashCode
 */
bool parcBufferPool_Equals(const PARCBufferPool *x, const PARCBufferPool *y);

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
 * If two instances are equal according to the {@link parcBufferPool_Equals} method,
 * then calling the {@link parcBufferPool_HashCode} method on each of the two instances must produce the same integer result.
 *
 * It is not required that if two instances are unequal according to the
 * {@link parcBufferPool_Equals} function,
 * then calling the `parcBufferPool_HashCode`
 * method on each of the two objects must produce distinct integer results.
 *
 * @param [in] instance A pointer to a valid PARCBufferPool instance.
 *
 * @return The hashcode for the given instance.
 *
 * Example:
 * @code
 * {
 *     PARCBufferPool *a = parcBufferPool_Create();
 *
 *     PARCHashCode hashValue = parcBufferPool_HashCode(buffer);
 *     parcBufferPool_Release(&a);
 * }
 * @endcode
 */
PARCHashCode parcBufferPool_HashCode(const PARCBufferPool *instance);

/**
 * Determine if an instance of `PARCBufferPool` is valid.
 *
 * Valid means the internal state of the type is consistent with its required current or future behaviour.
 * This may include the validation of internal instances of types.
 *
 * @param [in] instance A pointer to a valid PARCBufferPool instance.
 *
 * @return true The instance is valid.
 * @return false The instance is not valid.
 *
 * Example:
 * @code
 * {
 *     PARCBufferPool *a = parcBufferPool_Create();
 *
 *     if (parcBufferPool_IsValid(a)) {
 *         printf("Instance is valid.\n");
 *     }
 *
 *     parcBufferPool_Release(&a);
 * }
 * @endcode
 *
 */
bool parcBufferPool_IsValid(const PARCBufferPool *instance);

/**
 * Release a previously acquired reference to the given `PARCBufferPool` instance,
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
 *     PARCBufferPool *a = parcBufferPool_Create();
 *
 *     parcBufferPool_Release(&a);
 * }
 * @endcode
 */
void parcBufferPool_Release(PARCBufferPool **instancePtr);

/**
 * Create a `PARCJSON` instance (representation) of the given object.
 *
 * @param [in] instance A pointer to a valid PARCBufferPool instance.
 *
 * @return NULL Memory could not be allocated to contain the `PARCJSON` instance.
 * @return non-NULL An allocated C string that must be deallocated via parcMemory_Deallocate().
 *
 * Example:
 * @code
 * {
 *     PARCBufferPool *a = parcBufferPool_Create();
 *
 *     PARCJSON *json = parcBufferPool_ToJSON(a);
 *
 *     printf("JSON representation: %s\n", parcJSON_ToString(json));
 *     parcJSON_Release(&json);
 *
 *     parcBufferPool_Release(&a);
 * }
 * @endcode
 */
PARCJSON *parcBufferPool_ToJSON(const PARCBufferPool *instance);

/**
 * Produce a null-terminated string representation of the specified `PARCBufferPool`.
 *
 * The result must be freed by the caller via {@link parcMemory_Deallocate}.
 *
 * @param [in] instance A pointer to a valid PARCBufferPool instance.
 *
 * @return NULL Cannot allocate memory.
 * @return non-NULL A pointer to an allocated, null-terminated C string that must be deallocated via {@link parcMemory_Deallocate}.
 *
 * Example:
 * @code
 * {
 *     PARCBufferPool *a = parcBufferPool_Create();
 *
 *     char *string = parcBufferPool_ToString(a);
 *
 *     parcBufferPool_Release(&a);
 *
 *     parcMemory_Deallocate(&string);
 * }
 * @endcode
 *
 * @see parcBufferPool_Display
 */
char *parcBufferPool_ToString(const PARCBufferPool *instance);
#endif
