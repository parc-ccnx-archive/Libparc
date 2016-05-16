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
#include <errno.h>

#include <LongBow/runtime.h>

#include "internal_parc_Event.h"
#include <parc/algol/parc_EventScheduler.h>
#include <parc/algol/parc_EventQueue.h>
#include <parc/algol/parc_FileOutputStream.h>
#include <parc/logging/parc_Log.h>
#include <parc/logging/parc_LogReporterFile.h>

static int _parc_event_queue_debug_enabled = 0;

#define parcEventQueue_LogDebug(parcEventQueue, ...) \
    if (_parc_event_queue_debug_enabled) \
        parcLog_Debug(parcEventScheduler_GetLogger(parcEventQueue->eventScheduler), __VA_ARGS__)

/**
 * Current implementation based on top of libevent2
 */

#include <event2/buffer.h>
#include <event2/bufferevent.h>

/**
 * @typedef PARCEventQueue
 * @brief A structure containing private event state
 */
struct PARCEventQueue {
    // Event scheduler we have been queued with
    PARCEventScheduler *eventScheduler;

    struct bufferevent *buffereventBuffer;
    // Interpose on bufferevent callbacks
    PARCEventQueue_Callback *readCallback;
    void *readUserData;
    PARCEventQueue_Callback *writeCallback;
    void *writeUserData;
    PARCEventQueue_EventCallback *eventCallback;
    void *eventUserData;
};

struct PARCEventQueuePair {
    PARCEventQueue *up;
    PARCEventQueue *down;
};

static void
_parc_queue_read_callback(struct bufferevent *bev, void *ptr)
{
    PARCEventQueue *parcEventQueue = (PARCEventQueue *) ptr;
    parcEventQueue_LogDebug(parcEventQueue,
                            "_parc_queue_read_callback(bev=%p,ptr->buffereventBuffer=%p,parcEventQueue=%p)\n",
                            bev, parcEventQueue->buffereventBuffer, parcEventQueue);
    assertNotNull(parcEventQueue->readCallback, "parcEvent read callback called when NULL");

    parcEventQueue->readCallback(parcEventQueue, PARCEventType_Read, parcEventQueue->readUserData);
}

static void
_parc_queue_write_callback(struct bufferevent *bev, void *ptr)
{
    PARCEventQueue *parcEventQueue = (PARCEventQueue *) ptr;
    parcEventQueue_LogDebug(parcEventQueue,
                            "_parc_queue_write_callback(bev=%p,ptr->buffereventBuffer=%p,parcEventQueue=%p)\n",
                            bev, parcEventQueue->buffereventBuffer, parcEventQueue);
    assertNotNull(parcEventQueue->writeCallback, "parcEvent write callback called when NULL");

    parcEventQueue->writeCallback(parcEventQueue, PARCEventType_Write, parcEventQueue->writeUserData);
}

static void
_parc_queue_event_callback(struct bufferevent *bev, short events, void *ptr)
{
    PARCEventQueue *parcEventQueue = (PARCEventQueue *) ptr;
    int errno_forwarded = errno;
    parcEventQueue_LogDebug(parcEventQueue,
                            "_parc_queue_event_callback(bev=%p,events=%x,errno=%d,ptr->buffereventBuffer=%p,parcEventQueue=%p)\n",
                            bev, events, errno, parcEventQueue->buffereventBuffer, parcEventQueue);
    assertNotNull(parcEventQueue->eventCallback, "parcEvent event callback called when NULL");

    errno = errno_forwarded;
    parcEventQueue->eventCallback(parcEventQueue, internal_bufferevent_type_to_PARCEventQueueEventType(events), parcEventQueue->eventUserData);
}

void
parcEventQueue_SetCallbacks(PARCEventQueue *parcEventQueue,
                            PARCEventQueue_Callback *readCallback,
                            PARCEventQueue_Callback *writeCallback,
                            PARCEventQueue_EventCallback *eventCallback,
                            void *user_data)
{
    parcEventQueue_LogDebug(parcEventQueue,
                            "parcEventQueue_SetCallbacks(event=%p(buffer=%p),readcb=%p,writecb=%p,eventcb=%p,user_data=%p)\n",
                            parcEventQueue, parcEventQueue->buffereventBuffer,
                            readCallback, writeCallback, eventCallback,
                            user_data);

    parcEventQueue->readCallback = readCallback;
    parcEventQueue->readUserData = user_data;
    parcEventQueue->writeCallback = writeCallback;
    parcEventQueue->writeUserData = user_data;
    parcEventQueue->eventCallback = eventCallback;
    parcEventQueue->eventUserData = user_data;
    bufferevent_setcb(parcEventQueue->buffereventBuffer,
                      (readCallback) ? _parc_queue_read_callback : NULL,
                      (writeCallback) ? _parc_queue_write_callback : NULL,
                      (eventCallback) ? _parc_queue_event_callback : NULL,
                      parcEventQueue);
}

