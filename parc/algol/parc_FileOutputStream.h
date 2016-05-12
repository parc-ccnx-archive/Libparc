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
 * @file parc_FileOutputStream.h
 * @ingroup inputoutput
 * @brief A file output stream is an output stream for writing data to a File or to a FileDescriptor.
 *
 * Whether or not a file is available or may be created depends upon the underlying platform.
 * Some platforms, in particular, allow a file to be opened for writing by only one FileOutputStream
 * (or other file-writing object) at a time. In such situations the constructors in this class will
 * fail if the file involved is already open.
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */

#ifndef libparc_parc_FileOutputStream_h
#define libparc_parc_FileOutputStream_h

#include <parc/algol/parc_Buffer.h>
#include <parc/algol/parc_OutputStream.h>

struct parc_file_output_stream;
typedef struct parc_file_output_stream PARCFileOutputStream;

/**
 * The mapping of a `PARCFileOutputStream` to the generic `PARCInputStream`.
 *
 */
extern PARCOutputStreamInterface *PARCFileOutputStreamAsPARCOutputStream;

/**
 * Create a new output stream on a file descriptor.
 *
 * Caution: When the resulting `PARCFileOutputStream` is released, the file descriptor is closed.
 * If you wrap STDOUT_FILENO, for example, the standard output of the application will be closed
 * when this PARCFileOutputStream is released.
 * To avoid this, use dup(2) or dup2(2).
 *
 * @param [in] fileDescriptor The fileDescriptor for the file on which to create an output stream.
 *
 * @return A pointer to a new instance of `PARCFileOutputStream`
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
PARCFileOutputStream *parcFileOutputStream_Create(int fileDescriptor);

/**
 * Convert an instance of `PARCFileOutputStream` to a `PARCOutputStream`.
 *
 * @param [in] fileOutputStream A pointer to a valid PARCFileOutputStream.
 *
 * @return  A pointer to a new instance of `PARCOutputStream`.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCOutputStream *parcFileOutputStream_AsOutputStream(PARCFileOutputStream *fileOutputStream);

 /**
 * Acquire a new reference to an instance of `PARCFileOutputStream`.
 *
 * The reference count to the instance is incremented.
 *
 * @param [in] stream The instance of `PARCFileOutputStream` to which to refer.
 *
 * @return The same value as the input parameter @p stream
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
PARCFileOutputStream *parcFileOutputStream_Acquire(const PARCFileOutputStream *stream);

/**
 * Release a `PARCFileOutputStream` reference.
 *
 * Only the last invocation where the reference count is decremented to zero,
 * will actually destroy the `PARCFileOutputStream`.
 *
 * @param [in,out] streamPtr is a pointer to the `PARCFileOutputStream` reference.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void parcFileOutputStream_Release(PARCFileOutputStream **streamPtr);

/**
 * Write the contents of a {@link PARCBuffer} to the given `PARCFileOutputStream`.
 *
 * The contents of the `PARCBuffer` from the current position to the limit are written to the `PARCFileOutputStream`.
 * When this function returns the position is set to the end of the last successfully written byte of data.
 *
 * @param [in,out] outputStream The `PARCOutputStream` to write to.
 * @param [in] buffer The `PARCBuffer` to write, from the current position of the buffer to its limit.
 *
 * @return true The entire contents of the `PARCBuffer` were written.
 * @return false The entire contents of the `PARCBuffer` were not written.
 *
 * Example:
 * @code
 * {
 *     PARCFileOutputStream *stream =
 *         parcFileOutputStream_Create(open("/tmp/test_parc_FileOutputStream", O_CREAT|O_WRONLY|O_TRUNC, 0600));
 *
 *     PARCBuffer *buffer = parcBuffer_Allocate(16 * 1024*1024);
 *
 *     parcFileOutputStream_Write(stream, buffer);
 *
 *     assertFalse(parcBuffer_HasRemaining(buffer), "Expected the buffer to be emtpy");
 *
 *     parcBuffer_Release(&buffer);
 *
 *     parcFileOutputStream_Release(&stream);
 * }
 * @endcode
 */
bool parcFileOutputStream_Write(PARCFileOutputStream *outputStream, PARCBuffer *buffer);
#endif // libparc_parc_FileOutputStream_h
