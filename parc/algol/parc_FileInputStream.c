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

#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/stat.h>

#include <LongBow/runtime.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_File.h>
#include <parc/algol/parc_Buffer.h>
#include <parc/algol/parc_InputStream.h>
#include <parc/algol/parc_FileInputStream.h>
#include <parc/algol/parc_Object.h>

PARCInputStreamInterface *PARCFileInputStreamAsPARCInputStream = &(PARCInputStreamInterface) {
    .Acquire = (PARCInputStream * (*)(const PARCInputStream *))parcFileInputStream_Acquire,
    .Release = (void (*)(PARCInputStream **))parcFileInputStream_Release,
    .Read = (size_t (*)(PARCInputStream *, PARCBuffer *))parcFileInputStream_Read
};

struct parc_file_input_stream {
    int fd;
};

static void
_destroy(PARCFileInputStream **inputStreamPtr)
{
    PARCFileInputStream *inputStream = *inputStreamPtr;

    close(inputStream->fd);
}

parcObject_ExtendPARCObject(PARCFileInputStream, _destroy, NULL, NULL, NULL, NULL, NULL, NULL);

PARCFileInputStream *
parcFileInputStream_Open(const PARCFile *file)
{
    PARCFileInputStream *result = NULL;

    char *fileName = parcFile_ToString(file);
    if (fileName != NULL) {
        result = parcFileInputStream_Create(open(fileName, O_RDONLY));
        parcMemory_Deallocate((void **) &fileName);
    }

    return result;
}

PARCFileInputStream *
parcFileInputStream_Create(int fileDescriptor)
{
    trapIllegalValueIf(fileDescriptor < 0, "File descriptor must not be negative.");

    PARCFileInputStream *result = parcObject_CreateInstance(PARCFileInputStream);
    if (result != NULL) {
        result->fd = fileDescriptor;
    }

    return result;
}

parcObject_ImplementAcquire(parcFileInputStream, PARCFileInputStream);
parcObject_ImplementRelease(parcFileInputStream, PARCFileInputStream);

bool
parcFileInputStream_Read(PARCFileInputStream *inputStream, PARCBuffer *buffer)
{
    while (parcBuffer_HasRemaining(buffer)) {
        void *buf = parcBuffer_Overlay(buffer, 0);
        ssize_t nread = read(inputStream->fd, buf, parcBuffer_Remaining(buffer));
        if (nread < 0) {
            break;
        }
        parcBuffer_SetPosition(buffer, parcBuffer_Position(buffer) + nread);
    }
    return parcBuffer_HasRemaining(buffer);
}

PARCBuffer *
parcFileInputStream_ReadFile(PARCFileInputStream *inputStream)
{
    PARCBuffer *result = NULL;

    struct stat statbuf;

    if (fstat(inputStream->fd, &statbuf) == 0) {
        result = parcBuffer_Allocate(statbuf.st_size);
        if (result != NULL) {
            parcFileInputStream_Read(inputStream, result);
        }
    }

    return result;
}
