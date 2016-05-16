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
 * @file parc_RingBuffer.h
 * @ingroup threading
 * @brief A thread-safe ring buffer.
 *
 * A fixed size, thread safe ring buffer. It can have multiple producers and multiple
 * consumers. All exclusion is done inside the ring buffer.
 *
 * This is a non-blocking data structure.
 *
 * If the user knows there is only one producer and one consumer, you can create the ring buffer with
 * `parcRingBuffer_CreateSingleProducerSingleConsumer`. Such a ring buffer can have at most 2 references.
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */

#ifndef libparc_parc_RingBuffer_h
#define libparc_parc_RingBuffer_h

#include <stdbool.h>
#include <stdint.h>

struct parc_ringbuffer;
typedef struct parc_ringbuffer PARCRingBuffer;

/**< Will be called for each data item on destroy */
typedef void (RingBufferEntryDestroyer)(void **entryPtr);

struct parc_ringbuffer_impl;
typedef struct parc_ringbuffer_interface PARCRingBufferInterface;

struct parc_ringbuffer_interface {
    void *instance;
    void * (*acquire)(PARCRingBufferInterface *ring);
    void (*release)(PARCRingBufferInterface **ring);
    bool (*put)(PARCRingBufferInterface *ring, void *data);
    bool (*get)(PARCRingBufferInterface *ring, void *outputDataPtr);
    bool (*remaining)(PARCRingBufferInterface *ring);
};

/**
 * Creates a ring buffer of the given size, which must be a power of 2.
 *
 * The ring buffer can store up to (elements-1) items in the buffer.  The buffer can
 * be shared between multiple producers and consumers.  Each of them should be
 * given out from a call to {@link parcRingBuffer_Acquire} to create reference counted
 * copies.
 *
 * The reference count is "1" on return.
 *
 * @param [in] interface A pointer to the underlying instance and the interface functions for acquire,
 * release,put,get,and remaining.
 *
 * @return non-null An pointer to a new allocated `PARCRingBuffer`.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCRingBuffer *parcRingBuffer_Create(PARCRingBufferInterface *interface);

/**
 * Acquire a new reference to an instance of `PARCRingBuffer`.
 *
 * A RING WITHOUT LOCKS CAN ONLY HAVE 2 REFERENCES.
 * The reference count to the instance is incremented.
 *
 * @param [in] ring The instance of `PARCRingBuffer` to which to refer.
 *
 * @return The same value as the input parameter @p ring
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */

PARCRingBuffer *parcRingBuffer_Acquire(const PARCRingBuffer *ring);

/**
 * Releases a reference.  The buffer will be destroyed after the last release.
 *
 * If the destroyer was specified on create, it will be called on each entry in the buffer
 * when the buffer is destroyed.
 *
 * @param [in,out] ringPtr The pointer to the pointer of the `PARCRingBuffer` to be destroyed.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void parcRingBuffer_Release(PARCRingBuffer **ringPtr);

/**
 * Non-blocking attempt to put item on ring.  May return false if ring is full.
 *
 * @param [in,out] ring The instance of `PARCRingBuffer` to modify'
 * @param [in] data A pointer to teh data to put on the ring.
 *
 * @return true Data was put on the queue
 * @return false Would have blocked, the queue was full
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
bool parcRingBuffer_Put(PARCRingBuffer *ring, void *data);

/**
 * Gets the next item off the ring, or returns false if would have blocked.
 *
 * Non-blocking, gets an item off the ring, or returns false if would block
 *
 * @param [in] ring The pointer to the `PARCRingBuffer`
 * @param [out] outputDataPtr The output pointer
 *
 * @return true Data returned in the output argument
 * @return false Ring is empty, no data returned.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
bool parcRingBuffer_Get(PARCRingBuffer *ring, void **outputDataPtr);

/**
 * Returns the remaining capacity of the ring
 *
 * Returns the remaining capacity of the ring.  This does not guarantee the next
 * Put will not block, as other producers might consumer the space between calls.
 *
 * @param [in] ring The pointer to the `PARCRingBuffer`
 *
 * @return  The uint32_t remaining capacity of the ring.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
uint32_t parcRingBuffer_Remaining(PARCRingBuffer *ring);
#endif // libparc_parc_RingBuffer_h
