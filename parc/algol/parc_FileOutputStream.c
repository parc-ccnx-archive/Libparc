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
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>

#include <stdio.h>
#include <unistd.h>

#include <LongBow/runtime.h>

#include <parc/algol/parc_FileOutputStream.h>
#include <parc/algol/parc_Object.h>

PARCOutputStreamInterface *PARCFileOutputStreamAsPARCInputStream = &(PARCOutputStreamInterface) {
    .Acquire = (PARCOutputStream * (*)(PARCOutputStream *))parcFileOutputStream_Acquire,
    .Release = (void (*)(PARCOutputStream **))parcFileOutputStream_Release,
    .Write = (size_t (*)(PARCOutputStream *, PARCBuffer *))parcFileOutputStream_Write
};

struct parc_file_output_stream {
    int fd;
};

static void
_destroy(PARCFileOutputStream **streamPtr)
{
    PARCFileOutputStream *stream = *streamPtr;

    close(stream->fd);
}

parcObject_ExtendPARCObject(PARCFileOutputStream, _destroy, NULL, NULL, NULL, NULL, NULL, NULL);

PARCFileOutputStream *
parcFileOutputStream_Create(int fileDescriptor)
{
    assertTrue(fileDescriptor != -1, "Invalid file descriptor");

    PARCFileOutputStream *result = parcObject_CreateInstance(PARCFileOutputStream);
    result->fd = fileDescriptor;

    return result;
}

PARCOutputStream *
parcFileOutputStream_AsOutputStream(PARCFileOutputStream *fileOutputStream)
{
    return parcOutputStream_Create(parcFileOutputStream_Acquire(fileOutputStream), PARCFileOutputStreamAsPARCInputStream);
}

parcObject_ImplementAcquire(parcFileOutputStream, PARCFileOutputStream);

parcObject_ImplementRelease(parcFileOutputStream, PARCFileOutputStream);

bool
parcFileOutputStream_Write(PARCFileOutputStream *outputStream, PARCBuffer *buffer)
{
    const size_t maximumChunkSize = 1024 * 1024;

    while (parcBuffer_HasRemaining(buffer)) {
        size_t remaining = parcBuffer_Remaining(buffer);
        size_t chunkSize = remaining > maximumChunkSize ? maximumChunkSize : remaining;
        void *buf = parcBuffer_Overlay(buffer, chunkSize);
        ssize_t nwritten = write(outputStream->fd, buf, chunkSize);
        if (nwritten == -1) {
            break;
        }
    }

    return parcBuffer_HasRemaining(buffer) == false;
}
