/*
 * Copyright (c) 2016, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @author Glenn Scott, Palo Alto Research Center (PARC)
 * @copyright 2016, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include "../parc_BufferPool.c"

#include <LongBow/testing.h>
#include <LongBow/debugging.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_SafeMemory.h>
#include <parc/algol/parc_DisplayIndented.h>

#include <parc/testing/parc_MemoryTesting.h>
#include <parc/testing/parc_ObjectTesting.h>

LONGBOW_TEST_RUNNER(parc_BufferPool)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(CreateAcquireRelease);
    LONGBOW_RUN_TEST_FIXTURE(Object);
    LONGBOW_RUN_TEST_FIXTURE(Specialization);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_BufferPool)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_BufferPool)
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
        parcSafeMemory_ReportAllocation(1);
        return LONGBOW_STATUS_MEMORYLEAK;
    }

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(CreateAcquireRelease, CreateRelease)
{
    PARCBufferPool *instance = parcBufferPool_Create(3, 10);
    assertNotNull(instance, "Expected non-null result from parcBufferPool_Create();");

    parcObjectTesting_AssertAcquireReleaseContract(parcBufferPool_Acquire, instance);
    
    parcBufferPool_Release(&instance);
    assertNull(instance, "Expected null result from parcBufferPool_Release();");
}

LONGBOW_TEST_FIXTURE(Object)
{
    LONGBOW_RUN_TEST_CASE(Object, parcBufferPool_Display);
    LONGBOW_RUN_TEST_CASE(Object, parcBufferPool_IsValid);
    LONGBOW_RUN_TEST_CASE(Object, parcBufferPool_AssertValid);
}

LONGBOW_TEST_FIXTURE_SETUP(Object)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Object)
{
    if (!parcMemoryTesting_ExpectedOutstanding(0, "%s mismanaged memory.", longBowTestCase_GetFullName(testCase))) {
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Object, parcBufferPool_Display)
{
    PARCBufferPool *instance = parcBufferPool_Create(3, 10);
    parcBufferPool_Display(instance, 0);
    parcBufferPool_Release(&instance);
}

LONGBOW_TEST_CASE(Object, parcBufferPool_IsValid)
{
    PARCBufferPool *instance = parcBufferPool_Create(3, 10);
    assertTrue(parcBufferPool_IsValid(instance), "Expected parcBufferPool_Create to result in a valid instance.");
    
    parcBufferPool_Release(&instance);
    assertFalse(parcBufferPool_IsValid(instance), "Expected parcBufferPool_Release to result in an invalid instance.");
}

LONGBOW_TEST_CASE(Object, parcBufferPool_AssertValid)
{
    PARCBufferPool *instance = parcBufferPool_Create(3, 10);
    parcBufferPool_AssertValid(instance);
    
    parcBufferPool_Release(&instance);
    assertFalse(parcBufferPool_IsValid(instance), "Expected parcBufferPool_Release to result in an invalid instance.");
}

LONGBOW_TEST_FIXTURE(Specialization)
{
    LONGBOW_RUN_TEST_CASE(Specialization, parcBufferPool_GetInstance);
    LONGBOW_RUN_TEST_CASE(Specialization, parcBufferPool_GetHighWater);
    LONGBOW_RUN_TEST_CASE(Specialization, parcBufferPool_GetTotalInstances);
    LONGBOW_RUN_TEST_CASE(Specialization, parcBufferPool_GetCacheHits);
    LONGBOW_RUN_TEST_CASE(Specialization, parcBufferPool_SetLimit);
}

LONGBOW_TEST_FIXTURE_SETUP(Specialization)
{
    longBowTestCase_SetInt(testCase, "initialAllocations", parcMemory_Outstanding());
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Specialization)
{
    int initialAllocations = longBowTestCase_GetInt(testCase, "initialAllocations");
    
    if (parcMemory_Outstanding() > initialAllocations) {
        parcSafeMemory_ReportAllocation(1);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Specialization, parcBufferPool_GetInstance)
{
    PARCBufferPool *pool = parcBufferPool_Create(3, 10);
    
    PARCBuffer *buffer = parcBufferPool_GetInstance(pool);
    
    parcBuffer_AssertValid(buffer);
    parcBuffer_Release(&buffer);
    
    size_t highWater = parcBufferPool_GetHighWater(pool);
    
    assertTrue(highWater == 1, "Expected the highWater to be 1, actual %zu", highWater);
    
    parcBufferPool_Release(&pool);
}

LONGBOW_TEST_CASE(Specialization, parcBufferPool_GetHighWater)
{
    PARCBufferPool *pool = parcBufferPool_Create(3, 10);
    size_t highWater = parcBufferPool_GetHighWater(pool);
    
    assertTrue(highWater == 0, "Expected the highWater to be 0, actual %zu", highWater);
    
    PARCBuffer *buffer = parcBufferPool_GetInstance(pool);
    
    parcBuffer_AssertValid(buffer);
    parcBuffer_Release(&buffer);
    
    highWater = parcBufferPool_GetHighWater(pool);
    
    assertTrue(highWater == 1, "Expected the highWater to be 1, actual %zu", highWater);
    
    parcBufferPool_Release(&pool);
}

LONGBOW_TEST_CASE(Specialization, parcBufferPool_GetTotalInstances)
{
    PARCBufferPool *pool = parcBufferPool_Create(3, 10);
    size_t totalInstances = parcBufferPool_GetTotalInstances(pool);
    
    assertTrue(totalInstances == 0, "Expected the totalInstances to be 0, actual %zu", totalInstances);
    
    PARCBuffer *buffer = parcBufferPool_GetInstance(pool);
    
    parcBuffer_AssertValid(buffer);
    parcBuffer_Release(&buffer);
    
    totalInstances = parcBufferPool_GetTotalInstances(pool);
    
    assertTrue(totalInstances == 1, "Expected the totalInstances to be 1, actual %zu", totalInstances);
    
    parcBufferPool_Release(&pool);
}

LONGBOW_TEST_CASE(Specialization, parcBufferPool_GetCacheHits)
{
    PARCBufferPool *pool = parcBufferPool_Create(3, 10);
    size_t cacheHits = parcBufferPool_GetCacheHits(pool);
    
    assertTrue(cacheHits == 0, "Expected the cacheHits to be 0, actual %zu", cacheHits);
    
    PARCBuffer *buffer = parcBufferPool_GetInstance(pool);
    parcBuffer_AssertValid(buffer);
    parcBuffer_Release(&buffer);

    cacheHits = parcBufferPool_GetCacheHits(pool);
    assertTrue(cacheHits == 0, "Expected the cacheHits to be 0, actual %zu", cacheHits);
    
    buffer = parcBufferPool_GetInstance(pool);
    parcBuffer_AssertValid(buffer);
    parcBuffer_Release(&buffer);
    
    cacheHits = parcBufferPool_GetCacheHits(pool);
    assertTrue(cacheHits == 1, "Expected the cacheHits to be 1, actual %zu", cacheHits);
    
    parcBufferPool_Release(&pool);
}

LONGBOW_TEST_CASE(Specialization, parcBufferPool_SetLimit)
{
    PARCBufferPool *pool = parcBufferPool_Create(3, 10);
    size_t limit = parcBufferPool_SetLimit(pool, 2);
    
    assertTrue(limit == 3, "Expected the limit to be 3, actual %zu", limit);
    
    limit = parcBufferPool_SetLimit(pool, 2);
    assertTrue(limit == 2, "Expected the limit to be 2, actual %zu", limit);
    
    PARCBuffer *buffer1 = parcBufferPool_GetInstance(pool);
    PARCBuffer *buffer2 = parcBufferPool_GetInstance(pool);
    PARCBuffer *buffer3 = parcBufferPool_GetInstance(pool);
    parcBuffer_AssertValid(buffer1);
    parcBuffer_AssertValid(buffer2);
    parcBuffer_AssertValid(buffer3);
    parcBuffer_Release(&buffer1);
    parcBuffer_Release(&buffer2);
    parcBuffer_Release(&buffer3);
    
    parcBufferPool_Release(&pool);
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_BufferPool);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
