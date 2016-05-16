/*
 * Copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @author Kevin Fox, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>

#include <LongBow/runtime.h>

#include "internal_parc_Event.h"
#include <parc/algol/parc_Event.h>
#include <parc/algol/parc_EventBuffer.h>
#include <parc/algol/parc_FileOutputStream.h>
#include <parc/logging/parc_Log.h>
#include <parc/logging/parc_LogReporterFile.h>

static PARCLog *parcEventBufferDebugLog = NULL;

#define parcEventBuffer_LogDebug(parcEvent, ...) \
    if (parcEventBufferDebugLog) \
        parcLog_Debug(parcEventBufferDebugLog, __VA_ARGS__)

/**
 * Current implementation based on top of libevent2
 */

#include <event2/buffer.h>

/**
 * @typedef PARCEventBuffer
 * @brief A structure containing private libevent state data variables
 *
 * The evbuffer either points to an evbuffer owned by the bufferevent, or an
 * allocated evbuffer (allocated_evbuffer) that is our responsibility to destroy.
 */
struct PARCEventBuffer {
    struct evbuffer *evbuffer;
    struct evbuffer *allocated_evbuffer;
};

PARCEventBuffer *
parcEventBuffer_Create(void)
{
    internal_parc_initializeLibevent();
    struct evbuffer *new_evbuffer = evbuffer_new();
    assertNotNull(new_evbuffer, "libevent returned a null evbuffer.\n");

    PARCEventBuffer *parcEventBuffer = parcMemory_AllocateAndClear(sizeof(PARCEventBuffer));
    assertNotNull(parcEventBuffer, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(PARCEventBuffer));
    parcEventBuffer->allocated_evbuffer = new_evbuffer;
    parcEventBuffer->evbuffer = parcEventBuffer->allocated_evbuffer;

    parcEventBuffer_LogDebug(parcEventBuffer, "parcEventBuffer_Create() = %p\n", parcEventBuffer);

    return parcEventBuffer;
}

void
parcEventBuffer_Destroy(PARCEventBuffer **parcEventBuffer)
{
    parcEventBuffer_LogDebug((*parcEventBuffer), "parcEventBuffer_Destroy(parcEventBuffer=%p)\n", *parcEventBuffer);
    assertNotNull(parcEventBuffer, "parcEventBuffer_Destroy was passed a null parcEventBuffer pointer\n");
    assertNotNull(*parcEventBuffer, "parcEventBuffer_Destroy was passed a null parcEventBuffer\n");

    // Destroy allocated eveventBuffer if it was allocated by us, otherwise it's owned by bufferevent
    if ((*parcEventBuffer)->allocated_evbuffer) {
        parcEventBuffer_LogDebug((*parcEventBuffer), "parcEventBuffer_Destroy(parcEventBuffer=%p) freeing evbuffer %p\n", *parcEventBuffer, (*parcEventBuffer)->allocated_evbuffer);
        evbuffer_free((*parcEventBuffer)->allocated_evbuffer);
    }
    parcMemory_Deallocate((void **) parcEventBuffer);
    parcEventBuffer_LogDebug((*parcEventBuffer), "parcEventBuffer_Destroy() buffer already deallocated\n");
}

bool
parcEventBuffer_IsValid(const PARCEventBuffer *eventBuffer)
{
    bool result = false;

    if (eventBuffer != NULL) {
        if (eventBuffer->evbuffer) {
            result = true;
        }
    }

    return result;
}

void
parcEventBuffer_AssertValid(const PARCEventBuffer *eventBuffer)
{
    assertTrue(parcEventBuffer_IsValid(eventBuffer),
               "PARCEventBuffer@%p is not valid.", (void *) eventBuffer);
}

size_t
parcEventBuffer_GetLength(PARCEventBuffer *parcEventBuffer)
{
    parcEventBuffer_LogDebug(parcEventBuffer, "parcEventBuffer_GetLength(parcEventBuffer=%p)\n", parcEventBuffer);
//    parcEventBuffer_OptionalAssertValid(parcEventBuffer);
    assertNotNull(parcEventBuffer, "parcEventBuffer_GetLength was passed a null parcEventBuffer\n");

    if (parcEventBuffer->evbuffer) {
        return evbuffer_get_length(parcEventBuffer->evbuffer);
    } else {
        return 0;
    }
}

