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
 * @file parc_PathName.h
 * @ingroup inputoutput
 * @brief Path Name Manipulation
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_PathName_h
#define libparc_parc_PathName_h

struct parc_pathname;
typedef struct parc_pathname PARCPathName;

#include <parc/algol/parc_BufferComposer.h>

/**
 * Create an empty, relative, `PARCPathName`.
 *
 * @return A pointer to a `PARCPathName` instance.
 *
 * @see {@link parcPathName_MakeAbsolute}
 *
 * Example:
 * @code
 * {
 *     PARCPathName *result = parcPathName_Create();
 *     parcPathName_Destroy(&result);
 * }
 * <#example#>
 * @endcode
 */
PARCPathName *parcPathName_Create(void);

/**
 * Parse a null-terminated C string as a `PARCPathName` limited to specific length.
 * Components are separated by a single '/' character.
 *
 * @param [in] limit The limit to the length
 * @param [in] path The string to parse
 *
 * @return  A pointer to the new `PARCPathName`
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCPathName *parcPathName_ParseToLimit(size_t limit, const char path[limit]);

/**
 * Parse a null-terminated C string as a `PARCPathName`
 * Components are separated by a single '/' character.
 *
 * @param [in] path The string to be parsed
 * @return A pointer to the new `PARCPathName`
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCPathName *parcPathName_Parse(const char *path);

/**
 * Acquire a new reference to an instance of `PARCPathName`.
 *
 * The reference count to the instance is incremented.
 *
 * @param [in] pathName The instance of `PARCPathName` to which to refer.
 *
 * @return The same value as the input parameter @p pathName
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
PARCPathName *parcPathName_Acquire(const PARCPathName *pathName);

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
 * @param [in,out] pathNamePtr A pointer to a pointer to the instance to release.
 *
 *
 * Example:
 * @code
 * {
 *     PARCPathName *pathName = parcPathName_Parse("/tmp/foo");
 *
 *     parcPathName_Release(&pathName);
 * }
 * @endcode
 */
void parcPathName_Release(PARCPathName **pathNamePtr);

/**
 * Determine if two `PARCPathName` instances are equal.
 *
 * The following equivalence relations on non-null `PARCPathName` instances are maintained:
 *
 *   * It is reflexive: for any non-null reference value x, `parcPathName_Equals(x, x)` must return true.
 *
 *   * It is symmetric: for any non-null reference values x and y, `parcPathName_Equals(x, y)` must return true if and only if
 *        `parcPathName_Equals(y x)` returns true.
 *
 *   * It is transitive: for any non-null reference values x, y, and z, if
 *        `parcPathName_Equals(x, y)` returns true and
 *        `parcPathName_Equals(y, z)` returns true,
 *        then `parcPathName_Equals(x, z)` must return true.
 *
 *   * It is consistent: for any non-null reference values x and y, multiple invocations of `parcPathName_Equals(x, y)`
 *         consistently return true or consistently return false.
 *
 *   * For any non-null reference value x, `parcPathName_Equals(x, NULL)` must return false.
 *
 * @param [in] x A pointer to a `PARCPathName` instance.
 * @param [in] y A pointer to a `PARCPathName` instance.
 *
 * @return true `PARCPathName` x and y are equal.
 * @return false `PARCPathName` x and y are not equal.
 *
 * Example:
 * @code
 * {
 *    PARCPathName *a = parcPathName_Create();
 *    PARCPathName *b = parcPathName_Create();
 *
 *    if (parcPathName_Equals(a, b)) {
 *        // true
 *    } else {
 *        // false
 *    }
 * }
 * @endcode
 *
 */
bool parcPathName_Equals(const PARCPathName *x, const PARCPathName *y);

/**
 * Copy a pathName into a new instance of `PARCPathName`
 *
 * @param [in] pathName An instance of `PARCPathName` to be copied
 *
 * @return A new instance of `PARCPathName` that is a copy of @p pathName
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 *
 */
PARCPathName *parcPathName_Copy(const PARCPathName *pathName);

/**
 * Return true if the instance of `PARCPathName` is absolute
 *
 * An absolute path name is a fully specified path starting at the root as the left-most segment.
 * A relative path name is an incomplete path starting at an unspecified root.
 *
 * For example, an absolute UNIX file name path begins with a `/` character.
 * `/foo/bar`, `/tmp/test` are both absolute path names.
 * While, `gscott/parc` is a relative path name.
 *
 * @param [in] pathName The instance of `PARCPathName` to test for absoluteness
 * @return True is the path name is absolute, false otherwise.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
bool parcPathName_IsAbsolute(const PARCPathName *pathName);

/**
 * Make a `PARCPathName` absolute or relative.
 *
 * An absolute path name is a fully specified path starting at the root as the left-most segment.
 * A relative path name is an incomplete path starting at an unspecified root.
 *
 * For example, an absolute UNIX file name path begins with a `/` character.
 * `/foo/bar`, `/tmp/test` are both absolute path names.
 * While, `gscott/parc` is a relative path name.
 *
 *
 * @param [in,out] pathName A pointer to a `PARCPathName` instance to be modified
 * @param [in] absolute a flad as to whether the @p pathName should be set to absolute or relative; true indicates absolute
 *
 * @return true if the `PARCPathName` was previously an absolute path name.
 * @return false if the `PARCPathName` was previously a relative path name.
 *
 * Example:
 * @code
 * {
 *     parcPathName_MakeAbsolute(pathName true)
 * }
 * @endcode
 */
