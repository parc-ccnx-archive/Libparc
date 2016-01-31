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
#include <stdarg.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_OutputStream.h>

struct parc_output_stream {
    void *instance;
    const PARCOutputStreamInterface *interface;
};

static void
_destroy(PARCOutputStream **streamPtr)
{
    PARCOutputStream *stream = *streamPtr;
    (stream->interface->Release)((PARCOutputStream **) &stream->instance);
}

parcObject_ExtendPARCObject(PARCOutputStream, _destroy, NULL, NULL, NULL, NULL, NULL, NULL);

PARCOutputStream *
parcOutputStream_Create(void *instance, const PARCOutputStreamInterface *interface)
{
    PARCOutputStream *result = parcObject_CreateInstance(PARCOutputStream);
    result->instance = instance;
    result->interface = interface;

    return result;
}

parcObject_ImplementAcquire(parcOutputStream, PARCOutputStream);

parcObject_ImplementRelease(parcOutputStream, PARCOutputStream);

size_t
parcOutputStream_Write(PARCOutputStream *stream, PARCBuffer *buffer)
{
    return (stream->interface->Write)(stream->instance, buffer);
}

size_t
parcOutputStream_WriteCStrings(PARCOutputStream *stream, ...)
{
    va_list ap;
    va_start(ap, stream);

    size_t result = 0;

    for (char *string = va_arg(ap, char *); string != NULL; string = va_arg(ap, char *)) {
        result += parcOutputStream_WriteCString(stream, string);
    }

    return result;
}

size_t
parcOutputStream_WriteCString(PARCOutputStream *stream, const char *string)
{
    PARCBuffer *buffer = parcBuffer_WrapCString((char *) string);
    size_t result = (stream->interface->Write)(stream->instance, buffer);
    parcBuffer_Release(&buffer);
    return result;
}
