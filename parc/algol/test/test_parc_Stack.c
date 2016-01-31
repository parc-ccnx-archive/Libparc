/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @header <#Headline Name#>
 * @abstract <#Abstract#>
 * @discussion
 *     <#Discussion#>
 *
 * @author Scott, Glenn <Glenn.Scott@parc.com>, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include "../parc_Stack.c"

#include <LongBow/unit-test.h>
#include <LongBow/debugging.h>

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.

#include <parc/algol/parc_ArrayList.h>
#include <parc/algol/parc_Deque.h>
#include <parc/algol/parc_SafeMemory.h>

LONGBOW_TEST_RUNNER(test_parc_Stack)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(test_parc_Stack)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(test_parc_Stack)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcStack_IsEmpty_PARCDeque);
    LONGBOW_RUN_TEST_CASE(Global, parcStack_IsEmpty_PARCArrayList);
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

LONGBOW_TEST_CASE(Global, parcStack_IsEmpty_PARCDeque)
{
    PARCStackInterface dequeAsStack = {
        .parcStack_Release = (void (*)(void **))parcDeque_Release,
        .parcStack_IsEmpty = (bool (*)(const void *))parcDeque_IsEmpty,
        .parcStack_Peek    = (void *(*)(const void *))parcDeque_PeekLast,
        .parcStack_Pop     = (void *(*)(void *))parcDeque_RemoveLast,
        .parcStack_Push    = (void *(*)(void *,                          void *))parcDeque_Append,
        .parcStack_Search  = NULL
    };

    PARCStack *stack = parcStack(parcDeque_Create(), &dequeAsStack);

    bool actual = parcStack_IsEmpty(stack);
    parcStack_Release(&stack);
    assertTrue(actual, "Expected the stack to be empty.");
}

LONGBOW_TEST_CASE(Global, parcStack_IsEmpty_PARCArrayList)
{
    PARCStackInterface arrayListAsStack = {
        .parcStack_Release = (void (*)(void **))parcArrayList_Destroy,
        .parcStack_IsEmpty = (bool (*)(const void *))parcArrayList_IsEmpty,
        .parcStack_Peek    = (void *(*)(const void *))parcArrayList_Peek,
        .parcStack_Pop     = (void *(*)(void *))parcArrayList_Pop,
        .parcStack_Push    = (void *(*)(void *,                            void *))parcArrayList_Add,
        .parcStack_Search  = NULL
    };

    PARCStack *stack = parcStack(parcArrayList_Create(NULL), &arrayListAsStack);

    bool actual = parcStack_IsEmpty(stack);
    parcStack_Release(&stack);
    assertTrue(actual, "Expected the stack to be empty.");
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(test_parc_Stack);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
