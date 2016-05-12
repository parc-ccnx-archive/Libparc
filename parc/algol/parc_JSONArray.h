/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @file parc_JSONArray.h
 * @brief A JSON Array stores an array of JSON objects.
 * @ingroup inputoutput
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_JSONArray_h
#define libparc_parc_JSONArray_h

struct parcJSONArray;
typedef struct parcJSONArray PARCJSONArray;

#include <parc/algol/parc_JSONValue.h>

/**
 * Create an empty `PARCJSONArray` instance.
 *
 * @return A pointer to an empty `PARCJSONArray` instance.
 *
 * Example:
 * @code
 * {
 *     PARCJSONArray *x = parcJSONArray_Create();
 *     parcJSONArray_Release(&x);
 * }
 * @endcode
 *
 * @see parcJSONArray_AddValue
 */
PARCJSONArray *parcJSONArray_Create(void);

/**
 * Increase the number of references to a `PARCJSONArray`.
 *
 * Note that new `PARCJSONArray` is not created,
 * only that the given `PARCJSONArray` reference count is incremented.
 * Discard the reference by invoking {@link parcJSONArray_Release}.
 *
 * @param [in] array A pointer to a `PARCJSONArray` instance.
 *
 * @return The input `PARCJSONArray` pointer.
 *
 * Example:
 * @code
 * {
 *     PARCJSONArray *x = parcJSONArray_Create();
 *
 *     PARCJSONArray *x_2 = parcJSONArray_Acquire(x);
 *
 *     parcJSONArray_Release(&x);
 *     parcJSONArray_Release(&x_2);
 * }
 * @endcode
 */
PARCJSONArray *parcJSONArray_Acquire(const PARCJSONArray *array);

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
 * @param [in,out] arrayPtr A pointer to a pointer to the instance of `PARCJSONArray` to release.
 *
 * Example:
 * @code
 * {
 *     PARCJSONArray *x = parcJSONArray_Create();
 *
 *     parcJSONArray_Release(&x);
 * }
 * @endcode
 */
void parcJSONArray_Release(PARCJSONArray **arrayPtr);

/**
 * Assert that an instance of `PARCJSONArray` is valid.
 *
 * If the instance is not valid, terminate via {@link trapIllegalValue}
 *
 * Valid means the internal state of the type is consistent with its required current or future behaviour.
 * This may include the validation of internal instances of types.
 *
 * @param [in] array A pointer to a `PARCJSONArray` instance.
 */
void parcJSONArray_AssertValid(const PARCJSONArray *array);

/**
 * Determine if two `PARCJSONArray` instances are equal.
 *
 * The following equivalence relations on non-null `PARCJSONArray` instances are maintained:
 *
 *   * It is reflexive: for any non-null reference value x, `parcJSONArray_Equals(x, x)` must return true.
 *
 *   * It is symmetric: for any non-null reference values x and y, `parcJSONArray_Equals(x, y)` must return true if and only if
 *        `parcJSONArray_Equals(y x)` returns true.
 *
 *   * It is transitive: for any non-null reference values x, y, and z, if
 *        `parcJSONArray_Equals(x, y)` returns true and
 *        `parcJSONArray_Equals(y, z)` returns true,
 *        then `parcJSONArray_Equals(x, z)` must return true.
 *
 *   * It is consistent: for any non-null reference values x and y, multiple invocations of `parcJSONArray_Equals(x, y)`
 *         consistently return true or consistently return false.
 *
 *   * For any non-null reference value x, `parcJSONArray_Equals(x, NULL)` must return false.
 *
 * @param [in] x A pointer to a `PARCJSONArray` instance.
 * @param [in] y A pointer to a `PARCJSONArray` instance.
 *
 * @return true `PARCJSONArray` x and y are equal.
 * @return false `PARCJSONArray` x and y are not equal.
 *
 * Example:
 * @code
 * {
 *     PARCJSONArray *x = parcJSONArray_Create();
 *     PARCJSONArray *y = parcJSONArray_Create();
 *
 *     if (parcJSONArray_Equals(x, y)) {
 *         printf("Arrays are equal.\n");
 *     }  else {
 *         printf("Arrays are NOT equal.\n");
 *     }
 *
 *     parcJSONArray_Release(&x);
 *     parcJSONArray_Release(&y);
 * }
 * @endcode
 */
bool parcJSONArray_Equals(const PARCJSONArray *x, const PARCJSONArray *y);

/**
 * Add {@link PARCJSONValue} instance to the given `PARCJSONArray`.
 *
 * A new reference to the `PARCJSONValue` is acquired by this call.
 *
 * @param [in,out] array A pointer to a `PARCJSONArray` instance.
 * @param [in] value A pointer to a `PARCJSONValue` instance.
 *
 * @return A pointer to the @p array.
 *
 * Example:
 * @code
 * {
 *     PARCJSONArray *array = parcJSONArray_Create();
 *
 *     PARCJSONValue *value = parcJSONValue_CreateFromInteger(31415);
 *     parcJSONArray_AddValue(array, value);
 *     parcJSONValue_Release(&value);
 *
 *     parcJSONArray_Release(&array);
 * }
 * @endcode
 *
 */
PARCJSONArray *parcJSONArray_AddValue(PARCJSONArray *array, PARCJSONValue *value);

