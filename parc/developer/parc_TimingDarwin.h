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
 * @file parc_TimingDarwin.h
 * @brief Macros for timing code
 *
 * On linux will use clock_gettime with the MONOTONIC RAW clock, which does not speed up or slow down based on adj_time().
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_TimingDarwin_h
#define libparc_parc_TimingDarwin_h

#ifdef PARCTIMING_DARWIN
#include <stdint.h>
#include <time.h>
#include <mach/mach.h>
#include <mach/clock.h>
#include <mach/mach_time.h>

/*
 * This allocates the clock service which must be released in the Fini macro
 */
#define _private_parcTiming_Init(prefix) \
    clock_serv_t prefix ## _clockService; \
    mach_timespec_t prefix ## _ts0, prefix ## _ts1; \
    host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &(prefix ## _clockService));

#define _private_parcTiming_Start(prefix) \
    clock_get_time((prefix ## _clockService), &(prefix ## _ts0));

#define _private_parcTiming_Stop(prefix) \
    clock_get_time((prefix ## _clockService), &(prefix ## _ts1));

static inline uint64_t
_parcTiming_Delta(const mach_timespec_t *t0, const mach_timespec_t *t1)
{
    // SUB_MACH_TIMESPEC(t1, t2) => t1 -= t2

    mach_timespec_t delta;
    memcpy(&delta, t1, sizeof(delta));
    SUB_MACH_TIMESPEC(&delta, t0);

    return (uint64_t) delta.tv_sec * 1000000000ULL + delta.tv_nsec;
}

#define _private_parcTiming_Delta(prefix) _parcTiming_Delta(&(prefix ## _ts0), &(prefix ## _ts1))

#define _private_parcTiming_Fini(prefix) \
    mach_port_deallocate(mach_task_self(), &(prefix ## _clockService));

#endif // PARCTIMING_DARWIN
#endif // libparc_parc_TimingDarwin_h

