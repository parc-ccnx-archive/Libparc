/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>

#include <LongBow/runtime.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include <parc/algol/parc_DisplayIndented.h>

static char *_spaces = "                                                                                                           ";

static size_t _indentationFactor = 2;

static size_t
_indent(int indentation)
{
    size_t result = 0;

    if (indentation > 0) {
        result = write(1, _spaces, indentation * _indentationFactor);
        assertTrue(result == (indentation * _indentationFactor),
                   "Write(2) failed to write %zd bytes.", indentation * _indentationFactor);
    }
    return result;
}

static void
_parcDisplayIndented_Print(int indentation, char *string)
{
    char *start = string;
    char *end = strchr(start, '\n');

    while (start != NULL) {
        _indent(indentation);
        if (end != NULL) {
            ssize_t nwritten = write(1, start, end - start + 1);
            assertTrue(nwritten >= 0, "Error calling write");
            start = end + 1;
        } else {
            ssize_t nwritten = write(1, start, strlen(start));
            assertTrue(nwritten >= 0, "Error calling write");
            break;
        }
        end = strchr(start, '\n');
    }
}

void
parcDisplayIndented_PrintLine(int indentation, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    char *cString;
    int length = vasprintf(&cString, format, ap);
    assertTrue(length >= 0, "Error in vasprintf");

    va_end(ap);

    _parcDisplayIndented_Print(indentation, cString);

    ssize_t nwritten = write(1, "\n", 1);
    assertTrue(nwritten >= 0, "Error calling write");

    free(cString);
}

void
parcDisplayIndented_PrintMemory(int indentation, size_t length, const char memory[length])
{
    int bytesPerLine = 16;

    char accumulator[bytesPerLine + 1];
    memset(accumulator, ' ', bytesPerLine);
    accumulator[bytesPerLine] = 0;

    char *cString;
    for (size_t offset = 0; offset < length; /**/) {
        int nwritten = asprintf(&cString, "%p=[", &memory[offset]);
        assertTrue(nwritten >= 0, "Error calling asprintf");
        _parcDisplayIndented_Print(indentation, cString);
        free(cString);

        size_t bytesInLine = (length - offset) < bytesPerLine ? (length - offset) : bytesPerLine;
        for (size_t i = 0; i < bytesInLine; i++) {
            char c = memory[offset + i];
            printf("0x%02x, ", c & 0xFF);
            accumulator[i] = isprint(c) ? c : '.';
        }
        offset += bytesInLine;
    }
    printf("  %s]\n", accumulator);
}
