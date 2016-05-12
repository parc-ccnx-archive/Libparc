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
 * Priority Queue implemented over a Binary Heap.
 *
 * A Binary Heap will have average insert of O(1) and delete of O(log n).  The worst case
 * is O(log n) for both.  The average and worst case FindMin is O(1).
 *
 * The binary heap is implemented as a "0"-based array, so for node index n, the
 * children are at 2n+1 and 2n+2.  Its parent is at floor((n-1)/2).
 *
 * The Heap property is a[n] <= a[2n+1] and a[k] <= a[2k+2].  We need to move things around
 * sufficiently for this property to remain true.
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */

#include <config.h>

#include <LongBow/runtime.h>

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_PriorityQueue.h>

typedef struct heap_entry {
    void *data;
} HeapEntry;

struct parc_priority_queue {
    HeapEntry *array;
    size_t capacity;        // how many elements are allocated
    size_t size;            // how many elements are used

    PARCPriorityQueueCompareTo *compare;
    PARCPriorityQueueDestroyer *destroyer;
};

/**
 * 0-based array indexing, so use 2n+1
 */
static size_t
_leftChildIndex(size_t elementIndex)
{
    return 2 * elementIndex + 1;
}

/**
 * 0-based array indexing, so use 2n+2
 * IMPORTANT: This must be a larger index than the left child,
 * as the TrickleDown algorithm assumes that if the right child exists so
 * does the left child.
 */
static size_t
_rightChildIndex(size_t elementIndex)
{
    return 2 * elementIndex + 2;
}

/**
 * 0-based array indexing, so use (n-1)/2
 */
static size_t
_parentIndex(size_t elementIndex)
{
    return (elementIndex - 1) / 2;
}

/**
 * Exchange the data between two array locations
 */
static void
_swap(PARCPriorityQueue *queue, size_t firstIndex, size_t secondIndex)
{
    void *firstData = queue->array[firstIndex].data;
    queue->array[firstIndex].data = queue->array[secondIndex].data;
    queue->array[secondIndex].data = firstData;
}

/**
 * See parcPriorityQueue_TrickleDown for full details
 *
 * Case 1: Right child exists and r.value < n.value && r.value < l.value
 *   In this case, swap(n.index, r.index) and set n.index = r.index.
 *       50                6
 *      /  \     ===>     / \
 *     9    6            9   50
 *
 * Case 2: Right child exists and r.value < n.value && l.value <= r.value
 *   In this case swap(n.index, l.index) and set n.index = l.index
 *   This makes sense by transitivity that l <= r < n, so swap(n,l) satisfies the invariant.
 *       50                6
 *      /  \     ===>     / \
 *     6    9            50  9
 */
static size_t
_trickleRightChild(PARCPriorityQueue *queue, size_t elementIndex, size_t leftChildIndex, size_t rightChildIndex)
{
    if (queue->compare(queue->array[rightChildIndex].data, queue->array[elementIndex].data) < 0) {
        if (queue->compare(queue->array[rightChildIndex].data, queue->array[leftChildIndex].data) < 0) {
            // Case 1
            _swap(queue, rightChildIndex, elementIndex);
            elementIndex = rightChildIndex;
        } else {
            // Case 2
            _swap(queue, leftChildIndex, elementIndex);
            elementIndex = leftChildIndex;
        }
    }
    return elementIndex;
}

/**
 * See parcPriorityQueue_TrickleDown for full details
 *
 * Case 3: Left child exists (right does not) and l.value < n.value
 *   In this case, swap(n.index, l.index) and set n.index = l.index
 *       50                6
 *      /  \     ===>     / \
 *     6    x            50  x
 */
static size_t
_trickleLeftChild(PARCPriorityQueue *queue, size_t elementIndex, size_t leftChildIndex)
{
    if (queue->compare(queue->array[leftChildIndex].data, queue->array[elementIndex].data) < 0) {
        // Case 3
        _swap(queue, leftChildIndex, elementIndex);
        elementIndex = leftChildIndex;
    }
    return elementIndex;
}

/**
 * Moves an element down the heap until it satisfies the heap invariant.
 *
 * The value of node n must be less than or equal to both the left child and right child, if
 * they exist.  Here's the algorithm by example.  Let r.value, l.value and n.value be the values
 * of the right, left and node.  Let r.index, l.index, and n.index be their indicies.
 *
 * Case 1: Right child exists and r.value < n.value && r.value < l.value
 *   In this case, swap(n.index, r.index) and set n.index = r.index.
 *       50                6
 *      /  \     ===>     / \
 *     9    6            9   50
 *
 * Case 2: Right child exists and r.value < n.value && l.value <= r.value
 *   In this case swap(n.index, l.index) and set n.index = l.index
 *   This makes sense by transitivity that l <= r < n, so swap(n,l) satisfies the invariant.
 *       50                6
 *      /  \     ===>     / \
 *     6    9            50  9
 *
 * Case 3: Left child exists (right does not) and l.value < n.value
 *   In this case, swap(n.index, l.index) and set n.index = l.index
 *       50                6
 *      /  \     ===>     / \
 *     6    x            50  x
 *
 * Case 4: No child exists or already satisfies invariant
 *    Done
 *       50                50
 *      /  \     ===>     /  \
 *     x    x            x    x
 *                OR
 *       4                 4
 *      / \      ===>     / \
 *     9   6             9   6
 *
 *
 * @param [in] queue The priority queue to manipulate
 * @param [in] elementIndex The root element (n above) to trickle down
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
static void
_trickleDown(PARCPriorityQueue *queue, size_t elementIndex)
{
    bool finished = false;

    while (!finished) {
        size_t rightChildIndex = _rightChildIndex(elementIndex);
        size_t leftChildIndex = _leftChildIndex(elementIndex);

        if (rightChildIndex < queue->size) {
            // Case 1 and Case 2
            elementIndex = _trickleRightChild(queue, elementIndex, leftChildIndex, rightChildIndex);
        } else if (leftChildIndex < queue->size) {
            // Case 3
            elementIndex = _trickleLeftChild(queue, elementIndex, leftChildIndex);
        } else {
            // Case 4, we're done
            finished = true;
        }
    }
}

/**
 * Move the item at elementIndex up the tree until it satisfies the invariant.
 *
 * This is used when we insert an element at the bottom of the heap.  We bubble it up
 * the heap until it satisfies the heap invariant (i.e. it's parent is less than or
 * equal to it).
 *
 * @param [in] queue The priority queue to manipulate
 * @param [in] elementIndex The 0-based index of the element to bubble up
 */
