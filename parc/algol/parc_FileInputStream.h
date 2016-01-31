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
 * @file parc_FileInputStream.h
 * @ingroup inputoutput
 * @brief  A FileInputStream obtains input bytes from a file in a file system.
 *
 * What files are available depends on the host environment.
 * FileInputStream is meant for reading streams of raw bytes such as image data.
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

#ifndef libparc_parc_FileInputStream_h
#define libparc_parc_FileInputStream_h

#include <parc/algol/parc_File.h>
#include <parc/algol/parc_Buffer.h>
#include <parc/algol/parc_InputStream.h>

struct parc_file_input_stream;

/**
 * @typedef PARCFileInputStream
 * @brief Read streams of input
 */

typedef struct parc_file_input_stream PARCFileInputStream;

/**
 * The mapping of a `PARCFileInputStream` to the generic `PARCInputStream`.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCInputStreamInterface *PARCFileInputStreamAsPARCInputStream;

/**
 * Create a `PARCFileInputStream` instance.
 *
 * @param [in] fileDescriptor An abstract indicator for accessing a specific file
 *
 * @return non-NULL A pointer to an instance of `PARCFileInputStream`
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
PARCFileInputStream *parcFileInputStream_Create(int fileDescriptor);

/**
 * Create a `PARCFileInputStream` instance by opening an existing {@link PARCFile} instance.
 *
 * The file specified by `PARCFile` must exist and readable.
 *
 * @param [in] file A pointer to a `PARCFile` instance representing the existing file.
 *
 * @return non-NULL A pointer to a `PARCFileInputStream` instance.
 * @return NULL Memory could not be allocated.
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
PARCFileInputStream *parcFileInputStream_Open(const PARCFile *file);

/**
 * Acquire a new reference to an instance of `PARCFileInputStream`.
 *
 * The reference count to the instance is incremented.
 *
 * @param [in] inputStream The instance of `PARCFileInputStream` to which to refer.
 *
 * @return The same value as the input parameter @p inputStream
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
PARCFileInputStream *parcFileInputStream_Acquire(const PARCFileInputStream *inputStream);

/**
 * Release a `PARCFileInputStream` reference.
 *
 * Only the last invocation where the reference count is decremented to zero,
 * will actually destroy the `PARCFileInputStream`.
 *
 * @param [in,out] inputStreamPtr is a pointer to the `PARCFileInputStream` reference.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */

void parcFileInputStream_Release(PARCFileInputStream **inputStreamPtr);

/**
 * Read a `PARCFileInputStream` into a {@link PARCBuffer}.
 *
 * The contents of the `PARCBuffer` are filled from the current position to the limit.
 * When this function returns, the position is set to the end of the last successfully read byte of data.
 *
 * @param [in] inputStream The `PARCInputStream` to read.
 * @param [in] buffer The `PARCBuffer` to read, from the current position of the buffer to its limit.
 *
 * @return true The entire contents of the `PARCBuffer`, from the current position to the limit, were filled.
 * @return false The entire contents of the `PARCBuffer` were not filled.
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
 *
 */
bool parcFileInputStream_Read(PARCFileInputStream *inputStream, PARCBuffer *buffer);

/**
 * Read the content of a `PARCFileInputStream` into a {@link PARCBuffer}.
 *
 * @param [in] inputStream The `PARCFileInputStream` to read.
 *
 * @return non-NULL A pointer to a `PARCBuffer` instance containing the content of the `PARCFileInputStream`.
 * @return NULl Memory could not be allocated.
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
PARCBuffer *parcFileInputStream_ReadFile(PARCFileInputStream *inputStream);
#endif // libparc_parc_FileInputStream_h
