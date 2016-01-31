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
 * @file parc_TimingGeneric.h
 * @brief Macros for timing code
 *
 * We cannot do any better than gettimeofday
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libparc_parc_TimingLinux_h
#define libparc_parc_TimingLinux_h

#include <stdint.h>
#include <time.h>
#include <sys/time.h>

#ifdef PARCTIMING_GENERIC

#define _private_parcTiming_Init(prefix) \
    struct timeval prefix ## _ts0, prefix ## _ts1, prefix ## _delta;

#define _private_parcTiming_Start(prefix) \
    gettimeofday(&(prefix ## _ts0), NULL);

#define _private_parcTiming_Stop(prefix) \
    gettimeofday(&(prefix ## _ts1), NULL);

static inline uint64_t
_parcTimingGeneric_Delta(const struct timeval *t0, const struct timeval *t1)
{
    struct timeval delta;
    timersub(t1, t0, &delta);
    return (uint64_t) delta.tv_sec * 1000000ULL + delta.tv_usec;
}

#define _private_parcTiming_Delta(prefix) _parcTimingGeneric_Delta(&(prefix ## _ts0), &(prefix ## _ts1))

// No teardown work that we need to do
#define _private_parcTiming_Fini(prefix)

#endif // PARCTIMING_GENERIC
#endif // libparc_parc_TimingLinux_h

