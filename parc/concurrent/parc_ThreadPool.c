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
 * @author <#gscott#>, Computing Science Laboratory, PARC
 * @copyright 2015 Palo Alto Research Center, Inc. (PARC), A Xerox Company.  All Rights Reserved.
 */
#include <config.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_Memory.h>

#include "parc_ThreadPool.h"

struct PARCThreadPool {
    int poolSize;
};

static void
_parcThreadPool_Finalize(PARCThreadPool **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCThreadPool pointer.");
//    PARCThreadPool *instance = *instancePtr;
    
    
    /* cleanup the instance fields here */
}

parcObject_ImplementAcquire(parcThreadPool, PARCThreadPool);

parcObject_ImplementRelease(parcThreadPool, PARCThreadPool);

parcObject_ExtendPARCObject(PARCThreadPool, _parcThreadPool_Finalize, parcThreadPool_Copy, parcThreadPool_ToString, parcThreadPool_Equals, parcThreadPool_Compare, parcThreadPool_HashCode, parcThreadPool_ToJSON);


void
parcThreadPool_AssertValid(const PARCThreadPool *instance)
{
    assertTrue(parcThreadPool_IsValid(instance),
               "PARCThreadPool is not valid.");
}


PARCThreadPool *
parcThreadPool_Create(int poolSize)
{
    PARCThreadPool *result = parcObject_CreateInstance(PARCThreadPool);
    
    if (result != NULL) {
        result->poolSize = poolSize;
    }

    return result;
}

int
parcThreadPool_Compare(const PARCThreadPool *instance, const PARCThreadPool *other)
{
    int result = 0;
    
    return result;
}

PARCThreadPool *
parcThreadPool_Copy(const PARCThreadPool *original)
{
    PARCThreadPool *result = parcThreadPool_Create(original->poolSize);
    
    return result;
}

void
parcThreadPool_Display(const PARCThreadPool *instance, int indentation)
{
    parcDisplayIndented_PrintLine(indentation, "PARCThreadPool@%p {", instance);
    /* Call Display() functions for the fields here. */
    parcDisplayIndented_PrintLine(indentation, "}");
}

bool
parcThreadPool_Equals(const PARCThreadPool *x, const PARCThreadPool *y)
{
    bool result = false;
    
    if (x == y) {
        result = true;
    } else if (x == NULL || y == NULL) {
        result = false;
    } else {
        /* perform instance specific equality tests here. */
        if (x->poolSize == y->poolSize) {
            result = true;
        }
    }
    
    return result;
}

PARCHashCode
parcThreadPool_HashCode(const PARCThreadPool *instance)
{
    PARCHashCode result = 0;
    
    return result;
}

bool
parcThreadPool_IsValid(const PARCThreadPool *instance)
{
    bool result = false;
    
    if (instance != NULL) {
        result = true;
    }
    
    return result;
}

PARCJSON *
parcThreadPool_ToJSON(const PARCThreadPool *instance)
{
    PARCJSON *result = parcJSON_Create();
    
    if (result != NULL) {
        
    }
    
    return result;
}

char *
parcThreadPool_ToString(const PARCThreadPool *instance)
{
    char *result = parcMemory_Format("PARCThreadPool@%p\n", instance);

    return result;
}
