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

#include <LongBow/runtime.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include <stdint.h>
#include <stdbool.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_SafeMemory.h>
#include <parc/algol/parc_StdlibMemory.h>

static const PARCMemoryInterface *parcMemory = &PARCStdlibMemoryAsPARCMemory;

const PARCMemoryInterface *
parcMemory_SetInterface(const PARCMemoryInterface *memoryProvider)
{
    assertFalse(memoryProvider == &PARCMemoryAsPARCMemory,
                "You cannot use PARCMemoryAsPARCMemory as a memory provider for parcMemory.");
    const PARCMemoryInterface *result = parcMemory;
    parcMemory = memoryProvider;

    return result;
}

size_t
parcMemory_RoundUpToCacheLine(const size_t size)
{
    return parcMemory_RoundUpToMultiple(size, LEVEL1_DCACHE_LINESIZE);
}

size_t
parcMemory_RoundUpToMultiple(const size_t size, const size_t multiple)
{
    if (size == 0) {
        return multiple;
    }

    if (multiple == 0) {
        return size;
    }

    size_t remainder = size % multiple;
    if (remainder == 0) {
        return size;
    }
    return size + multiple - remainder;
}

void *
parcMemory_Allocate(const size_t size)
{
    return ((PARCMemoryAllocate *) parcMemory->Allocate)(size);
}

void *
parcMemory_AllocateAndClear(const size_t size)
{
    return ((PARCMemoryAllocateAndClear *) parcMemory->AllocateAndClear)(size);
}

int
parcMemory_MemAlign(void **pointer, const size_t alignment, const size_t size)
{
    return ((PARCMemoryMemAlign *) parcMemory->MemAlign)(pointer, alignment, size);
}

void
parcMemory_DeallocateImpl(void **pointer)
{
    ((PARCMemoryDeallocate *) parcMemory->Deallocate)(pointer);
}

void *
parcMemory_Reallocate(void *pointer, size_t newSize)
{
    return ((PARCMemoryReallocate *) parcMemory->Reallocate)(pointer, newSize);
}

char *
parcMemory_StringDuplicate(const char *string, const size_t length)
{
    return ((PARCMemoryStringDuplicate *) parcMemory->StringDuplicate)(string, length);
}

uint32_t
parcMemory_Outstanding(void)
{
    return ((PARCMemoryOutstanding *) parcMemory->Outstanding)();
}

char *
parcMemory_Format(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    va_list copy;
    va_copy(copy, ap);
    int length = vsnprintf(NULL, 0, format, copy);
    va_end(copy);

    char *result = NULL;
    if (length >= 0) {
        result = parcMemory_Allocate(length + 1);

        if (result != NULL) {
            vsprintf(result, format, ap);
        }
    }
    return result;
}

PARCMemoryInterface PARCMemoryAsPARCMemory = {
    .Allocate         = (uintptr_t) parcMemory_Allocate,
    .AllocateAndClear = (uintptr_t) parcMemory_AllocateAndClear,
    .MemAlign         = (uintptr_t) parcMemory_MemAlign,
    .Deallocate       = (uintptr_t) parcMemory_DeallocateImpl,
    .Reallocate       = (uintptr_t) parcMemory_Reallocate,
    .StringDuplicate  = (uintptr_t) parcMemory_StringDuplicate,
    .Outstanding      = (uintptr_t) parcMemory_Outstanding
};
