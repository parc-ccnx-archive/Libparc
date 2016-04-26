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
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
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
