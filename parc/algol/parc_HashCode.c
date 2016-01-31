/*
 * Copyright (c) 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>

#include <parc/algol/parc_HashCode.h>

#if PARCHashCodeSize == 64
static const PARCHashCode _fnv1a_prime = 0x00000100000001B3ULL;
const PARCHashCode parcHashCode_InitialValue = 0xCBF29CE484222325ULL;
#else
static const PARCHashCode _fnv1a_prime = 0x01000193;
const PARCHashCode parcHashCode_InitialValue = 0x811C9DC5;
#endif

PARCHashCode
parcHashCode_HashImpl(const uint8_t *memory, size_t length, PARCHashCode initialValue)
{
    // Standard FNV 64-bit prime: see http://www.isthe.com/chongo/tech/comp/fnv/#FNV-param

    PARCHashCode hash = initialValue;

    for (size_t i = 0; i < length; i++) {
        hash = hash ^ memory[i];
        hash = hash * _fnv1a_prime;
    }

    return hash;
}

PARCHashCode
parcHashCode_HashHashCode(PARCHashCode initialValue, PARCHashCode update)
{
    return parcHashCode_HashImpl((uint8_t *) &update, sizeof(PARCHashCode), initialValue);
}
