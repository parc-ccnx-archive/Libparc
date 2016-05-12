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
#include <stdio.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_Buffer.h>
#include <parc/algol/parc_ReadOnlyBuffer.h>
#include <parc/algol/parc_DisplayIndented.h>

struct parc_readonly_buffer {
    PARCBuffer *buffer;
};

static void
_readOnlyBuffer_Finalize(PARCReadOnlyBuffer **bufferPtr)
{
    PARCReadOnlyBuffer *buffer = *bufferPtr;

    parcBuffer_Release(&buffer->buffer);
}

parcObject_ExtendPARCObject(PARCReadOnlyBuffer, _readOnlyBuffer_Finalize, parcReadOnlyBuffer_Copy,
                            parcReadOnlyBuffer_ToString, parcReadOnlyBuffer_Equals, NULL, parcReadOnlyBuffer_HashCode, NULL);

PARCReadOnlyBuffer *
parcReadOnlyBuffer_Create(PARCBuffer *buffer)
{
    PARCReadOnlyBuffer *result = NULL;
    if (buffer != NULL) {
        result = parcObject_CreateInstance(PARCReadOnlyBuffer);
        if (result != NULL) {
            result->buffer = parcBuffer_WrapByteArray(parcBuffer_Array(buffer), parcBuffer_Position(buffer), parcBuffer_Limit(buffer));
        }
    }
    return result;
}

PARCReadOnlyBuffer *
parcReadOnlyBuffer_Wrap(uint8_t *array, size_t arrayLength, size_t position, size_t limit)
{
    PARCReadOnlyBuffer *result = parcObject_CreateInstance(PARCReadOnlyBuffer);
    if (result != NULL) {
        result->buffer = parcBuffer_Wrap(array, arrayLength, position, limit);
    }
    return result;
}

parcObject_ImplementAcquire(parcReadOnlyBuffer, PARCReadOnlyBuffer);

parcObject_ImplementRelease(parcReadOnlyBuffer, PARCReadOnlyBuffer);

size_t
parcReadOnlyBuffer_Capacity(const PARCReadOnlyBuffer *buffer)
{
    return parcBuffer_Capacity(buffer->buffer);
}

PARCReadOnlyBuffer *
parcReadOnlyBuffer_Clear(PARCReadOnlyBuffer *buffer)
{
    parcBuffer_Clear(buffer->buffer);
    return buffer;
}

bool
parcReadOnlyBuffer_Equals(const PARCReadOnlyBuffer *x, const PARCReadOnlyBuffer *y)
{
    if (x == y) {
        return true;
    }
    if (x == NULL || y == NULL) {
        return false;
    }

    return parcBuffer_Equals(x->buffer, y->buffer);
}

PARCByteArray *
parcReadOnlyBuffer_Array(const PARCReadOnlyBuffer *buffer)
{
    return parcBuffer_Array(buffer->buffer);
}

PARCReadOnlyBuffer *
parcReadOnlyBuffer_Copy(const PARCReadOnlyBuffer *original)
{
    PARCReadOnlyBuffer *result = parcObject_CreateInstance(PARCReadOnlyBuffer);

    if (result != NULL) {
        result->buffer = parcBuffer_Copy(original->buffer);
    }
    return result;
}

size_t
parcReadOnlyBuffer_ArrayOffset(const PARCReadOnlyBuffer *buffer)
{
    return parcBuffer_ArrayOffset(buffer->buffer);
}

PARCReadOnlyBuffer *
parcReadOnlyBuffer_Rewind(PARCReadOnlyBuffer *buffer)
{
    parcBuffer_Rewind(buffer->buffer);
    return buffer;
}

PARCReadOnlyBuffer *
parcReadOnlyBuffer_Reset(PARCReadOnlyBuffer *buffer)
{
    parcBuffer_Reset(buffer->buffer);
    return buffer;
}

size_t
parcReadOnlyBuffer_Limit(const PARCReadOnlyBuffer *buffer)
{
    return parcBuffer_Limit(buffer->buffer);
}

PARCReadOnlyBuffer *
parcReadOnlyBuffer_Mark(PARCReadOnlyBuffer *buffer)
{
    parcBuffer_Mark(buffer->buffer);
    return buffer;
}

PARCReadOnlyBuffer *
parcReadOnlyBuffer_SetLimit(PARCReadOnlyBuffer *buffer, size_t newLimit)
{
    parcBuffer_SetLimit(buffer->buffer, newLimit);

    return buffer;
}

void *
parcReadOnlyBuffer_Overlay(PARCReadOnlyBuffer *buffer, size_t length)
{
    return parcBuffer_Overlay(buffer->buffer, length);
}

size_t
parcReadOnlyBuffer_Position(const PARCReadOnlyBuffer *buffer)
{
    return parcBuffer_Position(buffer->buffer);
}

PARCReadOnlyBuffer *
parcReadOnlyBuffer_SetPosition(PARCReadOnlyBuffer *buffer, size_t newPosition)
{
    parcBuffer_SetPosition(buffer->buffer, newPosition);
    return buffer;
}

size_t
parcReadOnlyBuffer_Remaining(const PARCReadOnlyBuffer *buffer)
{
    return parcBuffer_Remaining(buffer->buffer);
}

bool
parcReadOnlyBuffer_HasRemaining(const PARCReadOnlyBuffer *buffer)
{
    return parcBuffer_HasRemaining(buffer->buffer);
}

PARCReadOnlyBuffer *
parcReadOnlyBuffer_Flip(PARCReadOnlyBuffer *buffer)
{
    parcBuffer_Flip(buffer->buffer);
    return buffer;
}

uint8_t
parcReadOnlyBuffer_GetAtIndex(const PARCReadOnlyBuffer *buffer, size_t index)
{
    return parcBuffer_GetAtIndex(buffer->buffer, index);
}

PARCReadOnlyBuffer *
parcReadOnlyBuffer_GetArray(PARCReadOnlyBuffer *buffer, uint8_t *array, size_t length)
{
    parcBuffer_GetBytes(buffer->buffer, length, array);
    return buffer;
}

uint8_t
parcReadOnlyBuffer_GetUint8(PARCReadOnlyBuffer *buffer)
{
    return parcBuffer_GetUint8(buffer->buffer);
}

uint16_t
parcReadOnlyBuffer_GetUint16(PARCReadOnlyBuffer *buffer)
{
    return parcBuffer_GetUint16(buffer->buffer);
}

uint32_t
parcReadOnlyBuffer_GetUint32(PARCReadOnlyBuffer *buffer)
{
    return parcBuffer_GetUint32(buffer->buffer);
}

uint64_t
parcReadOnlyBuffer_GetUint64(PARCReadOnlyBuffer *buffer)
{
    return parcBuffer_GetUint64(buffer->buffer);
}

PARCHashCode
parcReadOnlyBuffer_HashCode(const PARCReadOnlyBuffer *buffer)
{
    return parcBuffer_HashCode(buffer->buffer);
}

char *
parcReadOnlyBuffer_ToString(const PARCReadOnlyBuffer *buffer)
{
    return parcBuffer_ToString(buffer->buffer);
}

void
parcReadOnlyBuffer_Display(const PARCReadOnlyBuffer *buffer, int indentation)
{
    parcDisplayIndented_PrintLine(indentation, "PARCReadOnlyBuffer@%p {\n", (void *) buffer);
    parcBuffer_Display(buffer->buffer, indentation + 1);
    parcDisplayIndented_PrintLine(indentation, "}\n");
}
