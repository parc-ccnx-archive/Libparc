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
 * @author Christopher A. Wood, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../parc_BufferChunker.c"

#include <stdio.h>

#include <LongBow/unit-test.h>
#include <parc/algol/parc_SafeMemory.h>

LONGBOW_TEST_RUNNER(parc_BufferChunker)
{
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_BufferChunker)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_BufferChunker)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parc_Chunker_CreateFromBuffer);
    LONGBOW_RUN_TEST_CASE(Global, parc_Chunker_ForwardIterator_Buffer);
    LONGBOW_RUN_TEST_CASE(Global, parc_Chunker_ForwardIterator_BufferPartial);
    LONGBOW_RUN_TEST_CASE(Global, parc_Chunker_ForwardIterator_BufferSmall);
    LONGBOW_RUN_TEST_CASE(Global, parc_Chunker_ReverseIterator_Buffer);
    LONGBOW_RUN_TEST_CASE(Global, parc_Chunker_ReverseIterator_BufferPartial);
    LONGBOW_RUN_TEST_CASE(Global, parc_Chunker_ReverseIterator_BufferSmall);
    LONGBOW_RUN_TEST_CASE(Global, parc_Chunker_GetChunkSize);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDERR_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, parc_Chunker_CreateFromBuffer)
{
    PARCBuffer *buffer = parcBuffer_Allocate(1024);

    PARCBufferChunker *chunker = parcBufferChunker_Create(buffer, 32); // each chunk is 32 bytes
    assertNotNull(chunker, "Expected non-NULL Chunker");
    PARCBufferChunker *copy = parcBufferChunker_Acquire(chunker);
    assertNotNull(chunker, "Expected non-NULL Chunker");
    assertNotNull(copy, "Expected non-NULL copy of Chunker");
    parcBufferChunker_Release(&copy);

    parcBuffer_Release(&buffer);
    parcBufferChunker_Release(&chunker);
}

LONGBOW_TEST_CASE(Global, parc_Chunker_ForwardIterator_Buffer)
{
    PARCBuffer *buffer = parcBuffer_Allocate(1024);

    for (size_t i = 0; i < 32; i++) {
        for (size_t j = 0; j < 32; j++) {
            parcBuffer_PutUint8(buffer, i);
        }
    }
    parcBuffer_Flip(buffer);

    PARCBufferChunker *chunker = parcBufferChunker_Create(buffer, 32); // each chunk is 32 bytes
    assertNotNull(chunker, "Expected non-NULL Chunker");

    PARCIterator *itr = parcBufferChunker_ForwardIterator(chunker);
    size_t count = 0;
    while (parcIterator_HasNext(itr)) {
        PARCBuffer *payload = (PARCBuffer *) parcIterator_Next(itr);

        uint8_t *contents = parcBuffer_Overlay(payload, 0);
        for (size_t i = 0; i < 32; i++) {
            assertTrue(contents[i] == count, "Expected %zu at index %zu, got %d", count, i, contents[i]);
        }
        count++;

        parcBuffer_Release(&payload);
    }
    assertTrue(count == 32, "Expected to iterate over 32 content objects from the chunker, but for %zu", count);
    parcIterator_Release(&itr);

    parcBufferChunker_Release(&chunker);
    parcBuffer_Release(&buffer);
}

