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
 * @file parc_Stack.h
 * @ingroup datastructures
 * @brief PARC (Generic) Stack
 *
 * A stack interface implementation.
 *
 * Example
 * @code
 * #include <parc/algol/parc_Deque.h>
 *
 * int
 * main(int argc, char *argv[])
 * {
 *     PARCStackInterface dequeAsStack = {
 *         .parcStack_Release = parcDeque_Release,
 *         .parcStack_IsEmpty = parcDeque_IsEmpty,
 *         .parcStack_Peek = parcDeque_PeekLast,
 *         .parcStack_Pop = parcDeque_RemoveLast,
 *         .parcStack_Push = parcDeque_Append,
 *         .parcStack_Search = NULL
 *     };
 *
 *     PARCStack *stack = parcStack(parcDeque_Create(), &dequeAsStack);
 *
 *     parcStack_IsEmpty(stack);
 * }
 * @endcode
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 *
 */
#ifndef libparc_parc_Stack_h
#define libparc_parc_Stack_h

#include <stdbool.h>

struct parc_stack;
typedef struct parc_stack PARCStack;

typedef struct parc_stack_interface {
    /**
     * Release the instance
     *
     *
     * @param [in,out] instancePtr A pointer to the pointer of the instance to release.
     *
     * Example:
     * @code
     * <#example#>
     * @endcode
     */
    void (*parcStack_Release)(void **instancePtr);

    /**
     * Tests if this stack is empty.
     *
     * @param [in] instance A pointer to the instance to test.
     * @return true if the stack is empty
     * @return false if the stack is not empty
     *
     * Example:
     * @code
     * <#example#>
     * @endcode
     */
    bool (*parcStack_IsEmpty)(const void *instance);

    /**
     * Looks at the object at the top of this stack without removing it from the stack.
     *
     * @param [in] instance A pointer to the instance to look at.
     * @return The object at the top of the @p instance.
     *
     * Example:
     * @code
     * <#example#>
     * @endcode
     */
    void * (*parcStack_Peek)(const void *instance);

    /**
     * Removes the object at the top of this stack and returns that object as the value of this function.
     *
     *
     * @param [in,out] instance A pointer to the instance to check and modify.
     * @return The object at the top of the @p instance.
     *
     * Example:
     * @code
     * <#example#>
     * @endcode
     */
    void * (*parcStack_Pop)(void *instance);

    /**
     * Pushes an item onto the top of this stack.
     *
     *
     * @param [in,out] instance A pointer to the instance to modify.
     * @param [in] item A pointer to the object to push on the @p instance.
     * @return A pointer to the object that was pushed on the @p instance.
     *
     * Example:
     * @code
     * <#example#>
     * @endcode
     */
    void * (*parcStack_Push)(void *instance, void *item);

    /**
     * Returns the 1-based position where an object is on this stack.
     *
     *
     * @param [in] instance A pointer to the instance.
     * @param [in] element A pointer to the element to find on the @p instance.
     * @return The index of the position where @p element is found in the @p instance.
     *
     * Example:
     * @code
     * <#example#>
     * @endcode
     */
    int (*parcStack_Search)(void *instance, void *element);
} PARCStackInterface;
#endif // libparc_parc_Stack_h
