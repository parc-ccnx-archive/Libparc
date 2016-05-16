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
 * @author <#Glenn Scott <Glenn.Scott@parc.com>#>, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */

#include "../parc_AtomicUint16.c"

#include <LongBow/testing.h>
#include <LongBow/debugging.h>

#include <inttypes.h>

#include <parc/algol/parc_Memory.h>
#include <parc/testing/parc_MemoryTesting.h>
#include <parc/testing/parc_ObjectTesting.h>

LONGBOW_TEST_RUNNER(parc_AtomicUint16)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(CreateAcquireRelease);
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Macros);
    LONGBOW_RUN_TEST_FIXTURE(Performance);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_AtomicUint16)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_AtomicUint16)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(CreateAcquireRelease)
{
    LONGBOW_RUN_TEST_CASE(CreateAcquireRelease, CreateRelease);
}

LONGBOW_TEST_FIXTURE_SETUP(CreateAcquireRelease)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(CreateAcquireRelease)
{
    if (!parcMemoryTesting_ExpectedOutstanding(0, "%s leaked memory.", longBowTestCase_GetFullName(testCase))) {
        return LONGBOW_STATUS_MEMORYLEAK;
    }

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(CreateAcquireRelease, CreateRelease)
{
    PARCAtomicUint16 *instance = parcAtomicUint16_Create(7);
    assertNotNull(instance, "Expeced non-null result from parcAtomicUint16_Create();");

    parcObjectTesting_AssertAcquireReleaseContract(parcAtomicUint16_Acquire, instance);

    parcAtomicUint16_Release(&instance);
    assertNull(instance, "Expected null result from parcAtomicUint16_Release();");
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcAtomicUint16_Compare);
    LONGBOW_RUN_TEST_CASE(Global, parcAtomicUint16_Copy);
    LONGBOW_RUN_TEST_CASE(Global, parcAtomicUint16_Equals);
    LONGBOW_RUN_TEST_CASE(Global, parcAtomicUint16_HashCode);
    LONGBOW_RUN_TEST_CASE(Global, parcAtomicUint16_IsValid);

    LONGBOW_RUN_TEST_CASE(Global, parcAtomicUint16_SubtractImpl);
    LONGBOW_RUN_TEST_CASE(Global, parcAtomicUint16_AddImpl);
    LONGBOW_RUN_TEST_CASE(Global, parcAtomicUint16_CompareAndSwapImpl);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    if (!parcMemoryTesting_ExpectedOutstanding(0, "%s leaked memory.", longBowTestCase_GetFullName(testCase))) {
        return LONGBOW_STATUS_MEMORYLEAK;
    }

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global,  parcAtomicUint16_Compare)
{
    PARCAtomicUint16 *instance = parcAtomicUint16_Create(7);
    PARCAtomicUint16 *high = parcAtomicUint16_Create(8);
    PARCAtomicUint16 *low = parcAtomicUint16_Create(6);
    PARCAtomicUint16 *equal = parcAtomicUint16_Create(7);

    int actual = parcAtomicUint16_Compare(instance, high);
    assertTrue(actual < 0, "Expected < 0");
    actual = parcAtomicUint16_Compare(instance, low);
    assertTrue(actual > 0, "Expected > 0");
    actual = parcAtomicUint16_Compare(instance, equal);
    assertTrue(actual == 0, "Expected == 0");

    parcAtomicUint16_Release(&instance);
    parcAtomicUint16_Release(&high);
    parcAtomicUint16_Release(&low);
    parcAtomicUint16_Release(&equal);
}

LONGBOW_TEST_CASE(Global, parcAtomicUint16_Copy)
{
    PARCAtomicUint16 *instance = parcAtomicUint16_Create(7);
    PARCAtomicUint16 *copy = parcAtomicUint16_Copy(instance);

    assertTrue(parcAtomicUint16_Equals(instance, copy), "Expected the copy to be equal to the original");

    parcAtomicUint16_Release(&instance);
    parcAtomicUint16_Release(&copy);
}

