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

#include "parc_SecureRandom.h"

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
                    .destructor = (PARCObjectDestructor *) _parcSecureRandom_Destructor,
                    .copy = (PARCObjectCopy *) parcSecureRandom_Copy,
                    .toString = (PARCObjectToString *) parcSecureRandom_ToString,
                    .equals = (PARCObjectEquals *) parcSecureRandom_Equals,
                    .compare = (PARCObjectCompare *) parcSecureRandom_Compare,
                    .hashCode = (PARCObjectHashCode *) parcSecureRandom_HashCode,
                    .toJSON = (PARCObjectToJSON *) parcSecureRandom_ToJSON);

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
    PARCSecureRandom *result = parcObject_CreateInstance(PARCSecureRandom);

    if (result != NULL) {
        result->randomfd = open("/dev/urandom", O_RDWR);
        _parcSecureRandom_ReSeed(result, seed);
    }

    return result;
}

uint32_t
parcSecureRandom_Next(PARCSecureRandom *random)
{
    uint32_t value;
    ssize_t result = read(random->randomfd, &value, sizeof(value));
    return result;
}

int
parcSecureRandom_NextBytes(PARCSecureRandom *random, PARCBuffer *buffer)
{
    size_t length = parcBuffer_Remaining(buffer);
    ssize_t result = read(random->randomfd, parcBuffer_Overlay(buffer, length), length);
    return result;
}

int
parcSecureRandom_Compare(const PARCSecureRandom *instance, const PARCSecureRandom *other)
{
    int result = 0;

    return result;
}

PARCSecureRandom *
parcSecureRandom_Copy(const PARCSecureRandom *original)
{
    PARCSecureRandom *result = NULL;

    return result;
}

void
parcSecureRandom_Display(const PARCSecureRandom *instance, int indentation)
{
    parcDisplayIndented_PrintLine(indentation, "PARCSecureRandom@%p {", instance);
    /* Call Display() functions for the fields here. */
    parcDisplayIndented_PrintLine(indentation, "}");
}

bool
parcSecureRandom_Equals(const PARCSecureRandom *x, const PARCSecureRandom *y)
{
    bool result = false;

    if (x == y) {
        result = true;
    } else if (x == NULL || y == NULL) {
        result = false;
    } else {
        /* perform instance specific equality tests here. */
    }

    return result;
}

PARCHashCode
parcSecureRandom_HashCode(const PARCSecureRandom *instance)
{
    PARCHashCode result = 0;

    return result;
}

bool
parcSecureRandom_IsValid(const PARCSecureRandom *instance)
{
    bool result = false;

    if (instance != NULL) {
        result = true;
    }

    return result;
}

PARCJSON *
parcSecureRandom_ToJSON(const PARCSecureRandom *instance)
{
    PARCJSON *result = parcJSON_Create();

    if (result != NULL) {

    }

    return result;
}

char *
parcSecureRandom_ToString(const PARCSecureRandom *instance)
{
    char *result = parcMemory_Format("PARCSecureRandom@%p\n", instance);

    return result;
}
