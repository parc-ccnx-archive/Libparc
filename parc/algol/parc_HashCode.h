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
 * @file parc_HashCode.h
 * @brief The type returned from implementations of the _HashCode() function.
 *
 * The size of a PARCHashCode value may be different depending on the compile-time compilation environment.
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef PARC_Library_parc_HashCode_h
#define PARC_Library_parc_HashCode_h

#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>

#define PARCHashCodeSize 64
//#define PARCHashCodeSize 32

/**
 * @typedef PARCHashCode
 * @brief The type returned from implementations of the _HashCode() function.
 */
#if PARCHashCodeSize == 64
#define PRIPARCHashCode PRIu64
#define PRIXPARCHashCode PRIX64
#define PRIxPARCHashCode PRIx64
typedef uint64_t PARCHashCode;

#else
#define PRIPARCHashCode PRIu32
#define PRIXPARCHashCode PRIX32
#define PRIxPARCHashCode PRIx32
typedef uint32_t PARCHashCode;

#endif

extern const PARCHashCode parcHashCode_InitialValue;

#define parcHashCode_Hash(_memory_, _length_) parcHashCode_HashImpl(_memory_, _length_, parcHashCode_InitialValue)

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [in] memory A pointer to bytes used to generate the `PARCHashCode`.
 * @param [in] length The number of bytes in memory to use to generate the `PARCHashCode`
 * @param [in] initialValue An inital value for the `PARCHashCode`.
 *
 * @return The resulting `PARCHashCode` value.
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
PARCHashCode parcHashCode_HashImpl(const uint8_t *memory, size_t length, PARCHashCode initialValue);

/**
 * Hash a PARcHashCode into an existing PARCHashCode.
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [in] initialValue The PARCHashCode initial value
 * @param [in] update The PARCHashCode value to update the initial value.
 *
 * @return The updated PARCHashCode value
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
PARCHashCode parcHashCode_HashHashCode(PARCHashCode initialValue, PARCHashCode update);
#endif
