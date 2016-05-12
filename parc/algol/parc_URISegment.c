/*
 * Copyright (c) 2013-2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @copyright (c) 2013-2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>

#include <LongBow/runtime.h>

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdarg.h>

#include <parc/algol/parc_URISegment.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_Object.h>

struct parc_uri_segment {
    PARCBuffer *buffer;
    bool requiresEscaping;
};

static char *hexDigitsUpper = "0123456789ABCDEF";
static char *hexDigitsLower = "0123456789abcdef";

// Given a value, return the low nibble as a hex character.
static char
_toHexDigit(const char value)
{
    return hexDigitsUpper[value & 0xF];
}

/**
 * @function _fromHexDigit
 * @abstract Given a hex character, return the decimal value.
 * @discussion
 *   Given a simple character containing the ASCII value for a hexadecimal digit, return the actual value.
 *
 * @param hexDigit A hexadecimal digit as a character from the set of characters <code>0123456789ABCDEFabcdef</code>
 * @return Return the decimal value of the given hex character.  If not a hex character return a value > 15.
 */
static char
_fromHexDigit(const char hexDigit)
{
    for (char i = 0; i < 16; i++) {
        if (hexDigit == hexDigitsLower[(int) i] || hexDigit == hexDigitsUpper[(int) i]) {
            return i;
        }
    }
    return -1;
}

static const char *
_parsePercentEncoded(const char *string, unsigned char *value)
{
    char c = *string++;
    if (c != 0) {
        unsigned char hi = _fromHexDigit(c);
        if (hi > 15) {
            return NULL;
        }
        c = *string++;
        if (c != 0) {
            unsigned char lo = _fromHexDigit(c);
            if (lo > 15) {
                return NULL;
            }
            *value = (unsigned char) (hi << 4 | lo);
            return string;
        }
    }
    return NULL;
}

#define uriPlainSegmentChar(c) (c != 0 && index("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~", c) != NULL)

static PARCBufferComposer *
_parcURISegment_BuildString(const PARCURISegment *segment, PARCBufferComposer *composer)
{
    assertNotNull(composer, "Parameter must be a non-null pointer to a PARCBufferComposer.");

    for (size_t i = 0; i < parcBuffer_Limit(segment->buffer) && composer != NULL; i++) {
        unsigned char c = parcBuffer_GetAtIndex(segment->buffer, i);
        if (uriPlainSegmentChar(c)) {
            parcBufferComposer_PutChar(composer, c);
        } else {
            parcBufferComposer_PutChar(composer, '%');
            parcBufferComposer_PutChar(composer, _toHexDigit(c >> 4));
            parcBufferComposer_PutChar(composer, _toHexDigit(c));
        }
    }

    return composer;
}

static void
_parcURISegment_Finalize(PARCURISegment **segmentPtr)
{
    PARCURISegment *segment = *segmentPtr;

    parcBuffer_Release(&(segment->buffer));
}

parcObject_ExtendPARCObject(PARCURISegment, _parcURISegment_Finalize, parcURISegment_Clone, parcURISegment_ToString,
                            parcURISegment_Equals, parcURISegment_Compare, NULL, NULL);

PARCURISegment *
parcURISegment_CreateFromBuffer(PARCBuffer *buffer)
{
    PARCURISegment *result = parcObject_CreateInstance(PARCURISegment);
    if (result != NULL) {
        result->buffer = parcBuffer_Acquire(buffer);
    }
    return result;
}

PARCURISegment *
parcURISegment_Create(size_t length, const unsigned char segment[length])
{
    PARCURISegment *result = NULL;

    PARCBuffer *buffer = parcBuffer_Allocate(length);
    if (buffer != NULL) {
        parcBuffer_PutArray(buffer, length, segment);
        parcBuffer_Flip(buffer);
        result = parcURISegment_CreateFromBuffer(buffer);
        parcBuffer_Release(&buffer);
    }
    return result;
}

PARCURISegment *
parcURISegment_Parse(const char *string, const char **pointer)
{
    assertFalse(*string == '/', "Input parameter '%s' must NOT point to an initial '/' character.", string);

    unsigned char *segment = parcMemory_AllocateAndClear((strlen(string) + 1) * sizeof(unsigned char));
    assertNotNull(segment, "parcMemory_AllocateAndClear(%zu) returned NULL", (strlen(string) + 1) * sizeof(unsigned char));
    size_t length = 0;

    unsigned char *r = segment;

    const char *p = string;
    while (*p && *p != '/' && *p != '?' && *p != '#') {
        if (*p == '%') {
            unsigned char value;
            if ((p = _parsePercentEncoded(p + 1, &value)) == NULL) {
                parcMemory_Deallocate((void **) &segment);
                return NULL;
            }
            *r = value;
        } else {
            *r = *p++;
        }
        length++;
        r++;
    }
    if (*p != 0) {
        // absorb any extra slash characters.
        while (p[1] == '/') {
            p++;
        }
    }

    PARCURISegment *result = parcURISegment_Create(length, segment);
    parcMemory_Deallocate((void **) &segment);
    if (pointer != NULL) {
        *pointer = p;
    }
    return result;
}

parcObject_ImplementAcquire(parcURISegment, PARCURISegment);

parcObject_ImplementRelease(parcURISegment, PARCURISegment);

PARCBuffer *
parcURISegment_GetBuffer(const PARCURISegment *segment)
{
    parcBuffer_Rewind(segment->buffer);
    return segment->buffer;
}

size_t
parcURISegment_Length(const PARCURISegment *segment)
{
    parcBuffer_Rewind(segment->buffer);
    return parcBuffer_Remaining(segment->buffer);
}

bool
parcURISegment_Equals(const PARCURISegment *segmentA, const PARCURISegment *segmentB)
{
    if (segmentA == segmentB) {
        return true;
    }
    if (segmentA == NULL || segmentB == NULL) {
        return false;
    }
    return parcBuffer_Equals(segmentA->buffer, segmentB->buffer);
}

PARCURISegment *
parcURISegment_Clone(const PARCURISegment *segment)
{
    assertNotNull(segment, "Parameter must be a non-null PARC_URISegment pointer.");

    PARCBuffer *copy = parcBuffer_Copy(segment->buffer);
    PARCURISegment *result = parcURISegment_CreateFromBuffer(copy);
    parcBuffer_Release(&copy);
    return result;
}

int
parcURISegment_Compare(const PARCURISegment *a, const PARCURISegment *b)
{
    if (a == NULL) {
        if (b == NULL) {
            return 0;
        }
        return -1;
    } else {
        if (b == NULL) {
            return +1;
        }
    }

    if (parcURISegment_Length(a) < parcURISegment_Length(b)) {
        return -1;
    }
    if (parcURISegment_Length(a) > parcURISegment_Length(b)) {
        return +1;
    }
    return parcBuffer_Compare(a->buffer, b->buffer);
}

PARCBufferComposer *
parcURISegment_BuildString(const PARCURISegment *segment, PARCBufferComposer *composer)
{
    composer = _parcURISegment_BuildString(segment, composer);

    return composer;
}

char *
parcURISegment_ToString(const PARCURISegment *segment)
{
    PARCBufferComposer *composer = parcBufferComposer_Create();

    char *result = NULL;

    if (parcURISegment_BuildString(segment, composer)) {
        PARCBuffer *tempBuffer = parcBufferComposer_ProduceBuffer(composer);
        result = parcBuffer_ToString(tempBuffer);
        parcBuffer_Release(&tempBuffer);
    }
    parcBufferComposer_Release(&composer);

    return result;
}
