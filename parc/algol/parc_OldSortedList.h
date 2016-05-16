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
 * @file parc_SortedList.h
 * @ingroup datastructures
 * @brief A sorted list
 *
 * @author Ignacio Solis, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_SortedList_h
#define libparc_parc_SortedList_h

#include <stdlib.h>

struct parc_sorted_list;

typedef struct parc_sorted_list PARCSortedList;

/**
 * This is a compare function that must be defined for the sorted list to sort
 *
 * @param [in] object1 The first object to compare.
 * @param [in] object2 The second object to compare.
 * @return -1 if object1 < object2,  0 if object1 == object2, 1 if object1 > object2.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
typedef int (*parcSortedList_Compare)(const void *object1, const void *object2);

/**
 * Create a sorted list
 * This list will be sorted from smallest to largest based on the compare function.
 *
 * @param [in] compareFunction A compare function to determine how elements are sorted.
 * @return An allocated `PARCSortedList`.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCSortedList *parcSortedList_Create(parcSortedList_Compare compareFunction);

/**
 * Destroy an allocated sorted list
 *
 * @param [in,out] parcSortedListPointer A pointer to the allocated sorted list to destroy. The pointer will be set to NULL.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void parcSortedList_Destroy(PARCSortedList **parcSortedListPointer);

/**
 * Add an element to the sorted list.
 *
 * @param [in,out] parcSortedList A pointer to the `PARCSortedList` to modify.
 * @param newItem The new item to add to the list. This item must be comparable by the compare function.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void parcSortedList_Add(PARCSortedList *parcSortedList, void *newItem);

/**
 * Return the length of the list
 *
 * @param [in] parcSortedList a pointer to an allocated sorted list.
 * @return return the length of the list, the number of elements.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
size_t parcSortedList_Length(PARCSortedList *parcSortedList);

/**
 * Pop the first element on the  sorted list. This will remove the element from the list and return it to the caller.
 *
 * @param [in,out] parcSortedList A pointer to the `PARCSortedList` to modify.
 * @return The first element of @p parcSortedList.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void *parcSortedList_PopFirst(PARCSortedList *parcSortedList);

/**
 * Get the first element on the  sorted list. This will NOT remove the element from the list.
 *
 * @param [in] parcSortedList A pointer to the `PARCSortedList` .
 * @return The first element of the sorted list.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void *parcSortedList_GetFirst(PARCSortedList *parcSortedList);
#endif // libparc_parc_SortedList_h
