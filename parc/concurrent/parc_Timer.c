/*
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * Copyright 2015 Palo Alto Research Center, Inc. (PARC), a Xerox company.  All Rights Reserved.
 * The content of this file, whole or in part, is subject to licensing terms.
 * If distributing this software, include this License Header Notice in each
 * file and provide the accompanying LICENSE file.
 */
/**
 * @author <#gscott#>, Computing Science Laboratory, PARC
 * @copyright 2015 Palo Alto Research Center, Inc. (PARC), A Xerox Company.  All Rights Reserved.
 */
#include <config.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_Memory.h>

#include "parc_Timer.h"

struct PARCTimer {
    
};

static void
_parcTimer_Finalize(PARCTimer **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCTimer pointer.");
    PARCTimer *instance = *instancePtr;
    
    
    /* cleanup the instance fields here */
}

parcObject_ImplementAcquire(parcTimer, PARCTimer);

parcObject_ImplementRelease(parcTimer, PARCTimer);

parcObject_ExtendPARCObject(PARCTimer, _parcTimer_Finalize, parcTimer_Copy, parcTimer_ToString, parcTimer_Equals, parcTimer_Compare, parcTimer_HashCode, parcTimer_ToJSON);


void
parcTimer_AssertValid(const PARCTimer *instance)
{
    assertTrue(parcTimer_IsValid(instance),
               "PARCTimer is not valid.");
}


PARCTimer *
parcTimer_Create(void)
{
    PARCTimer *result = parcObject_CreateInstance(PARCTimer);
    
    if (result != NULL) {
        
    }

    return result;
}

int
parcTimer_Compare(const PARCTimer *instance, const PARCTimer *other)
{
    int result = 0;
    
    return result;
}

PARCTimer *
parcTimer_Copy(const PARCTimer *original)
{
    PARCTimer *result = NULL;
    
    return result;
}

void
parcTimer_Display(const PARCTimer *instance, int indentation)
{
    parcDisplayIndented_PrintLine(indentation, "PARCTimer@%p {", instance);
    /* Call Display() functions for the fields here. */
    parcDisplayIndented_PrintLine(indentation, "}");
}

bool
parcTimer_Equals(const PARCTimer *x, const PARCTimer *y)
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
parcTimer_HashCode(const PARCTimer *instance)
{
    PARCHashCode result = 0;
    
    return result;
}

bool
parcTimer_IsValid(const PARCTimer *instance)
{
    bool result = false;
    
    if (instance != NULL) {
        result = true;
    }
    
    return result;
}

PARCJSON *
parcTimer_ToJSON(const PARCTimer *instance)
{
    PARCJSON *result = parcJSON_Create();
    
    if (result != NULL) {
        
    }
    
    return result;
}

char *
parcTimer_ToString(const PARCTimer *instance)
{
    char *result = parcMemory_Format("PARCTimer@%p\n", instance);

    return result;
}
