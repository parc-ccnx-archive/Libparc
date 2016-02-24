/*
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * Copyright 2015 Palo Alto Research Center, Inc. (PARC), a Xerox company.  All Rights Reserved.
 * The content of this file, whole or in part, is subject to licensing terms.
 * If distributing this software, include this License Header Notice in each
 * file and provide the accompanying LICENSE file.
 */
/**
 * @file parc_Timer.h
 * @brief <#Brief Description#>
 *
 * <#Detailed Description#>
 *
 * @author <#gscott#>, Computing Science Laboratory, PARC
 * @copyright 2015 Palo Alto Research Center, Inc. (PARC), A Xerox Company.  All Rights Reserved.
 */
#ifndef PARCLibrary_parc_Timer
#define PARCLibrary_parc_Timer
#include <stdbool.h>

#include <parc/algol/parc_JSON.h>
#include <parc/algol/parc_HashCode.h>

struct PARCTimer;
typedef struct PARCTimer PARCTimer;

/**
 * Increase the number of references to a `PARCTimer` instance.
 *
 * Note that new `PARCTimer` is not created,
 * only that the given `PARCTimer` reference count is incremented.
 * Discard the reference by invoking `parcTimer_Release`.
 *
 * @param [in] instance A pointer to a valid PARCTimer instance.
 *
 * @return The same value as @p instance.
 *
 * Example:
 * @code
 * {
 *     PARCTimer *a = parcTimer_Create();
 *
 *     PARCTimer *b = parcTimer_Acquire();

 *     parcTimer_Release(&a);
 *     parcTimer_Release(&b);
 * }
 * @endcode
 */
PARCTimer *parcTimer_Acquire(const PARCTimer *instance);

#ifdef PARCLibrary_DISABLE_VALIDATION
#  define parcTimer_OptionalAssertValid(_instance_)
#else
#  define parcTimer_OptionalAssertValid(_instance_) parcTimer_AssertValid(_instance_)
#endif

/**
 * Assert that the given `PARCTimer` instance is valid.
 *
 * @param [in] instance A pointer to a valid PARCTimer instance.
 *
 * Example:
 * @code
 * {
 *     PARCTimer *a = parcTimer_Create();
 *
 *     parcTimer_AssertValid(a);
 *
 *     printf("Instance is valid.\n");
 *
 *     parcTimer_Release(&b);
 * }
 * @endcode
 */
void parcTimer_AssertValid(const PARCTimer *instance);

/**
 * Create an instance of PARCTimer
 *
 * <#Paragraphs Of Explanation#>
 *
 * @return non-NULL A pointer to a valid PARCTimer instance.
 * @return NULL An error occurred.
 *
 * Example:
 * @code
 * {
 *     PARCTimer *a = parcTimer_Create();
 *
 *     parcTimer_Release(&a);
 * }
 * @endcode
 */
PARCTimer *parcTimer_Create(void);

/**
 * Compares @p instance with @p other for order.
 *
 * Returns a negative integer, zero, or a positive integer as @p instance
 * is less than, equal to, or greater than @p other.
 *
 * @param [in] instance A pointer to a valid PARCTimer instance.
 * @param [in] other A pointer to a valid PARCTimer instance.
 *
 * @return <0 Instance is less than @p other.
 * @return 0 Instance a and instance b compare the same.
 * @return >0 Instance a is greater than instance b.
 *
 * Example:
 * @code
 * {
 *     PARCTimer *a = parcTimer_Create();
 *     PARCTimer *b = parcTimer_Create();
 *
 *     if (parcTimer_Compare(a, b) == 0) {
 *         printf("Instances are equal.\n");
 *     }
 *
 *     parcTimer_Release(&a);
 *     parcTimer_Release(&b);
 * }
 * @endcode
 *
 * @see parcTimer_Equals
 */
int parcTimer_Compare(const PARCTimer *instance, const PARCTimer *other);

/**
 * Create an independent copy the given `PARCBuffer`
 *
 * A new buffer is created as a complete copy of the original.
 *
 * @param [in] original A pointer to a valid PARCTimer instance.
 *
 * @return NULL Memory could not be allocated.
 * @return non-NULL A pointer to a new `PARCTimer` instance.
 *
 * Example:
 * @code
 * {
 *     PARCTimer *a = parcTimer_Create();
 *
 *     PARCTimer *copy = parcTimer_Copy(&b);
 *
 *     parcTimer_Release(&b);
 *     parcTimer_Release(&copy);
 * }
 * @endcode
 */
PARCTimer *parcTimer_Copy(const PARCTimer *original);

/**
 * Print a human readable representation of the given `PARCTimer`.
 *
 * @param [in] instance A pointer to a valid PARCTimer instance.
 * @param [in] indentation The indentation level to use for printing.
 *
 * Example:
 * @code
 * {
 *     PARCTimer *a = parcTimer_Create();
 *
 *     parcTimer_Display(a, 0);
 *
 *     parcTimer_Release(&a);
 * }
 * @endcode
 */
void parcTimer_Display(const PARCTimer *instance, int indentation);

