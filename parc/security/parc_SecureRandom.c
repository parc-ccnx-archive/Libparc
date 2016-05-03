/*
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * Copyright 2015 Palo Alto Research Center, Inc. (PARC), a Xerox company.  All Rights Reserved.
 * The content of this file, whole or in part, is subject to licensing terms.
 * If distributing this software, include this License Header Notice in each
 * file and provide the accompanying LICENSE file.
 */
/**
 * @author Christopher A. Wood, Computing Science Laboratory, PARC
 * @copyright 2016 Palo Alto Research Center, Inc. (PARC), A Xerox Company.  All Rights Reserved.
 */
#include <config.h>

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_Memory.h>

#include <parc/security/parc_SecureRandom.h>

struct parc_securerandom {
    int randomfd;
};

static bool
_parcSecureRandom_Destructor(PARCSecureRandom **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCSecureRandom pointer.");
    PARCSecureRandom *instance = *instancePtr;

    close(instance->randomfd);

    return true;
}

parcObject_ImplementAcquire(parcSecureRandom, PARCSecureRandom);
parcObject_ImplementRelease(parcSecureRandom, PARCSecureRandom);
parcObject_Override(PARCSecureRandom, PARCObject,
                    .destructor = (PARCObjectDestructor *) _parcSecureRandom_Destructor);

void
parcSecureRandom_AssertValid(const PARCSecureRandom *instance)
{
    assertTrue(parcSecureRandom_IsValid(instance),
               "PARCSecureRandom is not valid.");
}

PARCSecureRandom *
parcSecureRandom_Create()
{
    PARCSecureRandom *result = parcObject_CreateInstance(PARCSecureRandom);

    if (result != NULL) {
        result->randomfd = open("/dev/urandom", O_RDWR);
    }

    return result;
}

static void
_parcSecureRandom_ReSeed(PARCSecureRandom *random, PARCBuffer *buffer)
{
    size_t length = parcBuffer_Remaining(buffer);
    write(random->randomfd, parcBuffer_Overlay(buffer, length), length);
}

PARCSecureRandom *
parcSecureRandom_CreateWithSeed(PARCBuffer *seed)
{
    PARCSecureRandom *result = parcSecureRandom_Create();

    if (result != NULL) {
        _parcSecureRandom_ReSeed(result, seed);
    }

    return result;
}

uint32_t
parcSecureRandom_Next(PARCSecureRandom *random)
{
    uint32_t value;
    read(random->randomfd, &value, sizeof(value));
    return value;
}

ssize_t
parcSecureRandom_NextBytes(PARCSecureRandom *random, PARCBuffer *buffer)
{
    size_t length = parcBuffer_Remaining(buffer);
    ssize_t result = read(random->randomfd, parcBuffer_Overlay(buffer, 0), length);
    return result;
}

bool
parcSecureRandom_IsValid(const PARCSecureRandom *instance)
{
    bool result = false;

    if (instance == NULL) {
        return false;
    }

    if (instance->randomfd != 0) {
        result = true;
    }

    return result;
}
