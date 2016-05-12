/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @file parc_AtomicInteger.h
 * @ingroup threading
 * @brief An integer value that may be updated automatically.
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_AtomicInteger_h
#define libparc_parc_AtomicInteger_h

#include <stdint.h>

/*
 * If we are compiling with GCC or Clang, then
 * we have some compiler extensions to make use of processer
 * compare-and-swap and other kinds of synchronisation primitives.
 */
#if defined(__GNUC__) || defined(__clang__)
# define USE_GCC_EXTENSIONS 1
#endif

// Turn this off until the problem on the SMP machines is worked out, case 787.
#undef USE_GCC_EXTENSIONS

#ifdef USE_GCC_EXTENSIONS
uint32_t parcAtomicInteger_Uint32IncrementGCC(uint32_t *value);

uint32_t parcAtomicInteger_Uint32DecrementGCC(uint32_t *value);

uint64_t parcAtomicInteger_Uint64IncrementGCC(uint64_t *value);

uint64_t parcAtomicInteger_Uint64DecrementGCC(uint64_t *value);

#define parcAtomicInteger_Uint32Increment parcAtomicInteger_Uint32IncrementGCC

#define parcAtomicInteger_Uint32Decrement parcAtomicInteger_Uint32DecrementGCC

#define parcAtomicInteger_Uint64Increment parcAtomicInteger_Uint64IncrementGCC

#define parcAtomicInteger_Uint64Decrement parcAtomicInteger_Uint64DecrementGCC

#else
uint32_t parcAtomicInteger_Uint32IncrementPthread(uint32_t *value);

uint32_t parcAtomicInteger_Uint32DecrementPthread(uint32_t *value);

uint64_t parcAtomicInteger_Uint64IncrementPthread(uint64_t *value);

uint64_t parcAtomicInteger_Uint64DecrementPthread(uint64_t *value);

#define parcAtomicInteger_Uint32Increment parcAtomicInteger_Uint32IncrementPthread

#define parcAtomicInteger_Uint32Decrement parcAtomicInteger_Uint32DecrementPthread

#define parcAtomicInteger_Uint64Increment parcAtomicInteger_Uint64IncrementPthread

#define parcAtomicInteger_Uint64Decrement parcAtomicInteger_Uint64DecrementPthread
#endif
#endif // libparc_parc_AtomicInteger_h
