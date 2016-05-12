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
 * @file parc_StdlibMemory.h
 * @ingroup datastructures
 *
 * @brief Standard library memory mangement functions wrapped up to be suitable for use by parc_Memory.[ch]
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_StdlibMemory_h
#define libparc_parc_StdlibMemory_h

#include <parc/algol/parc_Memory.h>

extern PARCMemoryInterface PARCStdlibMemoryAsPARCMemory;

/**
 * Allocate memory.
 *
 * @param [in] size The size of memory to allocate
 *
 * @return A pointer to the allocated memory.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void *parcStdlibMemory_Allocate(size_t size);

/**
 * Allocate memory of size @p size and clear it.
 *
 * @param [in] size Size of memory to allocate
 *
 * @return A pointer to the allocated memory
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void *parcStdlibMemory_AllocateAndClear(size_t size);

/**
 * Allocate aligned memory.
 *
 * Allocates @p size bytes of memory such that the allocation's
 * base address is an exact multiple of alignment,
 * and returns the allocation in the value pointed to by @p pointer.
 *
 * The requested alignment must be a power of 2 greater than or equal to `sizeof(void *)`.
 *
 * Memory that is allocated can be used as an argument in subsequent call `Reallocate`, however
 * `Reallocate` is not guaranteed to preserve the original alignment.
 *
 * @param [out] pointer A pointer to a `void *` pointer that will be set to the address of the allocated memory.
 * @param [in] alignment A power of 2 greater than or equal to `sizeof(void *)`
 * @param [in] size The number of bytes to allocate.
 *
 * @return 0 Successful
 * @return EINVAL The alignment parameter is not a power of 2 at least as large as sizeof(void *)
 * @return ENOMEM Memory allocation error.
 *
 * Example:
 * @code
 * {
 *     void *allocatedMemory;
 *
 *     int failure = parcStdlibMemory_MemAlign(&allocatedMemory, sizeof(void *), 100);
 *     if (failure == 0) {
 *         parcStdlibMemory_Deallocate(&allocatedMemory);
 *         // allocatedMemory is now equal to zero.
 *     }
 * }
 * @endcode
 * @see {@link parcMemory_MemAlign}
 */
int parcStdlibMemory_MemAlign(void **pointer, size_t alignment, size_t size);

/**
 * Deallocate the memory pointed to by @p pointer
 *
 * @param [in,out] pointer A pointer to a pointer to the memory to be deallocated
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void parcStdlibMemory_Deallocate(void **pointer);

/**
 * Resizes previously allocated memory at @p pointer to @p newSize. If necessary,
 * new memory is allocated and the content copied from the old memory to the
 * new memory and the old memory is deallocated.
 *
 * @param [in,out] pointer A pointer to the memory to be reallocated.
 * @param [in] newSize The size that the memory to be resized to.
 *
 * @return A pointer to the memory
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void *parcStdlibMemory_Reallocate(void *pointer, size_t newSize);

/**
 * Allocate sufficient memory for a copy of the string @p string,
 * copy at most n characters from the string @p string into the allocated memory,
 * and return the pointer to allocated memory.
 *
 * The copied string is always null-terminated.
 *
 * @param [in] string A pointer to a null-terminated string.
 * @param [in] length  The maximum allowed length of the resulting copy.
 *
 * @return non-NULL A pointer to allocated memory.
 * @return NULL A an error occurred.
 *
 * Example:
 * @code
 * {
 *     char *string = "this is a string";
 *     char *copy = parcStdlibMemory_StringDuplicate(string, strlen(string));
 *
 *     if (copy != NULL) {
 *         . . .
 *         parcStdLibMemory_Deallocate(&copy);
 *     }
 * }
 * @endcode
 *
 * @see {@link parcStdlibMemory_Deallocate()}
 */
char *parcStdlibMemory_StringDuplicate(const char *string, size_t length);

/**
 * Return the number of outstanding allocations managed by this allocator.
 *
 * When you allocate memory, this count goes up by one. When you deallocate, it goes down by one.
 * A well-behaved program will terminate with a call to `parcStdlibMemory_Outstanding()` returning 0.
 *
 * @return The number of memory allocations still outstanding (remaining to be deallocated).
 *
 * Example:
 * @code
 * {
 *     uint32_t numberOfAllocations = parcStdlibMemory_Outstanding();
 * }
 * @endcode
 */
uint32_t parcStdlibMemory_Outstanding(void);


/**
 * Replacement function for realloc(3).
 *
 * The standard `realloc()` function tries to change the size of the allocation pointed to by @p oldAlloc to @p newSize,
 * and returns @p oldAlloc.  If there is not enough room to enlarge the memory allocation pointed to by @p oldAlloc,
 * realloc() creates a new allocation, copies as much of the old data pointed to by @p oldAlloc as will fit to the new allocation,
 * frees the old allocation, and returns a pointer to the allocated memory.
 *
 * If @p oldAlloc is `NULL`, `realloc()` is identical to a call to `malloc()` for size bytes.
 * If @p newSize is zero and @p oldAlloc is not NULL, a new, minimum sized object is allocated and the original object is freed.
 * When extending a region allocated with `calloc(3)`, `realloc(3)` does not guarantee that the additional memory
 * is also zero-filled.
 *
 * If the realloc(3) function is not compatible with the above constraints
 * (i.e., ‘realloc(NULL, 0)’ returns an invalid pointer), then autoconf tools will define
 * `realloc` to `rpl_realloc` so that the native realloc is not used in the main project.
 *
 * @param [in] oldAlloc A previously allocated
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
void *rpl_realloc(void *oldAlloc, size_t newSize);
#endif // libparc_parc_StdlibMemory_h
