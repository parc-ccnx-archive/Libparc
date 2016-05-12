/*
 * Copyright (c) 2014-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
#include "../parc_Key.c"

#include <LongBow/unit-test.h>
#include <parc/testing/parc_ObjectTesting.h>

LONGBOW_TEST_RUNNER(parc_Key)
{
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Errors);
}

LONGBOW_TEST_RUNNER_SETUP(parc_Key)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_RUNNER_TEARDOWN(parc_Key)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcKey_CreateFromDerEncodedPublicKey);
    LONGBOW_RUN_TEST_CASE(Global, parcKey_CreateFromSymmetricKey);
    LONGBOW_RUN_TEST_CASE(Global, parcKey_Equals);
    LONGBOW_RUN_TEST_CASE(Global, parcKey_Copy);
    LONGBOW_RUN_TEST_CASE(Global, parcKey_GetKey);
    LONGBOW_RUN_TEST_CASE(Global, parcKey_GetKeyId);
    LONGBOW_RUN_TEST_CASE(Global, parcKey_GetSigningAlgorithm);
    LONGBOW_RUN_TEST_CASE(Global, parcKey_Acquire);
    LONGBOW_RUN_TEST_CASE(Global, parcKey_ToString);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, parcKey_Copy)
{
    PARCBuffer *bb_id = parcBuffer_Wrap("choo choo", 9, 0, 9);
    PARCKeyId *keyid = parcKeyId_Create(bb_id);
    parcBuffer_Release(&bb_id);

    PARCBufferComposer *composer = parcBufferComposer_Create();
    parcBufferComposer_PutString(composer, "quack quack");
    PARCBuffer *bb_key = parcBufferComposer_ProduceBuffer(composer);

    PARCKey *key = parcKey_CreateFromDerEncodedPublicKey(keyid, PARCSigningAlgorithm_RSA, bb_key);
    parcKey_AssertValid(key);

    PARCKey *copy = parcKey_Copy(key);
    parcKey_AssertValid(copy);

    assertTrue(parcKey_Equals(key, copy), "Expected the original key instance and its copy to be equal");

    parcKey_Release(&copy);
    parcKey_Release(&key);
    parcBuffer_Release(&bb_key);
    parcBufferComposer_Release(&composer);
    parcKeyId_Release(&keyid);
}

LONGBOW_TEST_CASE(Global, parcKey_CreateFromDerEncodedPublicKey)
{
    PARCBuffer *bb_id = parcBuffer_Wrap("choo choo", 9, 0, 9);
    PARCKeyId *keyid = parcKeyId_Create(bb_id);
    parcBuffer_Release(&bb_id);

    PARCBufferComposer *composer = parcBufferComposer_Create();
    parcBufferComposer_PutString(composer, "quack quack");
    PARCBuffer *bb_key = parcBufferComposer_ProduceBuffer(composer);

    PARCKey *key = parcKey_CreateFromDerEncodedPublicKey(keyid, PARCSigningAlgorithm_RSA, bb_key);

    parcKey_AssertValid(key);

    parcKey_Release(&key);
    parcBuffer_Release(&bb_key);
    parcBufferComposer_Release(&composer);
    parcKeyId_Release(&keyid);
}

LONGBOW_TEST_CASE(Global, parcKey_CreateFromSymmetricKey)
{
    PARCBuffer *bb_id = parcBuffer_Wrap("choo choo", 9, 0, 9);
    PARCKeyId *keyid = parcKeyId_Create(bb_id);
    parcBuffer_Release(&bb_id);

    PARCBufferComposer *composer = parcBufferComposer_Create();
    parcBufferComposer_PutString(composer, "quack quack");
    PARCBuffer *bb_key = parcBufferComposer_ProduceBuffer(composer);
    PARCKey *key = parcKey_CreateFromSymmetricKey(keyid, PARCSigningAlgorithm_HMAC, bb_key);

    parcKey_AssertValid(key);

    parcKey_Release(&key);
    parcBuffer_Release(&bb_key);
    parcBufferComposer_Release(&composer);
    parcKeyId_Release(&keyid);
}

LONGBOW_TEST_CASE(Global, parcKey_Equals)
{
    PARCBuffer *bb_id_1 = parcBuffer_Wrap("choo choo", 9, 0, 9);
    PARCKeyId *keyid_1 = parcKeyId_Create(bb_id_1);
    parcBuffer_Release(&bb_id_1);

    PARCBuffer *bb_id_2 = parcBuffer_Wrap("chugga chugga", 13, 0, 13);
    PARCKeyId *keyid_2 = parcKeyId_Create(bb_id_2);
    parcBuffer_Release(&bb_id_2);

    PARCBufferComposer *composer1 = parcBufferComposer_Create();
    parcBufferComposer_PutString(composer1, "quack quack");
    PARCBuffer *bb_key_1 = parcBufferComposer_ProduceBuffer(composer1);

    PARCKey *x = parcKey_CreateFromDerEncodedPublicKey(keyid_1, PARCSigningAlgorithm_RSA, bb_key_1);
    PARCKey *y = parcKey_CreateFromDerEncodedPublicKey(keyid_1, PARCSigningAlgorithm_RSA, bb_key_1);
    PARCKey *z = parcKey_CreateFromDerEncodedPublicKey(keyid_1, PARCSigningAlgorithm_RSA, bb_key_1);

    PARCBufferComposer *composer2 = parcBufferComposer_Create();
    parcBufferComposer_PutString(composer2, "mew mew");
    PARCBuffer *bb_key_2 = parcBufferComposer_ProduceBuffer(composer2);
    PARCKey *u1 = parcKey_CreateFromDerEncodedPublicKey(keyid_1, PARCSigningAlgorithm_RSA, bb_key_2);
    PARCKey *u2 = parcKey_CreateFromDerEncodedPublicKey(keyid_2, PARCSigningAlgorithm_RSA, bb_key_1);

    parcObjectTesting_AssertEqualsFunction(parcKey_Equals, x, y, z, u1, u2);

    parcBuffer_Release(&bb_key_1);
    parcBufferComposer_Release(&composer1);
    parcBuffer_Release(&bb_key_2);
    parcBufferComposer_Release(&composer2);
    parcKeyId_Release(&keyid_1);
    parcKeyId_Release(&keyid_2);

    parcKey_Release(&x);
    parcKey_Release(&y);
    parcKey_Release(&z);
    parcKey_Release(&u1);
    parcKey_Release(&u2);
}

LONGBOW_TEST_CASE(Global, parcKey_GetKey)
{
    PARCBuffer *bb_id = parcBuffer_Wrap("choo choo", 9, 0, 9);
    PARCKeyId *keyid = parcKeyId_Create(bb_id);
    parcBuffer_Release(&bb_id);

    PARCBufferComposer *composer = parcBufferComposer_Create();
    parcBufferComposer_PutString(composer, "quack quack");
    PARCBuffer *bb_key = parcBufferComposer_ProduceBuffer(composer);
    PARCKey *key = parcKey_CreateFromDerEncodedPublicKey(keyid, PARCSigningAlgorithm_RSA, bb_key);

    parcKey_AssertValid(key);

    PARCBuffer *rawKey = parcKey_GetKey(key); // reference count is not incremented
    assertTrue(parcBuffer_Equals(rawKey, bb_key), "Expected the raw key buffers to be equal");

    parcKey_Release(&key);
    parcBuffer_Release(&bb_key);
    parcBufferComposer_Release(&composer);
    parcKeyId_Release(&keyid);
}

LONGBOW_TEST_CASE(Global, parcKey_GetKeyId)
{
    PARCBuffer *bb_id = parcBuffer_Wrap("choo choo", 9, 0, 9);
    PARCKeyId *keyid = parcKeyId_Create(bb_id);
    parcBuffer_Release(&bb_id);

    PARCBufferComposer *composer = parcBufferComposer_Create();
    parcBufferComposer_PutString(composer, "quack quack");
    PARCBuffer *bb_key = parcBufferComposer_ProduceBuffer(composer);
    PARCKey *key = parcKey_CreateFromDerEncodedPublicKey(keyid, PARCSigningAlgorithm_RSA, bb_key);

    parcKey_AssertValid(key);

    PARCKeyId *rawKeyId = parcKey_GetKeyId(key); // reference count is not incremented
    assertTrue(parcKeyId_Equals(rawKeyId, keyid), "Expected the raw KeyID buffers to be equal");

    parcKey_Release(&key);
    parcBuffer_Release(&bb_key);
    parcBufferComposer_Release(&composer);
    parcKeyId_Release(&keyid);
}

LONGBOW_TEST_CASE(Global, parcKey_GetSigningAlgorithm)
{
    // Check for PARCSigningAlgorithm_RSA value
    PARCBuffer *bb_id_1 = parcBuffer_Wrap("choo choo", 9, 0, 9);
    PARCKeyId *keyid_1 = parcKeyId_Create(bb_id_1);
    parcBuffer_Release(&bb_id_1);

    PARCBufferComposer *composer1 = parcBufferComposer_Create();
    parcBufferComposer_PutString(composer1, "quack quack");
    PARCBuffer *bb_key_1 = parcBufferComposer_ProduceBuffer(composer1);
    PARCKey *key_1 = parcKey_CreateFromDerEncodedPublicKey(keyid_1, PARCSigningAlgorithm_RSA, bb_key_1);

    parcKey_AssertValid(key_1);

    assertTrue((parcKey_GetSigningAlgorithm(key_1) == PARCSigningAlgorithm_RSA), "Signing Algorithms don't match");

    parcBuffer_Release(&bb_key_1);
    parcBufferComposer_Release(&composer1);
    parcKeyId_Release(&keyid_1);
    parcKey_Release(&key_1);

    // Check for PARCSigningAlgorithm_HMAC value
    PARCBuffer *bb_id_2 = parcBuffer_Wrap("choo choo", 9, 0, 9);
    PARCKeyId *keyid_2 = parcKeyId_Create(bb_id_2);
    parcBuffer_Release(&bb_id_2);

    PARCBufferComposer *composer2 = parcBufferComposer_Create();
    parcBufferComposer_PutString(composer2, "quack quack");
    PARCBuffer *bb_key_2 = parcBufferComposer_ProduceBuffer(composer2);
    PARCKey *key_2 = parcKey_CreateFromSymmetricKey(keyid_2, PARCSigningAlgorithm_HMAC, bb_key_2);

    parcKey_AssertValid(key_2);

    assertTrue((parcKey_GetSigningAlgorithm(key_2) == PARCSigningAlgorithm_HMAC), "Signing Algorithms don't match");

    parcBuffer_Release(&bb_key_2);
    parcBufferComposer_Release(&composer2);
    parcKeyId_Release(&keyid_2);
    parcKey_Release(&key_2);
}

LONGBOW_TEST_CASE(Global, parcKey_Acquire)
{
    PARCBuffer *bb_id = parcBuffer_Wrap("choo choo", 9, 0, 9);
    PARCKeyId *keyid = parcKeyId_Create(bb_id);
    parcBuffer_Release(&bb_id);

    PARCBufferComposer *composer = parcBufferComposer_Create();
    parcBufferComposer_PutString(composer, "quack quack");
    PARCBuffer *bb_key = parcBufferComposer_ProduceBuffer(composer);
    PARCKey *key = parcKey_CreateFromDerEncodedPublicKey(keyid, PARCSigningAlgorithm_RSA, bb_key);

    parcKey_AssertValid(key);

    parcObjectTesting_AssertAcquireReleaseContract(parcKey_Acquire, key);
    parcKey_Release(&key);
    parcBuffer_Release(&bb_key);
    parcBufferComposer_Release(&composer);
    parcKeyId_Release(&keyid);
}

LONGBOW_TEST_CASE(Global, parcKey_ToString)
{
    PARCBuffer *bb_id = parcBuffer_Wrap("choo choo", 9, 0, 9);
    PARCKeyId *keyid = parcKeyId_Create(bb_id);
    parcBuffer_Release(&bb_id);

    PARCBufferComposer *composer = parcBufferComposer_Create();
    parcBufferComposer_PutString(composer, "quack quack");
    PARCBuffer *bb_key = parcBufferComposer_ProduceBuffer(composer);
    PARCKey *key = parcKey_CreateFromDerEncodedPublicKey(keyid, PARCSigningAlgorithm_RSA, bb_key);

    char *keyString = parcKey_ToString(key);

    assertNotNull(keyString, "Expected non-null key representation string");
    assertTrue(strlen(keyString) > 0, "Expected non-null key representation string");

    parcKey_Release(&key);
    parcBuffer_Release(&bb_key);
    parcBufferComposer_Release(&composer);
    parcKeyId_Release(&keyid);
}

LONGBOW_TEST_FIXTURE(Errors)
{
    LONGBOW_RUN_TEST_CASE(Errors, parcKey_CreateFromDerEncodedPublicKey_InvalidAlgorithm);
    LONGBOW_RUN_TEST_CASE(Errors, parcKey_CreateFromSymmetricKey_InvalidAlgorithm);
}

LONGBOW_TEST_FIXTURE_SETUP(Errors)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Errors)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE_EXPECTS(Errors, parcKey_CreateFromDerEncodedPublicKey_InvalidAlgorithm, .event = &LongBowTrapIllegalValue)
{
    PARCBuffer *bb_id = parcBuffer_Wrap("choo choo", 9, 0, 9);
    PARCKeyId *keyid = parcKeyId_Create(bb_id);
    parcBuffer_Release(&bb_id);

    PARCBufferComposer *composer = parcBufferComposer_Create();
    parcBufferComposer_PutString(composer, "quack quack");
    PARCBuffer *bb_key = parcBufferComposer_ProduceBuffer(composer);
    PARCKey *key = parcKey_CreateFromDerEncodedPublicKey(keyid, PARCSigningAlgorithm_HMAC, bb_key);

    assertNull(key, "This should not be reached"); //To avoid a warning

    // HMAC is an illegal value for this constructor

    parcBuffer_Release(&bb_key);
    parcBufferComposer_Release(&composer);
    parcKeyId_Release(&keyid);
}

LONGBOW_TEST_CASE_EXPECTS(Errors, parcKey_CreateFromSymmetricKey_InvalidAlgorithm, .event = &LongBowTrapIllegalValue)
{
    PARCBuffer *bb_id = parcBuffer_Wrap("choo choo", 9, 0, 9);
    PARCKeyId *keyid = parcKeyId_Create(bb_id);
    parcBuffer_Release(&bb_id);

    PARCBufferComposer *composer = parcBufferComposer_Create();
    parcBufferComposer_PutString(composer, "quack quack");
    PARCBuffer *bb_key = parcBufferComposer_ProduceBuffer(composer);
    PARCKey *key = parcKey_CreateFromSymmetricKey(keyid, PARCSigningAlgorithm_RSA, bb_key);

    assertNull(key, "This should not be reached"); //To avoid a warning

    // RSA/DSA are illegal values for this constructor

    parcBuffer_Release(&bb_key);
    parcBufferComposer_Release(&composer);
    parcKeyId_Release(&keyid);
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_Key);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
