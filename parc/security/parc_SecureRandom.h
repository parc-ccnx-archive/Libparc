/*
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * Copyright 2015 Palo Alto Research Center, Inc. (PARC), a Xerox company.  All Rights Reserved.
 * The content of this file, whole or in part, is subject to licensing terms.
 * If distributing this software, include this License Header Notice in each
 * file and provide the accompanying LICENSE file.
 */
/**
 * @file parc_DTLSLink.h
 * @brief <#Brief Description#>
 *
 * <#Detailed Description#>
 *
 * @author Christopher A. Wood, Computing Science Laboratory, PARC
 * @copyright 2015 Palo Alto Research Center, Inc. (PARC), A Xerox Company.  All Rights Reserved.
 */
#ifndef Libparc_parc_SecureRandom
#define Libparc_parc_SecureRandom
#include <stdbool.h>

#include <parc/algol/parc_JSON.h>
#include <parc/algol/parc_HashCode.h>

struct parc_securerandom;
typedef struct parc_securerandom PARCSecureRandom;

/**
 * Increase the number of references to a `PARCSecureRandom` instance.
 *
 * Note that new `PARCSecureRandom` is not created,
 * only that the given `PARCSecureRandom` reference count is incremented.
 * Discard the reference by invoking `parcSecureRandom_Release`.
 *
 * @param [in] instance A pointer to a valid PARCSecureRandom instance.
 *
 * @return The same value as @p instance.
 *
 * Example:
 * @code
 * {
 *     PARCSecureRandom *a = parcSecureRandom_Create();
 *
 *     PARCSecureRandom *b = parcSecureRandom_Acquire();

 *     parcSecureRandom_Release(&a);
 *     parcSecureRandom_Release(&b);
 * }
 * @endcode
 */
PARCSecureRandom *parcSecureRandom_Acquire(const PARCSecureRandom *instance);

#ifdef Libparc_DISABLE_VALIDATION
#  define parcSecureRandom_OptionalAssertValid(_instance_)
#else
#  define parcSecureRandom_OptionalAssertValid(_instance_) parcSecureRandom_AssertValid(_instance_)
#endif

/**
 * Assert that the given `PARCSecureRandom` instance is valid.
 *
 * @param [in] instance A pointer to a valid PARCSecureRandom instance.
 *
 * Example:
 * @code
 * {
 *     PARCSecureRandom *a = parcSecureRandom_Create();
 *
 *     parcSecureRandom_AssertValid(a);
 *
 *     printf("Instance is valid.\n");
 *
 *     parcSecureRandom_Release(&b);
 * }
 * @endcode
 */
void parcSecureRandom_AssertValid(const PARCSecureRandom *instance);

/**
 * Create an instance of PARCSecureRandom
 *
 * <#Paragraphs Of Explanation#>
 *
 * @return non-NULL A pointer to a valid PARCSecureRandom instance.
 * @return NULL An error occurred.
 *
 * Example:
 * @code
 * {
 *     PARCSecureRandom *a = parcSecureRandom_Create();
 *
 *     parcSecureRandom_Release(&a);
 * }
 * @endcode
 */
PARCSecureRandom *parcSecureRandom_Create(void);

// CODO
PARCSecureRandom *parcSecureRandom_CreateWithSeed(PARCBuffer *seed);

// CODO
uint32_t parcSecureRandom_Next(PARCSecureRandom *random);

// CODO
int parcSecureRandom_NextBytes(PARCSecureRandom *random, PARCBuffer *buffer);

/**
 * Compares @p instance with @p other for order.
 *
 * Returns a negative integer, zero, or a positive integer as @p instance
 * is less than, equal to, or greater than @p other.
 *
 * @param [in] instance A pointer to a valid PARCSecureRandom instance.
 * @param [in] other A pointer to a valid PARCSecureRandom instance.
 *
 * @return <0 Instance is less than @p other.
 * @return 0 Instance a and instance b compare the same.
 * @return >0 Instance a is greater than instance b.
 *
 * Example:
 * @code
 * {
 *     PARCSecureRandom *a = parcSecureRandom_Create();
 *     PARCSecureRandom *b = parcSecureRandom_Create();
 *
 *     if (parcSecureRandom_Compare(a, b) == 0) {
 *         printf("Instances are equal.\n");
 *     }
 *
 *     parcSecureRandom_Release(&a);
 *     parcSecureRandom_Release(&b);
 * }
 * @endcode
 *
 * @see parcSecureRandom_Equals
 */
int parcSecureRandom_Compare(const PARCSecureRandom *instance, const PARCSecureRandom *other);

/**
 * Create an independent copy the given `PARCBuffer`
 *
 * A new buffer is created as a complete copy of the original.
 *
 * @param [in] original A pointer to a valid PARCSecureRandom instance.
 *
 * @return NULL Memory could not be allocated.
 * @return non-NULL A pointer to a new `PARCSecureRandom` instance.
 *
 * Example:
 * @code
 * {
 *     PARCSecureRandom *a = parcSecureRandom_Create();
 *
 *     PARCSecureRandom *copy = parcSecureRandom_Copy(&b);
 *
 *     parcSecureRandom_Release(&b);
 *     parcSecureRandom_Release(&copy);
 * }
 * @endcode
 */
PARCSecureRandom *parcSecureRandom_Copy(const PARCSecureRandom *original);

/**
 * Print a human readable representation of the given `PARCSecureRandom`.
 *
 * @param [in] instance A pointer to a valid PARCSecureRandom instance.
 * @param [in] indentation The indentation level to use for printing.
 *
 * Example:
 * @code
 * {
 *     PARCSecureRandom *a = parcSecureRandom_Create();
 *
 *     parcSecureRandom_Display(a, 0);
 *
 *     parcSecureRandom_Release(&a);
 * }
 * @endcode
 */