PARCEventQueue *
parcEventQueue_Create(PARCEventScheduler *eventScheduler, int fd, PARCEventQueueOption flags)
{
    assertNotNull(eventScheduler, "parcEventQueue_Create passed a NULL scheduler instance.");
    PARCEventQueue *parcEventQueue = parcMemory_AllocateAndClear(sizeof(PARCEventQueue));
    assertNotNull(parcEventQueue, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(PARCEventQueue));
    parcEventQueue->eventScheduler = eventScheduler;

    //
    // PARCEventQueue_CloseOnFree
    //     we close the underlying file descriptor/bufferevent/whatever
    //     when this bufferevent is freed.
    //
    // PARCEventQueue_DeferCallbacks
    //     callbacks are run deferred in the event loop.
    //
    parcEventQueue->buffereventBuffer = bufferevent_socket_new(parcEventScheduler_GetEvBase(eventScheduler), fd,
                                                               internal_PARCEventQueueOption_to_bufferevent_options(flags));
    assertNotNull(parcEventQueue->buffereventBuffer,
                  "Got null from bufferevent_socket_new for socket %d", fd);

    parcEventQueue_LogDebug(parcEventQueue,
                            "parcEventQueue_Create(eventScheduler=%p,libevent_base=%p) = %p\n",
                            eventScheduler,
                            parcEventScheduler_GetEvBase(eventScheduler),
                            parcEventQueue);

    return parcEventQueue;
}

void
parcEventQueue_Destroy(PARCEventQueue **parcEventQueue)
{
    parcEventQueue_LogDebug((*parcEventQueue), "parcEventQueue_Destroy(ptr=%p)\n", *parcEventQueue);
    assertNotNull((*parcEventQueue)->buffereventBuffer, "parcEventQueue_Destroy passed a null buffer!");

    bufferevent_free((*parcEventQueue)->buffereventBuffer);
    parcMemory_Deallocate((void *) parcEventQueue);
}

int
parcEventQueue_SetFileDescriptor(PARCEventQueue *parcEventQueue, int fd)
{
    return bufferevent_setfd(parcEventQueue->buffereventBuffer, fd);
}

int
parcEventQueue_GetFileDescriptor(PARCEventQueue *parcEventQueue)
{
    return bufferevent_getfd(parcEventQueue->buffereventBuffer);
}

PARCEventType
parcEventQueue_GetEnabled(PARCEventQueue *event)
{
    return internal_libevent_type_to_PARCEventType(bufferevent_get_enabled(event->buffereventBuffer));
}

void
parcEventQueue_Enable(PARCEventQueue *parcEventQueue, PARCEventType types)
{
    bufferevent_enable(parcEventQueue->buffereventBuffer, internal_PARCEventType_to_libevent_type(types));
}

void
parcEventQueue_Disable(PARCEventQueue *parcEventQueue, PARCEventType types)
{
    bufferevent_disable(parcEventQueue->buffereventBuffer, internal_PARCEventType_to_libevent_type(types));
}

int
parcEventQueue_ConnectSocket(PARCEventQueue *instance, struct sockaddr *address, int addrlen)
{
    return bufferevent_socket_connect(instance->buffereventBuffer, address, addrlen);
}

int
parcEventQueue_Flush(PARCEventQueue *parcEventQueue, PARCEventType types)
{
    return bufferevent_flush(parcEventQueue->buffereventBuffer, internal_PARCEventType_to_libevent_type(types), BEV_NORMAL);
}

int
parcEventQueue_Finished(PARCEventQueue *parcEventQueue, PARCEventType types)
{
    return bufferevent_flush(parcEventQueue->buffereventBuffer, internal_PARCEventType_to_libevent_type(types), BEV_FINISHED);
}

void
parcEventQueue_SetWatermark(PARCEventQueue *parcEventQueue, PARCEventType types, size_t low, size_t high)
{
    parcEventQueue_LogDebug(parcEventQueue, "parcEventQueue->buffereventBuffer=%p\n", parcEventQueue->buffereventBuffer);
    bufferevent_setwatermark(parcEventQueue->buffereventBuffer, internal_PARCEventType_to_libevent_type(types), low, high);
}

int
parcEventQueue_Printf(PARCEventQueue *parcEventQueue, const char *fmt, ...)
{
    struct evbuffer *buffer = bufferevent_get_output(parcEventQueue->buffereventBuffer);
    assertNotNull(buffer, "bufferevent_get_output returned NULL");

    va_list ap;

    va_start(ap, fmt);
    int result = evbuffer_add_vprintf(buffer, fmt, ap);
    va_end(ap);
    return result;
}

