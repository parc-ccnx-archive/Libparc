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
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>

#include <LongBow/runtime.h>

#include <stdio.h>
#include <stdlib.h>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include <parc/algol/parc_Varint.h>
#include <parc/algol/parc_Memory.h>

struct parc_varint {
    uint64_t value;
};

PARCVarint *
parcVarint_Create(void)
{
    PARCVarint *result = parcMemory_AllocateAndClear(sizeof(PARCVarint));
    if (result != NULL) {
        result->value = 0;
    }
    return result;
}

PARCVarint *
parcVarint_DecodeBuffer(PARCBuffer *buffer, size_t length)
{
    assertNotNull(buffer, "Parameter must be a non-null PARCBuffer pointer.");
    assertTrue(length < sizeof(size_t), "Length must be less then or equal to %zd", sizeof(size_t));
    assertTrue(length <= parcBuffer_Remaining(buffer), "Buffer does not contain at least %zd bytes", length);

    PARCVarint *result = parcVarint_Create();
    assertNotNull(result, "PARCVarint out of memory.");

    for (size_t i = 0; i < length; i++) {
        parcVarint_ShiftLeft(result, 8);
        parcVarint_OrUint8(result, parcBuffer_GetUint8(buffer));
    }

    return result;
}

PARCVarint *
parcVarint_DecodeElasticByteBuffer(const PARCBuffer *buffer, size_t length)
{
    assertNotNull(buffer, "Parameter must be a non-null PARCBuffer pointer.");
    assertTrue(length < sizeof(size_t), "Length must be less then or equal to %zd", sizeof(size_t));

    PARCVarint *result = parcVarint_Create();
    assertNotNull(result, "PARCVarint out of memory.");

    for (size_t i = 0; i < length; i++) {
        parcVarint_ShiftLeft(result, 8);
        parcVarint_OrUint8(result, parcBuffer_GetAtIndex(buffer, i));
    }

    return result;
}

PARCVarint *
parcVarint_Set(PARCVarint *varint, uint64_t newValue)
{
    varint->value = newValue;
    return varint;
}

PARCVarint *
parcVarint_FromElasticByteBuffer(const PARCBuffer *buffer)
{
    assertNotNull(buffer, "Parameter must be a non-null PARCBuffer pointer.");
    PARCVarint *result = parcVarint_Create();

    size_t length = parcBuffer_Remaining(buffer);

    for (size_t i = 0; i < length; i++) {
        parcVarint_ShiftLeft(result, 8);
        parcVarint_OrUint8(result, parcBuffer_GetAtIndex(buffer, i));
    }

    return result;
}

PARCVarint *
parcVarint_FromUTF8ByteBuffer(const PARCBuffer *buffer)
{
    assertNotNull(buffer, "Parameter must not be NULL.");
    PARCVarint *result = parcVarint_Create();

    if (result != NULL) {
        size_t length = parcBuffer_Remaining(buffer);

        for (size_t i = 0; i < length; i++) {
            parcVarint_Multiply(result, 10);
            parcVarint_Add(result, parcBuffer_GetAtIndex(buffer, i) - '0');
        }
    }

    return result;
}

PARCVarint *
parcVarint_FromUTF8Buffer(PARCBuffer *buffer)
{
    assertNotNull(buffer, "Parameter must be a non-null PARCBuffer pointer.");
    PARCVarint *result = parcVarint_Create();

    if (result != NULL) {
        size_t length = parcBuffer_Limit(buffer);

        for (size_t i = 0; i < length; i++) {
            parcVarint_Multiply(result, 10);
            parcVarint_Add(result, parcBuffer_GetAtIndex(buffer, i) - '0');
        }
    }

    return result;
}

/**
 *
 * @param uint
 * @return
 */
PARCVarint *
parcVarint_FromUint8(uint8_t uint)
{
    return parcVarint_FromUint32(uint);
}

/**
 *
 * @param uint
 * @return
 */
PARCVarint *
parcVarint_FromUint32(uint32_t uint)
{
    return parcVarint_FromUint64(uint);
}

/**
 *
 * @param uint
 * @return
 */
PARCVarint *
parcVarint_FromUint64(uint64_t uint)
{
    PARCVarint *result = parcMemory_AllocateAndClear(sizeof(PARCVarint));
    if (result != NULL) {
        result->value = uint;
    }
    return result;
}

/**
 *
 * @param varintP
 */
