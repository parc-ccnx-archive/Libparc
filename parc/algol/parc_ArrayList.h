/*
 * Copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @file parc_ArrayList.h
 * @ingroup datastructures
 * @brief A dynamic array of void * pointers.
 *
 * This module implements a dynamic array of simple void pointers.
 * Users can create an empty array, or preprovisioned with an initial capacity of N elements.
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_ArrayList_h
#define libparc_parc_ArrayList_h

#include <stdint.h>
#include <stdbool.h>

#include <parc/algol/parc_List.h>
#include <parc/algol/parc_Iterator.h>

#ifdef PARCLibrary_DISABLE_VALIDATION
#  define parcArrayList_OptionalAssertValid(_instance_)
#else
#  define parcArrayList_OptionalAssertValid(_instance_) parcArrayList_AssertValid(_instance_)
#endif

struct parc_array_list;
typedef struct parc_array_list PARCArrayList;

/**
 * The mapping of a `PARCArrayList` to the generic `PARCList`.
 */
extern PARCListInterface *PARCArrayListAsPARCList;

/**
 * Determine if a `PARCArrayList` is valid.
 *
 * @param [in] instance A pointer to a PARCArrayList instance.
 *
 * @return true The instance is valid.
 * @return true The instance is invalid.
 *
 * Example:
 * @code
 * {
 *     PARCArrayList *list = parcArrayList_Create(NULL);
 *     if (parcArrayList_IsValid(list)) {
 *         printf("The list is valid.");
 *     }
 *     parcArrayList_Destroy((list);
 * @endcode
 */
bool parcArrayList_IsValid(const PARCArrayList *instance);

/**
 * Assert that a `PARCArrayList` is valid.
 *
 * @param [in] instance A pointer to a PARCArrayList instance.
 *
 * Example:
 * @code
 * {
 *     PARCArrayList *list = parcArrayList_Create(NULL);
 *
 *     parcArrayList_AssertValid(list);
 *
 *     parcArrayList_Destroy((list);
 * @endcode
 */
void parcArrayList_AssertValid(const PARCArrayList *instance);

/**
 * Add an element to the given PARCArrayList.
 *
 * If the PARCArrayList was constructed with a destroyer,
 * the pointer will be destroyed when element is removed or the list is destroyed.
 *
 * @param [in] array A pointer to a `PARCArrayList`.
 * @param [in] pointer A void pointer to the item to be added to the list.
 *
 * @return True If the value was successfully added to the list.
 * @return False Otherwise
 *
 * Example:
 * @code
 * {
 *     PARCArrayList *array = parcArrayList_Create(parcArrayList_StdlibFreeFunction);
 *     void *elements[] = {
 *         strdup("a"),
 *         strdup("b"),
 *         strdup("c"),
 *     };
 *
 *     bool wasAdded = parcArrayList_Add(array, elements);
 *     printf("Successfully added the base pointer? %d\n", wasAdded);
 *
 *     ...
 *
 *     parcArrayList_Destroy(&array);
 * }
 * @endcode
 */
bool parcArrayList_Add(PARCArrayList *array, const void *pointer);

/**
 * Add all of the pointers in the given array of pointers to the `PARCArrayList`.
 *
 * This is synonymous with calling {@link parcArrayList_Add()} multiple times
 *
 * @param [in] array A pointer to `PARCArrayList`.
 * @param [in] argv A pointer to the base list of pointers.
 * @param [in] argc The number of items to add to the list, starting from the abse void pointer.
 *
 * @return A pointer to the modified PARCArrayList.
 *
 * Example:
 * @code
 * {
 *     PARCArrayList *array = parcArrayList_Create(parcArrayList_StdlibFreeFunction);
 *     void *elements[] = {
 *         strdup("a"),
 *         strdup("b"),
 *         strdup("c"),
 *     };
 *
 *     parcArrayList_AddAll(array, elements, 3);
 *
 *     ...
 *
 *     parcArrayList_Destroy(&array);
 * }
 * @endcode
 */
PARCArrayList *parcArrayList_AddAll(PARCArrayList *array, void *argv[], size_t argc);