LONGBOW_TEST_CASE(Global, parc_Chunker_ForwardIterator_BufferPartial)
{
    // Allocate something that's not divisible by the chunk size
    PARCBuffer *buffer = parcBuffer_Allocate(1030);

    for (size_t i = 0; i < 32; i++) {
        for (size_t j = 0; j < 32; j++) {
            parcBuffer_PutUint8(buffer, i);
        }
    }

    // Special 0xFF to mark the end...
    for (int i = 0; i < 6; i++) {
        parcBuffer_PutUint8(buffer, 0xFF);
    }

    parcBuffer_Flip(buffer);

    PARCBufferChunker *chunker = parcBufferChunker_Create(buffer, 32); // each chunk is 32 bytes
    assertNotNull(chunker, "Expected non-NULL Chunker");

    PARCIterator *itr = parcBufferChunker_ForwardIterator(chunker);
    size_t count = 0;
    while (parcIterator_HasNext(itr)) {
        PARCBuffer *payload = (PARCBuffer *) parcIterator_Next(itr);

        uint8_t *contents = parcBuffer_Overlay(payload, 0);
        if (count < 32) {
            for (size_t i = 0; i < 32; i++) {
                assertTrue(contents[i] == count, "Expected %zu at index %zu, got %d", count, i, contents[i]);
            }
        } else {
            for (size_t i = 0; i < 6; i++) {
                assertTrue(contents[i] == 0xFF, "Expected %zu at index %zu, got %d", (size_t) 0xFF, i, contents[i]);
            }
        }
        count++;

        parcBuffer_Release(&payload);
    }
    assertTrue(count == 33, "Expected to iterate over 33 content objects from the chunker, but for %zu", count);
    parcIterator_Release(&itr);

    parcBufferChunker_Release(&chunker);
    parcBuffer_Release(&buffer);
}

LONGBOW_TEST_CASE(Global, parc_Chunker_ForwardIterator_BufferSmall)
{
    // Allocate something that's not divisible by the chunk size
    PARCBuffer *buffer = parcBuffer_Allocate(16);

    // Special 0xFF to mark the end...
    for (int i = 0; i < 16; i++) {
        parcBuffer_PutUint8(buffer, 0xFF);
    }

    parcBuffer_Flip(buffer);

    PARCBufferChunker *chunker = parcBufferChunker_Create(buffer, 32); // each chunk is 32 bytes
    assertNotNull(chunker, "Expected non-NULL Chunker");

    PARCIterator *itr = parcBufferChunker_ForwardIterator(chunker);
    size_t count = 0;
    while (parcIterator_HasNext(itr)) {
        PARCBuffer *payload = (PARCBuffer *) parcIterator_Next(itr);

        uint8_t *contents = parcBuffer_Overlay(payload, 0);
        for (size_t i = 0; i < 16; i++) {
            assertTrue(contents[i] == 0xFF, "Expected %zu at index %zu, got %d", (size_t) 0xFF, i, contents[i]);
        }
        count++;

        parcBuffer_Release(&payload);
    }
    assertTrue(count == 1, "Expected to iterate over 1 content objects from the chunker, but for %zu", count);
    parcIterator_Release(&itr);

    parcBufferChunker_Release(&chunker);
    parcBuffer_Release(&buffer);
}

LONGBOW_TEST_CASE(Global, parc_Chunker_ReverseIterator_Buffer)
{
    PARCBuffer *buffer = parcBuffer_Allocate(1024);

    for (size_t i = 0; i < 32; i++) {
        for (size_t j = 0; j < 32; j++) {
            parcBuffer_PutUint8(buffer, i);
        }
    }
    parcBuffer_Flip(buffer);

    PARCBufferChunker *chunker = parcBufferChunker_Create(buffer, 32); // each chunk is 32 bytes
    assertNotNull(chunker, "Expected non-NULL Chunker");

    PARCIterator *itr = parcBufferChunker_ReverseIterator(chunker);
    size_t count = 0;
    while (parcIterator_HasNext(itr)) {
        PARCBuffer *payload = (PARCBuffer *) parcIterator_Next(itr);

        uint8_t *contents = parcBuffer_Overlay(payload, 0);
        for (size_t i = 0; i < 32; i++) {
            assertTrue(contents[i] == (31 - count), "Expected %zu at index %zu, got %d", (31 - count), i, contents[i]);
        }
        count++;

        parcBuffer_Release(&payload);
    }
    assertTrue(count == 32, "Expected to iterate over 32 content objects from the chunker, but for %zu", count);
    parcIterator_Release(&itr);

    parcBufferChunker_Release(&chunker);
    parcBuffer_Release(&buffer);
}

