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
#include <stdio.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_Memory.h>

#include <parc/algol/parc_LinkedList.h>

#include "parc_BufferPool.h"

struct PARCSimpleBufferPool {
    size_t bufferSize;
    size_t limit;
    PARCLinkedList *freeList;
    PARCObjectDescriptor *descriptor;
};

static bool
_parcSimpleBufferPool_Destructor(PARCSimpleBufferPool **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCSimpleBufferPool pointer.");

    PARCSimpleBufferPool *pool = *instancePtr;
    
    parcLinkedList_Apply(pool->freeList, (void (*)) parcObject_SetDescriptor, (const void *) &PARCBuffer_Descriptor);
    
    parcLinkedList_Release(&pool->freeList);
    
    return true;
}

static bool
_parcBuffer_PoolDestructor(PARCBuffer **bufferPtr)
{
    PARCBuffer *buffer = *bufferPtr;
    *bufferPtr = 0;
    
    PARCSimpleBufferPool *bufferPool = parcObjectDescriptor_GetTypeState(parcObject_GetDescriptor(buffer));
    
    if (bufferPool->limit > parcLinkedList_Size(bufferPool->freeList)) {
        parcLinkedList_Append(bufferPool->freeList, buffer);
    }
    
    return false;
}

parcObject_ImplementAcquire(parcSimpleBufferPool, PARCSimpleBufferPool);

parcObject_ImplementRelease(parcSimpleBufferPool, PARCSimpleBufferPool);

parcObject_Override(
	PARCSimpleBufferPool, PARCObject,
	.destructor = (PARCObjectDestructor *) _parcSimpleBufferPool_Destructor);

PARCSimpleBufferPool *
parcSimpleBufferPool_Create(size_t limit, size_t bufferSize)
{
    PARCSimpleBufferPool *result = parcObject_CreateInstance(PARCSimpleBufferPool);
    
    if (result != NULL) {
        result->limit = limit;
        result->bufferSize = bufferSize;
        result->freeList = parcLinkedList_Create();
        
        char *string;
        asprintf(&string, "PARCPooledBuffer=%zu", bufferSize);
        result->descriptor = parcObjectDescriptor_CreateExtension(&PARCBuffer_Descriptor, string);
        free(string);
        result->descriptor->destructor = (PARCObjectDestructor *) _parcBuffer_PoolDestructor;
        result->descriptor->typeState = (PARCObjectTypeState *) result;
    }
    
    return result;
}

PARCBuffer *
parcSimpleBufferPool_GetInstance(PARCSimpleBufferPool *bufferPool)
{
    PARCBuffer *result;
    
    if (parcLinkedList_Size(bufferPool->freeList) > 0) {
        result = parcLinkedList_RemoveFirst(bufferPool->freeList);
    } else {
        result = parcBuffer_Allocate(bufferPool->bufferSize);
        parcObject_SetDescriptor(result, bufferPool->descriptor);
    }
    
    return result;
}
