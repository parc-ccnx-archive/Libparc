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
 * @file parc_InputStream.h
 * @ingroup inputoutput
 * @brief Generalized Input Stream
 *
 * a `PARCFileInputStream` is a kind of `PARCInputStream`
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libparc_parc_InputStream_h
#define libparc_parc_InputStream_h

#include <parc/algol/parc_Buffer.h>


/**
 * @typedef `PARCInputStream`
 */
struct parc_input_stream;
typedef struct parc_input_stream PARCInputStream;

/**
 * @typedef PARCInputStreamInterface
 */

typedef struct parc_input_stream_interface {
    size_t (*Read)(PARCInputStream *inputStream, PARCBuffer *buffer);

    PARCInputStream *(*Acquire)(const PARCInputStream * instance);

    void (*Release)(PARCInputStream **instancePtr);
} PARCInputStreamInterface;

/**
 * Create an instance of a `PARCInputStream` given a pointer to an instance and interface.
 *
 *
 * @param [in] instance A pointer to a structure suitable for the given `PARCInputStreamInterface`.
 * @param [in] interface A pointer to a `PARCInputStreamInterface`
 *
 * @return non-NULL A pointer to a valid PARCInputStream
 * @return NULL Memory could not be allocated.
 *
 * Example:
 * @code
 * {
 * }
 * @endcode
 */
PARCInputStream *parcInputStream(void *instance, const PARCInputStreamInterface *interface);

/**
 * Read a `PARCInputStream` into a {@link PARCBuffer}.
 *
 * The contents of the `PARCBuffer` are filled from the current position to the limit.
 * When this function returns the position is set to the end of the last successfully read byte of data.
 *
 * @param [in] inputStream The `PARCInputStream` to read.
 * @param [in] buffer The `PARCBuffer` to fill, from the current position of the buffer to its limit.
 *
 * @return number of bytes read / filled.
 *
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
size_t parcInputStream_Read(PARCInputStream *inputStream, PARCBuffer *buffer);

/**
 * Acquire a new reference to an instance of `PARCInputStream`.
 *
 * The reference count to the instance is incremented.
 *
 * @param [in] instance The instance of `PARCInputStream` to which to refer.
 *
 * @return The same value as the input parameter @p instance
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
PARCInputStream *parcInputStream_Acquire(const PARCInputStream *instance);

/**
 * Release a `PARCInputStream` reference.
 *
 * Only the last invocation where the reference count is decremented to zero,
 * will actually destroy the `PARCInputStream`.
 *
 * @param [in,out] instancePtr is a pointer to the `PARCInputStream` reference.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void parcInputStream_Release(PARCInputStream **instancePtr);
#endif // libparc_parc_InputStream_h