static void
_bubbleUp(PARCPriorityQueue *queue, size_t elementIndex)
{
    size_t parentIndex = _parentIndex(elementIndex);
    while (elementIndex > 0 && queue->compare(queue->array[elementIndex].data, queue->array[parentIndex].data) < 0) {
        _swap(queue, elementIndex, parentIndex);
        // now move up the ladder
        elementIndex = parentIndex;
        parentIndex = _parentIndex(elementIndex);
    }

    // At this point, it is either at the top (elementIndex = 0) or statisfies the heap invariant.
}

/**
 * Add more elements to the backing aray
 *
 * Expand the array capacity.  We use a fixed x2 expansion each time, though this might not
 * be desirable when the capacity gets large.
 *
 * @param [in] queue The priority queue to manipulate
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
static void
_expand(PARCPriorityQueue *queue)
{
    queue->capacity *= 2;
    queue->array = parcMemory_Reallocate(queue->array, sizeof(HeapEntry) * queue->capacity);
}

// ================================
// Public API

void
parcPriorityQueue_ParcFreeDestroyer(void **elementPtr)
{
    assertNotNull(elementPtr, "Double pointer must be non-null");
    assertNotNull(*elementPtr, "Double pointer must dereference to non-null");
    void *element = *elementPtr;
    parcMemory_Deallocate((void **) &element);
    *elementPtr = NULL;
}

int
parcPriorityQueue_Uint64CompareTo(const void *a, const void *b)
{
    uint64_t value_a = *((uint64_t *) a);
    uint64_t value_b = *((uint64_t *) b);
    if (value_a < value_b) {
        return -1;
    } else if (value_a > value_b) {
        return +1;
    }
    return 0;
}

PARCPriorityQueue *
parcPriorityQueue_Create(PARCPriorityQueueCompareTo *compare, PARCPriorityQueueDestroyer *destroyer)
{
    assertNotNull(compare, "Parameter compare must be non-null");

    size_t initialSize = 128;
    PARCPriorityQueue *queue = parcMemory_AllocateAndClear(sizeof(PARCPriorityQueue));
    assertNotNull(queue, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(PARCPriorityQueue));
    queue->array = parcMemory_AllocateAndClear(sizeof(HeapEntry) * initialSize);
    assertNotNull(queue->array, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(HeapEntry) * initialSize);
    queue->capacity = initialSize;
    queue->size = 0;
    queue->compare = compare;
    queue->destroyer = destroyer;

    return queue;
}

void
parcPriorityQueue_Destroy(PARCPriorityQueue **queuePtr)
{
    assertNotNull(queuePtr, "Double pointer must be non-null");
    assertNotNull(*queuePtr, "Double pointer must dereference to non-null");
    PARCPriorityQueue *queue = *queuePtr;
    parcPriorityQueue_Clear(queue);
    parcMemory_Deallocate((void **) &(queue->array));
    parcMemory_Deallocate((void **) &queue);
    *queuePtr = NULL;
}

bool
parcPriorityQueue_Add(PARCPriorityQueue *queue, void *data)
{
    assertNotNull(queue, "Parameter queue must be non-null");
    assertNotNull(data, "Parameter data must be non-null");

    if (queue->size + 1 > queue->capacity) {
        _expand(queue);
    }

    // insert at the end of the array
    queue->array[queue->size].data = data;

    // increment the size before calling bubble up so invariants are true (i.e.
    // the index we're giving to BubbleUp is within the array size.
    queue->size++;
    _bubbleUp(queue, queue->size - 1);

    // we always allow duplicates, so always return true
    return true;
}

void
parcPriorityQueue_Clear(PARCPriorityQueue *queue)
{
    assertNotNull(queue, "Parameter queue must be non-null");
    if (queue->destroyer != NULL) {
        for (size_t i = 0; i < queue->size; i++) {
            queue->destroyer(&queue->array[i].data);
        }
    }

    queue->size = 0;
}

void *
parcPriorityQueue_Peek(PARCPriorityQueue *queue)
{
    assertNotNull(queue, "Parameter queue must be non-null");
    if (queue->size > 0) {
        return queue->array[0].data;
    }
    return NULL;
}

void *
parcPriorityQueue_Poll(PARCPriorityQueue *queue)
{
    assertNotNull(queue, "Parameter queue must be non-null");
    if (queue->size > 0) {
        void *data = queue->array[0].data;

        queue->size--;

        // if size == 1, then this is a no-op
        queue->array[0].data = queue->array[queue->size].data;

        // make sure the head satisifies the heap invariant
        _trickleDown(queue, 0);

        return data;
    }

    return NULL;
}

size_t
parcPriorityQueue_Size(const PARCPriorityQueue *queue)
{
    assertNotNull(queue, "Parameter queue must be non-null");
    return queue->size;
}