bool parcPathName_MakeAbsolute(PARCPathName *pathName, bool absolute);

/**
 * Append a name segment to a `PARCPathName`
 *
 * The C string, `name` is copied.
 *
 * @param [in,out] pathName The instance of `PARCPathName` to modify
 * @param [in] name A pointer to a null-terminated string.  The contents are appended to the @p pathName.
 *
 * @return The input @p pathName
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 */
PARCPathName *parcPathName_Append(PARCPathName *pathName, const char *name);

/**
 * Prepend a name segment to a `PARCPathName`
 *
 * The C string, `name` is copied.
 *
 * @param [in,out] pathName The instance of `PARCPathName` to modify
 * @param [in] name A pointer to a null-terminated string.  The contents are prepended to the @p pathName.
 *
 * @return The input @p pathName
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 */
PARCPathName *parcPathName_Prepend(PARCPathName *pathName, const char *name);

/**
 * Create a new `PARCPathName` instance that consists of the head of an existing `PARCPathName`.
 *
 * If the original `PARCPathName` is an absolute path, the new `PARCPathName` will also be absolute.
 * Otherwise, it will be a relative path.
 *
 * The new `PARCPathName` contains a copy of the requisite components of the orignal `PARCPathName`.
 *
 * @param [in] pathName The original `PARCPathName`
 * @param [in] size The maximum number of path segments to include in the new `PARCPathName`.
 *
 * @return a Pointer to the new `PARCPathName`
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCPathName *parcPathName_Head(const PARCPathName *pathName, size_t size);

/**
 * Create a new `PARCPathName` instance that consists of the tail of an existing `PARCPathName`.
 *
 * The new `PARCPathName` is a relative path and contains a copy of the requisite components of the orignal `PARCPathName`.
 *
 * @param [in] pathName The original `PARCPathName`
 * @param [in] size The maximum number of path segments to include in the new `PARCPathName`.
 *
 * @return a Pointer to the new `PARCPathName`
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCPathName *parcPathName_Tail(const PARCPathName *pathName, size_t size);

/**
 * Get a pointer to the null-terminated C string for the specified path name segment.
 *
 * @param [in] pathName A pointer to a `PARCPathName` instance.
 * @param [in] index The index of the segment
 *
 * @return a pointer to the null-terminate C string for the specified path name segment
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 */
char *parcPathName_GetAtIndex(const PARCPathName *pathName, size_t index);

/**
 * Get the number of path segments in a `PARCPathName`.
 *
 * @param [in] pathName A pointer to a `PARCPathName` instance.
 *
 * @return The number of path segments in the `PARCPathName`
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 */
size_t parcPathName_Size(const PARCPathName *pathName);

/**
 * Append a representation of the specified instance to the given {@link PARCBufferComposer}.
 *
 * @param [in] path A pointer to the `PARCPathName` whose representation should be added to the @p string.
 * @param [in] string A pointer to the `PARCBufferComposer` to which to append the representation of @p path

 *
 * @return NULL Cannot allocate memory.
 * @return non-NULL The given `PARCBufferComposer`.
 *
 * Example:
 * @code
 * {
 *     PARCBufferComposer *result = parcBufferComposer_Create();
 *
 *     parcPathName_BuildString(instance, result);
 *
 *     PARCBuffer *string = parcBufferComposer_FinalizeBuffer(result);
 *     printf("Hello: %s\n", parcBuffer_ToString(string));
 *     parcBuffer_Release(&string);
 *
 *     parcBufferComposer_Release(&result);
 * }
 * @endcode
 */
PARCBufferComposer *parcPathName_BuildString(const PARCPathName *path, PARCBufferComposer *string);

/**
 * Produce a C string representation of the given `PARCPathName`.
 *
 * Produce an allocated, null-terminated string representation of the given `PARCPathName`.
 * The result must be freed by the caller via the `parcMemory_Deallocate()` function.
 *
 * @param [in] pathName A pointer to the `PARCPathName` to convert to a `String`
*
 * @return The string representation of @p pathName
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 *
 */
char *parcPathName_ToString(const PARCPathName *pathName);

/**
 * Print a human readable representation of the given `PARCPathName`.
 *
 * @param [in] pathName A pointer to the instance of `PARCPathName` to display.
 * @param [in] indentation The level of indentation to use to pretty-print the output.
 *
 * Example:
 * @code
 * {
 *     PARCPathName *instance = parcPathName_Create();
 *
 *     parcPathName_Display(instance, 0);
 *
 *     parcPathName_Release(&instance);
 * }
 * @endcode
 *
 */
void parcPathName_Display(const PARCPathName *pathName, int indentation);
#endif // libparc_parc_PathName_h
