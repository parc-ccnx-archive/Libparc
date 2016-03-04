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
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
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
    return (list->interface->IsEmpty)(list->instance);
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
        trapNotImplemented("Clear");
    }
    (list->interface->Clear)(list->instance);
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

size_t
parcList_IndexOf(const PARCList *list, void *element)
{
    return (list->interface->IndexOf)(list->instance, element);
}

size_t
parcList_LastIndexOf(PARCList *list, void *element)
{
    return (list->interface->LastIndexOf)(list->instance, element);
}

void *
parcList_RemoveAtIndex(PARCList *list, size_t index)
{
    return (list->interface->RemoveAtIndex)(list->instance, index);
}

bool
parcList_Remove(PARCList *list, void *element)
{
    if (list->interface->Remove != NULL) {
        return (list->interface->Remove)(list->instance, element);
    }

    for (size_t i = 0; i < parcList_Size(list); i++) {
        void *e = parcList_GetAtIndex(list, i);
        if (e == element) {
            parcList_RemoveAtIndex(list, i);
            return true;
        }
    }
    return false;
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

void *
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
