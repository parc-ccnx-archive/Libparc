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
 * @author Glenn Scott , Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

#include <config.h>

#include <LongBow/runtime.h>

#include <parc/security/parc_KeyStore.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_Object.h>

struct parc_key_store {
    const char *fileName;
    const char *passWord;
};

void static
_finalize(PARCKeyStore **keyStorePtr)
{
    PARCKeyStore *keyStore = *keyStorePtr;
    parcMemory_Deallocate((void **) &(keyStore->fileName));
    parcMemory_Deallocate((void **) &(keyStore->passWord));
}


parcObject_ExtendPARCObject(PARCKeyStore, _finalize, NULL, NULL, NULL, NULL, NULL, NULL);

PARCKeyStore *
parcKeyStore_CreateFile(const char *fileName, const char *passWord)
{
    PARCKeyStore *instance = parcObject_CreateInstance(PARCKeyStore);
    instance->fileName = parcMemory_StringDuplicate(fileName, strlen(fileName));
    instance->passWord = parcMemory_StringDuplicate(passWord, strlen(passWord));

    return instance;
}

parcObject_ImplementAcquire(parcKeyStore, PARCKeyStore);

parcObject_ImplementRelease(parcKeyStore, PARCKeyStore);

const char *
parcKeyStore_GetFileName(const PARCKeyStore *keyStore)
{
    return keyStore->fileName;
}

const char *
parcKeyStore_GetPassWord(const PARCKeyStore *keyStore)
{
    return keyStore->passWord;
}
