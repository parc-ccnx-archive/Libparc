/*
 * Copyright (c) 2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @author Glenn Scott, Computing Science Laboratory, PARC
 * @copyright (c) 2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include "../parc_ThreadPool.c"

#include <stdio.h>

#include <LongBow/testing.h>
#include <LongBow/debugging.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_SafeMemory.h>
#include <parc/algol/parc_DisplayIndented.h>

#include <parc/testing/parc_MemoryTesting.h>
#include <parc/testing/parc_ObjectTesting.h>

LONGBOW_TEST_RUNNER(parc_Thread)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(CreateAcquireRelease);
    LONGBOW_RUN_TEST_FIXTURE(Object);
    LONGBOW_RUN_TEST_FIXTURE(Specialization);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_Thread)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_Thread)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(CreateAcquireRelease)
{
    LONGBOW_RUN_TEST_CASE(CreateAcquireRelease, CreateRelease);
}

LONGBOW_TEST_FIXTURE_SETUP(CreateAcquireRelease)
{
    longBowTestCase_SetInt(testCase, "initialAllocations", parcMemory_Outstanding());
    
    PARCBuffer *buffer = parcBuffer_Allocate(10);
    longBowTestCase_Set(testCase, "object", buffer);
    
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(CreateAcquireRelease)
{
    PARCBuffer *buffer = longBowTestCase_Get(testCase, "object");
    parcBuffer_Release(&buffer);
    
    uint32_t initialAllocations = (uint32_t) longBowTestCase_Get(testCase, "initialAllocations");
    if (!parcMemoryTesting_ExpectedOutstanding(initialAllocations, "%s leaked memory.", longBowTestCase_GetFullName(testCase))) {
        parcSafeMemory_ReportAllocation(1);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    
    return LONGBOW_STATUS_SUCCEEDED;
}

static void *
_function(PARCThread *thread, void *parameter)
{
    return NULL;
}

LONGBOW_TEST_CASE(CreateAcquireRelease, CreateRelease)
{
    PARCBuffer *buffer = longBowTestCase_Get(testCase, "object");
    PARCThread *thread = parcThread_Create(_function, buffer);

    assertNotNull(thread, "Expected non-null result from parcThread_Create();");

    parcObjectTesting_AssertAcquireReleaseContract(parcThread_Acquire, thread);
  
    parcThread_Release(&thread);
    assertNull(thread, "Expected null result from parcThread_Release();");
}

LONGBOW_TEST_FIXTURE(Object)
{
    LONGBOW_RUN_TEST_CASE(Object, parcThread_Compare);
    LONGBOW_RUN_TEST_CASE(Object, parcThread_Copy);
    LONGBOW_RUN_TEST_CASE(Object, parcThread_Display);
    LONGBOW_RUN_TEST_CASE(Object, parcThread_Equals);
    LONGBOW_RUN_TEST_CASE(Object, parcThread_HashCode);
    LONGBOW_RUN_TEST_CASE(Object, parcThread_IsValid);
    LONGBOW_RUN_TEST_CASE(Object, parcThread_ToJSON);
    LONGBOW_RUN_TEST_CASE(Object, parcThread_ToString);
}

LONGBOW_TEST_FIXTURE_SETUP(Object)
{
    longBowTestCase_SetInt(testCase, "initialAllocations", parcMemory_Outstanding());
    PARCBuffer *buffer = parcBuffer_Allocate(10);
    longBowTestCase_Set(testCase, "object", buffer);
    
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Object)
{
    PARCBuffer *buffer = longBowTestCase_Get(testCase, "object");
    parcBuffer_Release(&buffer);

    uint32_t initialAllocations = (uint32_t) longBowTestCase_Get(testCase, "initialAllocations");
    if (!parcMemoryTesting_ExpectedOutstanding(initialAllocations, "%s leaked memory.", longBowTestCase_GetFullName(testCase))) {
        parcSafeMemory_ReportAllocation(1);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Object,  parcThread_Compare)
{
//    testUnimplemented("");

}

LONGBOW_TEST_CASE(Object, parcThread_Copy)
{
    PARCBuffer *buffer = longBowTestCase_Get(testCase, "object");
    PARCThread *instance = parcThread_Create(_function, buffer);
    PARCThread *copy = parcThread_Copy(instance);
    
    assertTrue(parcThread_Equals(instance, copy), "Expected the copy to be equal to the original");
    
    parcThread_Release(&instance);
    parcThread_Release(&copy);
}

LONGBOW_TEST_CASE(Object, parcThread_Display)
{
    PARCBuffer *buffer = longBowTestCase_Get(testCase, "object");
    PARCThread *instance = parcThread_Create(_function, buffer);
    parcThread_Display(instance, 0);
    
    
    parcThread_Release(&instance);
}

LONGBOW_TEST_CASE(Object, parcThread_Equals)
{
    PARCBuffer *buffer = longBowTestCase_Get(testCase, "object");
    PARCThread *x = parcThread_Create(_function, buffer);
    PARCThread *y = parcThread_Create(_function, buffer);
    PARCThread *z = parcThread_Create(_function, buffer);

    parcObjectTesting_AssertEquals(x, y, z, NULL);
    
    parcThread_Release(&x);
    parcThread_Release(&y);
    parcThread_Release(&z);
}

LONGBOW_TEST_CASE(Object, parcThread_HashCode)
{
    PARCBuffer *buffer = longBowTestCase_Get(testCase, "object");
    PARCThread *x = parcThread_Create(_function, buffer);
    PARCThread *y = parcThread_Create(_function, buffer);
    
    parcObjectTesting_AssertHashCode(x, y);
    
    parcThread_Release(&x);
    parcThread_Release(&y);
}

LONGBOW_TEST_CASE(Object, parcThread_IsValid)
{
    PARCBuffer *buffer = longBowTestCase_Get(testCase, "object");
    PARCThread *instance = parcThread_Create(_function, buffer);
    assertTrue(parcThread_IsValid(instance), "Expected parcThread_Create to result in a valid instance.");
    

    parcThread_Release(&instance);
    assertFalse(parcThread_IsValid(instance), "Expected parcThread_Release to result in an invalid instance.");
}

LONGBOW_TEST_CASE(Object, parcThread_ToJSON)
{
    PARCBuffer *buffer = longBowTestCase_Get(testCase, "object");
    PARCThread *instance = parcThread_Create(_function, buffer);
    
    PARCJSON *json = parcThread_ToJSON(instance);

    parcJSON_Release(&json);
    
    
    parcThread_Release(&instance);
}

LONGBOW_TEST_CASE(Object, parcThread_ToString)
{
    PARCBuffer *buffer = longBowTestCase_Get(testCase, "object");
    PARCThread *instance = parcThread_Create(_function, buffer);
    
    char *string = parcThread_ToString(instance);
    
    assertNotNull(string, "Expected non-NULL result from parcThread_ToString");
    
    parcMemory_Deallocate((void **) &string);
        
    parcThread_Release(&instance);
}

LONGBOW_TEST_FIXTURE(Specialization)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);
    LONGBOW_RUN_TEST_CASE(Object, parcThread_Execute);
}

LONGBOW_TEST_FIXTURE_SETUP(Specialization)
{
    longBowTestCase_SetInt(testCase, "initialAllocations", parcMemory_Outstanding());
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Specialization)
{
    uint32_t initialAllocations = (uint32_t) longBowTestCase_Get(testCase, "initialAllocations");
    if (!parcMemoryTesting_ExpectedOutstanding(initialAllocations, "%s leaked memory.", longBowTestCase_GetFullName(testCase))) {
        parcSafeMemory_ReportAllocation(1);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Object, parcThread_Execute)
{
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_Thread);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}


