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
 *
 * ################################################################################
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
 * @author <#__FULLUSERNAME___#>, Palo Alto Research Center (PARC)
 * @copyright 2016, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
//#include <config.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_Memory.h>

#include "parc_StaticObject.h"


// Detect a compile time if a buffer is large enough to hold a structure.
#define parcObject_DefinePrivate(_type_, ...) \
    typedef struct { __VA_ARGS__ } _##_type_; \
    enum { bytesCompileTimeAssertion = 1/!!(sizeof(_type_) >= sizeof(_##_type_)) }

parcObject_DefinePrivate(PARCStaticObject,
                         int x;
                         double y;
                         double z;
                         );

static bool
_parcStaticObject_Destructor(PARCStaticObject **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCStaticObject pointer.");
    
    
    /* cleanup the instance fields here */
    return true;
}

parcObject_ImplementAcquire(parcStaticObject, PARCStaticObject);

parcObject_ImplementRelease(parcStaticObject, PARCStaticObject);

parcObject_Override(
	PARCStaticObject, PARCObject,
	.destructor = (PARCObjectDestructor *) _parcStaticObject_Destructor,
	.copy = (PARCObjectCopy *) parcStaticObject_Copy,
	.toString = (PARCObjectToString *)  parcStaticObject_ToString,
	.equals = (PARCObjectEquals *)  parcStaticObject_Equals,
	.compare = (PARCObjectCompare *)  parcStaticObject_Compare,
	.hashCode = (PARCObjectHashCode *)  parcStaticObject_HashCode,
	.toJSON = (PARCObjectToJSON *)  parcStaticObject_ToJSON);


void
parcStaticObject_AssertValid(const PARCStaticObject *instance)
{
    assertTrue(parcStaticObject_IsValid(instance),
               "PARCStaticObject is not valid.");
}

PARCStaticObject *
parcStaticObject_Wrap(PARCStaticObject *instance)
{
    PARCStaticObject *result = parcObject_Wrap(instance, &parcObject_DescriptorName(PARCStaticObject));
        
    return  result;
}

PARCStaticObject *
parcStaticObject_Create(int x, double y, double z)
{
    _PARCStaticObject *result = parcObject_CreateInstance(PARCStaticObject);
    
    if (result != NULL) {
        result->x = x;
        result->y = y;
        result->z = z;
    }
    
    return (PARCStaticObject *) result;
}

int
parcStaticObject_Compare(const PARCStaticObject *instance, const PARCStaticObject *other)
{
    int result = 0;
    
    return result;
}

PARCStaticObject *
parcStaticObject_Copy(const PARCStaticObject *original)
{
    PARCStaticObject *result = NULL;
    
    return result;
}

void
parcStaticObject_Display(const PARCStaticObject *instance, int indentation)
{
    parcDisplayIndented_PrintLine(indentation, "PARCStaticObject@%p {", instance);
    /* Call Display() functions for the fields here. */
    parcDisplayIndented_PrintLine(indentation, "}");
}

bool
parcStaticObject_Equals(const PARCStaticObject *x, const PARCStaticObject *y)
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
parcStaticObject_HashCode(const PARCStaticObject *instance)
{
    PARCHashCode result = 0;
    
    return result;
}

bool
parcStaticObject_IsValid(const PARCStaticObject *instance)
{
    bool result = false;
    
    if (instance != NULL) {
        result = true;
    }
    
    return result;
}

PARCJSON *
parcStaticObject_ToJSON(const PARCStaticObject *instance)
{
    PARCJSON *result = parcJSON_Create();
    
    if (result != NULL) {
        
    }
    
    return result;
}

char *
parcStaticObject_ToString(const PARCStaticObject *instance)
{
    _PARCStaticObject *object = (_PARCStaticObject *) instance;
    
    char *result = parcMemory_Format("PARCStaticObject@%p { .x=%d .y=%f .z=%f }", object, object->x, object->y, object->z);

    return result;
}
