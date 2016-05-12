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
 * @author Ignacio Solis, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>

#include <LongBow/runtime.h>

#include <parc/algol/parc_SortedList.h>

#include <parc/algol/parc_ArrayList.h>
#include <parc/algol/parc_Memory.h>

struct parc_sorted_list {
    parcSortedList_Compare compare;
    PARCArrayList *arrayList;
};

PARCSortedList *
parcSortedList_Create(parcSortedList_Compare compareFunction)
{
    PARCSortedList *sortedList = parcMemory_Allocate(sizeof(PARCSortedList));
    assertNotNull(sortedList, "parcMemory_Allocate(%zu) returned NULL", sizeof(PARCSortedList));
    sortedList->arrayList = parcArrayList_Create(NULL);
    sortedList->compare = compareFunction;
    return sortedList;
}

void
parcSortedList_Destroy(PARCSortedList **parcSortedListPointer)
{
    parcArrayList_Destroy(&((*parcSortedListPointer)->arrayList));
    parcMemory_Deallocate((void **) parcSortedListPointer);
    *parcSortedListPointer = NULL;
}

void
parcSortedList_Add(PARCSortedList *parcSortedList, void *newItem)
{
    assertNotNull(parcSortedList, "sortedList parameter can't be null");
    assertNotNull(parcSortedList->arrayList, "arrayList can't be null");
    assertNotNull(newItem, "newItem can't be null");

    size_t total_items = parcArrayList_Size(parcSortedList->arrayList);
    for (size_t i = 0; i < total_items; i++) {
        void *oldItem = parcArrayList_Get(parcSortedList->arrayList, i);
        if (parcSortedList->compare(newItem, oldItem) == -1) {
            // The old item at position i is bigger than the new item,
            // we must insert the newItem here.
            parcArrayList_InsertAtIndex(parcSortedList->arrayList, newItem, i);
            return;
        }
    }
    // We reached the end of the list, it must go here...
    parcArrayList_Add(parcSortedList->arrayList, newItem);
}

size_t
parcSortedList_Length(PARCSortedList *parcSortedList)
{
    return parcArrayList_Size(parcSortedList->arrayList);
}

void *
parcSortedList_PopFirst(PARCSortedList *parcSortedList)
{
    assertNotNull(parcSortedList, "sortedList parameter can't be null");
    assertNotNull(parcSortedList->arrayList, "arrayList can't be null");

    if (parcArrayList_Size(parcSortedList->arrayList) == 0) {
        return NULL;
    }
    void *item = parcArrayList_Get(parcSortedList->arrayList, 0);
    parcArrayList_RemoveAndDestroyAtIndex(parcSortedList->arrayList, 0);
    return item;
}

void *
parcSortedList_GetFirst(PARCSortedList *parcSortedList)
{
    assertNotNull(parcSortedList, "sortedList parameter can't be null");
    assertNotNull(parcSortedList->arrayList, "arrayList can't be null");

    if (parcArrayList_Size(parcSortedList->arrayList) == 0) {
        return NULL;
    }
    return parcArrayList_Get(parcSortedList->arrayList, 0);
}