LONGBOW_TEST_CASE(Global, parc_Chunker_ReverseIterator_BufferPartial)
{
    PARCBuffer *buffer = parcBuffer_Allocate(1030);

    // Special 0xFF to mark the start
    for (int i = 0; i < 6; i++) {
        parcBuffer_PutUint8(buffer, 0xFF);
    }

    for (size_t i = 0; i < 32; i++) {
        for (size_t j = 0; j < 32; j++) {
            parcBuffer_PutUint8(buffer, i);
        }
    }
    parcBuffer_Flip(buffer);

    PARCBufferChunker *chunker = parcBufferChunker_Create(buffer, 32); // each chunk is 32 bytes
    assertNotNull(chunker, "Expected non-NULL Chunker");

    PARCIterator *itr = parcBufferChunker_ReverseIterator(chunker);
    size_t count = 0;
    while (parcIterator_HasNext(itr)) {
        PARCBuffer *payload = (PARCBuffer *) parcIterator_Next(itr);

        uint8_t *contents = parcBuffer_Overlay(payload, 0);
        if (count < 32) {
            for (size_t i = 0; i < 32; i++) {
                assertTrue(contents[i] == (31 - count), "Expected %zu at index %zu, got %d", count, i, contents[i]);
            }
        } else {
            for (size_t i = 0; i < 6; i++) {
                assertTrue(contents[i] == 0xFF, "Expected %zu at index %zu, got %d", (size_t) 0xFF, i, contents[i]);
            }
        }
        count++;

        parcBuffer_Release(&payload);
    }
    assertTrue(count == 33, "Expected to iterate over 33 content objects from the chunker, but for %zu", count);
    parcIterator_Release(&itr);

    parcBufferChunker_Release(&chunker);
    parcBuffer_Release(&buffer);
}

LONGBOW_TEST_CASE(Global, parc_Chunker_ReverseIterator_BufferSmall)
{
    PARCBuffer *buffer = parcBuffer_Allocate(16);

    // Special 0xFF to mark the start
    for (int i = 0; i < 16; i++) {
        parcBuffer_PutUint8(buffer, 0xFF);
    }
    parcBuffer_Flip(buffer);

    PARCBufferChunker *chunker = parcBufferChunker_Create(buffer, 32); // each chunk is 32 bytes
    assertNotNull(chunker, "Expected non-NULL Chunker");

    PARCIterator *itr = parcBufferChunker_ReverseIterator(chunker);
    size_t count = 0;
    while (parcIterator_HasNext(itr)) {
        PARCBuffer *payload = (PARCBuffer *) parcIterator_Next(itr);

        uint8_t *contents = parcBuffer_Overlay(payload, 0);
        for (size_t i = 0; i < 16; i++) {
            assertTrue(contents[i] == 0xFF, "Expected %zu at index %zu, got %d", (size_t) 0xFF, i, contents[i]);
        }
        count++;

        parcBuffer_Release(&payload);
    }
    assertTrue(count == 1, "Expected to iterate over 1 content objects from the chunker, but for %zu", count);
    parcIterator_Release(&itr);

    parcBufferChunker_Release(&chunker);
    parcBuffer_Release(&buffer);
}

LONGBOW_TEST_CASE(Global, parc_Chunker_GetChunkSize)
{
    size_t expectedChunkSize = 32;
    PARCBuffer *buffer = parcBuffer_Allocate(16);
    PARCBufferChunker *chunker = parcBufferChunker_Create(buffer, expectedChunkSize); // each chunk is 32 bytes
    size_t actualChunkSize = parcBufferChunker_GetChunkSize(chunker);

    assertTrue(actualChunkSize == expectedChunkSize, "Expected chunk size of %zu, got %zu", expectedChunkSize, actualChunkSize);

    parcBuffer_Release(&buffer);
    parcBufferChunker_Release(&chunker);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_BufferChunker);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
