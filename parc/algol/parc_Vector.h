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
 * @file parc_Vector.h
 * @ingroup datastructures
 * @brief PARC Vector
 * A Vector consists of a pointer and associated length in bytes of data in memory.
 *
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libparc_parc_Vector_h
#define libparc_parc_Vector_h

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

struct parc_vector;

/**
 * A `PARCVector` is a tuple consisting of an address and a length.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
typedef struct parc_vector PARCVector;

/**
 * Create a new `PARCVector` consisting of a pointer and a length.
 *
 * @param [in] pointer A pointer to memory.
 * @param [in] length The length, in bytes.
 * @return An allocated `PARCVector` structure.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCVector *parcVector_Create(const void *pointer, const size_t length);

/**
 * Initialise the given `PARCVector` to the given values.
 *
 * @param [in,out] vector A pointer to the instance of `PARCVector` to initialize
 * @param [in] pointer A pointer to the memory with which to initialize @p vector.
 * @param [in] length The length of the @p pointer contents.
 * @return The pointer to the input @p vector.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCVector *parcVector_Init(PARCVector *vector, const void *pointer, const size_t length);

/**
 * Get the memory pointer for this `PARCVector`.
 *
 * @param [in] vector A pointer to the instance of `PARCVector` from which to get the memory pointer.
 * @return The memory pointer of @p vector
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
const void *parcVector_GetPointer(const PARCVector *vector);

/**
 * Get the length of of the `PARCVector`
 *
 * @param [in] vector The `PARCVector` instance of interest.
 * @return The length of @p vector.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
size_t parcVector_GetLength(const PARCVector *vector);

/**
 * Destroy the instance of `PARCVector`
 *
 * @param [in,out] vector The pointer to the pointer to the `PARCVector` instance to destroy.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void parcVector_Destroy(PARCVector **vector);
#endif // libparc_parc_Vector_h
