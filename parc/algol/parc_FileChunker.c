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
 * @author Christopher Wood, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */

#include <config.h>

#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_Buffer.h>
#include <parc/algol/parc_Memory.h>

#include <parc/algol/parc_RandomAccessFile.h>

#include <parc/algol/parc_FileChunker.h>

PARCChunkerInterface *PARCFileChunkerAsChunker = &(PARCChunkerInterface) {
    .ForwardIterator = (void *(*)(const void *)) parcFileChunker_ForwardIterator,
    .ReverseIterator = (void *(*)(const void *)) parcFileChunker_ReverseIterator,
    .GetChunkSize = (size_t (*)(const void *)) parcFileChunker_GetChunkSize
};

struct _parc_chunker_state {
    size_t chunkNumber;
    int direction;
    bool atEnd;
    size_t position;
    size_t nextChunkSize;
    size_t totalSize;
};

typedef struct _parc_chunker_state _ChunkerState;

struct parc_buffer_chunker {
    // State
    size_t chunkSize;

    // Container for the data to be parsed
    PARCFile *file;
    PARCRandomAccessFile *fhandle;

    // The current element of the iterator
    PARCBuffer *currentElement;
};

static void
_destroy(PARCFileChunker **chunkerP)
{
    if ((*chunkerP)->fhandle != NULL) {
        parcRandomAccessFile_Release(&(*chunkerP)->fhandle);
    }

    if ((*chunkerP)->file != NULL) {
        parcFile_Release(&(*chunkerP)->file);
    }

    if ((*chunkerP)->currentElement != NULL) {
        parcBuffer_Release(&(*chunkerP)->currentElement);
    }
}

static void *
_InitForward(PARCFileChunker *chunker)
{
    _ChunkerState *state = parcMemory_Allocate(sizeof(_ChunkerState));

    state->chunkNumber = 0;
    state->direction = 0;
    state->position = 0;
    state->atEnd = false;
    state->totalSize = parcFile_GetFileSize(chunker->file);

    if (state->totalSize < chunker->chunkSize) {
        state->position = 0;
        state->nextChunkSize = state->totalSize;
    } else {
        state->position = 0;
        state->nextChunkSize = chunker->chunkSize;
    }

    return state;
}

static void *
_InitReverse(PARCFileChunker *chunker)
{
    _ChunkerState *state = parcMemory_Allocate(sizeof(_ChunkerState));

    state->chunkNumber = 0;
    state->direction = 1;
    state->atEnd = false;
    state->totalSize = parcFile_GetFileSize(chunker->file);

    if (state->totalSize < chunker->chunkSize) {
        state->position = 0;
        state->nextChunkSize = state->totalSize;
    } else {
        state->position = state->totalSize - chunker->chunkSize;
        state->nextChunkSize = chunker->chunkSize;
    }

    return state;
}

static bool
_parcChunker_HasNext(PARCFileChunker *chunker, void *voidstate)
{
    _ChunkerState *state = (_ChunkerState *) voidstate;
    return !state->atEnd;
}

static void
_advanceStateForward(PARCFileChunker *chunker, _ChunkerState *state)
{
    state->position += state->nextChunkSize;
    size_t remaining = state->totalSize - state->position;

    if (remaining == 0) {
        state->atEnd = true;
    } else if (remaining > chunker->chunkSize) {
        state->nextChunkSize = chunker->chunkSize;
    } else {
        state->nextChunkSize = remaining;
    }
}

static void
_advanceStateBackward(PARCFileChunker *chunker, _ChunkerState *state)
{
    // Previous chunk size
    size_t chunkSize = state->nextChunkSize;
    if (chunkSize != chunker->chunkSize || state->position == 0) {
        state->atEnd = true;
    } else {
        if (state->position < chunkSize) {
            state->nextChunkSize = state->position; // on next read, go to the current position
            state->position = 0; // we reached the beginning
        } else {
            state->position -= chunkSize;
        }
    }
}

