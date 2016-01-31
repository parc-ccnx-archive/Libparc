/*
 * Copyright (c) 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
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

#include <parc/algol/parc_AtomicInteger.h>
#include <parc/algol/parc_StdlibMemory.h>

static uint32_t _parcStdlibMemory_OutstandingAllocations;

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
#ifdef PARCLibrary_DISABLE_ATOMICS
        parcAtomicInteger_Uint32Increment(&_parcStdlibMemory_OutstandingAllocations);
#else
        __sync_add_and_fetch(&_parcStdlibMemory_OutstandingAllocations, 1);
#endif
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

    parcAtomicInteger_Uint32Increment(&_parcStdlibMemory_OutstandingAllocations);

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

#ifdef PARCLibrary_DISABLE_ATOMICS
    parcAtomicInteger_Uint32Decrement(&_parcStdlibMemory_OutstandingAllocations);
#else
    __sync_sub_and_fetch(&_parcStdlibMemory_OutstandingAllocations, 1);
#endif
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
        parcAtomicInteger_Uint32Increment(&_parcStdlibMemory_OutstandingAllocations);
    }
    return result;
}

char *
parcStdlibMemory_StringDuplicate(const char *string, size_t length)
{
    parcAtomicInteger_Uint32Increment(&_parcStdlibMemory_OutstandingAllocations);
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
