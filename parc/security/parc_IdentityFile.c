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
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>

#include <LongBow/runtime.h>

#include <sys/stat.h>
#include <unistd.h>

#include <parc/security/parc_Identity.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>

#include <parc/security/parc_Pkcs12KeyStore.h>
#include <parc/security/parc_PublicKeySigner.h>

#include <parc/security/parc_IdentityFile.h>

struct parc_identity_file {
    const char *fileName;
    const char *passWord;
};

PARCIdentityInterface *PARCIdentityFileAsPARCIdentity = &(PARCIdentityInterface) {
    .Acquire        = (PARCIdentity * (*)(void *))              parcIdentityFile_Acquire,
    .Release        = (void (*)(void **))                       parcIdentityFile_Release,
    .GetPassWord    = (void *(*)(const void *))                 parcIdentityFile_GetPassWord,
    .GetFileName    = (void *(*)(const void *))                 parcIdentityFile_GetFileName,
    .GetSigner      = (PARCSigner *(*)(const void *))           parcIdentityFile_CreateSigner,
    .Equals         = (bool (*)(const void *, const void *))    parcIdentityFile_Equals,
    .Display        = (void (*)(const void *, size_t))          parcIdentityFile_Display
};

void static
_finalize(PARCIdentityFile **IdentityPtr)
{
    PARCIdentityFile *identity = *IdentityPtr;
    parcMemory_Deallocate((void **) &(identity->fileName));
    parcMemory_Deallocate((void **) &(identity->passWord));
}


parcObject_ExtendPARCObject(PARCIdentityFile, _finalize, NULL, NULL, NULL, NULL, NULL, NULL);

PARCIdentityFile *
parcIdentityFile_Create(const char *fileName, const char *passWord)
{
    PARCIdentityFile *instance = parcObject_CreateInstance(PARCIdentityFile);

    if (instance != NULL) {
        instance->fileName = parcMemory_StringDuplicate(fileName, strlen(fileName));
        instance->passWord = parcMemory_StringDuplicate(passWord, strlen(passWord));
    }

    return instance;
}

parcObject_ImplementAcquire(parcIdentityFile, PARCIdentityFile);

parcObject_ImplementRelease(parcIdentityFile, PARCIdentityFile);

bool
parcIdentityFile_Exists(const PARCIdentityFile *identity)
{
    bool result = false;

    struct stat statbuf;

    if (stat(parcIdentityFile_GetFileName(identity), &statbuf) != -1) {
        if (S_ISREG(statbuf.st_mode)) {
            result = (access(parcIdentityFile_GetFileName(identity), F_OK | R_OK) == 0);
        }
    }

    return result;
}

const char *
parcIdentityFile_GetFileName(const PARCIdentityFile *identity)
{
    return identity->fileName;
}

const char *
parcIdentityFile_GetPassWord(const PARCIdentityFile *identity)
{
    return identity->passWord;
}

PARCSigner *
parcIdentityFile_CreateSigner(const PARCIdentityFile *identity)
{
    PARCPkcs12KeyStore *keyStore = parcPkcs12KeyStore_Open(identity->fileName, identity->passWord, PARCCryptoHashType_SHA256);
    PARCKeyStore *publicKeyStore = parcKeyStore_Create(keyStore, PARCPkcs12KeyStoreAsKeyStore);
    parcPkcs12KeyStore_Release(&keyStore);

    PARCPublicKeySigner *signer = parcPublicKeySigner_Create(publicKeyStore, PARCSigningAlgorithm_RSA, PARCCryptoHashType_SHA256);
    PARCSigner *pkSigner = parcSigner_Create(signer, PARCPublicKeySignerAsSigner);
    parcPublicKeySigner_Release(&signer);
    parcKeyStore_Release(&publicKeyStore);

    return pkSigner;
}

bool
parcIdentityFile_Equals(const PARCIdentityFile *a, const PARCIdentityFile *b)
{
    if (a == b) {
        return true;
    }
    if (a == NULL || b == NULL) {
        return false;
    }
    if (strcmp(parcIdentityFile_GetFileName(a), parcIdentityFile_GetFileName(b)) != 0) {
        return false;
    }
    if (strcmp(parcIdentityFile_GetPassWord(a), parcIdentityFile_GetPassWord(b)) != 0) {
        return false;
    }
    return true;
}

void
parcIdentityFile_Display(const PARCIdentityFile *instance, int indentation)
{
    parcDisplayIndented_PrintLine(indentation, "PARCIdentityFile@%p {", instance);
    parcDisplayIndented_PrintLine(indentation + 1, ".fileName='%s', .passWord='%s'", instance->fileName, instance->passWord);
    parcDisplayIndented_PrintLine(indentation, "}", instance);
}