uint8_t *
parcEventBuffer_Pullup(PARCEventBuffer *parcEventBuffer, ssize_t size)
{
    parcEventBuffer_LogDebug(parcEventBuffer, "parcEventBuffer_Pullup(parcEventBuffer=%p,size=%zx)\n", parcEventBuffer, size);

    parcEventBuffer_OptionalAssertValid(parcEventBuffer);
//    assertNotNull(parcEventBuffer, "parcEventBuffer_Pullup was passed a null parcEventBuffer\n");
//    assertNotNull(parcEventBuffer->evbuffer, "parcEventBuffer_Pullup was passed a null libevent evbuffer\n");

    return evbuffer_pullup(parcEventBuffer->evbuffer, (ev_ssize_t) size);
}

int
parcEventBuffer_ReadIntoBuffer(PARCEventBuffer *source, PARCEventBuffer *destination, size_t length)
{
    parcEventBuffer_OptionalAssertValid(source);
    parcEventBuffer_OptionalAssertValid(destination);
//    assertNotNull(source, "parcEventBuffer_ReadIntoBuffer was passed a null source buffer\n");
//    assertNotNull(source->evbuffer, "parcEventBuffer_ReadIntoBuffer was passed a null source evbuffer\n");
//    assertNotNull(destination, "parcEventBuffer_ReadIntoBuffer was passed a null destination buffer\n");
//    assertNotNull(destination->evbuffer, "parcEventBuffer_ReadIntoBuffer was passed a null destination evbuffer\n");

    return evbuffer_remove_buffer(source->evbuffer, destination->evbuffer, length);
}

int
parcEventBuffer_Read(PARCEventBuffer *readBuffer, void *data, size_t length)
{
    parcEventBuffer_OptionalAssertValid(readBuffer);
//    assertNotNull(readBuffer, "parcEventBuffer_Read was passed a null buffer\n");
//    assertNotNull(readBuffer->evbuffer, "parcEventBuffer_Read was passed a null libevent evbuffer\n");

    if (data == NULL) {
        return evbuffer_drain(readBuffer->evbuffer, length);
    } else {
        return evbuffer_remove(readBuffer->evbuffer, data, length);
    }
}

int
parcEventBuffer_WriteToFileDescriptor(PARCEventBuffer *writeBuffer, int fd, ssize_t length)
{
    parcEventBuffer_OptionalAssertValid(writeBuffer);

//    assertNotNull(writeBuffer, "parcEventBuffer_WriteToFileDescriptor was passed a null buffer\n");
//    assertNotNull(writeBuffer->evbuffer, "parcEventBuffer_WriteToFileDescriptor was passed a null libevent evbuffer\n");

    return evbuffer_write_atmost(writeBuffer->evbuffer, fd, length);
}

int
parcEventBuffer_ReadFromFileDescriptor(PARCEventBuffer *readBuffer, int fd, size_t length)
{
    parcEventBuffer_OptionalAssertValid(readBuffer);
//    assertNotNull(readBuffer, "parcEventBuffer_ReadFromFileDescriptor was passed a null buffer\n");
//    assertNotNull(readBuffer->evbuffer, "parcEventBuffer_ReadFromFileDescriptor was passed a null libevent evbuffer\n");

    return evbuffer_read(readBuffer->evbuffer, fd, (int) length);
}

void
parcEventBuffer_FreeLine(PARCEventBuffer *readBuffer, char **line)
{
    parcEventBuffer_OptionalAssertValid(readBuffer);
//    assertNotNull(readBuffer, "parcEventBuffer_ReadLine was passed a null readBuffer\n");
//    assertNotNull(readBuffer->evbuffer, "parcEventBuffer_ReadLine was passed a null libevent evbuffer\n");

    parcMemory_Deallocate((void **) line);
}