/**
 * Remove an element at a specific index from a `PARCArrayList`.
 *
 * The element is destroyed via the function provided when calling {@link parcArrayList_Create()}.
 * The index must be 0 <= index < length.
 *
 * @param [in,out] array A pointer to `PARCArrayList`.
 * @param [in] index The index of the element to remove and destroy.
 *
 * @return A pointer to the modified `PARCArrayList`.
 *
 * Example:
 * @code
 * {
 *     PARCArrayList *array = parcArrayList_Create(parcArrayList_StdlibFreeFunction);
 *     void *elements[] = {
 *         strdup("a"),
 *         strdup("b"),
 *         strdup("c"),
 *     };
 *
 *     parcArrayList_AddAll(array, elements, 3);
 *     parcArrayList_RemoveAndDestroyAtIndex(array, 0);
 *
 *     size_t size = parcArrayList_Size(array);
 *     // size will now be three
 *
 *     ...
 *
 *     parcArrayList_Destroy(&array);
 * }
 * @endcode
 */
PARCArrayList *parcArrayList_RemoveAndDestroyAtIndex(PARCArrayList *array, size_t index);

/**
 * Return the element at index. Remove the element from the array.
 *
 * The index must be 0 <= index < length.
 *
 * @param [in,out] array A pointer to a `PARCArrayList` instance.
 * @param [in] index The index of the element to remove.
 *
 * @return void* A pointer (void *) to the element that was removed.
 *
 * Example:
 * @code
 * {
 *     PARCArrayList *array = parcArrayList_Create(parcArrayList_StdlibFreeFunction);
 *     void *elements[] = {
 *         strdup("a"),
 *         strdup("b"),
 *         strdup("c"),
 *     };
 *
 *     parcArrayList_AddAll(array, elements, 3);
 *     void *firstString = parcArrayList_RemoveAtIndex(array, 0);
 *
 *     printf("First string: %s\n", firstString);
 *
 *     ...
 *
 *     parcArrayList_Destroy(&array);
 * }
 * @endcode
 */
void *parcArrayList_RemoveAtIndex(PARCArrayList *array, size_t index);

/**
 * Insert an element at the index location. Elements will be moved up if required.
 *
 * You may not insert beyond the list end. (You can insert at the end, but not any more).
 *
 * @param [in,out] array A `PARCArrayList`
 * @param [in] pointer A pointer to the element to insert
 * @param [in] index An index in the `PARCArrayList`
 * @return A pointer to the modified `PARCArrayList`
 *
 * Example:
 * @code
 * {
 *     PARCArrayList *array = parcArrayList_Create(parcArrayList_StdlibFreeFunction);
 *
 *     parcArrayList_InsertAtIndex(array, strdup("a"), 0);
 *     void *firstString = parcArrayList_RemoveAtIndex(array, 0);
 *     printf("First string is 'a': %s\n", firstString);
 *
 *     ...
 *
 *     parcArrayList_Destroy(&array);
 * }
 * @endcode
 */
PARCArrayList *parcArrayList_InsertAtIndex(PARCArrayList *array, size_t index, const void *pointer);

/**
 * Create an instance of an empty `PARCArrayList`.
 *
 * @param [in] destroyElement
 *      A pointer to a function that will destroy (or equivalent) the element pointed to by `element`
 *
 * @return A pointer to the new `PARCArrayList` instance, or NULL if no memory could be allocated.
 *
 * Example:
 * @code
 * {
 *     PARCArrayList *array = parcArrayList_Create(parcArrayList_StdlibFreeFunction);
 *     ...
 * }
 * @endcode
 */
PARCArrayList *parcArrayList_Create(void (*destroyElement)(void **elementAddress));

/**
 * Create an instance of a PARCArrayList pre-provisioned to contain the specified number of elements.
 *
 * @param [in] equalsElement
 *      A pointer to a function that will determine equality between two elements in the array.
 * @param [in] destroyElement
 *      A pointer to a function that will destroy (or equivalent) the element pointed to by <code>element</code>
 * @param [in] size
 *      The number of initial elements to provision for.
 * @return A pointer to a new `PARCArrayList` instance, or NULL if no memory could be allocated.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCArrayList *parcArrayList_Create_Capacity(bool (*equalsElement)(void *x, void *y), void (*destroyElement)(void **elementAddress), size_t size);

/**
 * Copy a `PARCArrayList` instance.
 *
 * Create a new `PARCArrayList` instance with the same structure and content as the original.
 *
 * @param [in] array A pointer to a `PARCArrayList` instance to copy.
 *
 * @return A pointer to a new `PARCArrayList` instance with a copy of the original, or NULL if no memory could be allocated.
 *
 * Example:
 * @code
 * {
 *     PARCArrayList *array = parcArrayList_Create(parcArrayList_StdlibFreeFunction);
 *     parcArrayList_InsertAtIndex(array, strdup("a"), 0);
 *
 *     PARCArrayList *sameList = parcArrayList_Copy(array);
 *
 *     if (parcArrayList_Equals(array, sameList)) {
 *         // true
 *     } else {
 *         // false
 *     }
 *
 *     parcArrayList_Destroy(&array);
 * }
 * @endcode
 */
