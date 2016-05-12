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

#include <LongBow/runtime.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <parc/algol/parc_Deque.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_BufferComposer.h>

#include <parc/algol/parc_PathName.h>

struct parc_pathname {
    bool isAbsolute;
    PARCDeque *path;
};

static void
_destroy(PARCPathName **pathNamePtr)
{
    PARCPathName *pathName = *pathNamePtr;

    for (size_t i = 0; i < parcDeque_Size(pathName->path); i++) {
        void *name = parcDeque_GetAtIndex(pathName->path, i);
        parcMemory_Deallocate((void **) &name);
    }
    parcDeque_Release(&pathName->path);
}

static bool
_pathNameSegmentEquals(const void *x, const void *y)
{
    if (x == y) {
        return true;
    }
    if (x == NULL || y == NULL) {
        return false;
    }
    return strcmp((char *) x, (char *) y) == 0;
}

static void *
_pathNameSegmentCopy(const void *x)
{
    return parcMemory_StringDuplicate(x, strlen(x));
}

static const PARCObjectDescriptor parcPathNameSegment_ObjectInterface = {
    .destroy  = (PARCObjectDestroy *) NULL,
    .copy     = (PARCObjectCopy *) _pathNameSegmentCopy,
    .toString = (PARCObjectToString *) NULL,
    .equals   = (PARCObjectEquals *) _pathNameSegmentEquals,
    .compare  = (PARCObjectCompare *) NULL
};

parcObject_ExtendPARCObject(PARCPathName, _destroy, parcPathName_Copy, parcPathName_ToString,
                            parcPathName_Equals, NULL, NULL, NULL);

PARCPathName *
parcPathName_ParseToLimit(size_t limit, const char path[limit])
{
    PARCPathName *result = parcPathName_Create();

    if (limit > 0) {
        size_t index = 0;

        if (path[index] == '/') {
            result->isAbsolute = true;
            index++;
        }
        while (path[index] != 0 && index < limit) {
            while (path[index] == '/' && index < limit) {
                index++;
            }
            if (path[index] != 0 && index < limit) {
                size_t segment = index;
                while (path[index] != 0 && path[index] != '/' && index < limit) {
                    index++;
                }

                parcDeque_Append(result->path, parcMemory_StringDuplicate(&path[segment], index - segment));
            }
        }
    }

    return result;
}

PARCPathName *
parcPathName_Parse(const char *path)
{
    return parcPathName_ParseToLimit(strlen(path), path);
}

PARCPathName *
parcPathName_Create(void)
{
    PARCPathName *result = parcObject_CreateInstance(PARCPathName);

    result->isAbsolute = false;
    result->path = parcDeque_CreateObjectInterface(&parcPathNameSegment_ObjectInterface);
    return result;
}

parcObject_ImplementAcquire(parcPathName, PARCPathName);

parcObject_ImplementRelease(parcPathName, PARCPathName);

PARCPathName *
parcPathName_Copy(const PARCPathName *pathName)
{
    PARCPathName *result = parcObject_CreateInstance(PARCPathName);

    result->isAbsolute = pathName->isAbsolute;
    result->path = parcDeque_Copy(pathName->path);
    return result;
}

bool
parcPathName_Equals(const PARCPathName *x, const PARCPathName *y)
{
    if (x == y) {
        return true;
    }
    if (x == NULL || y == NULL) {
        return false;
    }

    if (x->isAbsolute == y->isAbsolute) {
        return parcDeque_Equals(x->path, y->path);
    }
    return false;
}

bool
parcPathName_IsAbsolute(const PARCPathName *pathName)
{
    return pathName->isAbsolute;
}

bool
parcPathName_MakeAbsolute(PARCPathName *pathName, bool absolute)
{
    bool result = parcPathName_IsAbsolute(pathName);
    pathName->isAbsolute = absolute;

    return result;
}

PARCPathName *
parcPathName_Prepend(PARCPathName *pathName, const char *name)
{
    parcDeque_Prepend(pathName->path, parcMemory_StringDuplicate(name, strlen(name)));
    return pathName;
}

PARCPathName *
parcPathName_Append(PARCPathName *pathName, const char *name)
{
    parcDeque_Append(pathName->path, parcMemory_StringDuplicate(name, strlen(name)));
    return pathName;
}

char *
parcPathName_GetAtIndex(const PARCPathName *pathName, size_t index)
{
    return (char *) parcDeque_GetAtIndex(pathName->path, index);
}

PARCPathName *
parcPathName_Head(const PARCPathName *pathName, size_t size)
{
    PARCPathName *result = parcPathName_Create();
    size_t maximum = parcPathName_Size(pathName) < size ? parcPathName_Size(pathName) : size;

    for (size_t i = 0; i < maximum; i++) {
        parcPathName_Append(result, parcPathName_GetAtIndex(pathName, i));
    }

    parcPathName_MakeAbsolute(result, parcPathName_IsAbsolute(pathName));

    return result;
}

PARCPathName *
parcPathName_Tail(const PARCPathName *pathName, size_t size)
{
    PARCPathName *result = parcPathName_Create();
    if (size > parcPathName_Size(pathName)) {
        size = parcPathName_Size(pathName);
    }

    for (size_t i = parcPathName_Size(pathName) - size; i < parcPathName_Size(pathName); i++) {
        parcPathName_Prepend(result, parcPathName_GetAtIndex(pathName, i));
    }

    parcPathName_MakeAbsolute(result, false);

    return result;
}

size_t
parcPathName_Size(const PARCPathName *pathName)
{
    return parcDeque_Size(pathName->path);
}

PARCBufferComposer *
parcPathName_BuildString(const PARCPathName *pathName, PARCBufferComposer *composer)
{
    char *separator = "/";

    //    an absolute path with no segments should just be '/'
    if (parcPathName_IsAbsolute(pathName)) {
        parcBufferComposer_PutString(composer, separator);
    }

    size_t length = parcDeque_Size(pathName->path);
    if (length > 0) {
        parcBufferComposer_PutString(composer, parcDeque_GetAtIndex(pathName->path, 0));
        for (size_t i = 1; i < length; i++) {
            parcBufferComposer_PutStrings(composer, separator, parcDeque_GetAtIndex(pathName->path, i), NULL);
        }
    }

    return composer;
}

char *
parcPathName_ToString(const PARCPathName *pathName)
{
    PARCBufferComposer *composer = parcBufferComposer_Create();
    parcPathName_BuildString(pathName, composer);

    PARCBuffer *tempBuffer = parcBufferComposer_ProduceBuffer(composer);
    char *result = parcBuffer_ToString(tempBuffer);
    parcBuffer_Release(&tempBuffer);

    parcBufferComposer_Release(&composer);

    return result;
}

void
parcPathName_Display(const PARCPathName *pathName, int indentation)
{
    if (pathName == NULL) {
        parcDisplayIndented_PrintLine(indentation, "PARCPathName@NULL\n");
    } else {
        parcDisplayIndented_PrintLine(indentation, "PARCPathName@%p {\n", (void *) pathName);
        parcDeque_Display(pathName->path, indentation + 1);
        parcDisplayIndented_PrintLine(indentation, "}\n");
    }
}