void
parcVarint_Destroy(PARCVarint **varintP)
{
    assertNotNull(varintP, "Parameter must be a non-null pointer to a pointer to a PARCVarint");
    assertNotNull(*varintP, "Parameter must be a non-null pointer to a PARCVarint");

    parcMemory_Deallocate((void **) varintP);
    *varintP = NULL;
}

/**
 * Shift the value {@code bits} to the left.
 *
 * @param varint
 * @param bits
 * @return
 */
PARCVarint *
parcVarint_ShiftLeft(PARCVarint *varint, int bits)
{
    assertNotNull(varint, "Parameter must be a non-null pointer to a PARCVarint.");
    varint->value <<= bits;

    return varint;
}

PARCVarint *
parcVarint_Add(PARCVarint *varint, int addend)
{
    assertNotNull(varint, "Parameter must be a non-null pointer to a PARCVarint.");
    varint->value += addend;

    return varint;
}

PARCVarint *
parcVarint_Subtract(PARCVarint *varint, int subtrahend)
{
    assertNotNull(varint, "Parameter must be a non-null pointer to a PARCVarint.");
    varint->value -= subtrahend;

    return varint;
}

PARCVarint *
parcVarint_Multiply(PARCVarint *varint, int multiplicand)
{
    assertNotNull(varint, "Parameter must be a non-null pointer to a PARCVarint.");
    varint->value *= multiplicand;

    return varint;
}

PARCVarint *
parcVarint_Divide(PARCVarint *varint, int divisor)
{
    assertNotNull(varint, "Parameter must be a non-null pointer to a PARCVarint.");
    varint->value /= divisor;

    return varint;
}

/**
 * Shift the value {@code bits} to the right.
 *
 * @param varint
 * @param bits
 * @return
 */
PARCVarint *
parcVarint_ShiftRight(PARCVarint *varint, int bits)
{
    assertNotNull(varint, "Parameter must be a non-null pointer to a PARCVarint.");
    varint->value >>= bits;
    return varint;
}

/**
 * Perform an AND operation on the given {@link PARCVarint} with the supplied {@code operand},
 * leaving the result in {@code varint}.
 *
 * @param varint
 * @param operand
 * @return
 */
PARCVarint *
parcVarint_And(PARCVarint *varint, PARCVarint *operand)
{
    assertNotNull(varint, "Parameter varint must not be NULL.");
    assertNotNull(operand, "Parameter operand must not be NULL.");
    varint->value &= operand->value;
    return varint;
}

/**
 *
 * @param varint
 * @param operand
 * @return
 */
PARCVarint *
parcVarint_AndUint8(PARCVarint *varint, uint8_t operand)
{
    assertNotNull(varint, "Parameter varint must not be NULL.");
    varint->value &= operand;
    return varint;
}

/**
 *
 * @param varint
 * @param operand
 * @return
 */
PARCVarint *
parcVarint_AndUint16(PARCVarint *varint, uint16_t operand)
{
    assertNotNull(varint, "Parameter varint must not be NULL.");
    varint->value &= operand;
    return varint;
}

/**
 *
 * @param varint
 * @param operand
 * @return
 */
PARCVarint *
parcVarint_AndUint32(PARCVarint *varint, uint32_t operand)
{
    assertNotNull(varint, "Parameter varint must not be NULL.");
    varint->value &= operand;
    return varint;
}

/**
 *
 * @param varint
 * @param operand
 * @return
 */
PARCVarint *
parcVarint_AndUint64(PARCVarint *varint, uint64_t operand)
{
    assertNotNull(varint, "Parameter must be a non-null PARCVarint pointer.");
    varint->value &= operand;
    return varint;
}

/**
 *
 * @param varint
 * @param operand
 * @return
 */
PARCVarint *
parcVarint_Or(PARCVarint *varint, PARCVarint *operand)
{
    assertNotNull(varint, "Parameter must be a non-null PARCVarint pointer.");
    varint->value |= operand->value;
    return varint;
}

/**
 * Perform an OR operation on the low 8-bits in the given {@link PARCVarint}.
 *
 * @param varint
 * @param operand
 * @return
 */
PARCVarint *
parcVarint_OrUint8(PARCVarint *varint, uint8_t operand)
{
    assertNotNull(varint, "Parameter varint must not be NULL.");
    varint->value |= operand;
    return varint;
}

/**
 * Perform an OR operation on the low 16-bits in the given {@link PARCVarint}.
 *
 * @param varint
 * @param operand
 * @return
 */
