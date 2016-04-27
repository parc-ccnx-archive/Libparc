/*
 * Copyright (c) 2016, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @author Glenn Scott, Palo Alto Research Center (PARC)
 * @copyright 2016, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>
#include <stdio.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_Memory.h>

#include <parc/algol/parc_LinkedList.h>

#include "parc_BufferPool.h"

struct PARCBufferPool {
    size_t bufferSize;
    size_t limit;
    size_t highWater;
    size_t totalInstances;
    size_t cacheHits;
    PARCLinkedList *freeList;
    PARCObjectDescriptor *descriptor;
};

static bool
_parcBufferPool_Destructor(PARCBufferPool **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCBufferPool pointer.");
    
    PARCBufferPool *pool = *instancePtr;
    
    parcLinkedList_Apply(pool->freeList, (void (*)) parcObject_SetDescriptor, (const void *) &PARCBuffer_Descriptor);
    
    parcLinkedList_Release(&pool->freeList);
    parcObjectDescriptor_Destroy(&pool->descriptor);
    
    return true;
}

static bool
_parcBufferPool_BufferDestructor(PARCBuffer **bufferPtr)
{
    PARCBuffer *buffer = *bufferPtr;
    
    PARCBufferPool *pool = parcObjectDescriptor_GetTypeState(parcObject_GetDescriptor(buffer));
    
    size_t freeListSize = parcLinkedList_Size(pool->freeList);
    
    if (pool->limit > freeListSize) {
        parcLinkedList_Append(pool->freeList, buffer);
        freeListSize++;
        if (pool->highWater < freeListSize) {
            pool->highWater = freeListSize;
        }
    } else {
        parcBuffer_Acquire(buffer);
        parcObject_SetDescriptor(buffer, &PARCBuffer_Descriptor);
        parcBuffer_Release(&buffer);
    }
    
    *bufferPtr = 0;
    return false;
}

parcObject_ImplementAcquire(parcBufferPool, PARCBufferPool);

parcObject_ImplementRelease(parcBufferPool, PARCBufferPool);

parcObject_Override(
	PARCBufferPool, PARCObject,
	.destructor = (PARCObjectDestructor *) _parcBufferPool_Destructor);


void
parcBufferPool_AssertValid(const PARCBufferPool *instance)
{
    assertTrue(parcBufferPool_IsValid(instance),
               "PARCBufferPool is not valid.");
}

PARCBufferPool *
parcBufferPool_Create(size_t limit, size_t bufferSize)
{
    PARCBufferPool *result = parcObject_CreateInstance(PARCBufferPool);
    
    if (result != NULL) {
        result->limit = limit;
        result->totalInstances = 0;
        result->cacheHits = 0;
        result->highWater = 0;
        result->bufferSize = bufferSize;
        result->freeList = parcLinkedList_Create();
        
        char *string;
        asprintf(&string, "PARCBufferPool=%zu", bufferSize);
        result->descriptor = parcObjectDescriptor_CreateExtension(&PARCBuffer_Descriptor, string);
        free(string);
        result->descriptor->destructor = (PARCObjectDestructor *) _parcBufferPool_BufferDestructor;
        result->descriptor->typeState = (PARCObjectTypeState *) result;
    }
    
    return result;
}

void
parcBufferPool_Display(const PARCBufferPool *instance, int indentation)
{
    parcDisplayIndented_PrintLine(indentation, "PARCBufferPool@%p {", instance);
    /* Call Display() functions for the fields here. */
    parcDisplayIndented_PrintLine(indentation, "}");
}

bool
parcBufferPool_IsValid(const PARCBufferPool *instance)
{
    bool result = false;
    
    if (instance != NULL) {
        result = true;
    }
    
    return result;
}

PARCBuffer *
parcBufferPool_GetInstance(PARCBufferPool *bufferPool)
{
    PARCBuffer *result;
    
    if (parcLinkedList_Size(bufferPool->freeList) > 0) {
        result = parcLinkedList_RemoveFirst(bufferPool->freeList);
        bufferPool->cacheHits++;
    } else {
        result = parcBuffer_Allocate(bufferPool->bufferSize);
        parcObject_SetDescriptor(result, bufferPool->descriptor);
    }
    bufferPool->totalInstances++;
    
    return result;
}

size_t
parcBufferPool_SetLimit(PARCBufferPool *bufferPool, size_t limit)
{
    size_t oldLimit = bufferPool->limit;
    bufferPool->limit = limit;
    for (size_t i = limit; i < oldLimit; i++) {
        parcLinkedList_RemoveLast(bufferPool->freeList);
    }
    return oldLimit;
}

size_t
parcBufferPool_GetHighWater(const PARCBufferPool *bufferPool)
{
    return bufferPool->highWater;
}

size_t
parcBufferPool_GetTotalInstances(const PARCBufferPool *bufferPool)
{
    return bufferPool->totalInstances;
}

size_t
parcBufferPool_GetCacheHits(const PARCBufferPool *bufferPool)
{
    return bufferPool->cacheHits;
}
