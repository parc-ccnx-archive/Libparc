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
 * Executes either the RDTSC or RDTSCP instruction, depending on platform availability
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */

#include <config.h>
#include <stdio.h>

#define PARCTIMING_ENABLE
#include <parc/developer/parc_Timing.h>

#ifdef PARCTIMING_INTEL
static bool _useRdtscp = false;
static bool _needCheckRdtscp = true;

#include <cpuid.h>

static void
_checkRdtscp(void)
{
    // See the CPUID instruction for description of the codes.

    // determine the maximum extended information set
    unsigned maxextended = __get_cpuid_max(0x80000000, NULL);

    // RDTSCP status flag is in the 0x800000001 feature set
    const unsigned feature = 0x80000001;
    const unsigned rdtscp_feature = 1 << 27;

    if (maxextended >= feature) {
        unsigned eax, ebx, ecx, edx;

        int success = __get_cpuid(feature, &eax, &ebx, &ecx, &edx);
        if (success) {
            _useRdtscp = (edx & rdtscp_feature ? true : false);
        }
    }
}
#endif // PARCTIMING_INTEL

void
parcTiminIntel_RuntimeInit(void)
{
#ifdef PARCTIMING_INTEL
    if (_needCheckRdtscp) {
        _needCheckRdtscp = false;
        _checkRdtscp();
    }
#endif // PARCTIMING_INTEL
}

void
parcTimingIntel_rdtsc(unsigned *hi, unsigned *lo)
{
    /*
     * Older CPUs do not support RDTSCP, which is the better instruction to use.
     * If we did not detect this opcode in autoconf, use the older RDTSC
     */

#ifdef PARCTIMING_INTEL
    if (_useRdtscp) {
        __asm volatile ("RDTSCP\n\t"
                        "mov %%edx,%0\n\t"
                        "mov %%eax,%1\n\t"
                        "CPUID\n\t" : "=r" (*hi), "=r" (*lo):: "%rax", "%rbx", "%rcx", "%rdx");
    } else {
        __asm volatile ("RDTSC\n\t"
                        "mov %%edx,%0\n\t"
                        "mov %%eax,%1\n\t"
                        "CPUID\n\t" : "=r" (*hi), "=r" (*lo):: "%rax", "%rbx", "%rcx", "%rdx");
    }
#endif  // PARCTIMING_INTEL
}