LONGBOW_TEST_CASE(Global, parcAtomicUint16_Equals)
{
    PARCAtomicUint16 *x = parcAtomicUint16_Create(7);
    PARCAtomicUint16 *y = parcAtomicUint16_Create(7);
    PARCAtomicUint16 *z = parcAtomicUint16_Create(7);
    PARCAtomicUint16 *u1 = parcAtomicUint16_Create(6);

    parcObjectTesting_AssertEquals(x, y, z, u1, NULL);

    parcAtomicUint16_Release(&x);
    parcAtomicUint16_Release(&y);
    parcAtomicUint16_Release(&z);
    parcAtomicUint16_Release(&u1);
}

LONGBOW_TEST_CASE(Global, parcAtomicUint16_HashCode)
{
    PARCAtomicUint16 *x = parcAtomicUint16_Create(7);
    parcAtomicUint16_HashCode(x);
    parcAtomicUint16_Release(&x);
}

LONGBOW_TEST_CASE(Global, parcAtomicUint16_IsValid)
{
    PARCAtomicUint16 *instance = parcAtomicUint16_Create(7);
    assertTrue(parcAtomicUint16_IsValid(instance), "Expected parcAtomicUint16_Create to result in a valid instance.");

    parcAtomicUint16_Release(&instance);
    assertFalse(parcAtomicUint16_IsValid(instance), "Expected parcAtomicUint16_Release to result in an invalid instance.");
}

LONGBOW_TEST_CASE(Global, parcAtomicUint16_SubtractImpl)
{
    PARCAtomicUint16 *instance = parcAtomicUint16_Create(7);

    parcAtomicUint16_SubtractImpl(instance, 1);

    uint16_t actual = parcAtomicUint16_GetValue(instance);

    assertTrue(actual == 6, "Expected 6, actual %" PRIu16, actual);
    parcAtomicUint16_Release(&instance);
}

LONGBOW_TEST_CASE(Global, parcAtomicUint16_AddImpl)
{
    PARCAtomicUint16 *instance = parcAtomicUint16_Create(7);

    parcAtomicUint16_AddImpl(instance, 1);

    uint16_t actual = parcAtomicUint16_GetValue(instance);

    assertTrue(actual == 8, "Expected 8, actual %" PRIu16, actual);
    parcAtomicUint16_Release(&instance);
}

LONGBOW_TEST_CASE(Global, parcAtomicUint16_CompareAndSwapImpl)
{
    PARCAtomicUint16 *instance = parcAtomicUint16_Create(7);

    bool actual = parcAtomicUint16_CompareAndSwapImpl(instance, 7, 8);

    assertTrue(actual, "Expected parcAtomicUint16_CompareAndSwapImpl to return true");
    parcAtomicUint16_Release(&instance);
}

LONGBOW_TEST_FIXTURE(Macros)
{
    LONGBOW_RUN_TEST_CASE(Macros, parcAtomicUint16_Subtract);
    LONGBOW_RUN_TEST_CASE(Macros, parcAtomicUint16_Add);
    LONGBOW_RUN_TEST_CASE(Macros, parcAtomicUint16_CompareAndSwap);
}

LONGBOW_TEST_FIXTURE_SETUP(Macros)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Macros)
{
    if (!parcMemoryTesting_ExpectedOutstanding(0, "%s leaked memory.", longBowTestCase_GetFullName(testCase))) {
        return LONGBOW_STATUS_MEMORYLEAK;
    }

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Macros, parcAtomicUint16_Subtract)
{
    PARCAtomicUint16 *instance = parcAtomicUint16_Create(7);

    parcAtomicUint16_Subtract(instance, 1);

    uint16_t actual = parcAtomicUint16_GetValue(instance);

    assertTrue(actual == 6, "Expected 6, actual %" PRIu16, actual);
    parcAtomicUint16_Release(&instance);
}

