/*
 * Copyright (c) 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Patent rights are not granted under this agreement. Patent rights are
 *       available under FRAND terms.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEROX or PARC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * @file parc_TimingLinux.h
 * @brief Macros for timing code
 *
 * On linux will use clock_gettime with the MONOTONIC RAW clock, which does not speed up or slow down based on adj_time().
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libparc_parc_TimingLinux_h
#define libparc_parc_TimingLinux_h

#ifdef PARCTIMING_LINUX
#include <stdint.h>
#include <time.h>
#include <sys/time.h>

#define _private_parcTiming_Init(prefix) \
    struct timespec prefix ## _ts0, prefix ## _ts1;

#define _private_parcTiming_Start(prefix) \
    clock_gettime(CLOCK_MONOTONIC_RAW, &(prefix ## _ts0));

#define _private_parcTiming_Stop(prefix) \
    clock_gettime(CLOCK_MONOTONIC_RAW, &(prefix ## _ts1));

static inline uint64_t
_parcTimingLinux_Delta(const struct timespec *t0, const struct timespec *t1)
{
    struct timespec delta;

    delta.tv_sec = t1->tv_sec - t0->tv_sec;
    delta.tv_nsec = t1->tv_nsec - t0->tv_nsec;
    if (delta.tv_nsec < 0) {
        --delta.tv_sec;
        delta.tv_nsec += 1000000000;
    }

    return (uint64_t) delta.tv_sec * 1000000000ULL + delta.tv_nsec;
}

#define _private_parcTiming_Delta(prefix) _parcTimingLinux_Delta(&(prefix ## _ts0), &(prefix ## _ts1))

// No teardown work that we need to do
#define _private_parcTiming_Fini(prefix)

#endif // PARCTIMING_LINUX
#endif // libparc_parc_TimingLinux_h

