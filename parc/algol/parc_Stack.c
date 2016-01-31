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
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>

#include <stdio.h>

#include <LongBow/runtime.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_Deque.h>
#include <parc/algol/parc_Stack.h>

struct parc_stack {
    void *instance;
    PARCStackInterface *interface;
};

PARCStack *
parcStack(void *instance, PARCStackInterface *interface)
{
    PARCStack *result = parcMemory_AllocateAndClear(sizeof(PARCStack));
    assertNotNull(result, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(PARCStack));
    result->instance = instance;
    result->interface = interface;

    return result;
}

void
parcStack_Release(PARCStack **stackPtr)
{
    PARCStack *stack = *stackPtr;
    (stack->interface->parcStack_Release)(&stack->instance);
    parcMemory_Deallocate((void **) &stack);

    *stackPtr = 0;
}

bool
parcStack_IsEmpty(const PARCStack *stack)
{
    return (stack->interface->parcStack_IsEmpty)(stack->instance);
}

void *
parcStack_Peek(const PARCStack *stack)
{
    return (stack->interface->parcStack_Peek)(stack->instance);
}

void *
parcStack_Pop(PARCStack *stack)
{
    return (stack->interface->parcStack_Pop)(stack->instance);
}

void *
parcStack_Push(PARCStack *stack, void *element)
{
    return (stack->interface->parcStack_Push)(stack->instance, element);
}
