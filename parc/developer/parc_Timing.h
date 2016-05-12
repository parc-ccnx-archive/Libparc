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
 * @file parc_Timing.h
 * @brief Macros for timing code
 *
 * These macros allow the developer to measure time spent in sections of code.
 *    On Intel platforms (i386 or x86_64), the timing is done with the TSC counter, so it will be measured in CPU cycles.
 *    On non-Intel Linux platforms, it will be be done with the nano-second oscillator clock (CLOCK_MONOTONIC_RAW).
 *    On Darwin, it will use the nano-second SYSTEM_CLOCK.
 *    Otherwise, uses gettimeofday(), which will be micro-second timing.
 *
 * This set of headers will define several macros for timing:
 *    parcTiming_Init(prefix)
 *    parcTiming_Fini(prefix)
 *    parcTiming_Start(prefix)
 *    parcTiming_Stop(prefix)
 *    (uint64_t) parcTiming_Delta(prefix)
 *
 * The units returned from parcTiming_Delta() will be consistent, but not necessarily
 * related to wall clock time, real time, or any discernable time unit.  For example, they
 * may be in CPU instruction cycles or raw oscillator ticks or nano-seconds.
 *
 * These macros only work if the user defines PARCTIMING_ENABLE.  Otherwise, they do not
 * generate any instructions and parcTiming_Delta will always return 0.
 *
 * @code
 * #define PARCTIMING_ENABLE 1
 * #include <parc/developer/parc_Timing.h>
 *
 * void
 * foo(void)
 * {
 *    parcTiming_Init(_foo);
 *    // ... other stuff ..
 *    parcTiming_Start(_foo);
 *    // ... stuff to measure ...
 *    parcTiming_Stop(_foo);
 *
 *    uint64_t delta = parcTiming_Delta(_foo);
 *    parcTiming_Fini(_foo)
 * }
 * @endcode
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_Timing_h
#define libparc_parc_Timing_h

#if defined(PARCTIMING_ENABLE)
// begin platform detection
#if defined(__i386__) || defined(__x86_64__)
#define PARCTIMING_INTEL
#include <parc/developer/parc_TimingIntel.h>
#elif defined(__APPLE__)
#define PARCTIMING_DARWIN
#include <parc/developer/parc_TimingDarwin.h>
#elif defined(__linux__)
#define PARCTIMING_LINUX
#include <parc/developer/parc_TimingLinux.h>
#else
#define PARCTIMING_GENERIC
#include <parc/developer/parc_TimingGeneric.h>
#endif // platform detection
#else // PARCTIMING_ENABLE
#define _private_parcTiming_Init(prefix)
#define _private_parcTiming_Start(prefix)
#define _private_parcTiming_Stop(prefix)
#define _private_parcTiming_Delta(prefix) ((uint64_t) 0)
#define _private_parcTiming_Fini(prefix)
#endif // PARCTIMING_ENABLE

/**
 * Initialize the timing facility for namespace prefix `prefix`
 *
 * Used inside a code block, this macro will define several variables prefixed with
 * the name `prefix`.  It may also allocate memory or system resources.  It must be used
 * within a function scope, not at a global scope.
 *
 * You must use parcTiming_Fini(prefix) when done with the timing facility.
 *
 * If `PARCTIMING_ENABLE` is not defined before including the header file, this function is a no-op.
 *
 * @param [in] prefix The namespace prefix used for variables in scope.
 *
 * Example:
 * @code
 * #define PARCTIMING_ENABLE 1
 * #include <parc/developer/parc_Timing.h>
 *
 * void
 * foo(void)
 * {
 *    parcTiming_Init(_foo);
 *    parcTiming_Start(_foo);
 *    // ... stuff to measure ...
 *    parcTiming_Stop(_foo);
 *
 *    uint64_t delta = parcTiming_Delta(_foo);
 *    parcTiming_Fini(_foo)
 * }
 * @endcode
 */
#define parcTiming_Init(prefix) _private_parcTiming_Init(prefix)

/**
 * Marks the time in the start variable
 *
 * Records the current time in the start variable for use by parcTiming_Delta().
 *
 * If PARCTIMING_ENABLE is not defined before including the header file, this function is a no-op.
 *
 * @param [in] prefix The namespace prefix used for variables in scope.
 *
 * Example:
 * @code
 * #define PARCTIMING_ENABLE 1
 * #include <parc/developer/parc_Timing.h>
 *
 * void
 * foo(void)
 * {
 *    parcTiming_Init(_foo);
 *    parcTiming_Start(_foo);
 *    // ... stuff to measure ...
 *    parcTiming_Stop(_foo);
 *
 *    uint64_t delta = parcTiming_Delta(_foo);
 *    parcTiming_Fini(_foo)
 * }
 * @endcode
 */
#define parcTiming_Start(prefix) _private_parcTiming_Start(prefix)

/**
 * Marks the time in the stop variable
 *
 * Records the current time in the stop variable for use by parcTiming_Delta().
 *
 * If PARCTIMING_ENABLE is not defined before including the header file, this function is a no-op.
 *
 * @param [in] prefix The namespace prefix used for variables in scope.
 *
 * Example:
 * @code
 * #define PARCTIMING_ENABLE 1
 * #include <parc/developer/parc_Timing.h>
 *
 * void
 * foo(void)
 * {
 *    parcTiming_Init(_foo);
 *    parcTiming_Start(_foo);
 *    // ... stuff to measure ...
 *    parcTiming_Stop(_foo);
 *
 *    uint64_t delta = parcTiming_Delta(_foo);
 *    parcTiming_Fini(_foo)
 * }
 * @endcode
 */
#define parcTiming_Stop(prefix) _private_parcTiming_Stop(prefix)

/**
 * Returns the number of ticks between calls to start and stop.
 *
 * The delta will be in whatever units the best clock and provide.  It may be CPU cycles
 * or oscillator ticks, or nanos, or in the worst case micros.
 *
 * If PARCTIMING_ENABLE is not defined before including the header file, this function is a no-op.
 *
 * @param [in] prefix The namespace prefix used for variables in scope.
 *
 * @return uint64_t The number of ticks between start and stop
 *
 * Example:
 * @code
 * #define PARCTIMING_ENABLE 1
 * #include <parc/developer/parc_Timing.h>
 *
 * void
 * foo(void)
 * {
 *    parcTiming_Init(_foo);
 *    parcTiming_Start(_foo);
 *    // ... stuff to measure ...
 *    parcTiming_Stop(_foo);
 *
 *    uint64_t delta = parcTiming_Delta(_foo);
 *    parcTiming_Fini(_foo)
 * }
 * @endcode
 */
#define parcTiming_Delta(prefix) _private_parcTiming_Delta(prefix)

/**
 * Finalized the timing, releasing any system resources or memory
 *
 * Must be called when done with each namespace initialized by parcTiming_Init().
 *
 * If PARCTIMING_ENABLE is not defined before including the header file, this function is a no-op.
 *
 * @param [in] prefix The namespace prefix used for variables in scope.
 *
 * Example:
 * @code
 * #define PARCTIMING_ENABLE 1
 * #include <parc/developer/parc_Timing.h>
 *
 * void
 * foo(void)
 * {
 *    parcTiming_Init(_foo);
 *    parcTiming_Start(_foo);
 *    // ... stuff to measure ...
 *    parcTiming_Stop(_foo);
 *
 *    uint64_t delta = parcTiming_Delta(_foo);
 *    parcTiming_Fini(_foo)
 * }
 * @endcode
 */
#define parcTiming_Fini(prefix) _private_parcTiming_Fini(prefix)
#endif // libparc_parc_Timing_h

