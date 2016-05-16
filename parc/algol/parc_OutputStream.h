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
 * @file parc_OutputStream.h
 * @ingroup inputoutput
 * @brief A polymophic interface to specific implementations of modules that implement the
 *        output stream capabilities.
 *
 *
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_OutputStream_h
#define libparc_parc_OutputStream_h

#include <parc/algol/parc_Buffer.h>

struct parc_output_stream;
typedef struct parc_output_stream PARCOutputStream;

typedef struct parc_output_stream_interface {
    size_t (*Write)(PARCOutputStream *stream, PARCBuffer *buffer);

    PARCOutputStream *(*Acquire)(PARCOutputStream * stream);

    void (*Release)(PARCOutputStream **streamPtr);
} PARCOutputStreamInterface;

/**
 * Create an valid PARCOutputStream instance from the given pointers to a properly
 * initialized `PARCOutputStreamInterface`
 * and specific instance structure that will be supplied to the underlying interface.
 *
 * @param [in] instance A pointer to a `PARCObject` that will be the parameter to the functions specifed by @p interface.
 * @param [in] interface A pointer to a `PARCOutputStreamInterface`.
 *
 * @return non-NULL A pointer to a valid PARCOutputStream instance.
 * @return NULL An error occurred.
 *
 * Example:
 * @code
 * {
 *     PARCFileOutputStream *fileOutput = parcFileOutputStream_Create(1);
 *     PARCOutputStream *output = parcOutputStream_Create(parcFileOutputStream_Acquire(fileOutputStream),
 *                                                        PARCFileOutputStreamAsPARCInputStream);
 * }
 * @endcode
 */
PARCOutputStream *parcOutputStream_Create(void *instance, const PARCOutputStreamInterface *interface);

/**
 * Write the contents of the given `PARCBuffer` to the output stream.
 *
 * The position of the `PARCBuffer` will be set to its limit as a side-effect.
 *
 * @param [in] stream A pointer to a valid `PARCOutputStream` instance.
 * @param [in] buffer A pointer to the `PARCBuffer` whose contents should be written to @p stream.
 *
 * @return The number of bytes written
 *
 * Example:
 * @code
 * {
 *     PARCFileOutputStream *fileOutput = parcFileOutputStream_Create(1);
 *     PARCOutputStream *output = parcOutputStream_Create(parcFileOutputStream_Acquire(fileOutputStream),
 *                                                        PARCFileOutputStreamAsPARCInputStream);
 *     PARCBuffer *buffer = parcBuffer_WrapCString("Hello World");
 *     parcOutputStream_Write(output, buffer);
 *     parcOutputStream_Release(&output);
 * }
 * @endcode
 */
size_t parcOutputStream_Write(PARCOutputStream *stream, PARCBuffer *buffer);

/**
 * Write a nul-terminated C string to the given `PARCOutputStream`.
 *
 * @param [in] stream A pointer to a valid `PARCOutputStream` instance.
 * @param [in] string A nul-terminated C string.
 *
 * @return The number of bytes written.
 *
 * Example:
 * @code
 * {
 *     PARCFileOutputStream *fileOutput = parcFileOutputStream_Create(1);
 *     PARCOutputStream *output = parcOutputStream_Create(parcFileOutputStream_Acquire(fileOutputStream),
 *                                                        PARCFileOutputStreamAsPARCInputStream);
 *
 *     parcOutputStream_WriteCStrings(output, "Hello", " ", "World", NULL);
 *
 *     parcOutputStream_Release(&output);
 * }
 * @endcode
 */
size_t parcOutputStream_WriteCString(PARCOutputStream *stream, const char *string);

/**
 * Write one or more nul-terminated C strings to the given `PARCOutputStream`.
 *
 * @param [in] stream A pointer to a valid `PARCOutputStream` instance.
 * @param [in] ... A NULL terminated variable argument list of nul-terminated C strings.
 *
 * @return The number of bytes written.
 *
 * Example:
 * @code
 * {
 *     PARCFileOutputStream *fileOutput = parcFileOutputStream_Create(1);
 *     PARCOutputStream *output = parcOutputStream_Create(parcFileOutputStream_Acquire(fileOutputStream),
 *                                                        PARCFileOutputStreamAsPARCInputStream);
 *
 *     parcOutputStream_WriteCStrings(output, "Hello", " ", "World", NULL);
 *
 *     parcOutputStream_Release(&output);
 * }
 * @endcode
 */
size_t parcOutputStream_WriteCStrings(PARCOutputStream *stream, ...);

/**
 * Increase the number of references to a `PARCOutputStream`.
 *
 * Note that new `PARCOutputStream` is not created,
 * only that the given `PARCOutputStream` reference count is incremented.
 * Discard the reference by invoking `parcOutputStream_Release`.
 *
 * @param [in] stream A pointer to a `PARCOutputStream` instance.
 *
 * @return The input `PARCOutputStream` pointer.
 *
 * Example:
 * @code
 * {
 *     PARCFileOutputStream *fileOutput = parcFileOutputStream_Create(1);
 *     PARCOutputStream *output = parcOutputStream_Create(parcFileOutputStream_Acquire(fileOutputStream),
 *                                                        PARCFileOutputStreamAsPARCInputStream);
 *     PARCBuffer *buffer = parcBuffer_WrapCString("Hello World");
 *     parcOutputStream_Write(output, buffer);
 *
 *     PARCOutputStream *x = parcOutputStream_Acquire(output);
 *
 *     parcBuffer_Release(&x);
 *     parcOutputStream_Release(&output);
 * }
 * @endcode
 * @see parcOutputStream_Release
 */
PARCOutputStream *parcOutputStream_Acquire(const PARCOutputStream *stream);

/**
 * Release a previously acquired reference to the specified instance,
 * decrementing the reference count for the instance.
 *
 * The pointer to the instance is set to NULL as a side-effect of this function.
 *
 * If the invocation causes the last reference to the instance to be released,
 * the instance is deallocated and the instance's interface will perform
 * additional cleanup and release other privately held references.
 *
 * @param [in] streamPtr A pointer to a pointer to the instance to release.
 *
 * Example:
 * @code
 * {
 *     PARCFileOutputStream *fileOutput = parcFileOutputStream_Create(1);
 *     PARCOutputStream *output = parcOutputStream_Create(parcFileOutputStream_Acquire(fileOutputStream),
 *                                                        PARCFileOutputStreamAsPARCInputStream);
 *     PARCBuffer *buffer = parcBuffer_WrapCString("Hello World");
 *     parcOutputStream_Write(output, buffer);
 *     parcOutputStream_Release(&output);
 * }
 * @endcode
 * @see parcOutputStream_Acquire
 * @see parcOutputStream_Create
 */
void parcOutputStream_Release(PARCOutputStream **streamPtr);
#endif // libparc_parc_OutputStream_h
