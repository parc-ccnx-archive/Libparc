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
 * A thread-safe fixed size ring buffer.
 *
 * The multiple producer, multiple consumer version uses a pthread mutex around a NxM ring buffer.
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_Object.h>
#include <LongBow/runtime.h>

#include <parc/concurrent/parc_RingBuffer_1x1.h>
#include <parc/concurrent/parc_RingBuffer_NxM.h>

struct parc_ringbuffer_NxM {
    PARCRingBuffer1x1 *onebyone;

    // This protectes the overall data structure for Acquire and Release
    pthread_mutex_t allocation_mutex;

    pthread_mutex_t writer_mutex;
    pthread_mutex_t reader_mutex;

    RingBufferEntryDestroyer *destroyer;
};

/*
 * Attemps a lock and returns false if we cannot get it.
 *
 * @endcode
 */
static bool
_lock(pthread_mutex_t *mutex)
{
    int failure = pthread_mutex_lock(mutex);
    assertFalse(failure, "Error locking mutex: (%d) %s\n", errno, strerror(errno));
    return true;
}

static bool
_unlock(pthread_mutex_t *mutex)
{
    int failure = pthread_mutex_unlock(mutex);
    assertFalse(failure, "Error unlocking mutex: (%d) %s\n", errno, strerror(errno));
    return true;
}

static void
_destroy(PARCRingBufferNxM **ringptr)
{
    PARCRingBufferNxM *ring = *ringptr;

    if (ring->destroyer) {
        void *ptr = NULL;
        while (parcRingBufferNxM_Get(ring, &ptr)) {
            ring->destroyer(&ptr);
        }
    }
    parcRingBuffer1x1_Release(&ring->onebyone);
}


parcObject_ExtendPARCObject(PARCRingBufferNxM, _destroy, NULL, NULL, NULL, NULL, NULL, NULL);

static PARCRingBufferNxM *
_create(uint32_t elements, RingBufferEntryDestroyer *destroyer)
{
    PARCRingBufferNxM *ring = parcObject_CreateInstance(PARCRingBufferNxM);
    assertNotNull(ring, "parcObject_Create returned NULL");

    ring->onebyone = parcRingBuffer1x1_Create(elements, destroyer);
    ring->destroyer = destroyer;
    pthread_mutex_init(&ring->allocation_mutex, NULL);
    pthread_mutex_init(&ring->writer_mutex, NULL);
    pthread_mutex_init(&ring->reader_mutex, NULL);
    return ring;
}

PARCRingBufferNxM *
parcRingBufferNxM_Create(uint32_t elements, RingBufferEntryDestroyer *destroyer)
{
    return _create(elements, destroyer);
}

PARCRingBufferNxM *
parcRingBufferNxM_Acquire(PARCRingBufferNxM *ring)
{
    PARCRingBufferNxM *acquired;

    _lock(&ring->allocation_mutex);
    acquired = parcObject_Acquire(ring);
    _unlock(&ring->allocation_mutex);

    return acquired;
}

void
parcRingBufferNxM_Release(PARCRingBufferNxM **ringPtr)
{
    PARCRingBufferNxM *ring = *ringPtr;
    _lock(&ring->allocation_mutex);
    parcObject_Release((void **) ringPtr);
    _unlock(&ring->allocation_mutex);
}

/**
 * Put is protected by the writer mutex.  This means that the tail mutex could
 * actually increase while this is happening. That's ok.  Increasing the tail
 * just means there is _more_ room in the ring.  We only modify writer_head.
 */
bool
parcRingBufferNxM_Put(PARCRingBufferNxM *ring, void *data)
{
    // **** LOCK
    _lock(&ring->writer_mutex);
    bool success = parcRingBuffer1x1_Put(ring->onebyone, data);
    // **** UNLOCK
    _unlock(&ring->writer_mutex);
    return success;
}

bool
parcRingBufferNxM_Get(PARCRingBufferNxM *ring, void **outputDataPtr)
{
    // **** LOCK
    _lock(&ring->reader_mutex);
    bool success = parcRingBuffer1x1_Get(ring->onebyone, outputDataPtr);
    // **** UNLOCK
    _unlock(&ring->reader_mutex);
    return success;
}

uint32_t
parcRingBufferNxM_Remaining(PARCRingBufferNxM *ring)
{
    _lock(&ring->writer_mutex);
    _lock(&ring->reader_mutex);

    uint32_t remaining = parcRingBuffer1x1_Remaining(ring->onebyone);

    _unlock(&ring->reader_mutex);
    _unlock(&ring->writer_mutex);

    return remaining;
}
