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
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */

#include <config.h>
#include <time.h>
#include <parc/algol/parc_Clock.h>

#if __APPLE__
#include <mach/mach.h>
#include <mach/clock.h>
#include <mach/mach_time.h>
#endif

// These are used by the counter Clock
#include <parc/algol/parc_AtomicInteger.h>
#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_Memory.h>
// ----

typedef struct counter_clock {
    uint64_t counter;
} _CounterClock;

parcObject_ExtendPARCObject(_CounterClock, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
static parcObject_ImplementAcquire(_counterClock, _CounterClock);

static PARCClock *
_counterClock_AcquireInterface(const PARCClock *clock)
{
    _CounterClock *cc = (_CounterClock *) clock->closure;
    _counterClock_Acquire(cc);
    return (PARCClock *) clock;
}

static void
_counterClock_ReleaseInterface(PARCClock **clockPtr)
{
    PARCClock *clock = *clockPtr;
    _CounterClock *cc = (_CounterClock *) clock->closure;

    PARCReferenceCount refcount = parcObject_Release((void **) &cc);
    if (refcount == 0) {
        parcMemory_Deallocate((void **) clockPtr);
    } else {
        *clockPtr = NULL;
    }
}

static void
_counterClock_GetTimeval(const PARCClock *clock, struct timeval *output)
{
    _CounterClock *cc = (_CounterClock *) clock->closure;
    uint64_t value = parcAtomicInteger_Uint64Increment(&cc->counter);

    // put 19 bits in the micro-seconds so it is never larger than 1E+6
    output->tv_sec = value >> 19;
    output->tv_usec = value & 0x7FFFF;
}

static uint64_t
_counterClock_GetTime(const PARCClock *clock)
{
    _CounterClock *cc = (_CounterClock *) clock->closure;
    return parcAtomicInteger_Uint64Increment(&cc->counter);
}

PARCClock *
parcClock_Counter(void)
{
    _CounterClock *cc = parcObject_CreateInstance(_CounterClock);
    cc->counter = 0;

    PARCClock *clock = parcMemory_Allocate(sizeof(PARCClock));
    clock->closure = cc;
    clock->acquire = _counterClock_AcquireInterface;
    clock->release = _counterClock_ReleaseInterface;
    clock->getTime = _counterClock_GetTime;
    clock->getTimeval = _counterClock_GetTimeval;
    return clock;
}

// ===========
// Wallclock

static void
_wallclock_GetTimeval(const PARCClock *dummy __attribute__((unused)), struct timeval *output)
{
#if __linux__
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME_COARSE, &ts);
    output->tv_sec = ts.tv_sec;
    output->tv_usec = ts.tv_nsec / 1000;
#else
    clock_serv_t clockService;
    mach_timespec_t mts;

    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &clockService);
    clock_get_time(clockService, &mts);
    mach_port_deallocate(mach_task_self(), clockService);

    output->tv_sec = mts.tv_sec;
    output->tv_usec = mts.tv_nsec / 1000;
#endif
}

static uint64_t
_wallclock_GetTime(const PARCClock *clock)
{
    struct timeval tv;
    _wallclock_GetTimeval(clock, &tv);
    uint64_t t = tv.tv_sec * 1000 + tv.tv_usec / 1000;

    return t;
}


static PARCClock *
_wallclock_Acquire(const PARCClock *clock)
{
    return (PARCClock *) clock;
}

static void
_wallclock_Release(PARCClock **clockPtr)
{
    *clockPtr = NULL;
}

static PARCClock _wallclock = {
    .closure    = NULL,
    .getTime    = _wallclock_GetTime,
    .getTimeval = _wallclock_GetTimeval,
    .acquire    = _wallclock_Acquire,
    .release    = _wallclock_Release
};

PARCClock *
parcClock_Wallclock(void)
{
    return &_wallclock;
}


// ==========================
// Monotonic clock

static void
_monoclock_GetTimeval(const PARCClock *dummy __attribute__((unused)), struct timeval *output)
{
#if __linux__
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    output->tv_sec = ts.tv_sec;
    output->tv_usec = ts.tv_nsec / 1000;
#else
    clock_serv_t clockService;
    mach_timespec_t mts;

    host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &clockService);
    clock_get_time(clockService, &mts);
    mach_port_deallocate(mach_task_self(), clockService);

    output->tv_sec = mts.tv_sec;
    output->tv_usec = mts.tv_nsec / 1000;
#endif
}

static uint64_t
_monoclock_GetTime(const PARCClock *clock)
{
    struct timeval tv;
    _monoclock_GetTimeval(clock, &tv);
    uint64_t t = tv.tv_sec * 1000 + tv.tv_usec / 1000;

    return t;
}

static PARCClock *
_monoclock_Acquire(const PARCClock *clock)
{
    return (PARCClock *) clock;
}

static void
_monoclock_Release(PARCClock **clockPtr)
{
    *clockPtr = NULL;
}

static PARCClock _monoclock = {
    .closure    = NULL,
    .getTime    = _monoclock_GetTime,
    .getTimeval = _monoclock_GetTimeval,
    .acquire    = _monoclock_Acquire,
    .release    = _monoclock_Release
};

PARCClock *
parcClock_Monotonic(void)
{
    return &_monoclock;
}

// ===========================
// Facade API

uint64_t
parcClock_GetTime(const PARCClock *clock)
{
    return clock->getTime(clock);
}

void
parcClock_GetTimeval(const PARCClock *clock, struct timeval *output)
{
    clock->getTimeval(clock, output);
}

PARCClock *
parcClock_Acquire(const PARCClock *clock)
{
    return clock->acquire(clock);
}

void
parcClock_Release(PARCClock **clockPtr)
{
    (*clockPtr)->release(clockPtr);
}

