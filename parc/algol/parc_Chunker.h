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
 * @file parc_Chunker.h
 * @ingroup ContentObject
 * @brief A Chunker is an object that breaks up a large piece of data from a `PARCBuffer`
 *        or a file and provides an iterator to walk over the chunks in sequential order.
 *
 * @author Christopher A. Wood, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */

#ifndef libparc_parc_Chunker_h
#define libparc_parc_Chunker_h

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_Iterator.h>

struct parc_chunker;
/**
 * @typedef PARCChunker
 * @brief The PARC Chunker
 */
typedef struct parc_chunker PARCChunker;

typedef struct PARCChunkerInterface {
    /**
     * @see parcChunker_ForwardIterator
     */
    void *(*ForwardIterator)(const void *original);

    /**
     * @see parcChunker_ReverseIterator
     */
    void *(*ReverseIterator)(const void *original);

    /**
     * @see parcChunker_GetChunkSize
     */
    size_t (*GetChunkSize)(const void *original);
} PARCChunkerInterface;

/**
 * Create a new chunker from the given concrete instance.
 *
 * @param [in] instance A `PARCChunker` instance.
 * @param [in] interface The interface implementation of the chunker.
 *
 * @retval PARCChunker A newly allocated `PARCChunker`
 * @retval NULL An error occurred.
 *
 * Example
 * @code
 * {
 *     PARCBuffer *dataToChunk = ...
 *     PARCChunkerBuffer *bufferChunker = parcBufferChunker_Create(dataToChunk, 32);
 *     PARCChunker *chunker = parcChunker_Create(bufferCunker, &PARCBufferChunkerAsChunker);
 * }
 */
PARCChunker *parcChunker_Create(PARCObject *instance, PARCChunkerInterface *interface);

/**
 * Create a new chunker to segment data contained in a file.
 *
 * @param [in] fileName The name of a file from which to read.
 * @param [in] chunkSize The size per chunk.
 *
 * @retval PARCChunker A newly allocated `PARCChunker`
 * @retval NULL An error occurred.
 *
 * Example
 * @code
 * {
 *     char *bigFileName = "big_file.bin";
 *     PARCChunker *chunker = parcChunker_CreateFromBuffer(bigFileName, 32);
 * }
 */
//PARCChunker *parcChunker_CreateFromFile(char *fileName, size_t maxDataSize);

/**
 * Increase the number of references to a `PARCChunker` instance.
 *
 * Note that new `PARCChunker` is not created,
 * only that the given `PARCChunker` reference count is incremented.
 * Discard the reference by invoking {@link parcChunker_Release}.
 *
 * @param [in] chunker A pointer to the original `PARCChunker`.
 * @return The value of the input parameter @p chunker.
 *
 * Example:
 * @code
 * {
 *     PARCChunker *original = ...
 *
 *     PARCChunker *reference = parcChunker_Acquire(original);
 *
 *     parcChunker_Release(&original);
 *     parcChunker_Release(&reference);
 * }
 * @endcode
 *
 * @see parcChunker_Release
 */
PARCChunker *parcChunker_Acquire(const PARCChunker *chunker);

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
 * @param [in,out] chunkerP A pointer to a pointer to the instance to release.
 *
 *
 * Example:
 * @code
 * {
 *     ...
 *
 *     PARCChunker *chunker = parcChunker_Acquire(instance);
 *
 *     parcChunker_Release(&chunker);
 * }
 * @endcode
 */
void parcChunker_Release(PARCChunker **chunkerP);

/**
 * Determine if two `PARCChunker` instances are equal.
 *
 * The following equivalence relations on non-null `PARCChunker` instances are maintained:
 *
 *  * It is reflexive: for any non-null reference value x, `parcChunker_Equals(x, x)`
 *      must return true.
 *
 *  * It is symmetric: for any non-null reference values x and y,
 *    `parcChunker_Equals(x, y)` must return true if and only if
 *        `parcChunker_Equals(y, x)` returns true.
 *
 *  * It is transitive: for any non-null reference values x, y, and z, if
 *        `parcChunker_Equals(x, y)` returns true and
 *        `parcChunker_Equals(y, z)` returns true,
 *        then  `parcChunker_Equals(x, z)` must return true.
 *
 *  * It is consistent: for any non-null reference values x and y, multiple
 *      invocations of `parcChunker_Equals(x, y)` consistently return true or
 *      consistently return false.
 *
 *  * For any non-null reference value x, `parcChunker_Equals(x, NULL)` must
 *      return false.
 *
 * @param chunkerA A pointer to a `PARCChunker` instance.
 * @param chunkerB A pointer to a `PARCChunker` instance.
 * @return true if the two `PARCChunker` instances are equal.
 *
 * Example:
 * @code
 * {
 *     PARCBuffer *dataToChunk = ...
 *     PARCChunker *chunkerA = ...
 *     PARCChunker *chunkerB = ...
 *
 *     bool equals = parcChunker_Equals(chunkerA, chunkerB);
 * }
 * @endcode
 */
bool parcChunker_Equals(const PARCChunker *chunkerA, const PARCChunker *chunkerB);

/**
 * Return an iterator to traverse the chunks of the underlying data in sequential order.
 *
 * This function can only be called once per chunker instance since the iterator
 * will mutate internal state of the chunker.
 *
 * @param [in] chunker A `PARCChunker` instance.
 *
 * @return a `PARCIterator` that traverses the chunks of the underlying data.
 *
 * Example
 * @code
 * {
 *     PARCBuffer *dataToChunk = ...
 *     PARCBufferChunker *chunker = ...
 *
 *     PARCIterator *itr = parcChunker_ForwardIterator(chunker);
 *
 *     // use the iterator to traverse the chunker
 * }
 * @endcode
 */
PARCIterator *parcChunker_ForwardIterator(const PARCChunker *chunker);

/**
 * Return an iterator to traverse the chunks of the underlying data in sequential order.
 *
 * This function can only be called once per chunker instance since the iterator
 * will mutate internal state of the chunker.
 *
 * @param [in] chunker A `PARCChunker` instance.
 *
 * @return a `PARCIterator` that traverses the chunks of the underlying data.
 *
 * Example
 * @code
 * {
 *     PARCBuffer *dataToChunk = ...
 *     PARCChunker *chunker = ...
 *
 *     PARCIterator *itr = parcChunker_ReverseIterator(chunker);
 *
 *     // use the iterator to traverse the chunker
 * }
 * @endcode
 */
PARCIterator *parcChunker_ReverseIterator(const PARCChunker *chunker);

/**
 * Get the chunk size of a `PARCChunker.`
 *
 * @param [in] chunker A `PARCChunker` instance.
 *
 * @return the chunk size
 *
 * Example
 * @code
 * {
 *     PARCBuffer *dataToChunk = ...
 *     PARCChunker *chunker = ...
 *
 *     size_t chunkSize = parcChunker_GetChunkSize(chunker);
 * }
 * @endcode
 */
size_t parcChunker_GetChunkSize(const PARCChunker *chunker);
#endif // libparc_parc_Chunker_h
