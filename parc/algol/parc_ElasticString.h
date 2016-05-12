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
 * @file parc_ElasticString.h
 * @ingroup memory
 * @brief An elastic C string.
 *
 * An elastic string is a dynamic array of characters which are readily expressed as a
 * nul-terminated C string.
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_ElasticString_h
#define libparc_parc_ElasticString_h

#include <stdarg.h>

#include <parc/algol/parc_Buffer.h>

struct parc_elastic_string;
typedef struct parc_elastic_string PARCElasticString;

/**
 * Perform validation on a pointer to a `PARCElasticString`.
 *
 * If invalid, this function will abort the running process.
 *
 * @param [in] string A pointer to a `PARCElasticString` to validate.
 *
 * Example:
 * @code
 * {
 *     PARElasticString *string = parcElasticString_Create();
 *
 *     parcElasticString_AssertValid(string);
 * }
 * @endcode
 */
void parcElasticString_AssertValid(const PARCElasticString *string);

/**
 * Create an empty `PARCElasticString` instance.
 *
 *   The instance will be empty upon initialization (i.e., `parcElasticString_ToString()` will
 *   return an empty string), but characters and strings may be inserted/appended to
 *   the instance to produce usable content,
 *
 * @return A pointer to an allocated `PARCElasticString` that must be freed with `parcElasticString_Release()`.
 *
 * Example:
 * @code
 * {
 *     PARElasticString *string = parcElasticString_Create();
 *
 *     // use the string as necessary
 *
 *     parcElasticString_Release(&string);
 * }
 * @endcode
 */
PARCElasticString *parcElasticString_Create(void);

/**
 * Release a previously acquired reference to the specified instance,
 * decrementing the reference count for the instance.
 *
 * The pointer to the instance is set to NULL as a side-effect of this function.
 *
 * If the invocation causes the last reference to the instance to be released,
 * the instance is deallocated and the instance's implementation will perform
 * additional cleanup and release other privately held references.
 *
 * @param [in] string A pointer to a pointer to the instance to release.
 *
 * @return The number of remaining references to the object.
 *
 * Example:
 * @code
 * {
 *     PARCElasticString *buffer = parcElasticString_Create();
 *
 *     parcElasticString_Release(&pathName);
 * }
 * @endcode
 */
void parcElasticString_Release(PARCElasticString **string);

/**
 * Retrieve the number of remaining bytes between the current position
 * in the string and its (flexible) limit.
 *
 * @param [in] string A pointer to an `PARCElasticString` instance.
 *
 * @return The non-negative number of characters remaining between the position and limit.
 *
 * Example:
 * @code
 * {
 *     char *inputString = "Hello World";
 *     size_t inputLength = strlen(input);
 *     PARElasticString *string = parcElasticString_Create();
 *     parcElasticString_PutString(string, inputString);
 *     parcElasticString_Flip(string);
 *     size_t numRemaining = parcElasticString_Remaining(string);
 *
 *     // numRemaining == inputLength
 *
 *     parcElasticString_Release(&string);
 * }
 * @endcode
 */
size_t parcElasticString_Remaining(const PARCElasticString *string);

/**
 * Set the limit to the current position, then the position to zero.
 * If the mark is defined, it is invalidated,
 * and any subsequent operation that requires the mark will abort until the mark
 * is set again via `parcElasticString_Mark`.
 *
 * @param [in] string A pointer to an `PARCElasticString` instance.
 *
 * @return The same pointer as the `string` parameter.
 *
 * Example:
 * @code
 * {
 *     char *inputString = "Hello World";
 *     size_t inputLength = strlen(input);
 *     PARElasticString *string = parcElasticString_Create();
 *     parcElasticString_PutString(string, inputString);
 *     parcElasticString_Flip(string);
 *     size_t numRemaining = parcElasticString_Remaining(string);
 *
 *     // numRemaining == inputLength
 *
 *     parcElasticString_Release(&string);
 * }
 * @endcode
 */
PARCElasticString *parcElasticString_Flip(PARCElasticString *string);

/**
 * Return the given `PARCElasticString`'s position.
 *
 * A buffer's position is the index of the next element to be read or written.
 * A buffer's position is never negative and is never greater than its limit.
 *
 * @param [in] string A pointer to a `PARCElasticString` instance.
 *
 * @return The given `PARCElasticString`'s position.
 *
 * Example:
 * @code
 * {
 *     size_t currentPosition = parcBuffer_Position(buffer);
 * }
 * @endcode
 *
 * @see parcElasticString_SetPosition
 */