char *
parcEventBuffer_ReadLine(PARCEventBuffer *readBuffer, size_t *length)
{
    parcEventBuffer_OptionalAssertValid(readBuffer);
//    assertNotNull(readBuffer, "parcEventBuffer_ReadLine was passed a null readBuffer\n");
//    assertNotNull(readBuffer->evbuffer, "parcEventBuffer_ReadLine was passed a null libevent evbuffer\n");

    return evbuffer_readln(readBuffer->evbuffer, length, EVBUFFER_EOL_CRLF);
}

int
parcEventBuffer_AppendBuffer(PARCEventBuffer *source, PARCEventBuffer *destination)
{
    parcEventBuffer_OptionalAssertValid(source);
    parcEventBuffer_OptionalAssertValid(destination);
//    assertNotNull(source, "parcEventBuffer_AppendBuffer was passed a null source parcEventBuffer\n");
//    assertNotNull(destination, "parcEventBuffer_AppendBuffer was passed a null destination parcEventBuffer\n");
//    assertNotNull(source->evbuffer, "parcEventBuffer_AppendBuffer was passed a null source libevent evbuffer\n");
//    assertNotNull(destination->evbuffer, "parcEventBuffer_AppendBuffer was passed a null destination libevent evbuffer\n");

    return evbuffer_add_buffer(destination->evbuffer, source->evbuffer);
}

int
parcEventBuffer_Append(PARCEventBuffer *parcEventBuffer, void *data, size_t length)
{
    parcEventBuffer_OptionalAssertValid(parcEventBuffer);
//    assertNotNull(parcEventBuffer, "parcEventBuffer_Append was passed a null parcEventBuffer\n");
//    assertNotNull(parcEventBuffer->evbuffer, "parcEventBuffer_Append was passed a null libevent evbuffer\n");
    assertNotNull(data, "parcEventBuffer_Append was passed a null data buffer\n");

    return evbuffer_add(parcEventBuffer->evbuffer, data, length);
}

int
parcEventBuffer_Prepend(PARCEventBuffer *readBuffer, void *data, size_t length)
{
    parcEventBuffer_OptionalAssertValid(readBuffer);
//    assertNotNull(readBuffer->evbuffer, "parcEventBuffer_Prepend was passed a null libevent evbuffer\n");
//    assertNotNull(readBuffer, "parcEventBuffer_Prepend was passed a null buffer\n");
    assertNotNull(data, "parcEventBuffer_Prepend was passed a null data buffer\n");

    return evbuffer_prepend(readBuffer->evbuffer, data, length);
}

PARCEventBuffer *
parcEventBuffer_GetQueueBufferInput(PARCEventQueue *queue)
{
    assertNotNull(queue, "parcEventBuffer_GetQueueBufferInput was passed a null queue\n");

    PARCEventBuffer *parcEventBuffer = parcMemory_AllocateAndClear(sizeof(PARCEventBuffer));
    assertNotNull(parcEventBuffer, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(PARCEventBuffer));

    parcEventBuffer->evbuffer = internal_parcEventQueue_GetEvInputBuffer(queue);
    parcEventBuffer_LogDebug(parcEventBuffer, "parcEventBuffer_GetQueueBufferInput(queue=%p)\n", queue);

    return parcEventBuffer;
}

PARCEventBuffer *
parcEventBuffer_GetQueueBufferOutput(PARCEventQueue *queue)
{
    assertNotNull(queue, "parcEventBuffer_GetQueueBufferOutput was passed a null queue\n");

    PARCEventBuffer *parcEventBuffer = parcMemory_AllocateAndClear(sizeof(PARCEventBuffer));
    assertNotNull(parcEventBuffer, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(PARCEventBuffer));

    parcEventBuffer->evbuffer = internal_parcEventQueue_GetEvOutputBuffer(queue);
    parcEventBuffer_LogDebug(parcEventBuffer, "parcEventBuffer_GetQueueBufferInput(queue=%p)\n", queue);

    return parcEventBuffer;
}

void
parcEventBuffer_EnableDebug(PARCLog *logger)
{
    parcEventBufferDebugLog = logger;
}

void
parcEventBuffer_DisableDebug(void)
{
    parcEventBufferDebugLog = NULL;
}
