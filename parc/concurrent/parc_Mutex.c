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
#include <config.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_Memory.h>

#include "parc_Mutex.h"

struct PARCMutex {
    
};

static bool
_parcMutex_Destructor(PARCMutex **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCMutex pointer.");
    PARCMutex *instance = *instancePtr;
    
    
    /* cleanup the instance fields here */
    return true;
}

parcObject_ImplementAcquire(parcMutex, PARCMutex);

parcObject_ImplementRelease(parcMutex, PARCMutex);

parcObject_Override(
	PARCMutex, PARCObject,
	.destructor = (PARCObjectDestructor *) _parcMutex_Destructor,
	.copy = (PARCObjectCopy *) parcMutex_Copy,
	.toString = (PARCObjectToString *)  parcMutex_ToString,
	.equals = (PARCObjectEquals *)  parcMutex_Equals,
	.compare = (PARCObjectCompare *)  parcMutex_Compare,
	.hashCode = (PARCObjectHashCode *)  parcMutex_HashCode,
	.toJSON = (PARCObjectToJSON *)  parcMutex_ToJSON);


void
parcMutex_AssertValid(const PARCMutex *instance)
{
    assertTrue(parcMutex_IsValid(instance),
               "PARCMutex is not valid.");
}


PARCMutex *
parcMutex_Create(void)
{
    PARCMutex *result = parcObject_CreateInstance(PARCMutex);
    
    if (result != NULL) {
        
    }

    return result;
}

int
parcMutex_Compare(const PARCMutex *instance, const PARCMutex *other)
{
    int result = 0;
    
    return result;
}

PARCMutex *
parcMutex_Copy(const PARCMutex *original)
{
    PARCMutex *result = NULL;
    
    return result;
}

void
parcMutex_Display(const PARCMutex *instance, int indentation)
{
    parcDisplayIndented_PrintLine(indentation, "PARCMutex@%p {", instance);
    /* Call Display() functions for the fields here. */
    parcDisplayIndented_PrintLine(indentation, "}");
}

bool
parcMutex_Equals(const PARCMutex *x, const PARCMutex *y)
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
parcMutex_HashCode(const PARCMutex *instance)
{
    PARCHashCode result = 0;
    
    return result;
}

bool
parcMutex_IsValid(const PARCMutex *instance)
{
    bool result = false;
    
    if (instance != NULL) {
        result = true;
    }
    
    return result;
}

PARCJSON *
parcMutex_ToJSON(const PARCMutex *instance)
{
    PARCJSON *result = parcJSON_Create();
    
    if (result != NULL) {
        
    }
    
    return result;
}

char *
parcMutex_ToString(const PARCMutex *instance)
{
    char *result = parcMemory_Format("PARCMutex@%p\n", instance);

    return result;
}


typedef bool PARCAtomicBool;

PARCAtomicBool *
parcAtomicBool_Create(bool initialValue)
{
    PARCAtomicBool *result = parcMemory_Allocate(sizeof(bool));
    
    return result;
}

void
parcAtomicBool_Toggle(PARCAtomicBool *atomic, bool newValue)
{
    bool predicate = !newValue;
    
    while (__sync_bool_compare_and_swap(atomic, predicate, newValue))
        ;
}

static inline bool
atomic_cas(PARCAtomicBool *atomic, bool predicate, bool value)
{
    return __sync_bool_compare_and_swap(atomic, predicate, value);
}

