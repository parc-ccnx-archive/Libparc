/*
 * Copyright (c) 2013-2016, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @copyright 2013-2016, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>

#include <LongBow/runtime.h>

#include <parc/security/parc_KeyStore.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_Object.h>

struct parc_key_store {
    PARCObject *instance;
    const PARCKeyStoreInterface *interface;
};

static bool
_parcKeyStore_Destructor(PARCKeyStore **keyStorePtr)
{
    PARCKeyStore *keyStore = *keyStorePtr;
    if (keyStore->interface != NULL && keyStore->instance != NULL) {
        parcObject_Release(&keyStore->instance);
    }

    return true;
}

parcObject_Override(PARCKeyStore, PARCObject,
                    .destructor = (PARCObjectDestructor *) _parcKeyStore_Destructor);

parcObject_ImplementAcquire(parcKeyStore, PARCKeyStore);
parcObject_ImplementRelease(parcKeyStore, PARCKeyStore);

PARCKeyStore *
parcKeyStore_Create(PARCObject *instance, const PARCKeyStoreInterface *interface)
{
    PARCKeyStore *keyStore = parcObject_CreateInstance(PARCKeyStore);

    if (keyStore != NULL) {
        keyStore->instance = parcObject_Acquire(instance);
        keyStore->interface = interface;
    }

    return keyStore;
}

PARCCryptoHash *
parcKeyStore_GetVerifierKeyDigest(const PARCKeyStore *interfaceContext)
{
    if (interfaceContext->interface != NULL) {
        return interfaceContext->interface->getVerifierKeyDigest(interfaceContext->instance);
    }
    return NULL;
}

PARCCryptoHash *
parcKeyStore_GetCertificateDigest(const PARCKeyStore *interfaceContext)
{
    if (interfaceContext->interface != NULL) {
        return interfaceContext->interface->getCertificateDigest(interfaceContext->instance);
    }
    return NULL;
}

PARCBuffer *
parcKeyStore_GetDEREncodedCertificate(const PARCKeyStore *interfaceContext)
{
    if (interfaceContext->interface != NULL) {
        return interfaceContext->interface->getDEREncodedCertificate(interfaceContext->instance);
    }
    return NULL;
}

PARCBuffer *
parcKeyStore_GetDEREncodedPublicKey(const PARCKeyStore *interfaceContext)
{
    if (interfaceContext->interface != NULL) {
        return interfaceContext->interface->getDEREncodedPublicKey(interfaceContext->instance);
    }
    return NULL;
}

PARCBuffer *
parcKeyStore_GetDEREncodedPrivateKey(const PARCKeyStore *interfaceContext)
{
    if (interfaceContext->interface != NULL) {
        return interfaceContext->interface->getDEREncodedPrivateKey(interfaceContext->instance);
    }
    return NULL;
}