/**
 * Determine if two `PARCTimer` instances are equal.
 *
 * The following equivalence relations on non-null `PARCTimer` instances are maintained: *
 *   * It is reflexive: for any non-null reference value x, `parcTimer_Equals(x, x)` must return true.
 *
 *   * It is symmetric: for any non-null reference values x and y, `parcTimer_Equals(x, y)` must return true if and only if
 *        `parcTimer_Equals(y x)` returns true.
 *
 *   * It is transitive: for any non-null reference values x, y, and z, if
 *        `parcTimer_Equals(x, y)` returns true and
 *        `parcTimer_Equals(y, z)` returns true,
 *        then `parcTimer_Equals(x, z)` must return true.
 *
 *   * It is consistent: for any non-null reference values x and y, multiple invocations of `parcTimer_Equals(x, y)`
 *         consistently return true or consistently return false.
 *
 *   * For any non-null reference value x, `parcTimer_Equals(x, NULL)` must return false.
 *
 * @param [in] x A pointer to a valid PARCTimer instance.
 * @param [in] y A pointer to a valid PARCTimer instance.
 *
 * @return true The instances x and y are equal.
 *
 * Example:
 * @code
 * {
 *     PARCTimer *a = parcTimer_Create();
 *     PARCTimer *b = parcTimer_Create();
 *
 *     if (parcTimer_Equals(a, b)) {
 *         printf("Instances are equal.\n");
 *     }
 *
 *     parcTimer_Release(&a);
 *     parcTimer_Release(&b);
 * }
 * @endcode
 * @see parcTimer_HashCode
 */
bool parcTimer_Equals(const PARCTimer *x, const PARCTimer *y);

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
 * If two instances are equal according to the {@link parcTimer_Equals} method,
 * then calling the {@link parcTimer_HashCode} method on each of the two instances must produce the same integer result.
 *
 * It is not required that if two instances are unequal according to the
 * {@link parcTimer_Equals} function,
 * then calling the `parcTimer_HashCode`
 * method on each of the two objects must produce distinct integer results.
 *
 * @param [in] instance A pointer to a valid PARCTimer instance.
 *
 * @return The hashcode for the given instance.
 *
 * Example:
 * @code
 * {
 *     PARCTimer *a = parcTimer_Create();
 *
 *     PARCHashCode hashValue = parcTimer_HashCode(buffer);
 *     parcTimer_Release(&a);
 * }
 * @endcode
 */
PARCHashCode parcTimer_HashCode(const PARCTimer *instance);

/**
 * Determine if an instance of `PARCTimer` is valid.
 *
 * Valid means the internal state of the type is consistent with its required current or future behaviour.
 * This may include the validation of internal instances of types.
 *
 * @param [in] instance A pointer to a valid PARCTimer instance.
 *
 * @return true The instance is valid.
 * @return false The instance is not valid.
 *
 * Example:
 * @code
 * {
 *     PARCTimer *a = parcTimer_Create();
 *
 *     if (parcTimer_IsValid(a)) {
 *         printf("Instance is valid.\n");
 *     }
 *
 *     parcTimer_Release(&a);
 * }
 * @endcode
 *
 */
bool parcTimer_IsValid(const PARCTimer *instance);

/**
 * Release a previously acquired reference to the given `PARCTimer` instance,
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
 *     PARCTimer *a = parcTimer_Create();
 *
 *     parcTimer_Release(&a);
 * }
 * @endcode
 */
void parcTimer_Release(PARCTimer **instancePtr);

/**
 * Create a `PARCJSON` instance (representation) of the given object.
 *
 * @param [in] instance A pointer to a valid PARCTimer instance.
 *
 * @return NULL Memory could not be allocated to contain the `PARCJSON` instance.
 * @return non-NULL An allocated C string that must be deallocated via parcMemory_Deallocate().
 *
 * Example:
 * @code
 * {
 *     PARCTimer *a = parcTimer_Create();
 *
 *     PARCJSON *json = parcTimer_ToJSON(a);
 *
 *     printf("JSON representation: %s\n", parcJSON_ToString(json));
 *     parcJSON_Release(&json);
 *
 *     parcTimer_Release(&a);
 * }
 * @endcode
 */
PARCJSON *parcTimer_ToJSON(const PARCTimer *instance);

/**
 * Produce a null-terminated string representation of the specified `PARCTimer`.
 *
 * The result must be freed by the caller via {@link parcMemory_Deallocate}.
 *
 * @param [in] instance A pointer to a valid PARCTimer instance.
 *
 * @return NULL Cannot allocate memory.
 * @return non-NULL A pointer to an allocated, null-terminated C string that must be deallocated via {@link parcMemory_Deallocate}.
 *
 * Example:
 * @code
 * {
 *     PARCTimer *a = parcTimer_Create();
 *
 *     char *string = parcTimer_ToString(a);
 *
 *     parcTimer_Release(&a);
 *
 *     parcMemory_Deallocate(&string);
 * }
 * @endcode
 *
 * @see parcTimer_Display
 */
char *parcTimer_ToString(const PARCTimer *instance);
#endif
