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
 * @author Christopher A. Wood, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>

#include <LongBow/runtime.h>

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdarg.h>

#include <parc/algol/parc_URIPath.h>

#include <parc/algol/parc_URISegment.h>
#include <parc/algol/parc_ArrayList.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_Object.h>

struct parc_uri_path {
    PARCArrayList *segments;
};

static void
_parcURIPath_Finalize(PARCURIPath **pathPtr)
{
    assertNotNull(pathPtr, "Parameter must be a non-null pointer to a pointer to a PARCURIPath instance.");

    PARCURIPath *path = *pathPtr;
    if (path != NULL) {
        parcArrayList_Destroy(&path->segments);
    }
}

parcObject_ExtendPARCObject(PARCURIPath, _parcURIPath_Finalize, parcURIPath_Copy, parcURIPath_ToString, parcURIPath_Equals,
                            parcURIPath_Compare, NULL, NULL);

PARCURIPath *
parcURIPath_Create(void)
{
    PARCURIPath *result = parcObject_CreateInstance(PARCURIPath);
    return result;
}

parcObject_ImplementAcquire(parcURIPath, PARCURIPath);

parcObject_ImplementRelease(parcURIPath, PARCURIPath);

PARCURIPath *
parcURIPath_Append(PARCURIPath *path, const PARCURISegment *segment)
{
    parcArrayList_Add(path->segments, segment);
    return path;
}

PARCURIPath *
parcURIPath_Trim(PARCURIPath *path, size_t numberToRemove)
{
    size_t length = parcArrayList_Size(path->segments);
    if (numberToRemove <= length) {
        while (numberToRemove--) {
            parcArrayList_RemoveAndDestroyAtIndex(path->segments, length - 1);
            length--;
        }
    }
    return path;
}

//PARCURIPath *
//parcURIPath_Parse(const char *string, const char **pointer)
//{
//    PARCURIPath *result = parcURIPath_Create();
//
//    result->segments = parcArrayList_Create((void (*)(void **))parcURISegment_Release);
//    if (*string != 0) {
//        assertTrue(*string == '/', "Expected initial '/' character.");
//        *pointer = string;
//        while (**pointer != 0 && **pointer != '?' && **pointer != '#') {
//            PARCURISegment *segment = parcURISegment_Parse(++(*pointer), pointer);
//            parcURIPath_Append(result, segment);
//        }
//    }
//
//    return result;
//}

PARCURIPath *
parcURIPath_Parse(const char *string, const char **pointer)
{
    PARCURIPath *result = parcURIPath_Create();
    result->segments = parcArrayList_Create((void (*)(void **))parcURISegment_Release);

    if (*string != 0) {
        if (*string != '/') {
            parcURIPath_Release(&result);
            return NULL;
        }

        *pointer = string;
        while (**pointer != 0 && **pointer != '?' && **pointer != '#') {
            PARCURISegment *segment = parcURISegment_Parse(++(*pointer), pointer);
            parcURIPath_Append(result, segment);
        }
    }

    return result;
}

bool
parcURIPath_Equals(const PARCURIPath *pathA, const PARCURIPath *pathB)
{
    if (pathA == pathB) {
        return true;
    }
    if (pathA == NULL || pathB == NULL) {
        return false;
    }

    if (parcArrayList_Size(pathA->segments) == parcArrayList_Size(pathB->segments)) {
        for (size_t i = 0; i < parcArrayList_Size(pathA->segments); i++) {
            PARCURISegment *segmentA = parcArrayList_Get(pathA->segments, i);
            PARCURISegment *segmentB = parcArrayList_Get(pathB->segments, i);
            if (!parcURISegment_Equals(segmentA, segmentB)) {
                return false;
            }
        }
        return true;
    }
    return false;
}

PARCURIPath *
parcURIPath_Copy(const PARCURIPath *path)
{
    assertNotNull(path, "Parameter must be a non-null PARC_URIPath pointer.");

    PARCURIPath *result = parcURIPath_Create();
    result->segments = parcArrayList_Create((void (*)(void **))parcURISegment_Release);

    size_t nSegments = parcURIPath_Count(path);

    for (size_t i = 0; i < nSegments; i++) {
        PARCURISegment *segment = parcURIPath_Get(path, i);
        PARCURISegment *segmentCopy = parcURISegment_Clone(segment);
        parcURIPath_Append(result, segmentCopy);
    }

    return result;
}

