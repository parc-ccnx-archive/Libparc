/*
 * Copyright (c) 2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
#include "../parc_Varint.c"

#include <LongBow/unit-test.h>

#include <parc/algol/parc_SafeMemory.h>
#include <parc/algol/parc_BufferComposer.h>

LONGBOW_TEST_RUNNER(parc_VarInt)
{
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Local);
}

LONGBOW_TEST_RUNNER_SETUP(parc_VarInt)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_RUNNER_TEARDOWN(parc_VarInt)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_And);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_AndUint16);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_AndUint32);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_AndUint64);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_AndUint8);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_AsSize);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_AsUint16);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_AsUint32);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_AsUint64);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_AsUint8);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_Destroy);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_Equals);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_EqualsUint16);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_EqualsUint32);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_EqualsUint64);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_EqualsUint8);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_FromByteBuffer);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_FromUTF8ByteBuffer);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_FromUint32);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_FromUint64);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_FromUint8);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_New);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_Or);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_OrUint16);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_OrUint32);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_OrUint64);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_OrUint8);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_Set);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_ShiftLeft);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_ShiftRight);
    LONGBOW_RUN_TEST_CASE(Global, parcVarint_ToString);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDOUT_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, parcVarint_And)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_CASE(Global, parcVarint_AndUint16)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_CASE(Global, parcVarint_AndUint32)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_CASE(Global, parcVarint_AndUint64)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_CASE(Global, parcVarint_AndUint8)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_CASE(Global, parcVarint_AsSize)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_CASE(Global, parcVarint_AsUint16)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_CASE(Global, parcVarint_AsUint32)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_CASE(Global, parcVarint_AsUint64)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_CASE(Global, parcVarint_AsUint8)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_CASE(Global, parcVarint_Destroy)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_CASE(Global, parcVarint_Equals)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_CASE(Global, parcVarint_EqualsUint16)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_CASE(Global, parcVarint_EqualsUint32)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_CASE(Global, parcVarint_EqualsUint64)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_CASE(Global, parcVarint_EqualsUint8)
{
    unsigned char expected = 5;
    PARCVarint *a = parcVarint_FromUint8(expected);
    PARCVarint *b = parcVarint_FromUint8(expected);

    assertTrue(parcVarint_Equals(a, b), "Equal instances failed to be equal.");
    parcVarint_Destroy(&a);
    parcVarint_Destroy(&b);
}

LONGBOW_TEST_CASE(Global, parcVarint_FromByteBuffer)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_CASE(Global, parcVarint_FromUTF8ByteBuffer)
{
    PARCBufferComposer *composer = parcBufferComposer_Create();
    parcBufferComposer_PutString(composer, "10");
    PARCBuffer *buffer = parcBufferComposer_ProduceBuffer(composer);

    PARCVarint *varint = parcVarint_FromUTF8ByteBuffer(buffer);

    uint32_t actual = parcVarint_AsUint32(varint);

    assertTrue(10 == actual, "Expected 10 actual %u", actual);
    parcBuffer_Release(&buffer);
    parcBufferComposer_Release(&composer);
    parcVarint_Destroy(&varint);
}

LONGBOW_TEST_CASE(Global, parcVarint_FromUint32)
{
    uint32_t value = 0x12345678;
    PARCVarint *a = parcVarint_FromUint32(value);
    assertNotNull(a, "Probably out of memory.");

    uint32_t actual = parcVarint_AsUint32(a);

    assertEqual(value, actual, "%u");

    parcVarint_Destroy(&a);
}

LONGBOW_TEST_CASE(Global, parcVarint_FromUint64)
{
    uint64_t value = 0x1234567812345678;
    PARCVarint *a = parcVarint_FromUint64(value);
    assertNotNull(a, "Probably out of memory.");

    uint64_t actual = parcVarint_AsUint64(a);

    assertEqual(value, actual, "%" PRIu64);

    parcVarint_Destroy(&a);
}

LONGBOW_TEST_CASE(Global, parcVarint_FromUint8)
{
    uint8_t value = 0x12;
    PARCVarint *a = parcVarint_FromUint8(value);
    assertNotNull(a, "Probably out of memory.");

    uint8_t actual = parcVarint_AsUint8(a);

    assertEqual(value, actual, "%d");

    parcVarint_Destroy(&a);
}

LONGBOW_TEST_CASE(Global, parcVarint_New)
{
    PARCVarint *a = parcVarint_Create();
    assertNotNull(a, "Probably out of memory.");

    parcVarint_Destroy(&a);
    assertNull(a, "Destroy failed to nullify.");
}

LONGBOW_TEST_CASE(Global, parcVarint_Or)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_CASE(Global, parcVarint_OrUint16)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_CASE(Global, parcVarint_OrUint32)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_CASE(Global, parcVarint_OrUint64)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_CASE(Global, parcVarint_OrUint8)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_CASE(Global, parcVarint_Set)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_CASE(Global, parcVarint_ShiftLeft)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_CASE(Global, parcVarint_ShiftRight)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_CASE(Global, parcVarint_ToString)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_FIXTURE(Local)
{
}

LONGBOW_TEST_FIXTURE_SETUP(Local)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Local)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_VarInt);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
