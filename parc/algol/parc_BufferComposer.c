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
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */

#include <config.h>

#include <LongBow/runtime.h>

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_BufferComposer.h>
#include <parc/algol/parc_Buffer.h>

struct parc_buffer_composer {
    size_t incrementHeuristic;
    PARCBuffer *buffer;
};

static void
_finalize(PARCBufferComposer **bufferPtr)
{
    if (bufferPtr != NULL) {
        PARCBufferComposer *composer = *bufferPtr;
        parcBufferComposer_OptionalAssertValid(composer);
        if (composer->buffer != NULL) {
            parcBuffer_Release(&composer->buffer);
        }
    }
}

parcObject_ExtendPARCObject(PARCBufferComposer, _finalize, NULL, NULL, parcBufferComposer_Equals, NULL, NULL, NULL);

static PARCBufferComposer *
_create(void)
{
    return parcObject_CreateInstance(PARCBufferComposer);
}

/**
 * Ensure that the given `PARCBufferComposer` has at least the required number of bytes remaining.
 *
 * If the remaining capacity (the difference between the capacity of the buffer and its current position)
 * of the underlying `PARCBuffer` is less than the required number of bytes,
 * the underlying PARCBuffer is expanded with sufficient space to accomodate the required number of bytes.
 *
 * The position, limit, and mark remain unchanged.
 * The capacity is increased.
 */
static PARCBufferComposer *
_ensureRemaining(PARCBufferComposer *composer, size_t required)
{
    parcBufferComposer_OptionalAssertValid(composer);

    size_t remainingCapacity = parcBuffer_Capacity(composer->buffer) - parcBuffer_Position(composer->buffer);

    if (remainingCapacity < required) {
        size_t incrementAmount = required;
        if (incrementAmount < composer->incrementHeuristic) {
            incrementAmount = composer->incrementHeuristic;
        }
        size_t newCapacity = parcBuffer_Capacity(composer->buffer) + incrementAmount;

        PARCBuffer *newBuffer = parcBuffer_Allocate(newCapacity);
        parcBuffer_Flip(composer->buffer);
        parcBuffer_PutBuffer(newBuffer, composer->buffer);
        parcBuffer_Release(&composer->buffer);
        composer->buffer = newBuffer;
    }

    return composer;
}

void
parcBufferComposer_AssertValid(const PARCBufferComposer *composer)
{
    trapIllegalValueIf(composer == NULL, "Parameter must be a non-null pointer to a valid PARCBufferComposer.");
    trapIllegalValueIf(composer->incrementHeuristic < sizeof(void *), "Heuristic cannot be < sizeof(void *) (%zd), actual %zd", sizeof(void *), composer->incrementHeuristic);
}

PARCBufferComposer *
parcBufferComposer_Create(void)
{
    return parcBufferComposer_Allocate(parcMemory_RoundUpToCacheLine(LEVEL1_DCACHE_LINESIZE));
}

PARCBufferComposer *
parcBufferComposer_Allocate(size_t size)
{
    PARCBufferComposer *result = _create();
    if (result != NULL) {
        result->buffer = parcBuffer_Allocate(size);
        result->incrementHeuristic = parcMemory_RoundUpToCacheLine(size);
        if (result->buffer == NULL) {
            result->incrementHeuristic = sizeof(void *); // minimum size
            parcBufferComposer_Release(&result);
        }
    }
    return result;
}

parcObject_ImplementAcquire(parcBufferComposer, PARCBufferComposer);

parcObject_ImplementRelease(parcBufferComposer, PARCBufferComposer);

bool
parcBufferComposer_Equals(const PARCBufferComposer *x, const PARCBufferComposer *y)
{
    if (x == y) {
        return true;
    }
    if (x == NULL || y == NULL) {
        return false;
    }

    if (x->incrementHeuristic == y->incrementHeuristic) {
        if (parcBuffer_Equals(x->buffer, y->buffer)) {
            return true;
        }
    }

    return false;
}