int
parcEventQueue_Read(PARCEventQueue *parcEventQueue, void *data, size_t dataLength)
{
    return bufferevent_read(parcEventQueue->buffereventBuffer, data, dataLength);
}

int
parcEventQueue_Write(PARCEventQueue *parcEventQueue, void *data, size_t dataLength)
{
    return bufferevent_write(parcEventQueue->buffereventBuffer, data, dataLength);
}

int
parcEventQueue_SetPriority(PARCEventQueue *eventQueue, PARCEventPriority priority)
{
    bufferevent_priority_set(eventQueue->buffereventBuffer, internal_PARCEventPriority_to_libevent_priority(priority));
    return 0;
}

PARCEventQueuePair *
parcEventQueue_CreateConnectedPair(PARCEventScheduler *eventScheduler)
{
    assertNotNull(eventScheduler, "parcEventQueue_CreateConnectedPair must be passed a valid Event Scheduler");
    PARCEventQueuePair *parcEventQueuePair = parcMemory_AllocateAndClear(sizeof(PARCEventQueuePair));
    assertNotNull(parcEventQueuePair, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(PARCEventQueuePair));

    parcEventQueuePair->up = parcMemory_AllocateAndClear(sizeof(PARCEventQueue));
    parcEventQueuePair->up->eventScheduler = eventScheduler;
    parcEventQueue_LogDebug(parcEventQueuePair->up,
                            "up instance parcEventQueue_Create(eventScheduler=%p,libevent_parcEventQueue=%p) = %p\n",
                            eventScheduler,
                            parcEventScheduler_GetEvBase(eventScheduler),
                            parcEventQueuePair->up);
    assertNotNull(parcEventQueuePair->up, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(PARCEventQueue));

    parcEventQueuePair->down = parcMemory_AllocateAndClear(sizeof(PARCEventQueue));
    parcEventQueuePair->down->eventScheduler = eventScheduler;
    parcEventQueue_LogDebug(parcEventQueuePair->down,
                            "down instance parcEventQueue_Create(eventScheduler=%p,libevent_parcEventQueue=%p) = %p\n",
                            eventScheduler,
                            parcEventScheduler_GetEvBase(eventScheduler),
                            parcEventQueuePair->down);
    assertNotNull(parcEventQueuePair->down, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(PARCEventQueue));

    struct bufferevent *evpair[2];
    int result = bufferevent_pair_new(parcEventScheduler_GetEvBase(eventScheduler), 0, evpair);
    if (result != 0) {
        parcMemory_Deallocate((void **) &(parcEventQueuePair->up));
        parcMemory_Deallocate((void **) &(parcEventQueuePair->down));
        parcMemory_Deallocate((void **) &parcEventQueuePair);
        return NULL;
    }

    parcEventQueuePair->up->buffereventBuffer = evpair[0];
    parcEventQueuePair->down->buffereventBuffer = evpair[1];

    (void) parcEventQueue_SetPriority(parcEventQueuePair->up, PARCEventPriority_Normal);
    (void) parcEventQueue_SetPriority(parcEventQueuePair->down, PARCEventPriority_Normal);

    return parcEventQueuePair;
}

void
parcEventQueue_DestroyConnectedPair(PARCEventQueuePair **queuePair)
{
    parcEventQueue_LogDebug((*queuePair)->up,
                            "parcEventQueue_DestroyPair(up ptr=%p)\n",
                            (*queuePair)->up);
    parcEventQueue_LogDebug((*queuePair)->down,
                            "parcEventQueue_DestroyPair(down ptr=%p)\n",
                            (*queuePair)->down);

    bufferevent_free((*queuePair)->up->buffereventBuffer);
    bufferevent_free((*queuePair)->down->buffereventBuffer);

    parcMemory_Deallocate((void **) &((*queuePair)->up));
    parcMemory_Deallocate((void **) &((*queuePair)->down));
    parcMemory_Deallocate((void **) queuePair);
}

PARCEventQueue *
parcEventQueue_GetConnectedUpQueue(PARCEventQueuePair *queuePair)
{
    return queuePair->up;
}

PARCEventQueue *
parcEventQueue_GetConnectedDownQueue(PARCEventQueuePair *queuePair)
{
    return queuePair->down;
}

struct evbuffer *
internal_parcEventQueue_GetEvInputBuffer(PARCEventQueue *queue)
{
    return bufferevent_get_input(queue->buffereventBuffer);
}

struct evbuffer *
internal_parcEventQueue_GetEvOutputBuffer(PARCEventQueue *queue)
{
    return bufferevent_get_output(queue->buffereventBuffer);
}

void
parcEventQueue_EnableDebug(void)
{
    _parc_event_queue_debug_enabled = 1;
}

void
parcEventQueue_DisableDebug(void)
{
    _parc_event_queue_debug_enabled = 0;
}
