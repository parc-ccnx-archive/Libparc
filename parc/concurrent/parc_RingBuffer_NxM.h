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
 * @file parc_RingBuffer_NxM.h
 * @ingroup threading
 * @brief A multiple producer, multiple consumer ring buffer
 *
 * This is useful for synchronizing one or more producers with one or more consumers.
 * The implementation may use locks.
 *
 * Complies with the PARCRingBuffer generic facade.
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

#ifndef libparc_parc_RingBuffer_NxM_h
#define libparc_parc_RingBuffer_NxM_h

#include <stdbool.h>
#include <stdint.h>
#include <parc/concurrent/parc_RingBuffer_1x1.h>

struct parc_ringbuffer_NxM;
/**
 * @typedef PARCRingBufferNxM
 */
typedef struct parc_ringbuffer_NxM PARCRingBufferNxM;

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
 * @param [in] elements A power of 2, indicating the maximum size of the buffer.
 * @param [in] destroyer Will be called for each ring entry when when the ring is destroyed.  May be null.
 *
 * @return non-null An allocated ring buffer.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCRingBufferNxM *parcRingBufferNxM_Create(uint32_t elements, RingBufferEntryDestroyer *destroyer);

/**
 * A reference counted copy of the buffer.
 *
 * A RING WITHOUT LOCKS CAN ONLY HAVE 2 REFERENCES.
 *
 * @param [in] ring A pointer to the `PARCRingBufferNxM` to be acquired.
 *
 * @return non-null A reference counted copy of the ring buffer
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCRingBufferNxM *parcRingBufferNxM_Acquire(PARCRingBufferNxM *ring);

/**
 * Releases a reference.  The buffer will be destroyed after the last release.
 *
 * If the destroyer was specified on create, it will be called on each entry in the buffer
 * when the buffer is destroyed.
 *
 * @param [in,out] ringPtr A pointer to the pointer to the `PARCRingBufferNxM` to be released.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void parcRingBufferNxM_Release(PARCRingBufferNxM **ringPtr);

/**
 * Non-blocking attempt to put item on ring.  May return false if ring is full.
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [in,out] ring A pointer to the `PARCRingBufferNxM` on which to put @p data.
 * @param [in] data A pointer to data to put on @p ring.
 *
 * @return `true` Data was put on the queue
 * @return `false` Would have blocked, the queue was full
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
bool parcRingBufferNxM_Put(PARCRingBufferNxM *ring, void *data);

/**
 * Gets the next item off the ring, or returns false if would have blocked.
 *
 * Non-blocking, gets an item off the ring, or returns false if would block
 *
 * @param [in] ring The ring buffer
 * @param [out] outputDataPtr The output pointer
 *
 * @return `true` Data returned in the output argument
 * @return `false` Ring is empty, no data returned.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
bool parcRingBufferNxM_Get(PARCRingBufferNxM *ring, void **outputDataPtr);

/**
 * Returns the remaining capacity of the ring
 *
 * Returns the remaining capacity of the ring.  This does not guarantee the next
 * Put will not block, as other producers might consumer the space between calls.
 *
 * @param [in] ring The ring buffer
 *
 * @return the remaining capacity of @p ring.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
uint32_t parcRingBufferNxM_Remaining(PARCRingBufferNxM *ring);
#endif // libparc_parc_RingBuffer_NxM_h