static void
_advanceState(PARCFileChunker *chunker, _ChunkerState *state)
{
    state->chunkNumber++;

    if (state->direction == 0) {
        _advanceStateForward(chunker, state);
    } else {
        _advanceStateBackward(chunker, state);
    }
}

static void *
_parcChunker_NextFromBuffer(PARCFileChunker *chunker, _ChunkerState *state)
{
    size_t chunkSize = state->nextChunkSize;

    parcRandomAccessFile_Seek(chunker->fhandle, state->position, PARCRandomAccessFilePosition_Start);

    PARCBuffer *slice = parcBuffer_Allocate(chunkSize);
    parcRandomAccessFile_Read(chunker->fhandle, slice);
    slice = parcBuffer_Flip(slice);

    _advanceState(chunker, state);

    return slice;
}

static void *
_parcChunker_Next(PARCFileChunker *chunker, void *state)
{
    PARCBuffer *buffer = _parcChunker_NextFromBuffer(chunker, state);

    if (chunker->currentElement != NULL) {
        parcBuffer_Release(&chunker->currentElement);
    }
    if (buffer != NULL) {
        chunker->currentElement = parcBuffer_Acquire(buffer);
    }

    return state;
}

static void
_parcChunker_RemoveAt(PARCFileChunker *chunker, void **state)
{
    // pass
}

static void *
_parcChunker_GetElement(PARCFileChunker *chunker, void *state)
{
    return chunker->currentElement;
}

static void
_parcChunker_Finish(PARCFileChunker *chunker, void *state)
{
    _ChunkerState *thestate = (_ChunkerState *) state;
    parcMemory_Deallocate(&thestate);
}

static void
_parcChunker_AssertValid(const void *state)
{
    // pass
}

parcObject_ExtendPARCObject(PARCFileChunker, _destroy, NULL, NULL, NULL, NULL, NULL, NULL);
parcObject_ImplementAcquire(parcFileChunker, PARCFileChunker);
parcObject_ImplementRelease(parcFileChunker, PARCFileChunker);

PARCFileChunker *
parcFileChunker_Create(PARCFile *file, size_t chunkSize)
{
    PARCFileChunker *chunker = parcObject_CreateInstance(PARCFileChunker);

    if (chunker != NULL) {
        chunker->chunkSize = chunkSize;
        chunker->file = parcFile_Acquire(file);
        chunker->fhandle = parcRandomAccessFile_Open(chunker->file);
        chunker->currentElement = NULL;
    }

    return chunker;
}


PARCIterator *
parcFileChunker_ForwardIterator(const PARCFileChunker *chunker)
{
    PARCIterator *iterator = parcIterator_Create((void *) chunker,
                                                 (void *(*)(PARCObject *))_InitForward,
                                                 (bool (*)(PARCObject *, void *))_parcChunker_HasNext,
                                                 (void *(*)(PARCObject *, void *))_parcChunker_Next,
                                                 (void (*)(PARCObject *, void **))_parcChunker_RemoveAt,
                                                 (void *(*)(PARCObject *, void *))_parcChunker_GetElement,
                                                 (void (*)(PARCObject *, void *))_parcChunker_Finish,
                                                 (void (*)(const void *))_parcChunker_AssertValid);

    return iterator;
}

PARCIterator *
parcFileChunker_ReverseIterator(const PARCFileChunker *chunker)
{
    PARCIterator *iterator = parcIterator_Create((void *) chunker,
                                                 (void *(*)(PARCObject *))_InitReverse,
                                                 (bool (*)(PARCObject *, void *))_parcChunker_HasNext,
                                                 (void *(*)(PARCObject *, void *))_parcChunker_Next,
                                                 (void (*)(PARCObject *, void **))_parcChunker_RemoveAt,
                                                 (void *(*)(PARCObject *, void *))_parcChunker_GetElement,
                                                 (void (*)(PARCObject *, void *))_parcChunker_Finish,
                                                 (void (*)(const void *))_parcChunker_AssertValid);

    return iterator;
}

size_t
parcFileChunker_GetChunkSize(const PARCFileChunker *chunker)
{
    return chunker->chunkSize;
}
