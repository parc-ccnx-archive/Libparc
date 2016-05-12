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
/**
 * @author Glenn Scott <Glenn.Scott@parc.com>, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include <parc/security/parc_KeyId.c>

#include <LongBow/unit-test.h>
#include <LongBow/debugging.h>

#include <parc/algol/parc_SafeMemory.h>
#include <parc/security/parc_Security.h>
#include <parc/testing/parc_ObjectTesting.h>

const char *testStr = "hello world";

LONGBOW_TEST_RUNNER(test_parc_KeyId)
{
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

LONGBOW_TEST_RUNNER_SETUP(test_parc_KeyId)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_RUNNER_TEARDOWN(test_parc_KeyId)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcKeyId_Create);
    LONGBOW_RUN_TEST_CASE(Global, parcKeyId_Acquire);
    LONGBOW_RUN_TEST_CASE(Global, parcKeyId_Copy);
    LONGBOW_RUN_TEST_CASE(Global, parcKeyId_Equals);
    LONGBOW_RUN_TEST_CASE(Global, parcKeyId_HashCode);
    LONGBOW_RUN_TEST_CASE(Global, parcKeyId_HashCodeFromVoid);
    LONGBOW_RUN_TEST_CASE(Global, parcKeyId_GetKeyId);
    LONGBOW_RUN_TEST_CASE(Global, parcKeyId_ToString);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    if (parcSafeMemory_ReportAllocation(STDOUT_FILENO) != 0) {
        printf("('%s' leaks memory by %d (allocs - frees)) ", longBowTestCase_GetName(testCase), parcMemory_Outstanding());
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, parcKeyId_Create)
{
    PARCBuffer *buffer = parcBuffer_Wrap((void *)testStr, strlen(testStr), 0, strlen(testStr));
    PARCKeyId *keyId = parcKeyId_Create(buffer);
    parcBuffer_Release(&buffer);

    assertNotNull(keyId, "Expected non-null");

    parcKeyId_Release(&keyId);
    assertNull(keyId, "Release did not null the pointer.");
}

LONGBOW_TEST_CASE(Global, parcKeyId_Acquire)
{
    PARCBuffer *buffer = parcBuffer_Wrap((void *)testStr, strlen(testStr), 0, strlen(testStr));
    PARCKeyId *keyId = parcKeyId_Create(buffer);
    parcBuffer_Release(&buffer);

    assertNotNull(keyId, "Expected non-null");
    parcObjectTesting_AssertAcquireReleaseContract(parcKeyId_Acquire, keyId);

    parcKeyId_Release(&keyId);
    assertNull(keyId, "Release did not null the pointer.");
}

LONGBOW_TEST_CASE(Global, parcKeyId_Copy)
{
    PARCBuffer *buffer = parcBuffer_Wrap((void *)testStr, strlen(testStr), 0, strlen(testStr));
    PARCKeyId *keyId = parcKeyId_Create(buffer);
    parcBuffer_Release(&buffer);

    assertNotNull(keyId, "Expected non-null");

    PARCKeyId *copy = parcKeyId_Copy(keyId);
    parcKeyId_AssertValid(keyId);
    parcKeyId_AssertValid(copy);

    parcKeyId_Release(&keyId);
    assertNull(keyId, "parcKeyId_Release did not null the keyId pointer.");

    parcKeyId_AssertValid(copy);
    parcKeyId_Release(&copy);
    assertNull(keyId, "parcKeyId_Release did not null the keyId copy pointer.");
}

LONGBOW_TEST_CASE(Global, parcKeyId_Equals)
{
    PARCBuffer *buffer1 = parcBuffer_Wrap((void *)testStr, strlen(testStr), 0, strlen(testStr));
    PARCKeyId *x = parcKeyId_Create(buffer1);
    parcBuffer_Release(&buffer1);

    PARCBuffer *buffer2 = parcBuffer_Wrap((void *)testStr, strlen(testStr), 0, strlen(testStr));
    PARCKeyId *y = parcKeyId_Create(buffer2);
    parcBuffer_Release(&buffer2);

    PARCBuffer *buffer3 = parcBuffer_Wrap((void *)testStr, strlen(testStr), 0, strlen(testStr));
    PARCKeyId *z = parcKeyId_Create(buffer3);
    parcBuffer_Release(&buffer3);

    PARCBuffer *buffer4 = parcBuffer_Wrap("hello worlx", 11, 0, 11);
    PARCKeyId *u1 = parcKeyId_Create(buffer4);
    parcBuffer_Release(&buffer4);

    parcObjectTesting_AssertEqualsFunction(parcKeyId_Equals, x, y, z, u1);

    parcKeyId_Release(&x);
    parcKeyId_Release(&y);
    parcKeyId_Release(&z);
    parcKeyId_Release(&u1);

    assertNull(x, "Release did not null the pointer.");
    assertNull(y, "Release did not null the pointer.");
    assertNull(z, "Release did not null the pointer.");
    assertNull(u1, "Release did not null the pointer.");
}

LONGBOW_TEST_CASE(Global, parcKeyId_HashCode)
{
    PARCBuffer *buffer = parcBuffer_Wrap((void *)testStr, strlen(testStr), 0, strlen(testStr));
    PARCKeyId *keyId = parcKeyId_Create(buffer);

    assertNotNull(keyId, "Expected non-null");

    assertTrue(parcKeyId_HashCode(keyId) == parcBuffer_HashCode(buffer), "Expected hash codes to be equal");

    parcBuffer_Release(&buffer);
    parcKeyId_Release(&keyId);
    assertNull(keyId, "Release did not null the pointer.");
}


LONGBOW_TEST_CASE(Global, parcKeyId_HashCodeFromVoid)
{
    PARCBuffer *buffer = parcBuffer_Wrap((void *)testStr, strlen(testStr), 0, strlen(testStr));
    PARCKeyId *keyId = parcKeyId_Create(buffer);

    assertNotNull(keyId, "Expected non-null");

    assertTrue(parcKeyId_HashCodeFromVoid((void *)keyId) == parcBuffer_HashCode(buffer), "Expected hash codes to be equal");

    parcBuffer_Release(&buffer);
    parcKeyId_Release(&keyId);
    assertNull(keyId, "Release did not null the pointer.");
}

LONGBOW_TEST_CASE(Global, parcKeyId_GetKeyId)
{
    PARCBuffer *buffer = parcBuffer_Wrap((void *)testStr, strlen(testStr), 0, strlen(testStr));
    PARCKeyId *keyId = parcKeyId_Create(buffer);

    assertNotNull(keyId, "Expected non-null");

    PARCBuffer *rawBuffer = (PARCBuffer *) parcKeyId_GetKeyId(keyId);
    assertTrue(parcBuffer_Equals(rawBuffer, buffer), "Expected the raw key buffers to be equal");

    parcBuffer_Release(&buffer);
    parcKeyId_Release(&keyId);
    assertNull(keyId, "Release did not null the pointer.");
}

LONGBOW_TEST_CASE(Global, parcKeyId_ToString)
{
    PARCBuffer *buffer = parcBuffer_Wrap((void *)testStr, strlen(testStr), 0, strlen(testStr));
    PARCKeyId *keyId = parcKeyId_Create(buffer);

    parcBuffer_Release(&buffer);

    assertNotNull(keyId, "Expected non-null");

    char *string = parcKeyId_ToString(keyId);
    printf("Hello: %s\n", string);
    parcMemory_Deallocate((void **)&string);

    parcKeyId_Release(&keyId);
    assertNull(keyId, "Release did not null the pointer.");
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(test_parc_KeyId);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
