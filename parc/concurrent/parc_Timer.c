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
 * @author <#gscott#>, Computing Science Laboratory, PARC
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_Memory.h>

#include "parc_Timer.h"

struct PARCTimer {
    int delay;
};

static void
_parcTimer_Finalize(PARCTimer **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCTimer pointer.");


    /* cleanup the instance fields here */
}

parcObject_ImplementAcquire(parcTimer, PARCTimer);

parcObject_ImplementRelease(parcTimer, PARCTimer);

parcObject_ExtendPARCObject(PARCTimer, _parcTimer_Finalize, parcTimer_Copy, parcTimer_ToString, parcTimer_Equals, parcTimer_Compare, parcTimer_HashCode, parcTimer_ToJSON);


void
parcTimer_AssertValid(const PARCTimer *instance)
{
    assertTrue(parcTimer_IsValid(instance),
               "PARCTimer is not valid.");
}

PARCTimer *
parcTimer_Create(void)
{
    PARCTimer *result = parcObject_CreateInstance(PARCTimer);

    if (result != NULL) {
    }

    return result;
}

int
parcTimer_Compare(const PARCTimer *instance, const PARCTimer *other)
{
    int result = 0;

    return result;
}

PARCTimer *
parcTimer_Copy(const PARCTimer *original)
{
    PARCTimer *result = parcTimer_Create();

    return result;
}

void
parcTimer_Display(const PARCTimer *instance, int indentation)
{
    parcDisplayIndented_PrintLine(indentation, "PARCTimer@%p {", instance);
    /* Call Display() functions for the fields here. */
    parcDisplayIndented_PrintLine(indentation, "}");
}

bool
parcTimer_Equals(const PARCTimer *x, const PARCTimer *y)
{
    bool result = false;

    if (x == y) {
        result = true;
    } else if (x == NULL || y == NULL) {
        result = false;
    } else {
        return true;
    }

    return result;
}

PARCHashCode
parcTimer_HashCode(const PARCTimer *instance)
{
    PARCHashCode result = 0;

    return result;
}

bool
parcTimer_IsValid(const PARCTimer *instance)
{
    bool result = false;

    if (instance != NULL) {
        result = true;
    }

    return result;
}

PARCJSON *
parcTimer_ToJSON(const PARCTimer *instance)
{
    PARCJSON *result = parcJSON_Create();

    if (result != NULL) {
    }

    return result;
}

char *
parcTimer_ToString(const PARCTimer *instance)
{
    char *result = parcMemory_Format("PARCTimer@%p\n", instance);

    return result;
}

void
parcTimer_Cancel(PARCTimer *timer)
{
}

int
parcTimer_Purge(PARCTimer *timer)
{
    return 0;
}

void
parcTimer_ScheduleAtTime(PARCTimer *timer, PARCFutureTask *task, time_t absoluteTime)
{
}

void
parcTimer_ScheduleAtTimeAndRepeat(PARCTimer *timer, PARCFutureTask *task, time_t firstTime, long period)
{
}

void
parcTimer_ScheduleAfterDelay(PARCTimer *timer, PARCFutureTask *task, long delay)
{
}

void
parcTimer_ScheduleAfterDelayAndRepeat(PARCTimer *timer, PARCFutureTask *task, long delay, long period)
{
}

