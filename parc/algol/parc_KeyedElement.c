/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 *
 * @author Nacho "Ignacio" Solis, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

#include <config.h>

#include <string.h>

#include <LongBow/runtime.h>

#include <parc/algol/parc_KeyedElement.h>
#include <parc/algol/parc_Memory.h>

struct parc_keyed_element {
    size_t keylen;
    void *key;
    void *element;
};

PARCKeyedElement *
parcKeyedElement_Create(void *data, const void *key, size_t keylen)
{
    PARCKeyedElement *keyedElement = parcMemory_Allocate(sizeof(PARCKeyedElement));
    assertNotNull(keyedElement, "parcMemory_Allocate(%zu) returned NULL", sizeof(PARCKeyedElement));
    keyedElement->element = data;
    keyedElement->key = parcMemory_Allocate(sizeof(PARCKeyedElement));
    assertNotNull(keyedElement->key, "parcMemory_Allocate(%zu) returned NULL", sizeof(PARCKeyedElement));
    memcpy(keyedElement->key, key, keylen);
    keyedElement->keylen = keylen;
    return keyedElement;
}

void
parcKeyedElement_Destroy(PARCKeyedElement **keyedElementPointer)
{
    parcMemory_Deallocate((void **) &((*keyedElementPointer)->key));
    parcMemory_Deallocate((void **) keyedElementPointer);
    *keyedElementPointer = NULL;
}

void
parcKeyedElement_SetData(PARCKeyedElement *keyedElement, void *data)
{
    keyedElement->element = data;
}

void *
parcKeyedElement_GetData(PARCKeyedElement *keyedElement)
{
    return keyedElement->element;
}

void *
parcKeyedElement_GetKey(PARCKeyedElement *keyedElement)
{
    return keyedElement->key;
}

size_t
parcKeyedElement_GetKeyLen(PARCKeyedElement *keyedElement)
{
    return keyedElement->keylen;
}