size_t parcElasticString_Position(const PARCElasticString *string);

/**
 * Set the position in the given `PARCElasticString`.
 *
 * If the mark is defined and larger than the new position then it is invalidated.
 *
 * @param [in] string A pointer to a `PARCElasticString` instance.
 * @param [in] newPosition The new position for the `PARCElasticString`.
 *
 * @return The same pointer as the `string` parameter.
 *
 * Example:
 * @code
 * {
 *     PARElasticString *string = parcElasticString_Create();
 *     parcElasticString_PutString(string, "Hello World");
 *
 *     parcElasticString_SetPosition(string, 0);
 *
 *     // position is now at 0, instead of at the end of "Hello World"
 *
 *     parcElasticString_Release(&string);
 * }
 * @endcode
 *
 * @see parcElasticString_Position
 */
PARCElasticString *parcElasticString_SetPosition(PARCElasticString *string, size_t newPosition);

/**
 * Append an array with the specified number of bytes to the end of this `PARCElasticString` instance.
 *
 * The position of the string is advanced by the length of the array.
 *
 * @param [in] string A pointer to a `PARCElasticString` instance.
 * @param [in] array A pointer to the array containing the bytes to append.
 * @param [in] length The length of the input array.
 *
 * @return The same pointer as the `string` parameter.
 *
 * Example:
 * @code
 * {
 *     PARElasticString *string = parcElasticString_Create();
 *
 *     uint8_t appendArray[5] = { 0x00, 0x01, 0x02, 0x03, 0x04 };
 *     parcElasticString_PutArray(string, appendArray, 5);
 *
 *     parcElasticString_Release(&string);
 * }
 * @endcode
 *
 * @see parcElasticString_PutString
 */
PARCElasticString *parcElasticString_PutArray(PARCElasticString *string, const char *array, size_t length);

/**
 * Append a C-string to the end of this `PARCElasticString` instance.
 *
 * The position of the string is advanced by the length of the string.
 *
 * @param [in] string A pointer to a `PARCElasticString` instance.
 * @param [in] cString A pointer to a nul-terminated C string to append to this `PARCElasticString`.
 *
 * @return The same pointer as the `string` parameter.
 *
 * Example:
 * @code
 * {
 *     PARElasticString *string = parcElasticString_Create();
 *
 *     parcElasticString_PutString(string, "Hello World");
 *     printf("String = %s\n", parcElasticString_ToString(string));
 *
 *     parcElasticString_Release(&string);
 * }
 * @endcode
 *
 * @see parcElasticString_PutArray
 */
PARCElasticString *parcElasticString_PutString(PARCElasticString *string, const char *cString);

/**
 * Append the contents of a `PARCBuffer` instance to the end of the `PARCElasticString` instance.
 *
 * The position of the string is advanced by the length of the buffer.
 *
 * @param [in] string A pointer to a `PARCElasticString` instance.
 * @param [in] buffer A pointer to a `PARCBuffer` instance.
 *
 * @return The same pointer as the `string` parameter.
 *
 * Example:
 * @code
 * {
 *     PARElasticString *string = parcElasticString_Create();
 *
 *     uint8_t array[5] = { 'H', 'e', 'l', 'l', 'o' };
 *     PARCBuffer *endBuffer = parcBuffer_Allocate(10);
 *     parcBuffer_PutArray(endBuffer, 5, array);
 *     parcElasticString_PutBuffer(string, endBuffer);
 *
 *     printf("String = %s\n", parcElasticString_ToString(string));
 *
 *     parcElasticString_Release(&string);
 * }
 * @endcode
 *
 * @see parcElasticString_PutString
 */
PARCElasticString *parcElasticString_PutBuffer(PARCElasticString *string, PARCBuffer *buffer);

/**
 * Append a single character (byte) to the end of this string.
 *
 * The position of the string is advanced by one (1).
 *
 * @param [in] string A pointer to `PARCElasticString`
 * @param [in] character A `char` value to append.
 *
 * @return The same pointer as the `string` parameter.
 *
 * Example:
 * @code
 * {
 *     PARElasticString *string = parcElasticString_Create();
 *
 *     parcElasticString_PutChar(string, 'H');
 *     parcElasticString_PutChar(string, 'e');
 *     parcElasticString_PutChar(string, 'l');
 *     parcElasticString_PutChar(string, 'l');
 *     parcElasticString_PutChar(string, 'o');
 *
 *     printf("String = %s\n", parcElasticString_ToString(string));
 *
 *     parcElasticString_Release(&string);
 * }
 * @endcode
 *
 * @see parcElasticString_PutString
 */
