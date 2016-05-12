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
 * @file parc_TimingIntel.h
 * @brief Macros for timing code
 *
 *  This code uses the Intel recommended benchmarking techniques described
 *  in the whitepaper "How to Benchmakr Code Execution Times on Intel (R) IA-32 and
 *  IA-64 Instruction Set Architectures" available at:
 *
 *  http://www.intel.com/content/dam/www/public/us/en/documents/white-papers/ia-32-ia-64-benchmark-code-execution-paper.pdf
 *
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_TimingIntel_h
#define libparc_parc_TimingIntel_h

#ifdef PARCTIMING_INTEL
#include <stdint.h>
#include <stdbool.h>

/**
 * Reads the TSC via the best available CPU instruction
 *
 * Will execute a RDTSC or RDTSCP instruction followed by an instruction pipeline block
 * CPUID instruction.
 *
 * @param [out] hi The high-order 32-bits
 * @param [out] lo The low-order 32-bits
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
void parcTimingIntel_rdtsc(unsigned *hi, unsigned *lo);

/**
 * Checks initialization for RDTSCP instruction availability
 *
 * Checks if RDTSCP is availalbe on the system and sets a global.
 *
 * Example:
 * @code
 * {
 *     parcTiminIntel_RuntimeInit();
 * }
 * @endcode
 */
void parcTiminIntel_RuntimeInit(void);

#define _private_parcTiming_Init(prefix) \
    parcTiminIntel_RuntimeInit(); \
    static unsigned prefix ## _cycles_low0, prefix ## _cycles_high0; \
    static unsigned prefix ## _cycles_low1, prefix ## _cycles_high1; \
   \
    __asm volatile ("CPUID\n\t" "RDTSC\n\t" \
                    "mov %%edx, %0\n\t" \
                    "mov %%eax, %1\n\t" : "=r" (prefix ## _cycles_high0), "=r" (prefix ## _cycles_low0):: \
                    "%rax", "%rbx", "%rcx", "%rdx"); \
    parcTimingIntel_rdtsc(&(prefix ## _cycles_high1), &(prefix ## _cycles_low1)); \
    __asm volatile ("CPUID\n\t" \
                    "RDTSC\n\t" \
                    "mov %%edx, %0\n\t" \
                    "mov %%eax, %1\n\t" : "=r" (prefix ## _cycles_high0), "=r" (prefix ## _cycles_low0):: \
                    "%rax", "%rbx", "%rcx", "%rdx"); \
    parcTimingIntel_rdtsc(&(prefix ## _cycles_high1), &(prefix ## _cycles_low1));

#define _private_parcTiming_Start(prefix) \
    parcTimingIntel_rdtsc(&(prefix ## _cycles_high0), &(prefix ## _cycles_low0));

#define _private_parcTiming_Stop(prefix) \
    parcTimingIntel_rdtsc(&(prefix ## _cycles_high1), &(prefix ## _cycles_low1));

#define _private_parcTiming_CalculateStartTime(prefix) (((uint64_t) prefix ## _cycles_high0 << 32) | prefix ## _cycles_low0)
#define _private_parcTiming_CalculateStopTime(prefix) (((uint64_t) prefix ## _cycles_high1 << 32) | prefix ## _cycles_low1)

#define _private_parcTiming_Delta(prefix) \
    (_private_parcTiming_CalculateStopTime(prefix) - _private_parcTiming_CalculateStartTime(prefix))

// No teardown work that we need to do
#define _private_parcTiming_Fini(prefix)

#endif // PARCTIMING_INTEL
#endif // libparc_parc_TimingIntel_h

