/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @file parc_File.h
 * @ingroup inputoutput
 * @brief File manipulation
 *
 * @author Glenn Scott, Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 *
 */
#ifndef libparc_parc_File_h
#define libparc_parc_File_h

#include <stdbool.h>

#include <parc/algol/parc_BufferComposer.h>

#ifdef PARCLibrary_DISABLE_VALIDATION
#  define parcFile_OptionalAssertValid(_instance_)
#else
#  define parcFile_OptionalAssertValid(_instance_) parcFile_AssertValid(_instance_)
#endif

struct parc_file;
typedef struct parc_file PARCFile;

/**
 * Creates a `PARCFile` object named by pathname.
 *
 * This operation does not imply any I/O operations.
 * The PARCFile instance only represents the pathname,
 * and does not necessarily reference a real file.
 *
 * @param [in] pathname is a pointer to a char array (string)
 * @return A pointer to an instance of `PARCFile`
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCFile *parcFile_Create(const char *pathname);

/**
 * Acquire a new reference to an instance of `PARCFile`.
 *
 * The reference count to the instance is incremented.
 *
 * @param [in] file The instance of `PARCFile` to which to refer.
 *
 * @return The same value as the input parameter @p file
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
PARCFile *parcFile_Acquire(const PARCFile *file);

/**
 * Assert that an instance of `PARCFile` is valid.
 *
 * Valid means the internal state of the type is consistent with its required current or future behaviour.
 * This may include the validation of internal instances of types.
 *
 * @param [in] instance A pointer to a `PARCFile` instance.
 *
 * Example:
 * @code
 * {
 *     PARCFile *file = parcFile_Create("/tmp/foo");
 *
 *     parcFile_AssertValid(file);
 * }
 * @endcode
 */
void parcFile_AssertValid(const PARCFile *instance);

/**
 * Release a `PARCFile` reference.
 *
 * Only the last invocation where the reference count is decremented to zero,
 * will actually destroy the `PARCFile`.
 *
 * @param [in,out] filePtr is a pointer to the `PARCFile` reference.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void parcFile_Release(PARCFile **filePtr);

/**
 * Create a new file on storage.
 *
 * @param [in] file A pointer to an instance of `PARCFile`
 *
 * @return true if succesful, false if not
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
bool parcFile_CreateNewFile(const PARCFile *file);

/**
 * Return true if the PARCFile exists on storage.
 *
 * If the pathname can be stat(2)'d, then it exists.
 *
 * @param [in] file A pointer to a `PARCFile` instance.
 * @return true if the file exists, false otherwise
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
bool parcFile_Exists(const PARCFile *file);

/**
 * Create a new directory.
 *
 * @param [in] file A pointer to a `PARCFile` instance.
 * @return true if the pathname exists, false otherwise
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
bool parcFile_Mkdir(const PARCFile *file);

/**
 * True if the specified `PARCFile` is an existing directory on storage.
 *
 * @param [in] file A pointer to a `PARCFile` instance.
 *
 * @return true if specified `PARCFile` is an existing directory on storage, false if not
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
bool parcFile_IsDirectory(const PARCFile *file);

/**
 * Deletes the file or directory on storage.
 *
 * For a directory, it does a recursive delete.
 *
 * @param [in] file The instance of `PARCFile` to be deleted.
 * @return `true` if everything deleted, false otherwise
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
bool parcFile_Delete(const PARCFile *file);

/**
 * Append a representation of the specified `PARCFile` instance to the given {@link PARCBufferComposer}.
 *
 * @param [in] file A pointer to the `PARCFile` instance whose contents should be appended to to string.
 * @param [in,out] string A pointer to the `PARCBufferComposer` instance to which the contents of file will be appended.
 *
 * @return NULL Cannot allocate memory.
 * @return non-NULL The instance of `PARCBufferComposer` with the appended contents.
 *
 * Example:
 * @code
 * {
 *     PARCBufferComposer *result = parcBufferComposer_Create();
 *     PARCFile *instance = parcFile_Create("/tmp/foo");
 *
 *     parcFile_BuildString(instance, result);
 *
 *     PARCBuffer *string = parcBufferComposer_FinalizeBuffer(result);
 *     printf("File: %s\n", parcBuffer_ToString(string));
 *     parcBuffer_Release(&string);
 *
 *     parcBufferComposer_Release(&result);
 * }
 * @endcode
 */
PARCBufferComposer *parcFile_BuildString(const PARCFile *file, PARCBufferComposer *string);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [<#in#> | <#out#> | <#in,out#>] <#name#> <#description#>
 *
 * @return <#value#> <#explanation#>
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
size_t parcFile_GetFileSize(const PARCFile *file);

/**
 * Create a PARCFile representation of the home directory of the current user.
 *
 * The return value must be released via `parcFile_Release`.
 *
 * @return non-NULL A pointer to a valid PARCFile instance.
 * @return NULL An error occurred.
 *
 * Example:
 * @code
 * {
 *     PARCFile *directory = parcFile_CreateHome();
 * }
 * @endcode
 */
PARCFile *parcFile_CreateHome(void);

/**
 * Produce a null-terminated string representation of the specified `PARCFile` instance.
 *
 * The non-null result must be freed by the caller via {@link parcMemory_Deallocate}.
 *
 * @param [in] file A pointer to the `PARCFile` instance.
 *
 * @return NULL Cannot allocate memory.
 * @return non-NULL A pointer to an allocated, null-terminated C string that must be deallocated via {@link parcMemory_Deallocate}.
 *
 * Example:
 * @code
 * {
 *     PARCFile *instance = parcFile_Create("/tmp/foo");
 *
 *     char *string = parcFile_ToString(instance);
 *
 *     if (string != NULL) {
 *         printf("Hello: %s\n", string);
 *         parcMemory_Deallocate((void **)&string);
 *     } else {
 *         printf("Cannot allocate memory\n");
 *     }
 *
 *     parcFile_Release(&instance);
 * }
 * @endcode
 *
 * @see {@link parcFile_BuildString}
 */
char *parcFile_ToString(const PARCFile *file);
#endif // libparc_parc_File_h
