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
#include <config.h>
#include <LongBow/unit-test.h>

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../parc_CryptoCache.c"
#include <parc/algol/parc_SafeMemory.h>
#include <parc/algol/parc_BufferComposer.h>
#include <parc/security/parc_CryptoHashType.h>

LONGBOW_TEST_RUNNER(parc_CryptoCache)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Allocate);
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_CryptoCache)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_CryptoCache)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Allocate)
{
    LONGBOW_RUN_TEST_CASE(Allocate, parcCryptoCache_Create_Destroy);
}

LONGBOW_TEST_FIXTURE_SETUP(Allocate)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Allocate)
{
    if (parcSafeMemory_ReportAllocation(STDOUT_FILENO) != 0) {
        printf("('%s' leaks memory by %d (allocs - frees)) ", longBowTestCase_GetName(testCase), parcMemory_Outstanding());
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Allocate, parcCryptoCache_Create_Destroy)
{
    PARCCryptoCache *cache = parcCryptoCache_Create();
    parcCryptoCache_Destroy(&cache);
}

static PARCCryptoCache *cache_under_test;

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcCryptoCache_AddGetKey);
    LONGBOW_RUN_TEST_CASE(Global, parcCryptoCache_GetMissingKey);
    LONGBOW_RUN_TEST_CASE(Global, parcCryptoCache_GetWrongKey);
    LONGBOW_RUN_TEST_CASE(Global, parcCryptoCache_RemoveKey);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    cache_under_test = parcCryptoCache_Create();
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    parcCryptoCache_Destroy(&cache_under_test);

    if (parcSafeMemory_ReportAllocation(STDOUT_FILENO) != 0) {
        printf("('%s' leaks memory by %d (allocs - frees)) ", longBowTestCase_GetName(testCase), parcMemory_Outstanding());
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, parcCryptoCache_AddGetKey)
{
    PARCBuffer *bb_id = parcBuffer_Wrap("choo choo", 9, 0, 9);
    PARCKeyId *keyid = parcKeyId_Create(bb_id);
    parcBuffer_Release(&bb_id);

    PARCBufferComposer *composer = parcBufferComposer_Create();
    parcBufferComposer_PutString(composer, "quack quack");
    PARCBuffer *bb_key = parcBufferComposer_ProduceBuffer(composer);
    PARCKey *key = parcKey_CreateFromDerEncodedPublicKey(keyid, PARCSigningAlgorithm_RSA, bb_key);

    parcCryptoCache_AddKey(cache_under_test, key);

    const PARCKey *test = parcCryptoCache_GetKey(cache_under_test, keyid);

    parcBuffer_Release(&bb_key);
    parcBufferComposer_Release(&composer);
    parcKeyId_Release(&keyid);

    assertTrue(parcKey_Equals(key, test), "did not return expected key from cache");
    parcKey_Release(&key);
}

LONGBOW_TEST_CASE(Global, parcCryptoCache_GetMissingKey)
{
    PARCBuffer *bb_id = parcBuffer_Wrap("choo choo", 9, 0, 9);
    PARCKeyId *keyid = parcKeyId_Create(bb_id);
    parcBuffer_Release(&bb_id);

    const PARCKey *test = parcCryptoCache_GetKey(cache_under_test, keyid);

    assertNull(test, "Get missing key returned something!");

    parcKeyId_Release(&keyid);
}

LONGBOW_TEST_CASE(Global, parcCryptoCache_GetWrongKey)
{
    PARCBuffer *bb_id = parcBuffer_Wrap("choo choo", 9, 0, 9);
    PARCBuffer *bb_id2 = parcBuffer_Wrap("not here!", 9, 0, 9);

    PARCKeyId *keyid = parcKeyId_Create(bb_id);
    parcBuffer_Release(&bb_id);

    PARCKeyId *keyid2 = parcKeyId_Create(bb_id2);
    parcBuffer_Release(&bb_id2);

    PARCBufferComposer *composer = parcBufferComposer_Create();
    parcBufferComposer_PutString(composer, "quack quack");
    PARCBuffer *bb_key = parcBufferComposer_ProduceBuffer(composer);
    PARCKey *key = parcKey_CreateFromDerEncodedPublicKey(keyid, PARCSigningAlgorithm_RSA, bb_key);

    parcCryptoCache_AddKey(cache_under_test, key);

    const PARCKey *test = parcCryptoCache_GetKey(cache_under_test, keyid2);
    assertNull(test, "Get missing key returned something!");

    parcKey_Release(&key);
    parcBuffer_Release(&bb_key);
    parcBufferComposer_Release(&composer);
    parcKeyId_Release(&keyid);
    parcKeyId_Release(&keyid2);
}

/**
 * Add in 2 keys, remove 1, fetch the other
 */
LONGBOW_TEST_CASE(Global, parcCryptoCache_RemoveKey)
{
    PARCBufferComposer *composer1 = parcBufferComposer_Create();
    parcBufferComposer_PutString(composer1, "quack quack");
    PARCBuffer *bb_key1 = parcBufferComposer_ProduceBuffer(composer1);

    PARCBufferComposer *composer2 = parcBufferComposer_Create();
    parcBufferComposer_PutString(composer2, "Come with me and you'll be");
    PARCBuffer *bb_key2 = parcBufferComposer_ProduceBuffer(composer2);

    PARCBuffer *bb_id = parcBuffer_Wrap("choo choo", 9, 0, 9);
    PARCBuffer *bb_id2 = parcBuffer_Wrap("not here!", 9, 0, 9);

    PARCKeyId *keyid1 = parcKeyId_Create(bb_id);
    parcBuffer_Release(&bb_id);
    PARCKeyId *keyid2 = parcKeyId_Create(bb_id2);
    parcBuffer_Release(&bb_id2);

    PARCKey *key1 = parcKey_CreateFromDerEncodedPublicKey(keyid1, PARCSigningAlgorithm_RSA, bb_key1);
    PARCKey *key2 = parcKey_CreateFromDerEncodedPublicKey(keyid2, PARCSigningAlgorithm_RSA, bb_key2);
    const PARCKey *test;

    parcCryptoCache_AddKey(cache_under_test, key1);
    parcCryptoCache_AddKey(cache_under_test, key2);

    test = parcCryptoCache_GetKey(cache_under_test, keyid1);
    assertTrue(parcKey_Equals(key1, test), "Got wrong key");

    test = parcCryptoCache_GetKey(cache_under_test, keyid2);
    assertTrue(parcKey_Equals(key2, test), "Got wrong key");

    // remove will free the key, so make a copy of it before removing
    PARCKeyId *keyid1_copy = parcKeyId_Copy(keyid1);
    parcCryptoCache_RemoveKey(cache_under_test, keyid1);

    test = parcCryptoCache_GetKey(cache_under_test, keyid1_copy);
    assertNull(test, "Get of deleted key returned non-null");

    test = parcCryptoCache_GetKey(cache_under_test, keyid2);
    assertTrue(parcKey_Equals(key2, test), "Got wrong key");


    parcKey_Release(&key1);
    parcKey_Release(&key2);

    parcBuffer_Release(&bb_key1);
    parcBufferComposer_Release(&composer1);
    parcKeyId_Release(&keyid1);
    parcBuffer_Release(&bb_key2);
    parcBufferComposer_Release(&composer2);
    parcKeyId_Release(&keyid2);

    parcKeyId_Release(&keyid1_copy);
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_CryptoCache);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
