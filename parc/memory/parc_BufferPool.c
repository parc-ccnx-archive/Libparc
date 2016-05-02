/*
 * Copyright (c) 2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
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
 */
/* ################################################################################
 * #
 * # PATENT NOTICE
 * #
 * # This software is distributed under the BSD 2-clause License (see LICENSE
 * # file).  This BSD License does not make any patent claims and as such, does
 * # not act as a patent grant.  The purpose of this file is for each contributor
 * # to define their intentions with respect to intellectual property.
 * #
 * # Each contributor to this source code is encouraged to state their patent
 * # claims and licensing mechanisms for any contributions made. At the end of
 * # this file contributors may each make their own statements.  Contributor's
 * # claims and grants only apply to the pieces (source code, programs, text,
 * # media, etc) that they have contributed directly to this software.
 * #
 * # There is no guarantee that this file is complete, up to date or accurate. It
 * # is up to the contributors to maintain their section in this file up to date
 * # and up to the user of the software to verify any claims herein.
 * #
 * # Do not remove this header notification.  The contents of this file must be
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
 * @author Glenn Scott, Palo Alto Research Center (PARC)
 * @copyright 2016, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_Memory.h>

#include <parc/algol/parc_LinkedList.h>

#include "parc_BufferPool.h"

struct PARCBufferPool {
    size_t bufferSize;
    size_t limit;
    size_t largestPoolSize;
    size_t totalInstances;
    size_t cacheHits;
    PARCLinkedList *freeList;
    PARCObjectDescriptor *descriptor;
    const PARCObjectDescriptor *originalDescriptor;
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
_parcBufferPool_ObjectDestructor(PARCBuffer **bufferPtr)
{
    PARCBuffer *buffer = *bufferPtr;
    
    PARCBufferPool *bufferPool = parcObjectDescriptor_GetTypeState(parcObject_GetDescriptor(buffer));
    
    parcObject_Mutex(bufferPool->freeList) {
        size_t freeListSize = parcLinkedList_Size(bufferPool->freeList);
        
        if (bufferPool->limit > freeListSize) {
            parcLinkedList_Append(bufferPool->freeList, buffer);
            freeListSize++;
            if (bufferPool->largestPoolSize < freeListSize) {
                bufferPool->largestPoolSize = freeListSize;
            }
        } else {
            parcBuffer_Acquire(buffer);
            parcObject_SetDescriptor(buffer, &PARCBuffer_Descriptor);
            parcBuffer_Release(&buffer);
        }
    }
    
    *bufferPtr = 0;
    return false;
}

parcObject_ImplementAcquire(parcBufferPool, PARCBufferPool);

parcObject_ImplementRelease(parcBufferPool, PARCBufferPool);

parcObject_Override(PARCBufferPool, PARCObject,
                    .destructor = (PARCObjectDestructor *) _parcBufferPool_Destructor,
                    .isLockable = true);


void
parcBufferPool_AssertValid(const PARCBufferPool *instance)
{
    assertTrue(parcBufferPool_IsValid(instance),
               "PARCBufferPool is not valid.");
}

PARCBufferPool *
parcBufferPool_CreateExtending(size_t limit, size_t bufferSize, PARCObjectDescriptor *descriptor)
{
    PARCBufferPool *result = parcObject_CreateInstance(PARCBufferPool);
    
    if (result != NULL) {
        result->limit = limit;
        result->totalInstances = 0;
        result->cacheHits = 0;
        result->largestPoolSize = 0;
        result->bufferSize = bufferSize;
        result->freeList = parcLinkedList_Create();
        
        char *string = parcMemory_Format("PARCBufferPool=%zu", bufferSize);
        result->descriptor = parcObjectDescriptor_CreateExtension(descriptor, string);
        parcMemory_Deallocate(&string);
        result->descriptor->destructor = (PARCObjectDestructor *) _parcBufferPool_ObjectDestructor;
        result->descriptor->typeState = (PARCObjectTypeState *) result;
    }
    
    return result;
}

PARCBufferPool *
parcBufferPool_Create(size_t limit, size_t bufferSize)
{
    PARCBufferPool *result = parcObject_CreateInstance(PARCBufferPool);
    
    if (result != NULL) {
        result->limit = limit;
        result->totalInstances = 0;
        result->cacheHits = 0;
        result->largestPoolSize = 0;
        result->bufferSize = bufferSize;
        result->freeList = parcLinkedList_Create();
        
        result->originalDescriptor = &PARCBuffer_Descriptor;
        
        char *string = parcMemory_Format("PARCBufferPool=%zu", bufferSize);
        result->descriptor = parcObjectDescriptor_CreateExtension(result->originalDescriptor, string);
        result->descriptor->destructor = (PARCObjectDestructor *) _parcBufferPool_ObjectDestructor;
        result->descriptor->typeState = (PARCObjectTypeState *) result;
        parcMemory_Deallocate(&string);
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
parcBufferPool_IsValid(const PARCBufferPool *bufferPool)
{
    bool result = false;
    
    if (bufferPool != NULL) {
        result = parcLinkedList_IsValid(bufferPool->freeList);
    }
    
    return result;
}

PARCBuffer *
parcBufferPool_GetInstance(PARCBufferPool *bufferPool)
{
    PARCBuffer *result = NULL;
    
    parcObject_Mutex(bufferPool->freeList) {
        if (parcLinkedList_Size(bufferPool->freeList) > 0) {
            result = parcLinkedList_RemoveFirst(bufferPool->freeList);
            bufferPool->cacheHits++;
        } else {
            result = parcBuffer_Allocate(bufferPool->bufferSize);
            parcObject_SetDescriptor(result, bufferPool->descriptor);
        }
        bufferPool->totalInstances++;
    }
    
    return result;
}

size_t
parcBufferPool_Drain(PARCBufferPool *bufferPool)
{
    size_t result = 0;
    
    parcObject_Mutex(bufferPool->freeList) {
        size_t freeListSize = parcLinkedList_Size(bufferPool->freeList);
        if (freeListSize > bufferPool->limit) {
            result = freeListSize - bufferPool->limit;
            for (size_t i = bufferPool->limit; i < freeListSize; i++) {
                PARCObject *object = parcLinkedList_RemoveLast(bufferPool->freeList);
                parcObject_SetDescriptor(object, &PARCBuffer_Descriptor);
                parcObject_Release(&object);
            }
        }
    }
    
    return result;
}

size_t
parcBufferPool_SetLimit(PARCBufferPool *bufferPool, size_t limit)
{
    size_t oldLimit = bufferPool->limit;
    
    if (limit < bufferPool->limit) {
        bufferPool->largestPoolSize = bufferPool->limit;
    }
    
    bufferPool->limit = limit;
    
    return oldLimit;
}

size_t
parcBufferPool_GetLimit(const PARCBufferPool *bufferPool)
{
    return bufferPool->limit;
}

size_t
parcBufferPool_GetCurrentPoolSize(const PARCBufferPool *bufferPool)
{
    size_t result = 0;
    
    parcObject_Mutex(bufferPool->freeList) {
        result = parcLinkedList_Size(bufferPool->freeList);
    }
    
    return result;
}

size_t
parcBufferPool_GetLargestPoolSize(const PARCBufferPool *bufferPool)
{
    return bufferPool->largestPoolSize;
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