PARCElasticString *parcElasticString_PutChar(PARCElasticString *string, const char character);

/**
 * Put a variable number of characters into the `PARCElasticString`.
 *
 * @param [in] string The `PARCElasticString` to receive the characters.
 * @param [in] count The number of characters to insert into the `PARCElasticString`
 * @param [in] ...  The characters to insert into the `PARCElasticString`.
 *
 * @return The same pointer as the `string` parameter.
 *
 * Example:
 * @code
 * {
 *     PARElasticString *string = parcElasticString_Create();
 *
 *     parcElasticString_PutChar(string, 5, 'H', 'e', 'l', 'l', 'o');
 *
 *     printf("String = %s\n", parcElasticString_ToString(string));
 *
 *     parcElasticString_Release(&string);
 * }
 * @endcode
 *
 * @see parcElasticString_PutString
 */
PARCElasticString *parcElasticString_PutChars(PARCElasticString *string, unsigned int count, ...);

/**
 * Append an arbitrary number of C-style strings to the given `PARCElasticString` instance.
 *
 * @param [in] string A pointer to `PARCElasticString`
 * @param [in] ... The nul-terminated, C-style strings to append to the given `PARCElasticString`.
 *
 * @return The same pointer as the `string` parameter.
 *
 * Example:
 * @code
 * {
 *     PARElasticString *baseString = parcElasticString_Create();
 *     parcElasticString_PutString(baseString, "Hello");
 *     uint8_t string1[2] = {' ', '\0'};
 *     uint8_t string2[6] = {'W', 'o', 'r', 'l', 'd', '\0'};
 *
 *     parcElasticString_PutStrings(baseString, string1, string2);
 *
 *     printf("String = %s\n", parcElasticString_ToString(baseString));
 *
 *     parcElasticString_Release(&baseString);
 * }
 * @endcode
 */
PARCElasticString *parcElasticString_PutStrings(PARCElasticString *string, ...);

/**
 * Append a formatted nul-terminated, C-style string string to the given `PARCElasticString` instance.
 *
 * @param [in] string A pointer to `PARCElasticString`.
 * @param [in] format The format string
 * @param [in] ... Remaining parameters used to format the string.
 *
 * @return The same pointer as the `string` parameter.
 *
 * Example:
 * @code
 * {
 *     PARElasticString *string = parcElasticString_Create();
 *
 *     parcElasticString_Format(string, "Hello %s\n", "World");
 *
 *     printf("String = %s\n", parcElasticString_ToString(string));
 *
 *     parcElasticString_Release(&string);
 * }
 * @endcode
 */
PARCElasticString *parcElasticString_Format(PARCElasticString *string, const char *format, ...) \
    __attribute__((format(printf, 2, 3)));

/**
 * Retrieve a handle to the `PARCBuffer` instance. The reference count is not increased.
 *
 * @param [in] string A pointer to `PARCElasticString`.
 *
 * @return The `PARCBuffer` instance used to encapsulate the `PARCElasticString` contents.
 *
 * Example:
 * @code
 * {
 *     PARElasticString *string = parcElasticString_Create();
 *     parcElasticString_PutString(string, "Hello World");
 *
 *     PARCBuffer *buffer = parcElasticString_ToBuffer(string);
 *     printf("String in hex = %s\n", parcBuffer_ToHexString(buffer));
 *
 *     parcElasticString_Release(&string);
 * }
 * @endcode
 */
PARCBuffer *parcElasticString_ToBuffer(const PARCElasticString *string);

/**
 * Produce a C string representation of the given `PARCElasticString`.
 *
 * Produce an allocated, nul-terminated string representation of the given `PARCElasticString`.
 * The result must be freed by the caller via the `parcMemory_Deallocate()` function.
 *
 * @param [in] string A pointer to `PARCElasticString`.
 *
 * @return A pointer to an allocated array containing a nul-terminated string the must be freed via `parcMemory_Deallocate()`.
 *
 * Example:
 * @code
 * {
 *     PARElasticString *string = parcElasticString_Create();
 *     parcElasticString_PutString(string, "Hello World");
 *
 *     printf("String = %s\n", parcElasticString_ToString(string));
 *
 *     parcElasticString_Release(&string);
 * }
 * @endcode
 */
char *parcElasticString_ToString(const PARCElasticString *string);
#endif // libparc_parc_ElasticString_h
