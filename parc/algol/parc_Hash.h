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
 * @file parc_Hash.h
 * @ingroup datastructures
 * @brief Implements the FNV-1a 64-bit and 32-bit hashes.
 *
 * These are some basic hashing functions for blocks of data and integers. They
 * generate 64 and 32 bit hashes (They are currently using the FNV-1a algorithm.)
 * There is also a cumulative version of the hashes that can be used if intermediary
 * hashes are required/useful.
 *
 * @author Ignacio Solis, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_Hash_h
#define libparc_parc_Hash_h

#include <stdint.h>
#include <stdlib.h>


struct parc_hash_32bits;
/**
 * @typedef PARCHash32Bits
 * @brief An accumulator
 */

typedef struct parc_hash_32bits PARCHash32Bits;

/**
 * Create a 32 bit hash generator
 *
 * @return A pointer to a `PARCHash32Bits` instance
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
PARCHash32Bits *parcHash32Bits_Create(void);

/**
 * Generate a 32 bit hash from a memory block starting with a previous hash
 *
 * This is a typed version of {@link parcHash32_Data_Cumulative}
 *
 * @param [in] hash the value of the last cumulative hash calculated
 * @param [in] data pointer to a memory block.
 * @param [in] length  length of the memory pointed to by data
 *
 * @return pointer to a {@link PARCHash32Bits} with the cumulative hash
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
PARCHash32Bits *parcHash32Bits_Update(PARCHash32Bits *hash, const void *data, size_t length);

/**
 * Generate a 32 bit hash from a uint32 starting with a previous hash
 * Update the cumulative state of the given {@link PARCHash32Bits}
 *
 * @param [in] hash the value of the last cumulative hash calculated
 * @param [in] value  The `uint32` to be hashed
 *
 * @return pointer to a `PARCHash32Bits` with the cumulative hash
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
PARCHash32Bits *parcHash32Bits_UpdateUint32(PARCHash32Bits *hash, uint32_t value);

/**
 * Get the current value of the cummulative state of the given {@link PARCHash32Bits}.
 *
 * @param [in] hash the value of the last cumulative hash calculated
 *
 * @return The hash value as an unsigned 32 bit integer
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
uint32_t parcHash32Bits_Hash(PARCHash32Bits *hash);

/**
 * Acquire a new reference to the given {@link PARCHash32Bits} instance.
 *
 * The reference count to the instance is incremented.
 *
 * @param [in] hash The instance of `PARCHash32Bits` to which to refer.
 *
 * @return The same value as the input parameter @p hash
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
PARCHash32Bits *parcHash32Bits_Acquire(const PARCHash32Bits *hash);

/**
 * Release a reference to the given {@link PARCHash32Bits} instance.
 *
 * Only the last invocation where the reference count is decremented to zero,
 * will actually destroy the `PARCHash32Bits`.
 *
 * @param [in,out] hash is a pointer to the `PARCHash32Bits` reference.
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
void parcHash32Bits_Release(PARCHash32Bits **hash);

/**
 * Generate a 64 bit hash from a memory block
 *
 * This function will generate a 64bit hash from a block of memory. The memory block
 * is not modified in any way.
 *
 * The output of this function can be used as input for the cumulative version of
 * this function
 *
 * @param [in] data A pointer to a memory block.
 * @param [in] len  The length of the memory pointed to by data
 *
 * @return hash_64bit A 64 bit hash of the memory block.
 *
 * Example:
 * @code
 *
 * char * data = "Hello world of hashing";
 * uint64_t myhash = parcHash64_Data(data,strlen(data));
 *
 * @endcode
 *
 * @see {@link parcHash64_Data_Cumulative}
 */
uint64_t parcHash64_Data(const void *data, size_t len);

/**
 * Generate a 64 bit hash from a memory block starting with a previous hash
 *
 * This function will generate a 64 bit hash from a block of memory and an initial
 * hash. This is used to cumulatively calculate a hash of larger block. So,
 * one could generate the hash of the first part of the data (A), then calculate the
 * hash including the next part of the data (AB) and then the hash of the next
 * part (ABC).  This is useful for not having to recalculate the hash of the parts
 * you have already hashed.
 *
 * A cumulative hash should have the same value as a full hash of the complete data.
 * So cumulative_hash(B,len(B),hash(A)) is equal to hash(AB,len(AB)).
 *
 * @param [in] data pointer to a memory block.
 * @param [in] len  length of the memory pointed to by data
 * @param [in] lastValue the vale of the last cumulative hash calculated
 *
 * @return cumulative_hash64 A 64 bit hash of the data _and_ the data that was
 * hashed before (represented by lastValue).
 *
 * Example:
 * @code
 *
 * char * data1 = "1234567890";
 * uint64_t myhash1 = parcHash64_Data(data1,10);
 * char * data2 = "abcdefghij";
 * uint64_t myhash2 = parcHash64_Data_Cumulative(data2,10,myhash1);
 * char * data3 = "1234567890abcdefghij";
 * uint64_t myhash3 = parcHash64_Data(data3,20);
 * // myhash3 will be equal to myhash2
 *
 * @endcode
 *
 * @see {@link parcHash64_Data}
 */
