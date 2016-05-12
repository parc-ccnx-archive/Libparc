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
 * A thread-safe fixed size ring buffer.
 *
 * A single-producer/single-consumer version is lock-free, along the lines of Lamport, "Proving the
 * Correctness of Multiprocess Programs," IEEE Trans on Software Engineering 3(2), Mar 1977, which
 * is based on reading/writing native types upto the data bus width being atomic operations.
 *
 * It can hold (elements-1) data items.  elements must be a power of 2.
 *
 * The writer_head is where the next element should be inserted.  The reader_tail is where the next element
 * should be read.
 *
 * All index variables are unbounded uint32_t.  This means they just keep counting up.  To get the actual
 * index in the ring, we mask with (elements-1).  For example, a ring with 16 elements will be masked with
 * 0x0000000F.  We call this the "ring_mask".
 *
 * Because we never let the writer_head and reader_tail differ by more than (elements-1), this technique of
 * masking works just the same as taking the modulus.  There's no problems at the uint32_t wraparound either.
 * The only math operation we are doing is "+1" which works just fine to wrap a uint32_t.
 *
 * Let's look at some exampls.  I'm going to use a uint16_t so its easier to write the numbers.  Let's assume
 * that the ring size is 16, so the first ring is (0 - 15).
 *              head     tail
 * initialize      0        0
 * put x 3         3        0
 * get x 2         3        2
 * put x 13       16        2
 * put x 1        17        2
 * put x 1      blocks          # (0x11 + 1) & 0x0F  == tail & 0x0F
 * get x 14       17       16
 * get x 1        17       17   # ring is now empty
 * ...
 * empty       65534    65534   # 0xFFFE  0xFFFE masked =  14   14
 * put x1      65535    65534   # 0xFFFF  0xFFFE masked =  15   14
 * put x1          0    65534   # 0x0000  0xFFFE masked =   0   14
 * ...
 *
 * The number of remaining available items is (ring_mask + reader_tail - writer_head) & ring_mask.
 *              head     tail   remaining
 * initialize      0        0   15 + 0 - 0 = 15
 * put x 3         3        0   15 + 0 - 3 = 12
 * get x 2         3        2
 * put x 13       16        2   15 + 2 - 16 = 1
 * put x 1        17        2   15 + 2 - 17 = 0
 * put x 1      blocks
 * get x 14       17       16   15 + 16 - 17 = 14
 * get x 1        17       17   15 + 17 - 17 = 15
 * ...
 * empty       65534    65534   15 + 65534 - 65534 = 13 - 65534 = 13 - (-2) = 15
 * put x1      65535    65534   15 + 65534 - 65535 = 13 - 65535 = 13 - (-1) = 14
 * put x1          0    65534   15 + 65534 -     0 = 13 - 65535 = 13 - ( 0) = 13
 * ...
 *
 * If (writer_head + 1) & ring_mask == reader_tail, then the ring is full.
 * If writer_head == reader_tail, then the ring is empty.
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_Object.h>
#include <LongBow/runtime.h>

#include <parc/concurrent/parc_RingBuffer_1x1.h>

#ifdef __GNUC__

// on x86 or x86_64, simple assignment will work
#if (__x86_64__ || __i386__)
#define ATOMIC_ADD_AND_FETCH(ptr, increment)      __sync_add_and_fetch(ptr, increment)
#define ATOMIC_BOOL_CAS(ptr, oldvalue, newvalue)  __sync_bool_compare_and_swap(ptr, oldvalue, newvalue)
#define ATOMIC_FETCH(ptr)                        *(ptr)
#define ATOMIC_SET(ptr, oldvalue, newvalue)      *(ptr) = newvalue
#else
#define ATOMIC_ADD_AND_FETCH(ptr, increment)      __sync_add_and_fetch(ptr, increment)
#define ATOMIC_BOOL_CAS(ptr, oldvalue, newvalue)  __sync_bool_compare_and_swap(ptr, oldvalue, newvalue)
#define ATOMIC_FETCH(ptr)                         ATOMIC_ADD_AND_FETCH(ptr, 0)
#define ATOMIC_SET(ptr, oldvalue, newvalue)       ATOMIC_BOOL_CAS(ptr, oldvalue, newvalue)
#endif

#else
#error "Only GNUC supported, we need atomic operations"
#endif

