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
 * @author Christopher A. Wood, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_Memory.h>

#include <parc/algol/parc_RandomAccessFile.h>

#include <stdio.h>

struct PARCRandomAccessFile {
    char *fname;
    FILE *fhandle;
};

static void
_parcRandomAccessFile_Finalize(PARCRandomAccessFile **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCRandomAccessFile pointer.");
    PARCRandomAccessFile *instance = *instancePtr;
    if (instance->fhandle != NULL) {
        fclose(instance->fhandle);
    }
    if (instance->fname != NULL) {
        parcMemory_Deallocate(&instance->fname);
    }
}

parcObject_ImplementAcquire(parcRandomAccessFile, PARCRandomAccessFile);

parcObject_ImplementRelease(parcRandomAccessFile, PARCRandomAccessFile);

parcObject_ExtendPARCObject(PARCRandomAccessFile, _parcRandomAccessFile_Finalize, NULL,
                            parcRandomAccessFile_ToString, parcRandomAccessFile_Equals, NULL,
                            parcRandomAccessFile_HashCode, parcRandomAccessFile_ToJSON);


void
parcRandomAccessFile_AssertValid(const PARCRandomAccessFile *instance)
{
    assertTrue(parcRandomAccessFile_IsValid(instance),
               "PARCRandomAccessFile is not valid.");
}

void
parcRandomAccessFile_Display(const PARCRandomAccessFile *instance, int indentation)
{
    parcDisplayIndented_PrintLine(indentation, "PARCRandomAccessFile@%p {", instance);
    parcDisplayIndented_PrintLine(indentation + 1, "File: %s", instance->fname);
    parcDisplayIndented_PrintLine(indentation, "}");
}

PARCHashCode
parcRandomAccessFile_HashCode(const PARCRandomAccessFile *instance)
{
    return parcHashCode_Hash((uint8_t *) instance->fname, strlen(instance->fname));
}

bool
parcRandomAccessFile_Equals(const PARCRandomAccessFile *x, const PARCRandomAccessFile *y)
{
    bool result = false;

    if (x == y) {
        result = true;
    } else if (x == NULL || y == NULL) {
        result = false;
    } else {
        if (strcmp(x->fname, y->fname) == 0) {
            result = true;
        }
    }

    return result;
}

bool
parcRandomAccessFile_IsValid(const PARCRandomAccessFile *instance)
{
    bool result = false;

    if (instance != NULL) {
        result = instance->fhandle != NULL;
    }

    return result;
}

PARCJSON *
parcRandomAccessFile_ToJSON(const PARCRandomAccessFile *instance)
{
    PARCJSON *result = parcJSON_Create();

    if (result != NULL) {
        parcJSON_AddString(result, "fname", instance->fname);
    }

    return result;
}

char *
parcRandomAccessFile_ToString(const PARCRandomAccessFile *instance)
{
    char *result = parcMemory_Format("PARCRandomAccessFile[%s]@%p\n", instance->fname, instance);
    return result;
}

PARCRandomAccessFile *
parcRandomAccessFile_Open(PARCFile *file)
{
    PARCRandomAccessFile *handle = parcObject_CreateAndClearInstance(PARCRandomAccessFile);
    if (handle != NULL) {
        char *fname = parcFile_ToString(file);
        handle->fhandle = fopen(fname, "r+");
        handle->fname = parcMemory_StringDuplicate(fname, strlen(fname));
        parcMemory_Deallocate(&fname);
    }
    return handle;
}

bool
parcRandomAccessFile_Close(PARCRandomAccessFile *fileHandle)
{
    assertNotNull(fileHandle->fhandle, "Can't fclose a null pointer. How did they get one anyway?");
    bool result = fclose(fileHandle->fhandle) == 0;
    fileHandle->fhandle = NULL;
    parcMemory_Deallocate(&fileHandle->fname);
    fileHandle->fname = NULL;
    return result;
}

// read (count * size) bytes into the provided buffer, and return the number of bytes actually read
size_t
parcRandomAccessFile_Read(PARCRandomAccessFile *fileHandle, PARCBuffer *buffer)
{
    size_t length = parcBuffer_Remaining(buffer);
    size_t numBytes = fread(parcBuffer_Overlay(buffer, length), 1, length, fileHandle->fhandle);
    return numBytes;
}

// write (count * size) bytes from `buffer` to the file, and return the number of bytes actually written
size_t
parcRandomAccessFile_Write(PARCRandomAccessFile *fileHandle, PARCBuffer *buffer)
{
    size_t length = parcBuffer_Remaining(buffer);
    size_t numBytes = fwrite(parcBuffer_Overlay(buffer, length), 1, length, fileHandle->fhandle);
    return numBytes;
}

size_t
parcRandomAccessFile_Seek(PARCRandomAccessFile *fileHandle, long offset, PARCRandomAccessFilePosition position)
{
    size_t result = 0;
    switch (position) {
        case PARCRandomAccessFilePosition_Start:
            result = fseek(fileHandle->fhandle, offset, SEEK_SET); // beginning of the file
            break;
        case PARCRandomAccessFilePosition_Current:
            result = fseek(fileHandle->fhandle, offset, SEEK_CUR); // current offset
            break;
        case PARCRandomAccessFilePosition_End:
            result = fseek(fileHandle->fhandle, offset, SEEK_END); // end of the file
            break;
        default:
            assertTrue(false, "Invalid position %d", position);
    }
    return result;
}