PARCURIPath *
parcURIPath_ComposeValist(const PARCURIPath *basePath, va_list varargs)
{
    PARCURIPath *result = parcURIPath_Copy(basePath);

    for (PARCURISegment *segment = va_arg(varargs, PARCURISegment *); segment != NULL; segment = va_arg(varargs, PARCURISegment *)) {
        parcURIPath_Append(result, parcURISegment_Clone(segment));
    }

    return result;
}

PARCURIPath *
parcURIPath_Compose(const PARCURIPath *basePath, ...)
{
    va_list arglist;
    va_start(arglist, basePath);

    PARCURIPath *result = parcURIPath_ComposeValist(basePath, arglist);
    va_end(arglist);

    return result;
}

bool
parcURIPath_StartsWith(const PARCURIPath *base, const PARCURIPath *prefix)
{
    size_t prefixSegmentCount = parcURIPath_Count(prefix);
    size_t baseSegmentCount = parcURIPath_Count(base);

    if (baseSegmentCount < prefixSegmentCount) {
        return false;
    }

    for (size_t i = 0; i < prefixSegmentCount; i++) {
        PARCURISegment *baseSegment = parcURIPath_Get(base, i);
        PARCURISegment *prefixSegment = parcURIPath_Get(prefix, i);
        if (parcURISegment_Compare(baseSegment, prefixSegment) != 0) {
            return false;
        }
    }

    return true;
}

int
parcURIPath_Compare(const PARCURIPath *pathA, const PARCURIPath *pathB)
{
    if (pathA == NULL) {
        if (pathB == NULL) {
            return 0;
        }
        return -1;
    } else {
        if (pathB == NULL) {
            return +1;
        }
    }

    ssize_t countDifference = parcURIPath_Count(pathA) - parcURIPath_Count(pathB);

    if (countDifference != 0) {
        return (countDifference > 0 ? 1 : (countDifference < 0) ? -1 : 0);
    }

    size_t nSegments = parcURIPath_Count(pathA);

    for (size_t i = 0; i < nSegments; i++) {
        PARCURISegment *segmentA = parcURIPath_Get(pathA, i);
        PARCURISegment *segmentB = parcURIPath_Get(pathB, i);
        int comparison = parcURISegment_Compare(segmentA, segmentB);
        if (comparison != 0) {
            return comparison;
        }
    }
    return 0;
}

PARCURISegment *
parcURIPath_Get(const PARCURIPath *path, size_t index)
{
    return (PARCURISegment *) parcArrayList_Get(path->segments, index);
}

size_t
parcURIPath_Count(const PARCURIPath *path)
{
    size_t nSegments = parcArrayList_Size(path->segments);
    return nSegments;
}

size_t
parcURIPath_Length(const PARCURIPath *path)
{
    size_t result = 0;

    size_t nSegments = parcURIPath_Count(path);

    for (size_t i = 0; i < nSegments; i++) {
        PARCURISegment *segment = parcURIPath_Get(path, i);
        result += parcURISegment_Length(segment);
        if (i < (nSegments - 1)) {
            result++; // Include the size of the '/' separators.
        }
    }

    return result;
}

PARCBufferComposer *
parcURIPath_BuildString(const PARCURIPath *path, PARCBufferComposer *composer)
{
    size_t nSegments = parcArrayList_Size(path->segments);

    for (size_t i = 0; i < nSegments && composer != NULL; i++) {
        if (parcURISegment_BuildString(parcURIPath_Get(path, i), composer) == NULL) {
            composer = NULL;
        }
        if (i < (nSegments - 1)) {
            composer = parcBufferComposer_PutChar(composer, '/');
        }
    }

    return composer;
}

char *
parcURIPath_ToString(const PARCURIPath *path)
{
    char *result = NULL;

    PARCBufferComposer *composer = parcBufferComposer_Create();
    if (composer != NULL) {
        if (parcURIPath_BuildString(path, composer) != NULL) {
            PARCBuffer *tempBuffer = parcBufferComposer_ProduceBuffer(composer);
            result = parcBuffer_ToString(tempBuffer);
            parcBuffer_Release(&tempBuffer);
        }
        parcBufferComposer_Release(&composer);
    }

    return result;
}
