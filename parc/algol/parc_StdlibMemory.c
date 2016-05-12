/*
 * Copyright (c) 2013,2014,2015,2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @copyright (c) 2013,2014,2015,2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <pthread.h>

#include <LongBow/runtime.h>

#include <parc/algol/parc_StdlibMemory.h>

static uint32_t _parcStdlibMemory_OutstandingAllocations;

#ifdef PARCLibrary_DISABLE_ATOMICS
static pthread_mutex_t _parcStdlibMemory_Mutex = PTHREAD_MUTEX_INITIALIZER;

static inline void
_parcStdlibMemory_IncrementOutstandingAllocations(void)
{
    pthread_mutex_lock(&_parcStdlibMemory_Mutex);
    _parcStdlibMemory_OutstandingAllocations++;
    pthread_mutex_unlock(&_parcStdlibMemory_Mutex);
}

static inline void
_parcStdlibMemory_DecrementOutstandingAllocations(void)
{
    pthread_mutex_lock(&_parcStdlibMemory_Mutex);
    _parcStdlibMemory_OutstandingAllocations--;
    pthread_mutex_unlock(&_parcStdlibMemory_Mutex);
}
#else

static inline void
_parcStdlibMemory_IncrementOutstandingAllocations(void)
{
    __sync_add_and_fetch(&_parcStdlibMemory_OutstandingAllocations, 1);
}

static inline void
_parcStdlibMemory_DecrementOutstandingAllocations(void)
{
    __sync_sub_and_fetch(&_parcStdlibMemory_OutstandingAllocations, 1);
}
#endif

#if HAVE_REALLOC == 0
static void *
_parcStdlibMemory_rplRealloc(void *oldAlloc, size_t newSize)
{
    if (newSize == 0) {
        newSize = 1;
    }

    char *newAlloc = malloc(newSize);

    if (oldAlloc != NULL) {
        memcpy(newAlloc, oldAlloc, newSize);
        free(oldAlloc);
    }
    return newAlloc;
}
#endif

void *
parcStdlibMemory_Allocate(size_t size)
{
    if (size == 0) {
        return NULL;
    }

    void *result = malloc(size);
    if (result != NULL) {
        _parcStdlibMemory_IncrementOutstandingAllocations();
    }

    return result;
}

void *
parcStdlibMemory_AllocateAndClear(size_t size)
{
    void *pointer = parcStdlibMemory_Allocate(size);
    if (pointer != NULL) {
        memset(pointer, 0, size);
    }
    return pointer;
}

int
parcStdlibMemory_MemAlign(void **pointer, size_t alignment, size_t size)
{
    if (size == 0) {
        return EINVAL;
    }

    int failure = posix_memalign(pointer, alignment, size);

    if (failure != 0) {
        return failure;
    }
    if (*pointer == NULL) {
        return ENOMEM;
    }

    _parcStdlibMemory_IncrementOutstandingAllocations();

    return 0;
}

void
parcStdlibMemory_Deallocate(void **pointer)
{
#ifndef PARCLibrary_DISABLE_VALIDATION
    trapIllegalValueIf(_parcStdlibMemory_OutstandingAllocations == 0,
                       "parcStdlibMemory_Deallocate invoked with nothing left to free (double free somewhere?)\n");
#endif
    free(*pointer);
    *pointer = NULL;

    _parcStdlibMemory_DecrementOutstandingAllocations();
}

void *
parcStdlibMemory_Reallocate(void *pointer, size_t newSize)
{
#if HAVE_REALLOC
    void *result = realloc(pointer, newSize);
#else
    void *result = _parcStdlibMemory_rplRealloc(pointer, newSize);
#endif

    if (pointer == NULL) {
        _parcStdlibMemory_IncrementOutstandingAllocations();
    }
    return result;
}

char *
parcStdlibMemory_StringDuplicate(const char *string, size_t length)
{
    _parcStdlibMemory_IncrementOutstandingAllocations();
    return strndup(string, length);
}

uint32_t
parcStdlibMemory_Outstanding(void)
{
    return _parcStdlibMemory_OutstandingAllocations;
}

PARCMemoryInterface PARCStdlibMemoryAsPARCMemory = {
    .Allocate         = (uintptr_t) parcStdlibMemory_Allocate,
    .AllocateAndClear = (uintptr_t) parcStdlibMemory_AllocateAndClear,
    .MemAlign         = (uintptr_t) parcStdlibMemory_MemAlign,
    .Deallocate       = (uintptr_t) parcStdlibMemory_Deallocate,
    .Reallocate       = (uintptr_t) parcStdlibMemory_Reallocate,
    .StringDuplicate  = (uintptr_t) parcStdlibMemory_StringDuplicate,
    .Outstanding      = (uintptr_t) parcStdlibMemory_Outstanding
};
