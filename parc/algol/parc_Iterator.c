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

#include <parc/algol/parc_Iterator.h>

struct parc_iterator {
    PARCObject *object;
    void *(*init)(PARCObject *);
    bool (*hasNext)(PARCObject *, void *);
    void *(*next)(PARCObject *, void *);
    void (*remove)(PARCObject *, void **);
    void *(*element)(PARCObject *, void *);
    void (*fini)(PARCObject *, void *);
    void (*assertValid)(const void *);
    bool initialized;
    void *state;
};

static void
_parcIterator_Destroy(PARCIterator **iteratorPtr)
{
    PARCIterator *iterator = *iteratorPtr;

    parcObject_Release(&(iterator->object));

    (iterator->fini(iterator->object, iterator->state));
}

parcObject_ExtendPARCObject(PARCIterator, _parcIterator_Destroy, NULL, NULL, NULL, NULL, NULL, NULL);

static void *
_parcIterator_Init(PARCIterator *iterator)
{
    if (iterator->init) {
        iterator->state = (iterator->init)(iterator->object);
    }

    if (iterator->assertValid) {
        (iterator->assertValid)(iterator->state);
    }

    iterator->initialized = true;
    return iterator->state;
}

static void
_parcIteratorState_AssertValid(const PARCIterator *iterator)
{
    if (iterator->assertValid) {
        (iterator->assertValid)(iterator->state);
    }
}

PARCIterator *
parcIterator_Create(PARCObject *object,
                    void *(*init)(PARCObject *),
                    bool (*hasNext)(PARCObject *, void *),
                    void *(*next)(PARCObject *, void *),
                    void (*remove)(PARCObject *, void **),
                    void *(*element)(PARCObject *, void *),
                    void (*fini)(PARCObject *, void *),
                    void (*assertValid)(const void *))
{
    assertNotNull(object, "PARCObject cannot be NULL.");
    assertNotNull(init, "'init' function cannot be NULL.");
    assertNotNull(hasNext, "'hasNext' function cannot be NULL.");
    assertNotNull(next, "'next' function cannot be NULL.");
    assertNotNull(element, "'element' function cannot be NULL.");
    assertNotNull(fini, "'fini' function cannot be NULL.");

    PARCIterator *result = parcObject_CreateInstance(PARCIterator);

    if (result != NULL) {
        result->object = parcObject_Acquire(object);
        result->init = init;
        result->hasNext = hasNext;
        result->next = next;
        result->remove = remove;
        result->element = element;
        result->fini = fini;
        result->assertValid = assertValid;

        result->initialized = false;
        _parcIterator_Init(result);
    }

    return result;
}

bool
parcIterator_IsValid(const PARCIterator *iterator)
{
    bool result = false;

    if (iterator != NULL) {
        if (parcObject_IsValid(iterator)) {
            result = true;
        }
    }

    return result;
}

void
parcIterator_AssertValid(const PARCIterator *iterator)
{
    assertTrue(parcIterator_IsValid(iterator), "PARCIterator is not valid.");
}

parcObject_ImplementAcquire(parcIterator, PARCIterator);

parcObject_ImplementRelease(parcIterator, PARCIterator);


void *
parcIterator_Next(PARCIterator *iterator)
{
    parcIterator_OptionalAssertValid(iterator);

    iterator->state = (iterator->next)(iterator->object, iterator->state);

    _parcIteratorState_AssertValid(iterator);

    return (iterator->element)(iterator->object, iterator->state);
}

bool
parcIterator_HasNext(const PARCIterator *iterator)
{
    parcIterator_OptionalAssertValid(iterator);

    return (iterator->hasNext)(iterator->object, iterator->state);
}

void
parcIterator_Remove(PARCIterator *iterator)
{
    if (iterator->remove != NULL) {
        (iterator->remove)(iterator->object, &iterator->state);
    }

    _parcIteratorState_AssertValid(iterator);
}
