/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
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