struct parc_ringbuffer_1x1 {
    // LP64     LP32
    volatile uint32_t writer_head;          //  0- 3       0
    volatile uint32_t reader_tail;          //  4- 7       4
    uint32_t elements;                      //  8-11       8
    uint32_t ring_mask;                     // 12-15      12

    RingBufferEntryDestroyer *destroyer;    // 16-23      16
    void **buffer;                          // 24-31      24
};

static bool
_isPowerOfTwo(uint32_t x)
{
    return ((x != 0) && !(x & (x - 1)));
}

static void
_destroy(PARCRingBuffer1x1 **ringptr)
{
    PARCRingBuffer1x1 *ring = *ringptr;

    if (ring->destroyer) {
        void *ptr = NULL;
        while (parcRingBuffer1x1_Get(ring, &ptr)) {
            ring->destroyer(&ptr);
        }
    }
    parcMemory_Deallocate((void **) &(ring->buffer));
}

parcObject_ExtendPARCObject(PARCRingBuffer1x1, _destroy, NULL, NULL, NULL, NULL, NULL, NULL);

static PARCRingBuffer1x1 *
_create(uint32_t elements, RingBufferEntryDestroyer *destroyer)
{
    PARCRingBuffer1x1 *ring = parcObject_CreateInstance(PARCRingBuffer1x1);
    assertNotNull(ring, "parcObject_Create returned NULL");

    ring->buffer = parcMemory_AllocateAndClear(sizeof(void *) * elements);
    assertNotNull((ring->buffer), "parcMemory_AllocateAndClear() failed to allocate array of %u pointers", elements);

    ring->writer_head = 0;
    ring->reader_tail = 0;
    ring->elements = elements;
    ring->destroyer = destroyer;
    ring->ring_mask = elements - 1;

    return ring;
}

PARCRingBuffer1x1 *
parcRingBuffer1x1_Create(uint32_t elements, RingBufferEntryDestroyer *destroyer)
{
    assertTrue(_isPowerOfTwo(elements), "Parameter elements must be a power of 2, got %u", elements);
    return _create(elements, destroyer);
}


parcObject_ImplementAcquire(parcRingBuffer1x1, PARCRingBuffer1x1);

parcObject_ImplementRelease(parcRingBuffer1x1, PARCRingBuffer1x1);

/**
 * Put is protected by the writer mutex.  This means that the tail mutex could
 * actually increase while this is happening. That's ok.  Increasing the tail
 * just means there is _more_ room in the ring.  We only modify writer_head.
 */
bool
parcRingBuffer1x1_Put(PARCRingBuffer1x1 *ring, void *data)
{
    // Our speculative operation
    // The consumer modifies reader_tail, so make sure that's an atomic read.
    // only the prodcuer modifies writer_head, so there's only us

    uint32_t writer_head = ring->writer_head;
    uint32_t reader_tail = ATOMIC_FETCH(&ring->reader_tail);

    uint32_t writer_next = (writer_head + 1) & ring->ring_mask;

    // ring is full
    if (writer_next == reader_tail) {
        return false;
    }

    assertNull(ring->buffer[writer_head], "Ring index %u is not null!", writer_head);
    ring->buffer[writer_head] = data;

    // we're using this just for atomic write to the integer
    ATOMIC_SET(&ring->writer_head, writer_head, writer_next);

    return true;
}

bool
parcRingBuffer1x1_Get(PARCRingBuffer1x1 *ring, void **outputDataPtr)
{
    // do our speculative operation.
    // The producer modifies writer_head, so make sure that's an atomic read.
    // only the consumer modifies reader_tail, so there's only us

    uint32_t writer_head = ATOMIC_FETCH(&ring->writer_head); // native type assignment is atomic
    uint32_t reader_tail = ring->reader_tail;
    uint32_t reader_next = (reader_tail + 1) & ring->ring_mask;

    // ring is empty
    if (writer_head == reader_tail) {
        return false;
    }

    // now try to commit it
    ATOMIC_SET(&ring->reader_tail, reader_tail, reader_next);

    *outputDataPtr = ring->buffer[reader_tail];

    // for sanity's sake
    ring->buffer[reader_tail] = NULL;

    return true;
}

uint32_t
parcRingBuffer1x1_Remaining(PARCRingBuffer1x1 *ring)
{
    uint32_t writer_head = ATOMIC_FETCH(&ring->writer_head);
    uint32_t reader_tail = ATOMIC_FETCH(&ring->reader_tail);

    return (ring->ring_mask + reader_tail - writer_head) & ring->ring_mask;
}
