/*
 * Copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>

#include <LongBow/runtime.h>

#include <stdio.h>
#include <stdarg.h>

#include <parc/algol/parc_Object.h>

#include <parc/algol/parc_List.h>
#include <parc/algol/parc_Memory.h>

struct parc_list {
    void *instance;
    const PARCListInterface *interface;
};

static void
_destroy(PARCList **listPtr)
{
    PARCList *list = *listPtr;

    (list->interface->Destroy)(&list->instance);
}

parcObject_ExtendPARCObject(PARCList, _destroy, parcList_Copy, NULL, parcList_Equals, NULL, parcList_HashCode, NULL);

PARCList *
parcList(void *instance, PARCListInterface *interface)
{
    PARCList *result = parcObject_CreateInstance(PARCList);
    if (result != NULL) {
        result->instance = instance;
        result->interface = interface;
    }

    return result;
}

PARCList *
parcList_Create(PARCObject *instance, PARCListInterface *interface)
{
    PARCList *result = parcObject_CreateInstance(PARCList);
    if (result != NULL) {
        result->instance = parcObject_Acquire(instance);
        result->interface = interface;
    }
    
    return result;
}

parcObject_ImplementAcquire(parcList, PARCList);

parcObject_ImplementRelease(parcList, PARCList);

PARCList *
parcList_Copy(const PARCList *list)
{
    PARCList *result = parcObject_CreateInstance(PARCList);
    if (result != NULL) {
        result->instance = (list->interface->Copy)(list->instance);
        result->interface = list->interface;
    }

    return result;
}

bool
parcList_IsEmpty(const PARCList *list)
{
    bool result = false;
    if (list->interface->IsEmpty) {
        result = (list->interface->IsEmpty)(list->instance);
    } else {
        result = (parcList_Size(list) == 0);
    }
    return result;
}

bool
parcList_Add(PARCList *list, void *element)
{
    return (list->interface->Add)(list->instance, element);
}

bool
parcList_AddAll(PARCList *list, size_t argc, void *argv[argc])
{
    for (int i = 0; i < argc; i++) {
        (list->interface->Add)(list->instance, argv[i]);
    }
    return true;
}

void
parcList_AddAtIndex(PARCList *list, int index, void *element)
{
    (list->interface->AddAtIndex)(list->instance, index, element);
}

bool
parcList_AddCollection(PARCList *list, PARCCollection *collection)
{
    return (list->interface->AddCollection)(list->instance, collection);
}

bool
parcList_AddCollectionAtIndex(PARCList *list, int index, PARCCollection *collection)
{
    return (list->interface->AddCollectionAtIndex)(list->instance, index, collection);
}

void
parcList_Clear(PARCList *list)
{
    if (!list->interface->Clear) {
        for (size_t i = 0; i < parcList_Size(list); i++) {
            parcList_RemoveAtIndex(list, i);
        }
    } else {
        (list->interface->Clear)(list->instance);
    }
}

bool
parcList_Contains(const PARCList *list, void *element)
{
    return (list->interface->Contains)(list->instance, element);
}

bool
parcList_ContainsCollection(PARCList *list, PARCCollection *collection)
{
    return (list->interface->ContainsCollection)(list->instance, collection);
}

bool
parcList_Equals(const PARCList *x, const PARCList *y)
{
    return (x->interface->Equals)(x->instance, y->instance);
}

void *
parcList_GetAtIndex(const PARCList *list, size_t index)
{
    return (list->interface->GetAtIndex)(list->instance, index);
}

int
parcList_HashCode(const PARCList *list)
{
    return (list->interface->HashCode)(list->instance);
}

ssize_t
parcList_IndexOf(const PARCList *list, PARCObject *element)
{
    ssize_t result = -1;
    
    if (list->interface->IndexOf) {
        result = (list->interface->IndexOf)(list->instance, element);
    } else {
        for (ssize_t i = 0; i < parcList_Size(list); i++) {
            PARCObject *e = parcList_GetAtIndex(list, i);
            if (parcObject_Equals(e, element)) {
                result = i;
                break;
            }
        }
    }
    
    return result;
}

ssize_t
parcList_LastIndexOf(const PARCList *list, PARCObject *element)
{
    ssize_t result = -1;
    
    if (list->interface->LastIndexOf) {
        result = (list->interface->LastIndexOf)(list->instance, element);
    } else {
        for (ssize_t i = parcList_Size(list) - 1; i >= 0; i--) {
            PARCObject *e = parcList_GetAtIndex(list, i);
            if (parcObject_Equals(e, element)) {
                result = i;
                break;
            }
        }
    }
    
    return result;
}

PARCObject *
parcList_RemoveAtIndex(PARCList *list, size_t index)
{
    if (list->interface->RemoveAtIndex) {
        return (list->interface->RemoveAtIndex)(list->instance, index);
    } else {
        return NULL;        
    }
}

bool
parcList_Remove(PARCList *list, PARCObject *element)
{
    bool result = false;
    
    if (list->interface->Remove != NULL) {
        result = (list->interface->Remove)(list->instance, element);
    } else {
        for (size_t i = 0; i < parcList_Size(list); i++) {
            void *e = parcList_GetAtIndex(list, i);
            if (parcObject_Equals(e, element)) {
                parcList_RemoveAtIndex(list, i);
                result = true;
                break;
            }
        }
    }
    
    return result;
}

bool
parcList_RemoveCollection(PARCList *list, PARCCollection *collection)
{
    return (list->interface->RemoveCollection)(list->instance, collection);
}

bool
parcList_RetainCollection(PARCList *list, PARCCollection *collection)
{
    return (list->interface->RetainCollection)(list->instance, collection);
}

PARCObject *
parcList_SetAtIndex(PARCList *list, size_t index, void *element)
{
    return (list->interface->SetAtIndex)(list->instance, index, element);
}

size_t
parcList_Size(const PARCList *list)
{
    return (list->interface->Size)(list->instance);
}

PARCList *
parcList_SubList(PARCList *list, size_t fromIndex, size_t toIndex)
{
    return (list->interface->SubList)(list->instance, fromIndex, toIndex);
}

void**
parcList_ToArray(PARCList *list)
{
    return (list->interface->ToArray)(list->instance);
}
