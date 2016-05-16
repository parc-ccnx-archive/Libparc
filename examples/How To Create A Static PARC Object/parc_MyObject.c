/*
 * Copyright (c) 2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
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
 * # not act as a patent grant.  The purpose of this section is for each contributor
 * # to define their intentions with respect to intellectual property.
 * #
 * # Each contributor to this source code is encouraged to state their patent
 * # claims and licensing mechanisms for any contributions made. At the end of
 * # this section contributors may each make their own statements.  Contributor's
 * # claims and grants only apply to the pieces (source code, programs, text,
 * # media, etc) that they have contributed directly to this software.
 * #
 * # There is no guarantee that this section is complete, up to date or accurate. It
 * # is up to the contributors to maintain their portion of this section and up to
 * # the user of the software to verify any claims herein.
 * #
 * # Do not remove this header notification.  The contents of this section must be
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
 * @copyright 2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
//#include <config.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_Memory.h>

#include "parc_MyObject.h"

// Detect a compile time if a buffer is large enough to hold a structure.
#define parcObject_DefineXXX(_type_, ...) \
    typedef struct { __VA_ARGS__ } _##_type_; \
    enum { bytesCompileTimeAssertion = 1/!!(sizeof(_type_) >= sizeof(_##_type_)) }

struct PARCMyObject {
    int x;
    double y;
    double z;
};

static bool
_parcMyObject_Destructor(PARCMyObject **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCMyObject pointer.");


    /* cleanup the instance fields here */
    return true;
}

parcObject_ImplementAcquire(parcMyObject, PARCMyObject);

parcObject_ImplementRelease(parcMyObject, PARCMyObject);

parcObject_Override(
	PARCMyObject, PARCObject,
	.destructor = (PARCObjectDestructor *) _parcMyObject_Destructor,
	.copy = (PARCObjectCopy *) parcMyObject_Copy,
	.toString = (PARCObjectToString *)  parcMyObject_ToString,
	.equals = (PARCObjectEquals *)  parcMyObject_Equals,
	.compare = (PARCObjectCompare *)  parcMyObject_Compare,
	.hashCode = (PARCObjectHashCode *)  parcMyObject_HashCode,
	.toJSON = (PARCObjectToJSON *)  parcMyObject_ToJSON);


void
parcMyObject_AssertValid(const PARCMyObject *instance)
{
    assertTrue(parcMyObject_IsValid(instance),
               "PARCMyObject is not valid.");
}

PARCMyObject *
parcMyObject_Wrap(void *origin)
{
    PARCMyObject *result = parcObject_Wrap(origin, PARCMyObject);

    return  result;
}

PARCMyObject *
parcMyObject_Init(PARCMyObject *object, int x, double y, double z)
{
    if (object != NULL) {
        object->x = x;
        object->y = y;
        object->z = z;
    }

    return object;
}

PARCMyObject *
parcMyObject_Create(int x, double y, double z)
{
    PARCMyObject *result = parcObject_CreateInstance(PARCMyObject);

    if (result != NULL) {
        result->x = x;
        result->y = y;
        result->z = z;
    }

    return (PARCMyObject *) result;
}

int
parcMyObject_Compare(const PARCMyObject *instance, const PARCMyObject *other)
{
    int result = 0;

    return result;
}

PARCMyObject *
parcMyObject_Copy(const PARCMyObject *original)
{
    PARCMyObject *result = NULL;

    return result;
}

void
parcMyObject_Display(const PARCMyObject *object, int indentation)
{
    parcDisplayIndented_PrintLine(indentation, "PARCMyObject@%p { .x=%d .y=%f .z=%f }", object, object->x, object->y, object->z);
}

bool
parcMyObject_Equals(const PARCMyObject *x, const PARCMyObject *y)
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
parcMyObject_HashCode(const PARCMyObject *instance)
{
    PARCHashCode result = 0;

    return result;
}

bool
parcMyObject_IsValid(const PARCMyObject *instance)
{
    bool result = false;

    if (instance != NULL) {
        result = true;
    }

    return result;
}

PARCJSON *
parcMyObject_ToJSON(const PARCMyObject *instance)
{
    PARCJSON *result = parcJSON_Create();

    if (result != NULL) {

    }

    return result;
}

char *
parcMyObject_ToString(const PARCMyObject *object)
{
    char *result = parcMemory_Format("PARCMyObject@%p { .x=%d .y=%f .z=%f }", object, object->x, object->y, object->z);

    return result;
}
