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
#include <parc/algol/parc_EventTimer.h>

static int _parc_event_timer_debug_enabled = 0;

#define parcEventTimer_LogDebug(parcEventTimer, ...) \
    if (_parc_event_timer_debug_enabled) \
        parcLog_Debug(parcEventScheduler_GetLogger(parcEventTimer->eventScheduler), __VA_ARGS__)

/**
 * Current implementation based on top of libevent2
 */
#include <event2/event.h>
#include <event2/util.h>

struct PARCEventTimer {
    /**
     * The event instance.
     */
    struct event *event;

    // Event scheduler we have been queued with
    PARCEventScheduler *eventScheduler;

    PARCEventTimer_Callback *callback;
    void *callbackUserData;
};

static void
_parc_event_timer_callback(evutil_socket_t fd, short flags, void *context)
{
    PARCEventTimer *parcEventTimer = (PARCEventTimer *) context;
    parcEventTimer_LogDebug(parcEventTimer,
                            "_parc_event_timer_callback(fd=%x,flags=%x,parcEventTimer=%p)\n",
                            fd, flags, parcEventTimer);
    parcEventTimer->callback((int) fd, internal_libevent_type_to_PARCEventType(flags),
                             parcEventTimer->callbackUserData);
}

PARCEventTimer *
parcEventTimer_Create(PARCEventScheduler *eventScheduler, PARCEventType flags, PARCEvent_Callback *callback, void *callbackArgs)
{
    PARCEventTimer *parcEventTimer = parcMemory_Allocate(sizeof(PARCEventTimer));
    assertNotNull(parcEventTimer, "parcMemory_Allocate(%zu) returned NULL", sizeof(PARCEventTimer));

    parcEventTimer->eventScheduler = eventScheduler;
    parcEventTimer->callback = callback;
    parcEventTimer->callbackUserData = callbackArgs;

    // NB: the EV_TIMEOUT flag is ignored when constructing an event
    parcEventTimer->event = event_new(parcEventScheduler_GetEvBase(eventScheduler), -1,
                                      internal_PARCEventType_to_libevent_type(flags),
                                      _parc_event_timer_callback, parcEventTimer);
    assertNotNull(parcEventTimer->event, "Could not create a new event!");

    parcEventTimer_LogDebug(parcEventTimer,
                            "parcEventTimer_Create(base=%p,events=%x,cb=%p,args=%p) = %p\n",
                            parcEventScheduler_GetEvBase(eventScheduler), flags,
                            callback, callbackArgs, parcEventTimer);

    return parcEventTimer;
}

int
parcEventTimer_Start(PARCEventTimer *parcEventTimer, struct timeval *timeout)
{
    parcEventTimer_LogDebug(parcEventTimer,
                            "parcEventTimer_Start(event=%p, timeout=%d:%d)\n",
                            parcEventTimer, timeout->tv_sec, timeout->tv_usec);
    assertNotNull(parcEventTimer, "parcEventTimer_Start must be passed a valid event!");

    int result = event_add(parcEventTimer->event, timeout);
    return result;
}

int
parcEventTimer_Stop(PARCEventTimer *parcEventTimer)
{
    parcEventTimer_LogDebug(parcEventTimer, "parcEventTimer_Stop(event=%p)\n", parcEventTimer);
    assertNotNull(parcEventTimer, "parcEventTimer_Stop must be passed a valid event!");

    int result = event_del(parcEventTimer->event);
    return result;
}

void
parcEventTimer_Destroy(PARCEventTimer **parcEventTimer)
{
    parcEventTimer_LogDebug((*parcEventTimer), "parcEventTimer_Destroy(parcEventTimer=%p)\n", *parcEventTimer);
    assertNotNull(*parcEventTimer, "parcEventTimer_Destroy must be passed a valid parcEventTimer!");
    assertNotNull((*parcEventTimer)->event, "parcEventTimer_Destroy passed a null event!");

    event_free((*parcEventTimer)->event);
    parcMemory_Deallocate((void **) parcEventTimer);
}

void
parcEventTimer_EnableDebug(void)
{
    _parc_event_timer_debug_enabled = 1;
}

void
parcEventTimer_DisableDebug(void)
{
    _parc_event_timer_debug_enabled = 0;
}