PARCArrayList *parcArrayList_Copy(const PARCArrayList *array);

/**
 * Destroy a `PARCArrayList` instance.
 *
 * Destroy the given `PARCArrayList` by freeing all memory used by it.
 *
 * @param [in,out] arrayPtr A pointer to the pointer of the `PARCArrayList` to be destroyed.
 *
 * Example:
 * @code
 * {
 *     PARCArrayList *array = parcArrayList_Create(parcArrayList_StdlibFreeFunction);
 *
 *     ...
 *
 *     parcArrayList_Destroy(&array);
 * }
 * @endcode
 */
void parcArrayList_Destroy(PARCArrayList **arrayPtr);

/**
 * Set an element from the given `PARCArrayList` at a specified index to a value.
 *
 * The index must be 0 <= index < length.
 *
 * @param [in] array A pointer to a `PARCArrayList` instance.
 * @param [in] index The index into the `PARCArrayList` instance.
 * @param [in] pointer A void pointer to the item to be assigned to the index
 *
 * Example:
 * @code
 * {
 *     PARCArrayList *array = parcArrayList_Create(parcArrayList_StdlibFreeFunction);
 *     void *elements[] = {
 *         strdup("a"),
 *         strdup("b"),
 *         strdup("c"),
 *     };
 *
 *     bool wasAdded = parcArrayList_Add(array, elements);
 *     // how to free the pointer at index 1 without deallocating the slot? XXX
 *     parcArrayList_Set(array, 1, strdup("d");
 *     parcArrayList_Destroy(&array);
 * }
 * @endcode
 */
void parcArrayList_Set(const PARCArrayList *array, size_t index, void *pointer);

/**
 * Get an element from the given `PARCArrayList` at a specified index.
 *
 * The index must be 0 <= index < length.
 *
 * @param [in] array A pointer to a `PARCArrayList` instance.
 * @param [in] index The index into the `PARCArrayList` instance.
 *
 * @return A pointer (void *) to the element in the list.
 *
 * Example:
 * @code
 * {
 *     PARCArrayList *array = parcArrayList_Create(parcArrayList_StdlibFreeFunction);
 *     void *elements[] = {
 *         strdup("a"),
 *         strdup("b"),
 *         strdup("c"),
 *     };
 *
 *     parcArrayList_AddAll(array, elements, 3);
 *     void *firstString = parcArrayList_Get(array, 0);
 *
 *     printf("First string: %s\n", firstString);
 *
 *     ...
 *
 *     parcArrayList_Destroy(&array);
 * }
 * @endcode
 */
void *parcArrayList_Get(const PARCArrayList *array, size_t index);

/**
 * Return the number of elements in the given `PARCArrayList`.
 *
 * The size is the number of elements, NOT the size in bytes of all elements maintained in the list.
 *
 * @param [in] array A pointer to a `PARCArrayList` instance.
 *
 * @return A size_t of the number of elements in the given `PARCArrayList`.
 *
 * Example:
 * @code
 * {
 *     PARCArrayList *array = parcArrayList_Create(parcArrayList_StdlibFreeFunction);
 *     void *elements[] = {
 *         strdup("a"),
 *         strdup("b"),
 *         strdup("c"),
 *     };
 *
 *     parcArrayList_AddAll(array, elements, 3);
 *
 *     size_t size = parcArrayList_Size(array);
 *     printf("Size =?= 3: %d\n", size == 3);
 *
 *     parcArrayList_Destroy(&array);
 * }
 * @endcode
 */
size_t parcArrayList_Size(const PARCArrayList *array);