/**
 * Get the length of the given `PARCJSONArray` instance.
 *
 * @param [in] array A pointer to a `PARCJSONArray` instance.
 *
 * @return The number of elements in the array.
 *
 * Example:
 * @code
 * {
 *     PARCJSONArray *array = parcJSONArray_Create();
 *
 *     PARCJSONValue *value = parcJSONValue_CreateFromInteger(31415);
 *     parcJSONArray_AddValue(array, value);
 *     parcJSONValue_Release(&value);
 *
 *     parcJSONValue_GetLength(array);
 *
 *     parcJSONArray_Release(&array);
 * }
 * @endcode
 */
size_t parcJSONArray_GetLength(const PARCJSONArray *array);

/**
 * Get the {@link PARCJSONValue} stored at the given index in the `PARCJSONArray`.
 * A new reference is not acquired.
 * The caller must acquire its own reference if needed.
 *
 * @param [in] array A pointer to a `PARCJSONArray` instance.
 * @param [in] index The index of the `PARCJSONValue` to get.
 *
 * @return The pointer to the requested `PARCJSONValue`, or NULL if the index exceeded the length of the array.
 *
 * Example:
 * @code
 * {
 *     PARCJSONArray *array = parcJSONArray_Create();
 *
 *     PARCJSONValue *value = parcJSONValue_CreateFromInteger(31415);
 *     parcJSONArray_AddValue(array, value);
 *     parcJSONValue_Release(&value);
 *
 *     PARCJSONValue *actualValue = parcJSONValue_GetValue(array, 0);
 *
 *     parcJSONArray_Release(&array);
 * }
 * @endcode
 */
PARCJSONValue *parcJSONArray_GetValue(const PARCJSONArray *array, size_t index);

/**
 * Append a representation of the specified `PARCJSONArray` instance to the given
 * {@link PARCBufferComposer}.
 *
 * @param [in] array A pointer to the `PARCJSONArray` instance.
 * @param [in,out] composer A pointer to the `PARCBufferComposer` instance.
 *
 * @return NULL Cannot allocate memory.
 * @return non-NULL The given `PARCBufferComposer`.
 *
 * Example:
 * @code
 * {
 *     PARCBufferComposer *result = parcBufferComposer_Create();
 *
 *     parcJSONArray_BuildString(instance, result);
 *
 *     PARCBuffer *string = parcBufferComposer_FinalizeBuffer(result);
 *     printf("JSON Array: %s\n", parcBuffer_ToString(string));
 *     parcBuffer_Release(&string);
 *
 *     parcBufferComposer_Release(&result);
 * }
 * @endcode
 */
PARCBufferComposer *parcJSONArray_BuildString(const PARCJSONArray *array, PARCBufferComposer *composer, bool compact);

/**
 * Produce a null-terminated string representation of the specified instance.
 *
 * The result must be freed by the caller via {@link parcMemory_Deallocate}.
 *
 * @param [in] array A pointer to the instance.
 *
 * @return NULL Cannot allocate memory.
 * @return non-NULL A pointer to an allocated, null-terminated C string that must be deallocated via {@link parcMemory_Deallocate}.
 *
 * Example:
 * @code
 * {
 *     PARCJSONArray *array = parcJSONArray_Create();
 *
 *     PARCJSONValue *value = parcJSONValue_CreateFromInteger(31415);
 *     parcJSONArray_AddValue(array, value);
 *     parcJSONValue_Release(&value);
 *
 *     const char *string = parcJSONValue_ToString(array);
 *
 *     parcMemory_Deallocate((void **) &string);
 *
 *     parcJSONArray_Release(&array);
 * }
 * @endcode
 *
 * @see parcJSONArray_Display
 */
char *parcJSONArray_ToString(const PARCJSONArray *array);

/**
 * Produce a null-terminated compact string representation of the specified instance.
 *
 * The result must be freed by the caller via {@link parcMemory_Deallocate}.
 *
 * @param [in] array A pointer to the instance.
 *
 * @return NULL Cannot allocate memory.
 * @return non-NULL A pointer to an allocated, null-terminated C string that must be deallocated via {@link parcMemory_Deallocate}.
 *
 * Example:
 * @code
 * {
 *     PARCJSONArray *array = parcJSONArray_Create();
 *
 *     PARCJSONValue *value = parcJSONValue_CreateFromInteger(31415);
 *     parcJSONArray_AddValue(array, value);
 *     parcJSONValue_Release(&value);
 *
 *     const char *string = parcJSONValue_ToCompactString(array);
 *
 *     parcMemory_Deallocate((void **) &string);
 *
 *     parcJSONArray_Release(&array);
 * }
 * @endcode
 *
 * @see parcJSONArray_Display
 */
char *parcJSONArray_ToCompactString(const PARCJSONArray *array);

/**
 * Pretty print the given `PARCJSONArray` instance.
 *
 * @param [in] array The `PARCJSONArray` instance to be printed.
 * @param [in] indentation The amount of indentation to prefix each line of output
 *
 * Example:
 * @code
 * {
 *     PARCJSONArray *array = parcJSON_ParseString("{ \"string\" : \"xyzzy\" }");
 *     parcJSONArray_Display(array, 0);
 * }
 * @endcode
 */
void parcJSONArray_Display(const PARCJSONArray *array, int indentation);
#endif // libparc_parc_JSONArray_h
