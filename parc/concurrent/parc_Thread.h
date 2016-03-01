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
 * @file parc_Thread.h
 * @brief <#Brief Description#>
 *
 * <#Detailed Description#>
 *
 * @author <#gscott#>, Computing Science Laboratory, PARC
 * @copyright 2015 Palo Alto Research Center, Inc. (PARC), A Xerox Company.  All Rights Reserved.
 */
#ifndef PARCLibrary_parc_Thread
#define PARCLibrary_parc_Thread
#include <stdbool.h>

#include <parc/algol/parc_JSON.h>
#include <parc/algol/parc_HashCode.h>

struct PARCThread;
typedef struct PARCThread PARCThread;

/**
 * Increase the number of references to a `PARCThread` instance.
 *
 * Note that new `PARCThread` is not created,
 * only that the given `PARCThread` reference count is incremented.
 * Discard the reference by invoking `parcThread_Release`.
 *
 * @param [in] instance A pointer to a valid PARCThread instance.
 *
 * @return The same value as @p instance.
 *
 * Example:
 * @code
 * {
 *     PARCThread *a = parcThread_Create();
 *
 *     PARCThread *b = parcThread_Acquire();

 *     parcThread_Release(&a);
 *     parcThread_Release(&b);
 * }
 * @endcode
 */
PARCThread *parcThread_Acquire(const PARCThread *instance);

#ifdef PARCLibrary_DISABLE_VALIDATION
#  define parcThread_OptionalAssertValid(_instance_)
#else
#  define parcThread_OptionalAssertValid(_instance_) parcThread_AssertValid(_instance_)
#endif

/**
 * Assert that the given `PARCThread` instance is valid.
 *
 * @param [in] instance A pointer to a valid PARCThread instance.
 *
 * Example:
 * @code
 * {
 *     PARCThread *a = parcThread_Create();
 *
 *     parcThread_AssertValid(a);
 *
 *     printf("Instance is valid.\n");
 *
 *     parcThread_Release(&b);
 * }
 * @endcode
 */
void parcThread_AssertValid(const PARCThread *instance);

/**
 * Create an instance of PARCThread
 *
 * @return non-NULL A pointer to a valid PARCThread instance.
 * @return NULL An error occurred.
 *
 * Example:
 * @code
 * {
 *
 *     MyTask *task = myTask_Create();
 *     PARCThread *thread = parcThread_Create(myTask_Run, myTask);
 *
 *     parcThread_Start(a);
 *
 *     parcThread_Release(&a);
 * }
 * @endcode
 */
#define parcThread_Create(_runFunction_, _argument_) parcThread_CreateImpl((void *(*)(PARCObject *)) _runFunction_, (PARCObject *) _argument_)

PARCThread *parcThread_CreateImpl(PARCObject *(*run)(PARCObject *), PARCObject *restrict argument);

/**
 * Compares @p instance with @p other for order.
 *
 * Returns a negative integer, zero, or a positive integer as @p instance
 * is less than, equal to, or greater than @p other.
 *
 * @param [in] instance A pointer to a valid PARCThread instance.
 * @param [in] other A pointer to a valid PARCThread instance.
 *
 * @return <0 Instance is less than @p other.
 * @return 0 Instance a and instance b compare the same.
 * @return >0 Instance a is greater than instance b.
 *
 * Example:
 * @code
 * {
 *     PARCThread *a = parcThread_Create();
 *     PARCThread *b = parcThread_Create();
 *
 *     if (parcThread_Compare(a, b) == 0) {
 *         printf("Instances are equal.\n");
 *     }
 *
 *     parcThread_Release(&a);
 *     parcThread_Release(&b);
 * }
 * @endcode
 *
 * @see parcThread_Equals
 */
int parcThread_Compare(const PARCThread *instance, const PARCThread *other);

/**
 * Create an independent copy the given `PARCBuffer`
 *
 * A new buffer is created as a complete copy of the original.
 *
 * @param [in] original A pointer to a valid PARCThread instance.
 *
 * @return NULL Memory could not be allocated.
 * @return non-NULL A pointer to a new `PARCThread` instance.
 *
 * Example:
 * @code
 * {
 *     PARCThread *a = parcThread_Create();
 *
 *     PARCThread *copy = parcThread_Copy(&b);
 *
 *     parcThread_Release(&b);
 *     parcThread_Release(&copy);
 * }
 * @endcode
 */
PARCThread *parcThread_Copy(const PARCThread *original);

/**
 * Print a human readable representation of the given `PARCThread`.
 *
 * @param [in] instance A pointer to a valid PARCThread instance.
 * @param [in] indentation The indentation level to use for printing.
 *
 * Example:
 * @code
 * {
 *     PARCThread *a = parcThread_Create();
 *
 *     parcThread_Display(a, 0);
 *
 *     parcThread_Release(&a);
 * }
 * @endcode
 */
void parcThread_Display(const PARCThread *instance, int indentation);

