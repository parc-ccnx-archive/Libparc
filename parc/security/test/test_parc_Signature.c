/*
 * Copyright (c) 2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../parc_Signature.c"
#include <inttypes.h>

#include <parc/algol/parc_SafeMemory.h>
#include <LongBow/unit-test.h>
#include <parc/testing/parc_ObjectTesting.h>

LONGBOW_TEST_RUNNER(parc_Signature)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Local);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_Signature)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_Signature)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcSignature_Create);
    LONGBOW_RUN_TEST_CASE(Global, parcSignature_Acquire);
    LONGBOW_RUN_TEST_CASE(Global, parcSignature_Release);
    LONGBOW_RUN_TEST_CASE(Global, parcSignature_Equals);
    LONGBOW_RUN_TEST_CASE(Global, parcSignature_GetHashType);
    LONGBOW_RUN_TEST_CASE(Global, parcSignature_GetSignature);
    LONGBOW_RUN_TEST_CASE(Global, parcSignature_GetSigningAlgorithm);
    LONGBOW_RUN_TEST_CASE(Global, parcSignature_ToString);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDERR_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, parcSignature_Create)
{
    PARCBuffer *bits = parcBuffer_Allocate(10); // arbitrary buffer size -- not important
    PARCSignature *signature = parcSignature_Create(PARCSigningAlgorithm_DSA, PARC_HASH_SHA256, bits);
    parcBuffer_Release(&bits);

    assertNotNull(signature, "Expected non-NULL PARCSignature");
    PARCReferenceCount referenceCount = parcObject_GetReferenceCount(signature);
    assertTrue(referenceCount == 1,
               "Expected reference count to be equal to 1, got %" PRIu64 "",
               referenceCount);

    parcSignature_Release(&signature);
}

LONGBOW_TEST_CASE(Global, parcSignature_Acquire)
{
    PARCBuffer *bits = parcBuffer_Allocate(10); // arbitrary buffer size -- not important
    PARCSignature *signature = parcSignature_Create(PARCSigningAlgorithm_DSA, PARC_HASH_SHA256, bits);
    PARCSignature *handle = parcSignature_Acquire(signature);
    parcBuffer_Release(&bits);

    assertNotNull(signature, "Expected non-NULL PARCSignature");
    assertNotNull(handle, "Expected non-NULL PARCSignature after acquisition");
    PARCReferenceCount referenceCount = parcObject_GetReferenceCount(handle);
    assertTrue(referenceCount == 2,
               "Expected reference count to be equal to 2, got %" PRIu64 "",
               referenceCount);

    parcSignature_Release(&signature);
    parcSignature_Release(&handle);
}

LONGBOW_TEST_CASE(Global, parcSignature_Release)
{
    PARCBuffer *bits = parcBuffer_Allocate(10); // arbitrary bufer size -- not important
    PARCSignature *signature = parcSignature_Create(PARCSigningAlgorithm_DSA, PARC_HASH_SHA256, bits);
    PARCSignature *handle = parcSignature_Acquire(signature);
    parcBuffer_Release(&bits);

    assertNotNull(signature, "Expected non-NULL PARCSignature");
    assertNotNull(handle, "Expected non-NULL PARCSignature after acquisition");
    PARCReferenceCount referenceCount = parcObject_GetReferenceCount(handle);
    assertTrue(referenceCount == 2,
               "Expected reference count to be equal to 2, got %" PRIu64 "",
               referenceCount);

    parcSignature_Release(&signature);
    parcSignature_Release(&handle);

    assertNull(signature, "Expected NULL PARCSignature");
}

LONGBOW_TEST_CASE(Global, parcSignature_Equals)
{
    PARCBuffer *bits = parcBuffer_Allocate(10); // arbitrary bufer size -- not important
    PARCBuffer *otherBits = parcBuffer_Allocate(strlen("hello"));
    parcBuffer_PutArray(otherBits, strlen("hello"), (uint8_t *) "hello");

    PARCSignature *x = parcSignature_Create(PARCSigningAlgorithm_DSA, PARC_HASH_SHA256, bits);
    PARCSignature *y = parcSignature_Create(PARCSigningAlgorithm_DSA, PARC_HASH_SHA256, bits);
    PARCSignature *z = parcSignature_Create(PARCSigningAlgorithm_DSA, PARC_HASH_SHA256, bits);
    PARCSignature *unequal1 = parcSignature_Create(PARCSigningAlgorithm_HMAC, PARC_HASH_SHA256, bits);
    PARCSignature *unequal2 = parcSignature_Create(PARCSigningAlgorithm_DSA, PARC_HASH_CRC32C, bits);
    PARCSignature *unequal3 = parcSignature_Create(PARCSigningAlgorithm_DSA, PARC_HASH_SHA256, otherBits);

    parcObjectTesting_AssertEqualsFunction(parcSignature_Equals, x, y, z, unequal1, unequal2, unequal3, NULL);

    parcSignature_Release(&x);
    parcSignature_Release(&y);
    parcSignature_Release(&z);
    parcSignature_Release(&unequal1);
    parcSignature_Release(&unequal2);
    parcSignature_Release(&unequal3);

    parcBuffer_Release(&bits);
    parcBuffer_Release(&otherBits);
}

LONGBOW_TEST_CASE(Global, parcSignature_GetHashType)
{
    PARCBuffer *bits = parcBuffer_Allocate(strlen("Hello"));
    parcBuffer_PutArray(bits, strlen("Hello"), (uint8_t *) "Hello");
    PARCCryptoHashType expected = PARC_HASH_SHA256;
    PARCSignature *signature = parcSignature_Create(PARCSigningAlgorithm_DSA, expected, bits);
    parcBuffer_Release(&bits);

    PARCCryptoHashType actual = parcSignature_GetHashType(signature);

    assertTrue(expected == actual, "Expected %d, actual %d", expected, actual);
    parcSignature_Release(&signature);
}

LONGBOW_TEST_CASE(Global, parcSignature_GetSignature)
{
    PARCBuffer *expected = parcBuffer_Allocate(strlen("Hello"));
    parcBuffer_PutArray(expected, strlen("Hello"), (uint8_t *) "Hello");
    PARCSignature *signature = parcSignature_Create(PARCSigningAlgorithm_DSA, PARC_HASH_SHA256, expected);

    PARCBuffer *actual = parcSignature_GetSignature(signature);

    assertTrue(parcBuffer_Equals(expected, actual), "Expected the original signature bits to be equal to the actual bits");
    parcSignature_Release(&signature);
    parcBuffer_Release(&expected);
}

LONGBOW_TEST_CASE(Global, parcSignature_GetSigningAlgorithm)
{
    PARCBuffer *signatureBits = parcBuffer_Allocate(strlen("Hello"));
    parcBuffer_PutArray(signatureBits, strlen("Hello"), (uint8_t *) "Hello");
    PARCSigningAlgorithm expected = PARCSigningAlgorithm_DSA;
    PARCSignature *signature = parcSignature_Create(expected, PARC_HASH_SHA256, signatureBits);

    PARCSigningAlgorithm actual = parcSignature_GetSigningAlgorithm(signature);

    assertTrue(expected == actual, "Expected %d, actual %d", expected, actual);
    parcSignature_Release(&signature);
    parcBuffer_Release(&signatureBits);
}

LONGBOW_TEST_CASE(Global, parcSignature_ToString)
{
    PARCBuffer *signatureBits = parcBuffer_Allocate(strlen("Hello"));
    parcBuffer_PutArray(signatureBits, strlen("Hello"), (uint8_t *) "Hello");
    PARCSigningAlgorithm expected = PARCSigningAlgorithm_DSA;
    PARCSignature *signature = parcSignature_Create(expected, PARC_HASH_SHA256, signatureBits);

    char *string = parcSignature_ToString(signature);

    assertNotNull(string, "Expected non-NULL result from parcSignature_ToString");

    parcMemory_Deallocate((void **) &string);
    parcSignature_Release(&signature);
    parcBuffer_Release(&signatureBits);
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
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_Signature);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
