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
#include <parc/algol/parc_EventScheduler.h>
#include <parc/algol/parc_EventSignal.h>
#include <parc/algol/parc_FileOutputStream.h>
#include <parc/logging/parc_Log.h>
#include <parc/logging/parc_LogReporterFile.h>

static int _parc_event_signal_debug_enabled = 0;

#define parcEventSignal_LogDebug(parcEventSignal, ...) \
    if (_parc_event_signal_debug_enabled) \
        parcLog_Debug(parcEventScheduler_GetLogger(parcEventSignal->eventScheduler), __VA_ARGS__)

/**
 * Current implementation based on top of libevent2
 */
#include <event2/event.h>
#include <event2/util.h>

struct PARCEventSignal {
    /**
     * The event instance.
     */
    struct event *event;

    // Event scheduler we have been queued with
    PARCEventScheduler *eventScheduler;

    PARCEventSignal_Callback *callback;
    void *callbackUserData;
};

static void
_parc_event_signal_callback(evutil_socket_t fd, short flags, void *context)
{
    PARCEventSignal *parcEventSignal = (PARCEventSignal *) context;
    parcEventSignal_LogDebug(parcEventSignal,
                             "_parc_event_signal_callback(fd=%x,flags=%x,parcEventSignal=%p)\n",
                             fd, flags, parcEventSignal);
    parcEventSignal->callback((int) fd, internal_libevent_type_to_PARCEventType(flags),
                              parcEventSignal->callbackUserData);
}

PARCEventSignal *
parcEventSignal_Create(PARCEventScheduler *eventScheduler, int signal, PARCEventType flags, PARCEvent_Callback *callback, void *callbackArgs)
{
    PARCEventSignal *parcEventSignal = parcMemory_Allocate(sizeof(PARCEventSignal));
    assertNotNull(parcEventSignal, "parcMemory_Allocate(%zu) returned NULL", sizeof(PARCEventSignal));

    parcEventSignal->eventScheduler = eventScheduler;
    parcEventSignal->callback = callback;
    parcEventSignal->callbackUserData = callbackArgs;

    parcEventSignal->event = event_new(parcEventScheduler_GetEvBase(eventScheduler), signal,
                                       internal_PARCEventType_to_libevent_type(flags),
                                       _parc_event_signal_callback, parcEventSignal);
    assertNotNull(parcEventSignal->event, "Could not create a new event!");

    parcEventSignal_LogDebug(parcEventSignal,
                             "parcEventSignal_Create(base=%p,signal=%x,flags=%x,cb=%p,args=%p) = %p\n",
                             parcEventScheduler_GetEvBase(eventScheduler), signal, flags,
                             callback, callbackArgs, parcEventSignal);
    return parcEventSignal;
}

int
parcEventSignal_Start(PARCEventSignal *parcEventSignal)
{
    parcEventSignal_LogDebug(parcEventSignal, "parcEventSignal_Start(event=%p)\n", parcEventSignal);
    assertNotNull(parcEventSignal, "parcEventStart_Signal must be passed a valid event!");

    int result = event_add(parcEventSignal->event, NULL);
    return result;
}

int
parcEventSignal_Stop(PARCEventSignal *parcEventSignal)
{
    parcEventSignal_LogDebug(parcEventSignal, "parcEventSignal_Stop(event=%p)\n", parcEventSignal);
    assertNotNull(parcEventSignal, "parcEvent_Stop must be passed a valid event!");

    int result = event_del(parcEventSignal->event);
    return result;
}

void
parcEventSignal_Destroy(PARCEventSignal **parcEventSignal)
{
    parcEventSignal_LogDebug((*parcEventSignal), "parcEventSignal_Destroy(event=%p)\n", parcEventSignal);
    assertNotNull(*parcEventSignal, "parcEvent_Destroy must be passed a valid parcEventSignal!");
    assertNotNull((*parcEventSignal)->event, "parcEvent_Destroy passed a null event!");
    event_free((*parcEventSignal)->event);
    parcMemory_Deallocate((void **) parcEventSignal);
}

void
parcEventSignal_EnableDebug(void)
{
    _parc_event_signal_debug_enabled = 1;
}

void
parcEventSignal_DisableDebug(void)
{
    _parc_event_signal_debug_enabled = 0;
}
