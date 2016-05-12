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

#include <stdio.h>
#include <unistd.h>

#include "internal_parc_Event.h"
#include <parc/algol/parc_EventScheduler.h>
#include <parc/algol/parc_FileOutputStream.h>
#include <parc/logging/parc_Log.h>
#include <parc/logging/parc_LogReporterFile.h>

/**
 * Current implementation based on top of libevent2
 */
#include <event2/event.h>

static int _parc_event_scheduler_debug_enabled = 0;

#define parcEventScheduler_LogDebug(parcEventScheduler, ...) \
    if (_parc_event_scheduler_debug_enabled) \
        parcLog_Debug(parcEventScheduler->log, __VA_ARGS__)

struct PARCEventScheduler {
    /**
     * Base of the libevent manager.
     */
    struct event_base *evbase;
    PARCLog *log;
};

static PARCLog *
_parc_logger_create(void)
{
    PARCFileOutputStream *fileOutput = parcFileOutputStream_Create(dup(STDOUT_FILENO));
    PARCOutputStream *output = parcFileOutputStream_AsOutputStream(fileOutput);
    parcFileOutputStream_Release(&fileOutput);

    PARCLogReporter *reporter = parcLogReporterFile_Create(output);
    parcOutputStream_Release(&output);

    PARCLog *log = parcLog_Create("localhost", "test_parc_Log", NULL, reporter);
    parcLogReporter_Release(&reporter);

    parcLog_SetLevel(log, PARCLogLevel_All);
    return log;
}

void *
parcEventScheduler_GetEvBase(PARCEventScheduler *parcEventScheduler)
{
    return (void *) parcEventScheduler->evbase;
}

PARCEventScheduler *
parcEventScheduler_Create(void)
{
    internal_parc_initializeLibevent();

    PARCEventScheduler *parcEventScheduler = parcMemory_Allocate(sizeof(PARCEventScheduler));
    assertNotNull(parcEventScheduler, "parcMemory_Allocate(%zu) returned NULL", sizeof(PARCEventScheduler));

    // Initialize libevent base pointer.
    parcEventScheduler->evbase = event_base_new();

    assertNotNull(parcEventScheduler->evbase, "Could not obtain an event base!");
    int result = event_base_priority_init(parcEventScheduler->evbase, PARCEventPriority_NumberOfPriorities);
    assertTrue(result == 0, "Could not set scheduler priorities (%d)", result);

    parcEventScheduler->log = _parc_logger_create();
    assertNotNull(parcEventScheduler->log, "Could not create parc logger");

    parcEventScheduler_LogDebug(parcEventScheduler, "parcEventScheduler_Create() = %p\n", parcEventScheduler);

    return parcEventScheduler;
}

int
parcEventScheduler_Start(PARCEventScheduler *parcEventScheduler, PARCEventSchedulerDispatchType type)
{
    parcEventScheduler_LogDebug(parcEventScheduler, "parcEventScheduler_Start(%p, %d)\n", parcEventScheduler, type);
    assertNotNull(parcEventScheduler, "parcEventScheduler_Start must be passed a valid base parcEventScheduler!");
    int result = event_base_loop(parcEventScheduler->evbase,
                                 internal_PARCEventSchedulerDispatchType_to_eventloop_options(type));
    return result;
}

int
parcEventScheduler_DispatchBlocking(PARCEventScheduler *parcEventScheduler)
{
    return parcEventScheduler_Start(parcEventScheduler, PARCEventSchedulerDispatchType_Blocking);
}

int
parcEventScheduler_DispatchNonBlocking(PARCEventScheduler *parcEventScheduler)
{
    return parcEventScheduler_Start(parcEventScheduler, PARCEventSchedulerDispatchType_NonBlocking);
}

int
parcEventScheduler_Stop(PARCEventScheduler *parcEventScheduler, struct timeval *delay)
{
    parcEventScheduler_LogDebug(parcEventScheduler, "parcEventScheduler_Stop(%p, %p)\n", parcEventScheduler, delay);
    assertNotNull(parcEventScheduler, "parcEventScheduler_Stop must be passed a valid base parcEventScheduler!");
    int result = event_base_loopexit(parcEventScheduler->evbase, delay);
    return result;
}

int
parcEventScheduler_Abort(PARCEventScheduler *parcEventScheduler)
{
    parcEventScheduler_LogDebug(parcEventScheduler, "parcEventScheduler_Abort(%p)\n", parcEventScheduler);
    assertNotNull(parcEventScheduler, "parcEventScheduler_Abort must be passed a valid base parcEventScheduler!");
    int result = event_base_loopbreak(parcEventScheduler->evbase);
    return result;
}

static int _event_enable_debug_mode_called = 0;

void
parcEventScheduler_EnableDebug(void)
{
    _parc_event_scheduler_debug_enabled = 1;
    if (_event_enable_debug_mode_called == 0) {
        event_enable_debug_mode();
        _event_enable_debug_mode_called = 1;
    }
}

void
parcEventScheduler_DisableDebug(void)
{
    _parc_event_scheduler_debug_enabled = 0;
}

void
parcEventScheduler_Destroy(PARCEventScheduler **parcEventScheduler)
{
    parcEventScheduler_LogDebug((*parcEventScheduler), "parcEventScheduler_Destroy(%p)\n", *parcEventScheduler);

    assertNotNull(*parcEventScheduler, "parcEventScheduler_Destroy must be passed a valid base parcEventScheduler!");
    assertNotNull((*parcEventScheduler)->evbase, "parcEventScheduler_Destroy passed a NULL event base member!");

    event_base_free((*parcEventScheduler)->evbase);
    parcLog_Release(&((*parcEventScheduler)->log));
    parcMemory_Deallocate((void **) parcEventScheduler);
}

PARCLog *
parcEventScheduler_GetLogger(PARCEventScheduler *parcEventScheduler)
{
    return parcEventScheduler->log;
}