uint64_t parcHash64_Data_Cumulative(const void *data, size_t len, uint64_t lastValue);

/**
 * Generate a 64 bit hash from a 64 bit Integer
 *
 * This function hashes a 64 bit integer into a 64 bit hash
 *
 * @param [in] int64 A 64 bit integer
 *
 * @return hash64 A 64 bit hash of the 64 bit integer
 *
 * Example:
 * @code
 * uint64_t id64 = 1234567890123456;
 * uint64_t hash64 = parcHash64_Int64(id64);
 * @endcode
 *
 */
uint64_t parcHash64_Int64(uint64_t int64);

/**
 * Generate a 64 bit hash from a 32 bit Integer
 *
 * This function hashes a 32 bit integer into a 64 bit hash
 *
 * @param [in] int32 A 32 bit integer
 *
 * @return hash64 A 64 bit hash of the 32 bit integer
 *
 * Example:
 * @code
 * uint32_t id32 = 70;
 * uint64_t hash64 = parcHash64_Int32(id32);
 * @endcode
 *
 */
uint64_t parcHash64_Int32(uint32_t int32);

/**
 * Generate a 32 bit hash from a memory block
 *
 * This function will generate a 32bit hash from a block of memory. The memory block
 * is not modified in any way.
 * The output of this function can be used as input for the cumulative version of
 * this function.
 *
 * @param [in] data pointer to a memory block.
 * @param [in] len  length of the memory pointed to by data
 *
 * @return hash_32bit A 32 bit hash of the memory block.
 *
 * Example:
 * @code
 *
 * char * data = "Hello world of hashing";
 * uint32_t myhash = parcHash32_Data(data,strlen(data));
 *
 * @endcode
 *
 * @see {@link parcHash32_Data_Cumulative}
 */
uint32_t parcHash32_Data(const void *data, size_t len);

/**
 * Generate a 32 bit hash from a memory block starting with a previous hash
 *
 * This function will generate a 32 bit hash from a block of memory and an initial
 * hash. This is used to cumulatively calculate a hash of larger block. So,
 * one could generate the hash of the first part of the data (A), then calculate the
 * hash including the next part of the data (AB) and then the hash of the next
 * part (ABC).  This is useful for not having to recalculate the hash of the parts
 * you have already hashed.
 *
 * A cumulative hash should have the same value as a full hash of the complete data.
 * So cumulative_hash(B,len(B),hash(A)) is equal to hash(AB,len(AB)).
 *
 * @param [in] data pointer to a memory block.
 * @param [in] len  length of the memory pointed to by data
 * @param [in] lastValue the vale of the last cumulative hash calculated
 *
 * @return cumulative_hash32 A 32 bit hash of the data _and_ the data that was
 * hashed before (represented by lastValue).
 *
 * Example:
 * @code
 *
 * char * data1 = "1234567890";
 * uint32_t myhash1 = parcHash32_Data(data1,10);
 * char * data2 = "abcdefghij";
 * uint32_t myhash2 = parcHash32_Data_Cumulative(data2,10,myhash1);
 * char * data3 = "1234567890abcdefghij";
 * uint32_t myhash3 = parcHash32_Data(data3,20);
 * // myhash3 will be equal to myhash2
 *
 * @endcode
 *
 * @see {@link parcHash32_Data}
 */
uint32_t parcHash32_Data_Cumulative(const void *data, size_t len, uint32_t lastValue);

/**
 * Generate a 32 bit hash from a 64 bit Integer
 *
 * This function hashes a 64 bit integer into a 32 bit hash
 *
 * @param [in] int64 A 64 bit integer
 *
 * @return hash32 A 32 bit hash of the 64 bit integer
 *
 * Example:
 * @code
 * uint64_t id64 = 1234567890123456;
 * uint32_t hash32 = parcHash32_Int64(id64);
 * @endcode
 *
 */
uint32_t parcHash32_Int64(uint64_t int64);

/**
 * Generate a 32 bit hash from a 32 bit Integer
 *
 * This function hashes a 32 bit integer into a 32 bit hash
 *
 * @param [in] int32 A 32 bit integer
 *
 * @return hash32 A 32 bit hash of the 32 bit integer
 *
 * Example:
 * @code
 * uint32_t id32 = 1234567890123456;
 * uint32_t hash32 = parcHash32_Int32(id32);
 * @endcode
 *
 */
uint32_t parcHash32_Int32(uint32_t int32);
#endif // libparc_parc_Hash_h