PARCVarint *
parcVarint_OrUint16(PARCVarint *varint, uint16_t operand)
{
    assertNotNull(varint, "Parameter varint must not be NULL.");
    varint->value |= operand;
    return varint;
}

/**
 * Perform an OR operation on the low 32-bits in the given {@link PARCVarint}.
 *
 * @param varint
 * @param operand
 * @return
 */
PARCVarint *
parcVarint_OrUint32(PARCVarint *varint, uint32_t operand)
{
    assertNotNull(varint, "Parameter varint must not be NULL.");
    varint->value |= operand;
    return varint;
}

/**
 * Perform an OR operation on the low 64-bits in the given {@link PARCVarint}.
 *
 * @param varint
 * @param operand
 * @return
 */
PARCVarint *
parcVarint_OrUint64(PARCVarint *varint, uint64_t operand)
{
    assertNotNull(varint, "Parameter varint must not be NULL.");
    varint->value |= operand;
    return varint;
}

/**
 * Return {@code true} (non-zero) if the two {@link PARCVarint} structures contain equal data.
 *
 * @param varint
 * @param operand
 * @return
 */
int
parcVarint_Equals(PARCVarint *varint, PARCVarint *operand)
{
    assertNotNull(varint, "Parameter varint must not be NULL.");
    return varint->value == operand->value;
}

/**
 *
 * @param varint
 * @param value
 * @return
 */
int
parcVarint_EqualsUint64(PARCVarint *varint, uint64_t value)
{
    assertNotNull(varint, "Parameter varint must not be NULL.");
    return varint->value == value;
}

/**
 *
 * @param varint
 * @param value
 * @return
 */
int
parcVarint_EqualsUint32(PARCVarint *varint, uint32_t value)
{
    assertNotNull(varint, "Parameter varint must not be NULL.");
    return parcVarint_EqualsUint64(varint, (uint64_t) value);
}

/**
 *
 * @param varint
 * @param value
 * @return
 */
int
parcVarint_EqualsUint16(PARCVarint *varint, uint16_t value)
{
    assertNotNull(varint, "Parameter varint must not be NULL.");
    return parcVarint_EqualsUint64(varint, (uint64_t) value);
}

/**
 *
 * @param varint
 * @param value
 * @return
 */
int
parcVarint_EqualsUint8(PARCVarint *varint, uint8_t value)
{
    assertNotNull(varint, "Parameter varint must not be NULL.");
    return parcVarint_EqualsUint64(varint, (uint64_t) value);
}

/**
 * Produce the 8 low-order bits of this {@code PARCVarint}.
 *
 * @param varint
 * @return
 */
uint8_t
parcVarint_AsUint8(const PARCVarint *varint)
{
    assertNotNull(varint, "Parameter varint must not be NULL.");
    return (uint8_t) varint->value;
}

/**
 * Produce the 16 low-order bits of this {@code PARCVarint}.
 *
 * @param varint
 * @return
 */
uint16_t
parcVarint_AsUint16(const PARCVarint *varint)
{
    assertNotNull(varint, "Parameter varint must not be NULL.");
    return (uint16_t) varint->value;
}

/**
 * Produce the 32 low-order bits of this {@code PARCVarint}.
 *
 * @param varint
 * @return
 */
uint32_t
parcVarint_AsUint32(const PARCVarint *varint)
{
    assertNotNull(varint, "Parameter varint must not be NULL.");
    return (uint32_t) varint->value;
}

uint64_t
parcVarint_AsUint64(const PARCVarint *varint)
{
    assertNotNull(varint, "Parameter varint must not be NULL.");
    return varint->value;
}

/**
 * Produce the value of this {@code PARCVarint} as a {@code size_t} value.
 *
 * @param varint
 * @return
 */
size_t
parcVarint_AsSize(const PARCVarint *varint)
{
    assertNotNull(varint, "Parameter varint must not be NULL.");
    return (size_t) varint->value;
}

/**
 * Produce a string representation of this {@link PARCVarint}.
 * The returned value must be freed by the called using the {@code stdlib.h} {@code free}.
 * @param string
 * @param varint
 * @return
 */
char *
parcVarint_ToString(char **string, PARCVarint *varint)
{
    assertNotNull(varint, "Parameter varint must not be NULL.");
    int nwritten = asprintf(string, "%" PRIu64, varint->value);
    assertTrue(nwritten >= 0, "Error calling asprintf");
    return *string;
}
