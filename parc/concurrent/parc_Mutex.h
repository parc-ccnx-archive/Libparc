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
 * @file parc_Mutex.h
 * @brief <#Brief Description#>
 *
 * <#Detailed Description#>
 *
 * @author <#gscott#>, Palo Alto Research Center (PARC)
 * @copyright 2016, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef PARCLibrary_parc_Mutex
#define PARCLibrary_parc_Mutex
#include <stdbool.h>

#include <parc/algol/parc_JSON.h>
#include <parc/algol/parc_HashCode.h>

struct PARCMutex;
typedef struct PARCMutex PARCMutex;

/**
 * Increase the number of references to a `PARCMutex` instance.
 *
 * Note that new `PARCMutex` is not created,
 * only that the given `PARCMutex` reference count is incremented.
 * Discard the reference by invoking `parcMutex_Release`.
 *
 * @param [in] instance A pointer to a valid PARCMutex instance.
 *
 * @return The same value as @p instance.
 *
 * Example:
 * @code
 * {
 *     PARCMutex *a = parcMutex_Create();
 *
 *     PARCMutex *b = parcMutex_Acquire();

 *     parcMutex_Release(&a);
 *     parcMutex_Release(&b);
 * }
 * @endcode
 */
PARCMutex *parcMutex_Acquire(const PARCMutex *instance);

#ifdef PARCLibrary_DISABLE_VALIDATION
#  define parcMutex_OptionalAssertValid(_instance_)
#else
#  define parcMutex_OptionalAssertValid(_instance_) parcMutex_AssertValid(_instance_)
#endif

/**
 * Assert that the given `PARCMutex` instance is valid.
 *
 * @param [in] instance A pointer to a valid PARCMutex instance.
 *
 * Example:
 * @code
 * {
 *     PARCMutex *a = parcMutex_Create();
 *
 *     parcMutex_AssertValid(a);
 *
 *     printf("Instance is valid.\n");
 *
 *     parcMutex_Release(&b);
 * }
 * @endcode
 */
void parcMutex_AssertValid(const PARCMutex *instance);

/**
 * Create an instance of PARCMutex
 *
 * <#Paragraphs Of Explanation#>
 *
 * @return non-NULL A pointer to a valid PARCMutex instance.
 * @return NULL An error occurred.
 *
 * Example:
 * @code
 * {
 *     PARCMutex *a = parcMutex_Create();
 *
 *     parcMutex_Release(&a);
 * }
 * @endcode
 */
PARCMutex *parcMutex_Create(void);

/**
 * Compares @p instance with @p other for order.
 *
 * Returns a negative integer, zero, or a positive integer as @p instance
 * is less than, equal to, or greater than @p other.
 *
 * @param [in] instance A pointer to a valid PARCMutex instance.
 * @param [in] other A pointer to a valid PARCMutex instance.
 *
 * @return <0 Instance is less than @p other.
 * @return 0 Instance a and instance b compare the same.
 * @return >0 Instance a is greater than instance b.
 *
 * Example:
 * @code
 * {
 *     PARCMutex *a = parcMutex_Create();
 *     PARCMutex *b = parcMutex_Create();
 *
 *     if (parcMutex_Compare(a, b) == 0) {
 *         printf("Instances are equal.\n");
 *     }
 *
 *     parcMutex_Release(&a);
 *     parcMutex_Release(&b);
 * }
 * @endcode
 *
 * @see parcMutex_Equals
 */
int parcMutex_Compare(const PARCMutex *instance, const PARCMutex *other);

/**
 * Create an independent copy the given `PARCBuffer`
 *
 * A new buffer is created as a complete copy of the original.
 *
 * @param [in] original A pointer to a valid PARCMutex instance.
 *
 * @return NULL Memory could not be allocated.
 * @return non-NULL A pointer to a new `PARCMutex` instance.
 *
 * Example:
 * @code
 * {
 *     PARCMutex *a = parcMutex_Create();
 *
 *     PARCMutex *copy = parcMutex_Copy(&b);
 *
 *     parcMutex_Release(&b);
 *     parcMutex_Release(&copy);
 * }
 * @endcode
 */
PARCMutex *parcMutex_Copy(const PARCMutex *original);

/**
 * Print a human readable representation of the given `PARCMutex`.
 *
 * @param [in] instance A pointer to a valid PARCMutex instance.
 * @param [in] indentation The indentation level to use for printing.
 *
 * Example:
 * @code
 * {
 *     PARCMutex *a = parcMutex_Create();
 *
 *     parcMutex_Display(a, 0);
 *
 *     parcMutex_Release(&a);
 * }
 * @endcode
 */
void parcMutex_Display(const PARCMutex *instance, int indentation);

