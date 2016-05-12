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
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>

#include <stdio.h>
#include <string.h>

#include <LongBow/runtime.h>

#include <parc/security/parc_KeyId.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_Hash.h>

struct parc_keyid {
    PARCBuffer *keyid;
    PARCHashCode hashcode;
};

static void
_parcKeyId_Destroy(PARCKeyId **keyIdPtr)
{
    PARCKeyId *keyId = *keyIdPtr;

    parcBuffer_Release(&keyId->keyid);
}

parcObject_ExtendPARCObject(PARCKeyId, _parcKeyId_Destroy, NULL, NULL, NULL, NULL, NULL, NULL);

void
parcKeyId_AssertValid(const PARCKeyId *keyId)
{
    assertNotNull(keyId, "Pointer must be a non-null pointer to a PARCKeyId");
}

PARCKeyId *
parcKeyId_Create(PARCBuffer *preComputedKeyId)
{
    PARCKeyId *keyid = parcObject_CreateInstance(PARCKeyId);

    if (keyid != NULL) {
        keyid->keyid = parcBuffer_Acquire(preComputedKeyId);
        keyid->hashcode = parcBuffer_HashCode(preComputedKeyId);
    }
    return keyid;
}

parcObject_ImplementAcquire(parcKeyId, PARCKeyId);
parcObject_ImplementRelease(parcKeyId, PARCKeyId);

bool
parcKeyId_Equals(const PARCKeyId *keyidA, const PARCKeyId *keyidB)
{
    if (keyidA == keyidB) {
        return true;
    }

    if (keyidA == NULL || keyidB == NULL) {
        return false;
    }

    return parcBuffer_Equals(keyidA->keyid, keyidB->keyid);
}

PARCHashCode
parcKeyId_HashCode(const PARCKeyId *keyid)
{
    parcKeyId_OptionalAssertValid(keyid);

    return keyid->hashcode;
}

PARCHashCode
parcKeyId_HashCodeFromVoid(const void *keyid)
{
    parcKeyId_OptionalAssertValid(keyid);

    return ((PARCKeyId *) keyid)->hashcode;
}

const PARCBuffer *
parcKeyId_GetKeyId(const PARCKeyId *keyid)
{
    parcKeyId_OptionalAssertValid(keyid);

    return keyid->keyid;
}

PARCKeyId *
parcKeyId_Copy(const PARCKeyId *original)
{
    parcKeyId_OptionalAssertValid(original);

    PARCBuffer *bufferCopy = parcBuffer_Copy(original->keyid);
    PARCKeyId *result = parcKeyId_Create(bufferCopy);
    parcBuffer_Release(&bufferCopy);
    return result;
}

PARCBufferComposer *
parcKeyId_BuildString(const PARCKeyId *keyid, PARCBufferComposer *composer)
{
    // output format = "0x<hex>\00"
    parcBufferComposer_Format(composer, "0x");
    for (int i = 0; i < parcBuffer_Capacity(keyid->keyid); i += 2) {
        parcBufferComposer_Format(composer, "%02X", parcBuffer_GetAtIndex(keyid->keyid, i));
    }
    return composer;
}

char *
parcKeyId_ToString(const PARCKeyId *keyid)
{
    char *result = NULL;

    PARCBufferComposer *composer = parcBufferComposer_Create();
    if (composer != NULL) {
        parcKeyId_BuildString(keyid, composer);
        PARCBuffer *tempBuffer = parcBufferComposer_ProduceBuffer(composer);
        result = parcBuffer_ToString(tempBuffer);
        parcBuffer_Release(&tempBuffer);
        parcBufferComposer_Release(&composer);
    }
    return result;
}