/**
 * Determine if two `PARCThread` instances are equal.
 *
 * The following equivalence relations on non-null `PARCThread` instances are maintained: *
 *   * It is reflexive: for any non-null reference value x, `parcThread_Equals(x, x)` must return true.
 *
 *   * It is symmetric: for any non-null reference values x and y, `parcThread_Equals(x, y)` must return true if and only if
 *        `parcThread_Equals(y x)` returns true.
 *
 *   * It is transitive: for any non-null reference values x, y, and z, if
 *        `parcThread_Equals(x, y)` returns true and
 *        `parcThread_Equals(y, z)` returns true,
 *        then `parcThread_Equals(x, z)` must return true.
 *
 *   * It is consistent: for any non-null reference values x and y, multiple invocations of `parcThread_Equals(x, y)`
 *         consistently return true or consistently return false.
 *
 *   * For any non-null reference value x, `parcThread_Equals(x, NULL)` must return false.
 *
 * @param [in] x A pointer to a valid PARCThread instance.
 * @param [in] y A pointer to a valid PARCThread instance.
 *
 * @return true The instances x and y are equal.
 *
 * Example:
 * @code
 * {
 *     PARCThread *a = parcThread_Create();
 *     PARCThread *b = parcThread_Create();
 *
 *     if (parcThread_Equals(a, b)) {
 *         printf("Instances are equal.\n");
 *     }
 *
 *     parcThread_Release(&a);
 *     parcThread_Release(&b);
 * }
 * @endcode
 * @see parcThread_HashCode
 */
bool parcThread_Equals(const PARCThread *x, const PARCThread *y);

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
 * If two instances are equal according to the {@link parcThread_Equals} method,
 * then calling the {@link parcThread_HashCode} method on each of the two instances must produce the same integer result.
 *
 * It is not required that if two instances are unequal according to the
 * {@link parcThread_Equals} function,
 * then calling the `parcThread_HashCode`
 * method on each of the two objects must produce distinct integer results.
 *
 * @param [in] instance A pointer to a valid PARCThread instance.
 *
 * @return The hashcode for the given instance.
 *
 * Example:
 * @code
 * {
 *     PARCThread *a = parcThread_Create();
 *
 *     PARCHashCode hashValue = parcThread_HashCode(buffer);
 *     parcThread_Release(&a);
 * }
 * @endcode
 */
PARCHashCode parcThread_HashCode(const PARCThread *instance);

/**
 * Determine if an instance of `PARCThread` is valid.
 *
 * Valid means the internal state of the type is consistent with its required current or future behaviour.
 * This may include the validation of internal instances of types.
 *
 * @param [in] instance A pointer to a valid PARCThread instance.
 *
 * @return true The instance is valid.
 * @return false The instance is not valid.
 *
 * Example:
 * @code
 * {
 *     PARCThread *a = parcThread_Create();
 *
 *     if (parcThread_IsValid(a)) {
 *         printf("Instance is valid.\n");
 *     }
 *
 *     parcThread_Release(&a);
 * }
 * @endcode
 *
 */
bool parcThread_IsValid(const PARCThread *instance);

/**
 * Release a previously acquired reference to the given `PARCThread` instance,
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
 *     PARCThread *a = parcThread_Create();
 *
 *     parcThread_Release(&a);
 * }
 * @endcode
 */
void parcThread_Release(PARCThread **instancePtr);

/**
 * Create a `PARCJSON` instance (representation) of the given object.
 *
 * @param [in] instance A pointer to a valid PARCThread instance.
 *
 * @return NULL Memory could not be allocated to contain the `PARCJSON` instance.
 * @return non-NULL An allocated C string that must be deallocated via parcMemory_Deallocate().
 *
 * Example:
 * @code
 * {
 *     PARCThread *a = parcThread_Create();
 *
 *     PARCJSON *json = parcThread_ToJSON(a);
 *
 *     printf("JSON representation: %s\n", parcJSON_ToString(json));
 *     parcJSON_Release(&json);
 *
 *     parcThread_Release(&a);
 * }
 * @endcode
 */
PARCJSON *parcThread_ToJSON(const PARCThread *instance);

/**
 * Produce a null-terminated string representation of the specified `PARCThread`.
 *
 * The result must be freed by the caller via {@link parcMemory_Deallocate}.
 *
 * @param [in] instance A pointer to a valid PARCThread instance.
 *
 * @return NULL Cannot allocate memory.
 * @return non-NULL A pointer to an allocated, null-terminated C string that must be deallocated via {@link parcMemory_Deallocate}.
 *
 * Example:
 * @code
 * {
 *     PARCThread *a = parcThread_Create();
 *
 *     char *string = parcThread_ToString(a);
 *
 *     parcThread_Release(&a);
 *
 *     parcMemory_Deallocate(&string);
 * }
 * @endcode
 *
 * @see parcThread_Display
 */
char *parcThread_ToString(const PARCThread *instance);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in#> | <#out#> | <#in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
void parcThread_Start(PARCThread *thread);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in#> | <#out#> | <#in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
PARCObject *parcThread_GetArgument(const PARCThread *thread);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in#> | <#out#> | <#in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
bool parcThread_Cancel(const PARCThread *thread);
#endif
