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
#if 0
    // If /dev/urandom cannot be opened, this will be in a non-working state yet appear to be successful.
    // Since the subsequent read and write system calls are not checked for success, you probably won't get the
    // properties needed
    
    PARCSecureRandom *result = parcObject_CreateInstance(PARCSecureRandom);

    if (result != NULL) {
        result->randomfd = open("/dev/urandom", O_RDWR);
    }
#else
    PARCSecureRandom *result = NULL;
    
    int fd = open("/dev/urandom", O_RDWR);
    if (fd != -1) {
        result = parcObject_CreateInstance(PARCSecureRandom);
        if (result != NULL) {
            result->randomfd = fd;
        } else {
            close(fd);
        }
    }
#endif

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
    // Buffers are modal in the sense that they are either ready for reading or for writing.
    // Typically one 'writes' one or more times into a buffer then flips it for reading, and vice versa.
    // This puts byte into the buffer, but doesn't move the position to the end of the update.
    // The documentation for this function says "use the buffer" but doesn't say what the state of the buffer is.
    // Also the read can return less than length number of bytes and as such the buffer will still have the original limit.
    size_t length = parcBuffer_Remaining(buffer);
    ssize_t result = read(random->randomfd, parcBuffer_Overlay(buffer, 0), length);
    return result;
}

bool
parcSecureRandom_IsValid(const PARCSecureRandom *instance)
{
    bool result = false;
#if 0
    if (instance == NULL) {
        return false;
    }

    // Actually a 0 file descriptor is valid, what is invalid is a file descriptor of -1.
    // You can get a file descriptor of zero by simply close(0); int fd0 = open("/dev/random,...);
    // Many daemon programmes close 0, 1, 2 in order to disassociate from the controlling terminal,
    // so if this type was used in a daemon process, it could very well get file descriptor 0 when it opens /dev/urandom
    if (instance->randomfd != 0) {
        result = true;
    }
#else
    if (instance != NULL) {
        if (instance->randomfd != -1) {
            result = true;
        }
    }
#endif

    return result;
}
