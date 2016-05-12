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

#include <inttypes.h>
#include <stdarg.h>

#include <parc/logging/parc_LogEntry.h>

#include <parc/algol/parc_Time.h>
#include <parc/algol/parc_BufferComposer.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_Object.h>
#include <parc/logging/parc_LogLevel.h>

static const char _parcLog_Version = 1;

struct PARCLogEntry {
    PARCLogLevel level;
    char version;
    struct timeval timeStamp;
    char *hostName;
    char *applicationName;
    char *processName;
    uint64_t messageId;

    PARCBuffer *payload;
};

static void
_parcLogEntry_Destroy(PARCLogEntry **entryPtr)
{
    PARCLogEntry *entry = *entryPtr;

    parcMemory_Deallocate((void **) &entry->hostName);
    parcMemory_Deallocate((void **) &entry->applicationName);
    parcMemory_Deallocate((void **) &entry->processName);
    parcBuffer_Release(&entry->payload);
}

static char *
_toString(const PARCLogEntry *entry)
{
    PARCBufferComposer *composer = parcBufferComposer_Create();

    parcBufferComposer_Format(composer, "%ld.%06d %d ",
                              (long) entry->timeStamp.tv_sec, (int) entry->timeStamp.tv_usec, entry->level);

    size_t position = parcBuffer_Position(entry->payload);
    parcBufferComposer_PutBuffer(composer, entry->payload);
    parcBuffer_SetPosition(entry->payload, position);

    PARCBuffer *buffer = parcBufferComposer_GetBuffer(composer);
    parcBuffer_Rewind(buffer);

    char *result = parcBuffer_ToString(buffer);
    parcBufferComposer_Release(&composer);

    return result;
}

parcObject_ExtendPARCObject(PARCLogEntry, _parcLogEntry_Destroy, NULL, _toString, NULL, NULL, NULL, NULL);

PARCLogEntry *
parcLogEntry_Create(PARCLogLevel level,
                    const char *hostName,
                    const char *applicationName,
                    const char *processName,
                    const uint64_t messageId,
                    const struct timeval timeStamp,
                    PARCBuffer *payload)
{
    PARCLogEntry *result = parcObject_CreateInstance(PARCLogEntry);
    if (result == NULL) {
        trapOutOfMemory("Creating an instance of PARCLogEntry.");
    }
    result->version = _parcLog_Version;
    result->timeStamp = timeStamp;
    result->hostName = parcMemory_StringDuplicate(hostName, strlen(hostName));
    result->applicationName = parcMemory_StringDuplicate(applicationName, strlen(applicationName));
    result->processName = parcMemory_StringDuplicate(processName, strlen(processName));
    result->messageId = messageId;
    result->level = level;
    result->payload = parcBuffer_Acquire(payload);

    return result;
}

parcObject_ImplementAcquire(parcLogEntry, PARCLogEntry);

parcObject_ImplementRelease(parcLogEntry, PARCLogEntry);

PARCBuffer *
parcLogEntry_GetPayload(const PARCLogEntry *instance)
{
    return instance->payload;
}

const struct timeval *
parcLogEntry_GetTimeStamp(const PARCLogEntry *instance)
{
    return &instance->timeStamp;
}

PARCLogLevel
parcLogEntry_GetLevel(const PARCLogEntry *instance)
{
    return instance->level;
}

int
parcLogEntry_GetVersion(const PARCLogEntry *instance)
{
    return instance->version;
}

const char *
parcLogEntry_GetHostName(const PARCLogEntry *instance)
{
    return instance->hostName;

}

const char *
parcLogEntry_GetApplicationName(const PARCLogEntry *instance)
{
    return instance->applicationName;
}

const char *
parcLogEntry_GetProcessName(const PARCLogEntry *instance)
{
    return instance->processName;
}

uint64_t
parcLogEntry_GetMessageId(const PARCLogEntry *instance)
{
    return instance->messageId;
}

char *
parcLogEntry_ToString(const PARCLogEntry *entry)
{
    return _toString(entry);
}
