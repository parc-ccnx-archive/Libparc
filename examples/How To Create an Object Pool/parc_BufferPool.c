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
 * @author <#__FULLUSERNAME___#>, Palo Alto Research Center (PARC)
 * @copyright 2016, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
//#include <config.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_Memory.h>

#include <parc/algol/parc_LinkedList.h>

#include "parc_BufferPool.h"

struct PARCBufferPool {
    size_t bufferSize;
    size_t highWater;
    PARCLinkedList *freeList;
};

static PARCBufferPool *_parcBufferPool;

void
parcLinkedList_Apply(PARCLinkedList *list, void (*func)(PARCObject *, const void *), const void *parameter)
{
    PARCIterator *iterator = parcLinkedList_CreateIterator(list);

    while (parcIterator_HasNext(iterator)) {
        PARCObject *object = parcIterator_Next(iterator);
        func(object, parameter);
    }
    
    parcIterator_Release(&iterator);
}

static void
_parcBufferPool_ResetDescriptor(PARCObject *object, const void *parameter)
{
    parcObject_SetDescriptor(object, &PARCBuffer_Descriptor);
}

static bool
_parcBufferPool_Destructor(PARCBufferPool **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCBufferPool pointer.");

    PARCBufferPool *pool = *instancePtr;
    
    parcLinkedList_Apply(pool->freeList, _parcBufferPool_ResetDescriptor, NULL);
    
    parcLinkedList_Release(&pool->freeList);
    
    return true;
}

static bool
_parcBuffer_PoolDestructor(PARCBuffer **bufferPtr)
{
    PARCBuffer *buffer = *bufferPtr;
    *bufferPtr = 0;
    
    if (_parcBufferPool->highWater > parcLinkedList_Size(_parcBufferPool->freeList)) {
        parcLinkedList_Append(_parcBufferPool->freeList, buffer);
    }
    
    return false;
}

parcObject_ImplementAcquire(parcBufferPool, PARCBufferPool);

parcObject_ImplementRelease(parcBufferPool, PARCBufferPool);

parcObject_Override(
	PARCBufferPool, PARCObject,
	.destructor = (PARCObjectDestructor *) _parcBufferPool_Destructor,
	.copy = (PARCObjectCopy *) parcBufferPool_Copy,
	.toString = (PARCObjectToString *)  parcBufferPool_ToString,
	.equals = (PARCObjectEquals *)  parcBufferPool_Equals,
	.compare = (PARCObjectCompare *)  parcBufferPool_Compare,
	.hashCode = (PARCObjectHashCode *)  parcBufferPool_HashCode,
	.toJSON = (PARCObjectToJSON *)  parcBufferPool_ToJSON);


void
parcBufferPool_AssertValid(const PARCBufferPool *instance)
{
    assertTrue(parcBufferPool_IsValid(instance),
               "PARCBufferPool is not valid.");
}


PARCBufferPool *
parcBufferPool_Create(size_t bufferSize)
{
    if (_parcBufferPool == NULL) {
        _parcBufferPool = parcObject_CreateInstance(PARCBufferPool);
        
        if (_parcBufferPool != NULL) {
            _parcBufferPool->highWater = 3;
            _parcBufferPool->bufferSize = bufferSize;
            _parcBufferPool->freeList = parcLinkedList_Create();
        }
    }
    return _parcBufferPool;
}

int
parcBufferPool_Compare(const PARCBufferPool *instance, const PARCBufferPool *other)
{
    int result = 0;
    
    return result;
}

PARCBufferPool *
parcBufferPool_Copy(const PARCBufferPool *original)
{
    PARCBufferPool *result = NULL;
    
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
parcBufferPool_Equals(const PARCBufferPool *x, const PARCBufferPool *y)
{
    bool result = false;
    
    if (x == y) {
        result = true;
    } else if (x == NULL || y == NULL) {
        result = false;
    } else {
        /* perform instance specific equality tests here. */
    }
    
    return result;
}

PARCHashCode
parcBufferPool_HashCode(const PARCBufferPool *instance)
{
    PARCHashCode result = 0;
    
    return result;
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

PARCJSON *
parcBufferPool_ToJSON(const PARCBufferPool *instance)
{
    PARCJSON *result = parcJSON_Create();
    
    if (result != NULL) {
        
    }
    
    return result;
}

char *
parcBufferPool_ToString(const PARCBufferPool *instance)
{
    char *result = parcMemory_Format("PARCBufferPool@%p\n", instance);

    return result;
}

parcObject_Extends(PARCPooledBuffer, PARCBuffer,
                   .destructor = (PARCObjectDestructor *) _parcBuffer_PoolDestructor);

PARCBuffer *
parcBufferPool_GetInstance(PARCBufferPool *bufferPool)
{
    PARCBuffer *result;
    
    if (parcLinkedList_Size(_parcBufferPool->freeList) > 0) {
        result = parcLinkedList_RemoveFirst(_parcBufferPool->freeList);
    } else {
        result = parcBuffer_Allocate(bufferPool->bufferSize);
        parcObject_SetDescriptor(result, &parcObject_DescriptorName(PARCPooledBuffer));
    }
    
    return result;
    
}