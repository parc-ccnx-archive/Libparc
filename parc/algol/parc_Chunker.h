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
 * @file parc_Chunker.h
 * @ingroup ContentObject
 * @brief A Chunker is an object that breaks up a large piece of data from a `PARCBuffer`
 *        or a file and provides an iterator to walk over the chunks in sequential order.
 *
 * @author Christopher A. Wood, Palo Alto Research Center (Xerox PARC)
 * @copyright 2015-2016, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

#ifndef libparc_parc_Chunker_h
#define libparc_parc_Chunker_h

#include <config.h>

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
     * @see parcChunker_Release
     */
    void (*Release)(void **originalP);
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
PARCChunker *parcChunker_Create(void *instance, PARCChunkerInterface *interface);

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
#endif // libparc_parc_Chunker_h