LONGBOW_TEST_CASE(Macros, parcAtomicUint16_Add)
{
    PARCAtomicUint16 *instance = parcAtomicUint16_Create(7);

    parcAtomicUint16_Add(instance, 1);

    uint16_t actual = parcAtomicUint16_GetValue(instance);

    assertTrue(actual == 8, "Expected 8, actual %" PRIu16, actual);
    parcAtomicUint16_Release(&instance);
}

LONGBOW_TEST_CASE(Macros, parcAtomicUint16_CompareAndSwap)
{
    PARCAtomicUint16 *instance = parcAtomicUint16_Create(7);

    bool actual = parcAtomicUint16_CompareAndSwap(instance, 7, 8);

    assertTrue(actual, "Expected parcAtomicUint16_CompareAndSwap to return true");
    parcAtomicUint16_Release(&instance);
}


LONGBOW_TEST_FIXTURE_OPTIONS(Performance, .enabled = false)
{
    LONGBOW_RUN_TEST_CASE(Performance, parcAtomicUint16_Subtract_MACRO);
    LONGBOW_RUN_TEST_CASE(Performance, parcAtomicUint16_Add_MACRO);
    LONGBOW_RUN_TEST_CASE(Performance, parcAtomicUint16_CompareAndSwap_MACRO);
    LONGBOW_RUN_TEST_CASE(Performance, parcAtomicUint16_SubtractImpl);
    LONGBOW_RUN_TEST_CASE(Performance, parcAtomicUint16_AddImpl);
    LONGBOW_RUN_TEST_CASE(Performance, parcAtomicUint16_CompareAndSwapImpl);
}

LONGBOW_TEST_FIXTURE_SETUP(Performance)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Performance)
{
    if (!parcMemoryTesting_ExpectedOutstanding(0, "%s leaked memory.", longBowTestCase_GetFullName(testCase))) {
        return LONGBOW_STATUS_MEMORYLEAK;
    }

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Performance, parcAtomicUint16_Subtract_MACRO)
{
    PARCAtomicUint16 *instance = parcAtomicUint16_Create(65535);

    while (parcAtomicUint16_Subtract(instance, 1) > 0)
        ;

    parcAtomicUint16_Release(&instance);
}

LONGBOW_TEST_CASE(Performance, parcAtomicUint16_Add_MACRO)
{
    PARCAtomicUint16 *instance = parcAtomicUint16_Create(1);

    while (parcAtomicUint16_Add(instance, 1) < 65535)
        ;

    parcAtomicUint16_Release(&instance);
}

LONGBOW_TEST_CASE(Performance, parcAtomicUint16_CompareAndSwap_MACRO)
{
    PARCAtomicUint16 *instance = parcAtomicUint16_Create(0);

    for (uint16_t i = 0; i < 65535; i++) {
        bool actual = parcAtomicUint16_CompareAndSwap(instance, i, i+1);
        assertTrue(actual, "Expected parcAtomicUint16_CompareAndSwap to return true");
    }

    parcAtomicUint16_Release(&instance);
}

LONGBOW_TEST_CASE(Performance, parcAtomicUint16_SubtractImpl)
{
    PARCAtomicUint16 *instance = parcAtomicUint16_Create(65535);

    while (parcAtomicUint16_SubtractImpl(instance, 1) > 0)
        ;

    parcAtomicUint16_Release(&instance);
}

LONGBOW_TEST_CASE(Performance, parcAtomicUint16_AddImpl)
{
    PARCAtomicUint16 *instance = parcAtomicUint16_Create(1);

    while (parcAtomicUint16_AddImpl(instance, 1) < 65535)
        ;

    parcAtomicUint16_Release(&instance);
}

LONGBOW_TEST_CASE(Performance, parcAtomicUint16_CompareAndSwapImpl)
{
    PARCAtomicUint16 *instance = parcAtomicUint16_Create(0);

    for (uint16_t i = 0; i < 65535; i++) {
        bool actual = parcAtomicUint16_CompareAndSwapImpl(instance, i, i+1);
        assertTrue(actual, "Expected parcAtomicUint16_CompareAndSwapImpl to return true");
    }

    parcAtomicUint16_Release(&instance);
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_AtomicUint16);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