PARCBufferComposer *
parcBufferComposer_PutArray(PARCBufferComposer *composer, const unsigned char *bytes, size_t length)
{
    if (length > 0) {
        composer = _ensureRemaining(composer, length);
        if (composer != NULL) {
            parcBuffer_PutArray(composer->buffer, length, bytes);
        }
    }

    return composer;
}

PARCBufferComposer *
parcBufferComposer_PutChar(PARCBufferComposer *composer, char val)
{
    return parcBufferComposer_PutUint8(composer, (uint8_t) val);
}

PARCBufferComposer *
parcBufferComposer_PutUint8(PARCBufferComposer *composer, uint8_t byte)
{
    composer = _ensureRemaining(composer, 1);
    if (composer != NULL) {
        parcBuffer_PutUint8(composer->buffer, byte);
    }

    return composer;
}

PARCBufferComposer *
parcBufferComposer_PutUint16(PARCBufferComposer *composer, uint16_t value)
{
    composer = _ensureRemaining(composer, 2);
    if (composer != NULL) {
        parcBuffer_PutUint16(composer->buffer, value);
    }

    return composer;
}

PARCBufferComposer *
parcBufferComposer_PutUint32(PARCBufferComposer *composer, uint32_t value)
{
    composer = _ensureRemaining(composer, 4);
    if (composer != NULL) {
        parcBuffer_PutUint32(composer->buffer, value);
    }

    return composer;
}

PARCBufferComposer *
parcBufferComposer_PutUint64(PARCBufferComposer *composer, uint64_t value)
{
    composer = _ensureRemaining(composer, 8);
    if (composer != NULL) {
        parcBuffer_PutUint64(composer->buffer, value);
    }

    return composer;
}

PARCBufferComposer *
parcBufferComposer_PutBuffer(PARCBufferComposer *composer, const PARCBuffer *source)
{
    composer = _ensureRemaining(composer, parcBuffer_Remaining(source));
    if (composer != NULL) {
        parcBuffer_PutBuffer(composer->buffer, source);
    }

    return composer;
}

PARCBufferComposer *
parcBufferComposer_PutString(PARCBufferComposer *composer, const char *string)
{
    PARCBuffer *wrapper = parcBuffer_AllocateCString(string);

    composer = _ensureRemaining(composer, parcBuffer_Remaining(wrapper));
    if (composer != NULL) {
        parcBuffer_PutBuffer(composer->buffer, wrapper);
    }

    parcBuffer_Release(&wrapper);

    return composer;
}

PARCBufferComposer *
parcBufferComposer_PutStrings(PARCBufferComposer *composer, ...)
{
    va_list ap;
    va_start(ap, composer);

    char *arg;
    while ((arg = va_arg(ap, char *)) != NULL) {
        parcBufferComposer_PutString(composer, arg);
    }
    va_end(ap);

    return composer;
}

PARCBufferComposer *
parcBufferComposer_Format(PARCBufferComposer *composer, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    char *cString;
    ssize_t written = vasprintf(&cString, format, ap);
    assertTrue(written >= 0, "Got error from vasprintf");
    va_end(ap);

    parcBufferComposer_PutString(composer, cString);
    free(cString);

    return composer;
}

PARCBuffer *
parcBufferComposer_GetBuffer(const PARCBufferComposer *composer)
{
    return composer->buffer;
}

PARCBuffer *
parcBufferComposer_CreateBuffer(PARCBufferComposer *composer)
{
    return parcBuffer_Duplicate(composer->buffer);
}

PARCBuffer *
parcBufferComposer_ProduceBuffer(PARCBufferComposer *composer)
{
    return parcBuffer_Acquire(parcBuffer_Flip(composer->buffer));
}

char *
parcBufferComposer_ToString(PARCBufferComposer *composer)
{
    PARCBuffer *buffer = parcBuffer_Flip(parcBuffer_Duplicate(composer->buffer));

    char *result = parcBuffer_ToString(buffer);
    parcBuffer_Release(&buffer);

    return result;
}