/**
 * Determine if two `PARCMutex` instances are equal.
 *
 * The following equivalence relations on non-null `PARCMutex` instances are maintained: *
 *   * It is reflexive: for any non-null reference value x, `parcMutex_Equals(x, x)` must return true.
 *
 *   * It is symmetric: for any non-null reference values x and y, `parcMutex_Equals(x, y)` must return true if and only if
 *        `parcMutex_Equals(y x)` returns true.
 *
 *   * It is transitive: for any non-null reference values x, y, and z, if
 *        `parcMutex_Equals(x, y)` returns true and
 *        `parcMutex_Equals(y, z)` returns true,
 *        then `parcMutex_Equals(x, z)` must return true.
 *
 *   * It is consistent: for any non-null reference values x and y, multiple invocations of `parcMutex_Equals(x, y)`
 *         consistently return true or consistently return false.
 *
 *   * For any non-null reference value x, `parcMutex_Equals(x, NULL)` must return false.
 *
 * @param [in] x A pointer to a valid PARCMutex instance.
 * @param [in] y A pointer to a valid PARCMutex instance.
 *
 * @return true The instances x and y are equal.
 *
 * Example:
 * @code
 * {
 *     PARCMutex *a = parcMutex_Create();
 *     PARCMutex *b = parcMutex_Create();
 *
 *     if (parcMutex_Equals(a, b)) {
 *         printf("Instances are equal.\n");
 *     }
 *
 *     parcMutex_Release(&a);
 *     parcMutex_Release(&b);
 * }
 * @endcode
 * @see parcMutex_HashCode
 */
bool parcMutex_Equals(const PARCMutex *x, const PARCMutex *y);

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
 * If two instances are equal according to the {@link parcMutex_Equals} method,
 * then calling the {@link parcMutex_HashCode} method on each of the two instances must produce the same integer result.
 *
 * It is not required that if two instances are unequal according to the
 * {@link parcMutex_Equals} function,
 * then calling the `parcMutex_HashCode`
 * method on each of the two objects must produce distinct integer results.
 *
 * @param [in] instance A pointer to a valid PARCMutex instance.
 *
 * @return The hashcode for the given instance.
 *
 * Example:
 * @code
 * {
 *     PARCMutex *a = parcMutex_Create();
 *
 *     PARCHashCode hashValue = parcMutex_HashCode(buffer);
 *     parcMutex_Release(&a);
 * }
 * @endcode
 */
PARCHashCode parcMutex_HashCode(const PARCMutex *instance);

/**
 * Determine if an instance of `PARCMutex` is valid.
 *
 * Valid means the internal state of the type is consistent with its required current or future behaviour.
 * This may include the validation of internal instances of types.
 *
 * @param [in] instance A pointer to a valid PARCMutex instance.
 *
 * @return true The instance is valid.
 * @return false The instance is not valid.
 *
 * Example:
 * @code
 * {
 *     PARCMutex *a = parcMutex_Create();
 *
 *     if (parcMutex_IsValid(a)) {
 *         printf("Instance is valid.\n");
 *     }
 *
 *     parcMutex_Release(&a);
 * }
 * @endcode
 *
 */
bool parcMutex_IsValid(const PARCMutex *instance);

/**
 * Release a previously acquired reference to the given `PARCMutex` instance,
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
 *     PARCMutex *a = parcMutex_Create();
 *
 *     parcMutex_Release(&a);
 * }
 * @endcode
 */
void parcMutex_Release(PARCMutex **instancePtr);

/**
 * Create a `PARCJSON` instance (representation) of the given object.
 *
 * @param [in] instance A pointer to a valid PARCMutex instance.
 *
 * @return NULL Memory could not be allocated to contain the `PARCJSON` instance.
 * @return non-NULL An allocated C string that must be deallocated via parcMemory_Deallocate().
 *
 * Example:
 * @code
 * {
 *     PARCMutex *a = parcMutex_Create();
 *
 *     PARCJSON *json = parcMutex_ToJSON(a);
 *
 *     printf("JSON representation: %s\n", parcJSON_ToString(json));
 *     parcJSON_Release(&json);
 *
 *     parcMutex_Release(&a);
 * }
 * @endcode
 */
PARCJSON *parcMutex_ToJSON(const PARCMutex *instance);

/**
 * Produce a null-terminated string representation of the specified `PARCMutex`.
 *
 * The result must be freed by the caller via {@link parcMemory_Deallocate}.
 *
 * @param [in] instance A pointer to a valid PARCMutex instance.
 *
 * @return NULL Cannot allocate memory.
 * @return non-NULL A pointer to an allocated, null-terminated C string that must be deallocated via {@link parcMemory_Deallocate}.
 *
 * Example:
 * @code
 * {
 *     PARCMutex *a = parcMutex_Create();
 *
 *     char *string = parcMutex_ToString(a);
 *
 *     parcMutex_Release(&a);
 *
 *     parcMemory_Deallocate(&string);
 * }
 * @endcode
 *
 * @see parcMutex_Display
 */
char *parcMutex_ToString(const PARCMutex *instance);
#endif
