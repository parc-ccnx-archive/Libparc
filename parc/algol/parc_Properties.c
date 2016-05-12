/*
 * Copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @author <#Glenn Scott <Glenn.Scott@parc.com>#>, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>
#include <stdio.h>

#include <LongBow/runtime.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_HashMap.h>
#include <parc/algol/parc_Buffer.h>
#include <parc/algol/parc_Memory.h>

#include "parc_Properties.h"

struct PARCProperties {
    PARCHashMap *properties;
};

static void
_parcProperties_Finalize(PARCProperties **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCProperties pointer.");
    PARCProperties *instance = *instancePtr;

    parcProperties_OptionalAssertValid(instance);

    parcHashMap_Release(&instance->properties);
}

parcObject_ImplementAcquire(parcProperties, PARCProperties);

parcObject_ImplementRelease(parcProperties, PARCProperties);

parcObject_ExtendPARCObject(PARCProperties, _parcProperties_Finalize, parcProperties_Copy, parcProperties_ToString, parcProperties_Equals, parcProperties_Compare, parcProperties_HashCode, parcProperties_ToJSON);


void
parcProperties_AssertValid(const PARCProperties *instance)
{
    assertTrue(parcProperties_IsValid(instance),
               "PARCProperties is not valid.");
}

PARCProperties *
parcProperties_Create(void)
{
    PARCProperties *result = parcObject_CreateInstance(PARCProperties);

    if (result != NULL) {
        result->properties = parcHashMap_Create();
    }

    return result;
}

int
parcProperties_Compare(const PARCProperties *instance, const PARCProperties *other)
{
    int result = 0;


    return result;
}

PARCProperties *
parcProperties_Copy(const PARCProperties *original)
{
    PARCProperties *result = parcObject_CreateInstance(PARCProperties);

    if (result != NULL) {
        result->properties = parcHashMap_Copy(original->properties);
    }

    return result;
}

void
parcProperties_Display(const PARCProperties *properties, int indentation)
{
    parcDisplayIndented_PrintLine(indentation, "PARCProperties@%p {", properties);
    trapCannotObtainLockIf(parcHashMap_Lock(properties->properties) == false, "Cannot lock PARCProperties object.");

    PARCIterator *iterator = parcHashMap_CreateKeyIterator(properties->properties);
    while (parcIterator_HasNext(iterator)) {
        char *key = parcBuffer_ToString(parcIterator_Next(iterator));
        const char *value = parcProperties_GetProperty(properties, key);
        parcDisplayIndented_PrintLine(indentation + 1, "%s=%s", key, value);

        parcMemory_Deallocate(&key);
    }

    parcIterator_Release(&iterator);

    parcHashMap_Unlock(properties->properties);

    parcDisplayIndented_PrintLine(indentation, "}");
}

bool
parcProperties_Equals(const PARCProperties *x, const PARCProperties *y)
{
    bool result = false;

    if (x == y) {
        result = true;
    } else if (x == NULL || y == NULL) {
        result = false;
    } else {
        return parcHashMap_Equals(x->properties, y->properties);
    }

    return result;
}

PARCHashCode
parcProperties_HashCode(const PARCProperties *instance)
{
    return parcHashMap_HashCode(instance->properties);
}

bool
parcProperties_IsValid(const PARCProperties *instance)
{
    bool result = false;

    if (instance != NULL) {
        result = true;
    }

    return result;
}

PARCJSON *
parcProperties_ToJSON(const PARCProperties *properties)
{
    PARCJSON *result = parcJSON_Create();

    trapCannotObtainLockIf(parcHashMap_Lock(properties->properties) == false, "Cannot lock PARCProperties object.");

    PARCIterator *iterator = parcHashMap_CreateKeyIterator(properties->properties);
    while (parcIterator_HasNext(iterator)) {
        char *key = parcBuffer_ToString(parcIterator_Next(iterator));
        const char *value = parcProperties_GetProperty(properties, key);
        parcJSON_AddString(result, key, value);
        parcMemory_Deallocate(&key);
    }

    parcIterator_Release(&iterator);

    parcHashMap_Unlock(properties->properties);
    return result;
}

PARCBufferComposer *
parcProperties_BuildString(const PARCProperties *properties, PARCBufferComposer *composer)
{
    trapCannotObtainLockIf(parcHashMap_Lock(properties->properties) == false, "Cannot lock PARCProperties object.");

    PARCIterator *iterator = parcHashMap_CreateKeyIterator(properties->properties);
    while (parcIterator_HasNext(iterator)) {
        char *key = parcBuffer_ToString(parcIterator_Next(iterator));
        const char *value = parcProperties_GetProperty(properties, key);
        parcBufferComposer_PutStrings(composer, key, "=", value, "\n", NULL);
        parcMemory_Deallocate(&key);
    }

    parcIterator_Release(&iterator);

    parcHashMap_Unlock(properties->properties);
    return composer;
}

char *
parcProperties_ToString(const PARCProperties *properties)
{
    PARCBufferComposer *composer = parcBufferComposer_Create();
    parcProperties_BuildString(properties, composer);
    char *result = parcBufferComposer_ToString(composer);

    parcBufferComposer_Release(&composer);

    return result;
}

void
parcProperties_SetParsedProperty(PARCProperties *properties, char *string)
{
    char *equals = index(string, '=');
    *equals++ = 0;

    parcProperties_SetProperty(properties, string, equals);
}

bool
parcProperties_SetProperty(PARCProperties *properties, const char *name, const char *string)
{
    bool result = false;

    PARCBuffer *key = parcBuffer_AllocateCString(name);
    PARCBuffer *value = parcBuffer_AllocateCString(string);

    parcHashMap_Put(properties->properties, key, value);
    parcBuffer_Release(&key);
    parcBuffer_Release(&value);
    return result;
}

const char *
parcProperties_GetProperty(const PARCProperties *properties, const char *name)
{
    char *result = NULL;

    PARCBuffer *key = parcBuffer_AllocateCString(name);
    PARCBuffer *value = (PARCBuffer *) parcHashMap_Get(properties->properties, key);
    if (value != NULL) {
        result = parcBuffer_Overlay(value, 0);
    }

    parcBuffer_Release(&key);
    return result;
}

const char *
parcProperties_GetPropertyDefault(const PARCProperties *properties, const char *restrict name, const char *restrict defaultValue)
{
    char *result = (char *) defaultValue;

    PARCBuffer *key = parcBuffer_AllocateCString(name);
    PARCBuffer *value = (PARCBuffer *) parcHashMap_Get(properties->properties, key);
    if (value != NULL) {
        result = parcBuffer_Overlay(value, 0);
    }

    parcBuffer_Release(&key);
    return result;
}

bool
parcProperties_GetAsBoolean(const PARCProperties *properties, const char *name, bool defaultValue)
{
    bool result = defaultValue;

    const char *value = parcProperties_GetProperty(properties, name);
    if (value != NULL) {
        if (strcmp(value, "true") == 0) {
            result = true;
        } else {
            result = false;
        }
    }

    return result;
}

int64_t
parcProperties_GetAsInteger(const PARCProperties *properties, const char *name, int64_t defaultValue)
{
    int64_t result = defaultValue;

    const char *value = parcProperties_GetProperty(properties, name);
    if (value != NULL) {
        result = strtol(value, NULL, 10);
    }

    return result;
}

typedef struct {
    PARCBuffer *element;
    PARCIterator *hashMapIterator;
} _PARCPropertiesIterator;

static _PARCPropertiesIterator *
_parcPropertiesIterator_Init(const PARCProperties *object)
{
    _PARCPropertiesIterator *state = parcMemory_AllocateAndClear(sizeof(_PARCPropertiesIterator));
    state->hashMapIterator = parcHashMap_CreateKeyIterator(object->properties);
    return state;
}

static bool
_parcPropertiesIterator_HasNext(PARCProperties *properties __attribute__((unused)), _PARCPropertiesIterator *state)
{
    return parcIterator_HasNext(state->hashMapIterator);
}

static _PARCPropertiesIterator *
_parcPropertiesIterator_Next(PARCProperties *properties __attribute__((unused)), _PARCPropertiesIterator *state)
{
   state->element = (PARCBuffer *) parcIterator_Next(state->hashMapIterator);
   return state;
}

static void
_parcPropertiesIterator_Remove(PARCProperties *properties __attribute__((unused)), _PARCPropertiesIterator **state)
{
    parcIterator_Remove((*state)->hashMapIterator);
}

static char *
_parcPropertiesIterator_Element(PARCProperties *properties __attribute__((unused)), _PARCPropertiesIterator *state)
{
    return parcBuffer_Overlay(state->element, 0);
}

static void
_parcPropertiesIterator_Fini(PARCProperties *properties __attribute__((unused)), _PARCPropertiesIterator *state)
{
    parcIterator_Release(&state->hashMapIterator);
    parcMemory_Deallocate(&state);
}

PARCIterator *
parcProperties_CreateIterator(const PARCProperties *properties)
{
    PARCIterator *iterator = parcIterator_Create((PARCObject *) properties,
                                                 (void *(*)(PARCObject *))         _parcPropertiesIterator_Init,
                                                 (bool (*)(PARCObject *, void *))  _parcPropertiesIterator_HasNext,
                                                 (void *(*)(PARCObject *, void *)) _parcPropertiesIterator_Next,
                                                 (void (*)(PARCObject *, void **)) _parcPropertiesIterator_Remove,
                                                 (void *(*)(PARCObject *, void *)) _parcPropertiesIterator_Element,
                                                 (void  (*)(PARCObject *, void *)) _parcPropertiesIterator_Fini,
                                                 NULL);

    return iterator;
}
