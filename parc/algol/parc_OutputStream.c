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
