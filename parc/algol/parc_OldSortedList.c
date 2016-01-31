/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @author Ignacio Solis, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
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
