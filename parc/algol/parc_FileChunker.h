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
 * @file parc_FileChunker.h
 * @ingroup ContentObject
 * @brief A FileChunker is a chunker that segments the content of a file.
 *
 * @author Christopher A. Wood, Palo Alto Research Center (Xerox PARC)
 * @copyright 2015-2016, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

#ifndef libparc_parc_FileChunker_h
#define libparc_parc_FileChunker_h

#include <parc/algol/parc_Chunker.h>

#include <parc/algol/parc_File.h>

struct parc_buffer_chunker;
/**
 * @typedef PARCFileChunker
 * @brief The PARC Chunker
 */
typedef struct parc_buffer_chunker PARCFileChunker;

/**
 * The mapping of a `PARCFileChunker` to the generic `PARCChunker`.
 */
extern PARCChunkerInterface *PARCFileChunkerAsChunker;

/**
 * Create a new chunker to segment data contained in a `PARCBuffer.`
 *
 * @param [in] file A `PARCFile` from which the data will be read.
 * @param [in] chunkSize The size per chunk.
 *
 * @retval PARCFileChunker A newly allocated `PARCFileChunker`
 * @retval NULL An error occurred.
 *
 * Example
 * @code
 * {
 *     PARCBuffer *dataToChunk = ...
 *     PARCFileChunker *chunker = PARCFileChunker_CreateFromBuffer(dataToChunk, 32);
 * }
 */
PARCFileChunker *parcFileChunker_Create(PARCFile *file, size_t chunkSize);

/**
 * Increase the number of references to a `PARCFileChunker` instance.
 *
 * Note that new `PARCFileChunker` is not created,
 * only that the given `PARCFileChunker` reference count is incremented.
 * Discard the reference by invoking {@link PARCFileChunker_Release}.
 *
 * @param [in] chunker A pointer to the original `PARCFileChunker`.
 * @return The value of the input parameter @p chunker.
 *
 * Example:
 * @code
 * {
 *     PARCFileChunker *original = parcFileChunker_Create(...);
 *
 *     PARCFileChunker *reference = parcFileChunker_Acquire(original);
 *
 *     parcFileChunker_Release(&original);
 *     parcFileChunker_Release(&reference);
 * }
 * @endcode
 *
 * @see parcFileChunker_Release
 */
PARCFileChunker *parcFileChunker_Acquire(const PARCFileChunker *chunker);

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
 *     PARCFileChunker *chunker = parcFileChunker_Acquire(instance);
 *
 *     parcFileChunker_Release(&chunker);
 * }
 * @endcode
 */
void parcFileChunker_Release(PARCFileChunker **chunkerP);

/**
 * Determine if two `PARCFileChunker` instances are equal.
 *
 * The following equivalence relations on non-null `PARCFileChunker` instances are maintained:
 *
 *  * It is reflexive: for any non-null reference value x, `parcFileChunker_Equals(x, x)`
 *      must return true.
 *
 *  * It is symmetric: for any non-null reference values x and y,
 *    `parcFileChunker_Equals(x, y)` must return true if and only if
 *        `parcFileChunker_Equals(y, x)` returns true.
 *
 *  * It is transitive: for any non-null reference values x, y, and z, if
 *        `parcFileChunker_Equals(x, y)` returns true and
 *        `parcFileChunker_Equals(y, z)` returns true,
 *        then  `parcFileChunker_Equals(x, z)` must return true.
 *
 *  * It is consistent: for any non-null reference values x and y, multiple
 *      invocations of `parcFileChunker_Equals(x, y)` consistently return true or
 *      consistently return false.
 *
 *  * For any non-null reference value x, `parcFileChunker_Equals(x, NULL)` must
 *      return false.
 *
 * @param chunkerA A pointer to a `PARCFileChunker` instance.
 * @param chunkerB A pointer to a `PARCFileChunker` instance.
 * @return true if the two `PARCFileChunker` instances are equal.
 *
 * Example:
 * @code
 * {
 *     char *fileToChunk = ...
 *     PARCFileChunker *chunkerA = parcFileChunker_Create(fileToChunk, 32);
 *     PARCFileChunker *chunkerB = parcFileChunker_Create(fileToChunk, 32);
 *
 *     bool equals = parcFileChunker_Equals(chunkerA, chunkerB);
 * }
 * @endcode
 */
bool parcFileChunker_Equals(const PARCFileChunker *chunkerA, const PARCFileChunker *chunkerB);

/**
 * Return an iterator to traverse the chunks of the underlying data in sequential order.
 *
 * This function can only be called once per chunker instance since the iterator
 * will mutate internal state of the chunker.
 *
 * @param [in] chunker A `PARCFileChunker` instance.
 *
 * @return a `PARCIterator` that traverses the chunks of the underlying data.
 *
 * Example
 * @code
 * {
 *     char *fileToChunk = ...
 *     PARCFileChunker *chunker = parcFileChunker_Create(fileToChunk, 32);
 *
 *     PARCIterator *itr = parcFileChunker_ForwardIterator(chunker);
 *
 *     // use the iterator to traverse the chunker
 * }
 * @endcode
 */
PARCIterator *parcFileChunker_ForwardIterator(const PARCFileChunker *chunker);

/**
 * Return an iterator to traverse the chunks of the underlying data in sequential order.
 *
 * This function can only be called once per chunker instance since the iterator
 * will mutate internal state of the chunker.
 *
 * @param [in] chunker A `PARCFileChunker` instance.
 *
 * @return a `PARCIterator` that traverses the chunks of the underlying data.
 *
 * Example
 * @code
 * {
 *     char *fileToChunk = ...
 *     PARCFileChunker *chunker = parcFileChunker_Create(fileToChunk, 32);
 *
 *     PARCIterator *itr = parcFileChunker_ReverseIterator(chunker);
 *
 *     // use the iterator to traverse the chunker
 * }
 * @endcode
 */
PARCIterator *parcFileChunker_ReverseIterator(const PARCFileChunker *chunker);

/**
 * Get the chunk size of a `PARCFileChunker.`
 *
 * @param [in] chunker A `PARCFileChunker` instance.
 *
 * @return the chunk size
 *
 * Example
 * @code
 * {
 *     PARCBuffer *dataToChunk = ...
 *     PARCFileChunker *chunker = ...
 *
 *     size_t chunkSize = parcFileChunker_GetChunkSize(chunker);
 * }
 * @endcode
 */
size_t parcFileChunker_GetChunkSize(const PARCFileChunker *chunker);
#endif // libparc_parc_FileChunker_h