void parcSecureRandom_Display(const PARCSecureRandom *instance, int indentation);

/**
 * Determine if two `PARCSecureRandom` instances are equal.
 *
 * The following equivalence relations on non-null `PARCSecureRandom` instances are maintained: *
 *   * It is reflexive: for any non-null reference value x, `parcSecureRandom_Equals(x, x)` must return true.
 *
 *   * It is symmetric: for any non-null reference values x and y, `parcSecureRandom_Equals(x, y)` must return true if and only if
 *        `parcSecureRandom_Equals(y x)` returns true.
 *
 *   * It is transitive: for any non-null reference values x, y, and z, if
 *        `parcSecureRandom_Equals(x, y)` returns true and
 *        `parcSecureRandom_Equals(y, z)` returns true,
 *        then `parcSecureRandom_Equals(x, z)` must return true.
 *
 *   * It is consistent: for any non-null reference values x and y, multiple invocations of `parcSecureRandom_Equals(x, y)`
 *         consistently return true or consistently return false.
 *
 *   * For any non-null reference value x, `parcSecureRandom_Equals(x, NULL)` must return false.
 *
 * @param [in] x A pointer to a valid PARCSecureRandom instance.
 * @param [in] y A pointer to a valid PARCSecureRandom instance.
 *
 * @return true The instances x and y are equal.
 *
 * Example:
 * @code
 * {
 *     PARCSecureRandom *a = parcSecureRandom_Create();
 *     PARCSecureRandom *b = parcSecureRandom_Create();
 *
 *     if (parcSecureRandom_Equals(a, b)) {
 *         printf("Instances are equal.\n");
 *     }
 *
 *     parcSecureRandom_Release(&a);
 *     parcSecureRandom_Release(&b);
 * }
 * @endcode
 * @see parcSecureRandom_HashCode
 */
bool parcSecureRandom_Equals(const PARCSecureRandom *x, const PARCSecureRandom *y);

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
 * If two instances are equal according to the {@link parcSecureRandom_Equals} method,
 * then calling the {@link parcSecureRandom_HashCode} method on each of the two instances must produce the same integer result.
 *
 * It is not required that if two instances are unequal according to the
 * {@link parcSecureRandom_Equals} function,
 * then calling the `parcSecureRandom_HashCode`
 * method on each of the two objects must produce distinct integer results.
 *
 * @param [in] instance A pointer to a valid PARCSecureRandom instance.
 *
 * @return The hashcode for the given instance.
 *
 * Example:
 * @code
 * {
 *     PARCSecureRandom *a = parcSecureRandom_Create();
 *
 *     PARCHashCode hashValue = parcSecureRandom_HashCode(buffer);
 *     parcSecureRandom_Release(&a);
 * }
 * @endcode
 */
PARCHashCode parcSecureRandom_HashCode(const PARCSecureRandom *instance);

/**
 * Determine if an instance of `PARCSecureRandom` is valid.
 *
 * Valid means the internal state of the type is consistent with its required current or future behaviour.
 * This may include the validation of internal instances of types.
 *
 * @param [in] instance A pointer to a valid PARCSecureRandom instance.
 *
 * @return true The instance is valid.
 * @return false The instance is not valid.
 *
 * Example:
 * @code
 * {
 *     PARCSecureRandom *a = parcSecureRandom_Create();
 *
 *     if (parcSecureRandom_IsValid(a)) {
 *         printf("Instance is valid.\n");
 *     }
 *
 *     parcSecureRandom_Release(&a);
 * }
 * @endcode
 *
 */
bool parcSecureRandom_IsValid(const PARCSecureRandom *instance);

/**
 * Release a previously acquired reference to the given `PARCSecureRandom` instance,
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
 *     PARCSecureRandom *a = parcSecureRandom_Create();
 *
 *     parcSecureRandom_Release(&a);
 * }
 * @endcode
 */
void parcSecureRandom_Release(PARCSecureRandom **instancePtr);

/**
 * Create a `PARCJSON` instance (representation) of the given object.
 *
 * @param [in] instance A pointer to a valid PARCSecureRandom instance.
 *
 * @return NULL Memory could not be allocated to contain the `PARCJSON` instance.
 * @return non-NULL An allocated C string that must be deallocated via parcMemory_Deallocate().
 *
 * Example:
 * @code
 * {
 *     PARCSecureRandom *a = parcSecureRandom_Create();
 *
 *     PARCJSON *json = parcSecureRandom_ToJSON(a);
 *
 *     printf("JSON representation: %s\n", parcJSON_ToString(json));
 *     parcJSON_Release(&json);
 *
 *     parcSecureRandom_Release(&a);
 * }
 * @endcode
 */
PARCJSON *parcSecureRandom_ToJSON(const PARCSecureRandom *instance);

/**
 * Produce a null-terminated string representation of the specified `PARCSecureRandom`.
 *
 * The result must be freed by the caller via {@link parcMemory_Deallocate}.
 *
 * @param [in] instance A pointer to a valid PARCSecureRandom instance.
 *
 * @return NULL Cannot allocate memory.
 * @return non-NULL A pointer to an allocated, null-terminated C string that must be deallocated via {@link parcMemory_Deallocate}.
 *
 * Example:
 * @code
 * {
 *     PARCSecureRandom *a = parcSecureRandom_Create();
 *
 *     char *string = parcSecureRandom_ToString(a);
 *
 *     parcSecureRandom_Release(&a);
 *
 *     parcMemory_Deallocate(&string);
 * }
 * @endcode
 *
 * @see parcSecureRandom_Display
 */
char *parcSecureRandom_ToString(const PARCSecureRandom *instance);
#endif
