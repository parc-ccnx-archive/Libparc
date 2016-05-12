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
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>
#include <pthread.h>

#include <parc/algol/parc_AtomicInteger.h>

#ifdef USE_GCC_EXTENSIONS
uint32_t
parcAtomicInteger_Uint32IncrementGCC(uint32_t *value)
{
    return __sync_add_and_fetch(value, 1);
}

uint32_t
parcAtomicInteger_Uint32DecrementGCC(uint32_t *value)
{
    return __sync_sub_and_fetch(value, 1);
}

uint64_t
parcAtomicInteger_Uint64IncrementGCC(uint64_t *value)
{
    return __sync_add_and_fetch(value, 1);
}

uint64_t
parcAtomicInteger_Uint64DecrementGCC(uint64_t *value)
{
    return __sync_sub_and_fetch(value, 1);
}
#endif


// Since there is no per integer mutex, we must use something to protect an integer from multiple,
// simultaneous competitors.
// Unfortunately, this is an indiscrimiate mutex causing all atomic integer updates to be
// serialized rather than each individually protected.
static pthread_mutex_t _parcAtomicInteger_PthreadMutex = PTHREAD_MUTEX_INITIALIZER;

uint32_t
parcAtomicInteger_Uint32IncrementPthread(uint32_t *value)
{
    pthread_mutex_lock(&_parcAtomicInteger_PthreadMutex);
    *value = *value + 1;
    uint32_t result = *value;
    pthread_mutex_unlock(&_parcAtomicInteger_PthreadMutex);
    return result;
}

uint32_t
parcAtomicInteger_Uint32DecrementPthread(uint32_t *value)
{
    pthread_mutex_lock(&_parcAtomicInteger_PthreadMutex);
    *value = *value - 1;
    uint32_t result = *value;
    pthread_mutex_unlock(&_parcAtomicInteger_PthreadMutex);
    return result;
}

uint64_t
parcAtomicInteger_Uint64IncrementPthread(uint64_t *value)
{
    pthread_mutex_lock(&_parcAtomicInteger_PthreadMutex);
    *value = *value + 1;
    uint64_t result = *value;
    pthread_mutex_unlock(&_parcAtomicInteger_PthreadMutex);
    return result;
}

uint64_t
parcAtomicInteger_Uint64DecrementPthread(uint64_t *value)
{
    pthread_mutex_lock(&_parcAtomicInteger_PthreadMutex);
    *value = *value - 1;
    uint64_t result = *value;
    pthread_mutex_unlock(&_parcAtomicInteger_PthreadMutex);
    return result;
}