/**
 * Determine if two `PARCArrayList` instances are equal.
 *
 * The following equivalence relations on non-null `PARCArrayList` instances are maintained:
 *
 *   * It is reflexive: for any non-null reference value x, parcArrayList_Equals(x, x) must return true.
 *
 *   * It is symmetric: for any non-null reference values x and y, parcArrayList_Equals(x, y) must return true if and only if
 *        parcArrayList_Equals(y x) returns true.
 *
 *   * It is transitive: for any non-null reference values x, y, and z, if
 *        parcArrayList_Equals(x, y) returns true and
 *        parcArrayList_Equals(y, z) returns true,
 *        then  parcArrayList_Equals(x, z) must return true.
 *
 *   * It is consistent: for any non-null reference values x and y, multiple invocations of parcArrayList_Equals(x, y)
 *         consistently return true or consistently return false.
 *
 *   * For any non-null reference value x, parcArrayList_Equals(x, NULL)) must return false.
 *
 * @param [in] a The first `PARCArrayList` instance.
 * @param [in] b The second `PARCArrayList` instance.
 *
 * @return true the given PARCArrayList instances are equal.
 * @return false the given PARCArrayList instances are not equal.
 *
 * Example:
 * @code
 * {
 *    PARCArrayList *a = parcArrayList_Create();
 *    PARCArrayList *b = parcArrayList_Create();
 *
 *    if (parcArrayList_Equals(a, b)) {
 *        // true
 *    } else {
 *        // false
 *    }
 * }
 * @endcode
 */
bool parcArrayList_Equals(const PARCArrayList *a, const PARCArrayList *b);

/**
 * Standard library free(3) wrapper for use as a destructor function for elements of a `PARCArrayList`.
 *
 *   The create functions for `PARCArrayList` have an input parameter that is a pointer to a function that
 *   will be called for each element when the `PARCArrayList` is destroyed, and when an element is removed via
 *   {@link parcArrayList_RemoveAndDestroyAtIndex}.
 *   This destroy function has a different calling signature than the standard library's free(3) function.
 *   This function is a wrapper providing the correct facade for the standard library free(3) function.
 *
 * @param [in,out] element A pointer to the element to be destroyed.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void parcArrayList_StdlibFreeFunction(void **element);

/**
 * PARC_Memory free() wrapper for use as a destructor function for elements of a `PARCArrayList`.
 *
 *   The create functions for `PARCArrayList` have an input parameter that is a pointer to a function that
 *   will be called for each element when the `PARCArrayList` is destroyed, and when an element is removed via
 *   {@link parcArrayList_RemoveAndDestroyAtIndex}.
 *   This destroy function has a different calling signature than the PARC_Memory's free(3) function.
 *   This function is a wrapper providing the correct facade for the PARC_Memory's free(3) function.
 *
 * @param [in,out] elementPtr A pointer to the element to be destroyed.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void parcArrayList_PARCMemoryFreeFunction(void **elementPtr);

/**
 * Tests if this list is empty.
 *
 *
 * @param [in] list A pointer to the `PARCArrayList` to test.
 * @return true if the stack is empty.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
bool parcArrayList_IsEmpty(const PARCArrayList *list);

/**
 * Returns a pointer to object at the top of this stack without removing it from the stack.
 *
 * @param [in] list A pointer to `PARCArrayList`.
 * @return  A pointer to the first element of the `PARCArrayList`.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void *parcArrayList_Peek(const PARCArrayList *list);

/**
 * Removes the object at the top of this stack and returns that object as the value of this function.
 *
 * @param [in,out] list A pointer to the `PARCArrayList`.
 * @return  A pointer to what was the first element of the `PARCArrayList`.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void *parcArrayList_Pop(PARCArrayList *list);

/**
 * Removes all objects from this stack.
 *
 * @param [in,out] list A pointer to the `PARCArrayList`.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void parcArrayList_Clear(PARCArrayList *list);

/**
 * Pushes an item onto the top of this stack.
 *
 * @param [in,out] list A pointer to a `PARCArrayList`.
 * @param [in] item A pointer to an element to be added to the `PARCArrayList`.
 * @return  A pointer to the item just pushed on to the `PARCArrayLis.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void *parcArrayList_Push(PARCArrayList *list, void *item);

/**
 * Returns the 1-based position where an object is on this stack.
 *
 * @param [in] list A pointer to `PARCArrayList`.
 * @param [in] element A pointer to the element to locate in the `PARCArrayList`.
 * @return int The index of the located element in the `PARCArrayList`
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
int parcArrayList_Search(PARCArrayList *list, void *element);
#endif // libparc_parc_ArrayList_h
