/*
 * Copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * This hash is based on FNV-1a, using different lengths.  Please see the FNV-1a
 * website for details on the algorithm: http://www.isthe.com/chongo/tech/comp/fnv
 *
 * @author Ignacio Solis, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>

#include <stdint.h>
#include <unistd.h>

#include <parc/algol/parc_Hash.h>
#include <parc/algol/parc_Object.h>

struct parc_hash_32bits {
    uint32_t accumulator;
};

parcObject_ExtendPARCObject(PARCHash32Bits, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

PARCHash32Bits *
parcHash32Bits_Create(void)
{
    PARCHash32Bits *result = parcObject_CreateInstance(PARCHash32Bits);
    if (result != NULL) {
        result->accumulator = 0;
    }

    return result;
}

PARCHash32Bits *
parcHash32Bits_Update(PARCHash32Bits *hash, const void *data, size_t length)
{
    hash->accumulator = parcHash32_Data_Cumulative(data, length, hash->accumulator);
    return hash;
}

PARCHash32Bits *
parcHash32Bits_UpdateUint32(PARCHash32Bits *hash, uint32_t value)
{
    hash->accumulator = parcHash32_Data_Cumulative(&value, sizeof(value), hash->accumulator);
    return hash;
}

uint32_t
parcHash32Bits_Hash(PARCHash32Bits *hash)
{
    return hash->accumulator;
}

parcObject_ImplementAcquire(parcHash32Bits, PARCHash32Bits);

parcObject_ImplementRelease(parcHash32Bits, PARCHash32Bits);

/*
 * Based on 64-bit FNV-1a
 */
uint64_t
parcHash64_Data(const void *data, size_t len)
{
    // Standard FNV 64-bit offset: see http://www.isthe.com/chongo/tech/comp/fnv/#FNV-param
    const uint64_t fnv1a_offset = 0xCBF29CE484222325ULL;
    return parcHash64_Data_Cumulative(data, len, fnv1a_offset);
}

uint64_t
parcHash64_Data_Cumulative(const void *data, size_t len, uint64_t lastValue)
{
    // Standard FNV 64-bit prime: see http://www.isthe.com/chongo/tech/comp/fnv/#FNV-param
    const uint64_t fnv1a_prime = 0x00000100000001B3ULL;
    uint64_t hash = lastValue;
    const char *chardata = data;

    for (size_t i = 0; i < len; i++) {
        hash = hash ^ chardata[i];
        hash = hash * fnv1a_prime;
    }

    return hash;
}

uint64_t
parcHash64_Int64(uint64_t int64)
{
    return parcHash64_Data(&int64, sizeof(uint64_t));
}

uint64_t
parcHash64_Int32(uint32_t int32)
{
    return parcHash64_Data(&int32, sizeof(uint32_t));
}

uint32_t
parcHash32_Data(const void *data, size_t len)
{
    // Standard FNV 32-bit offset: see http://www.isthe.com/chongo/tech/comp/fnv/#FNV-param
    const uint32_t fnv1a_offset = 0x811C9DC5;
    return parcHash32_Data_Cumulative(data, len, fnv1a_offset);
}

uint32_t
parcHash32_Data_Cumulative(const void *data, size_t len, uint32_t lastValue)
{
    // Standard FNV 32-bit prime: see http://www.isthe.com/chongo/tech/comp/fnv/#FNV-param
    const uint32_t fnv1a_prime = 0x01000193;
    uint32_t hash = lastValue;

    const char *chardata = data;

    for (size_t i = 0; i < len; i++) {
        hash = hash ^ chardata[i];
        hash = hash * fnv1a_prime;
    }

    return hash;
}

uint32_t
parcHash32_Int64(uint64_t int64)
{
    return parcHash32_Data(&int64, sizeof(uint64_t));
}

uint32_t
parcHash32_Int32(uint32_t int32)
{
    return parcHash32_Data(&int32, sizeof(uint32_t));
}
