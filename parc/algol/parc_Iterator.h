/*
 * Copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @file parc_Iterator.h
 * @ingroup memory
 * @brief An iterator over any kind of iteratable collection
 *
 * Implementations of a PARCIterator must provide the following functions,
 * each of which supports the operation of the iterator.
 * @code
 * _TYPEIterator * PREFIX_Init(TYPE *object)
 *
 * bool PREFIX_Fini(TYPE *map, _TYPEIterator *state)
 *
 * _PARCHashMapIterator *PREFIX_Next(PARCHashMap *map, _TYPEIterator *state)
 *
 * void PREFIX_Remove(PARCHashMap *map, _TYPEIterator **statePtr )
 *
 * bool PREFIX_HasNext(PARCHashMap *map, _TYPEIterator *state)
 *
 * PARCObject *PREFIX_Element(PARCHashMap *map, const _TYPEIterator *state)
 *
 * PARCObject *PREFIX_Element(TYPE *map, const _TYPEIterator *state)
 * @endcode
 *
 * @author Glenn Scott , Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_Iterator_h
#define libparc_parc_Iterator_h

#include <stdbool.h>
#include <parc/algol/parc_Object.h>

struct parc_iterator;
typedef struct parc_iterator PARCIterator;

/**
 * Create a new instance of `PARCIterator`
 *
 * Each instance must be provided pointers to functions implementing the iteration primitives for the given PARCObject.
 *
 * * `init`
 * This function is called only when a PARCIterator is created.
 * The function is expected to initialise and return a pointer to whatever internal state necessary
 * to provide the subsequent operations.
 * In subsequent operations of hasNext, next, getElement, and fini, this pointer is provided as a function parameter.
 *
 * * `hasNext`
 * This function returns true if the iteration has more elements.
 *
 * * `next`
 * Returns the next element in the iteration.
 * If there are no remaining elements in the iteration, then this function must induce a trapOutOfBounds
 *
 * * `remove`
 * Removes the element returned by the `next` function.
 *
 * * `getElement`
 * This function is invoked only from within the `parcIterator_Next` function and
 * returns the actual iteration value from the current iterator's state.
 *
 * * `fini`
 * This function is called only when the PARCIterator is being destroyed.
 *
 * @param [in] object A pointer to the PARCObject that implements the underlying iteration.
 * @param [in] hasNext A pointer to a function that returns true if there are more elements.
 * @param [in] next A pointer to a function that returns the next element.
 * @param [in] remove A pointer to a function that removes the element last returned by the @p next function.
 * @param [in] getElement A pointer to a function that, given the current position, return the element at the current position.
 * @param [in] fini A pointer to a function that will be invoked when the PARCIterator is finally deallocated.
 * @param [in] isValid A pointer to a function that performs validation of the iterator state.
 *
 * @return A `PARCIterator`
 */
PARCIterator *parcIterator_Create(PARCObject *object,
                                  void *(*init)(PARCObject * object),
                                  bool (*hasNext)(PARCObject *object, void *state),
                                  void *(*next)(PARCObject * object, void *state),
                                  void (*remove)(PARCObject *, void **state),
                                  void *(*getElement)(PARCObject * object, void *state),
                                  void (*fini)(PARCObject *object, void *state),
                                  void (*isValid)(const void *));

/**
 * Determine if an instance of `PARCIterator` is valid.
 *
 * Valid means the internal state of the type is consistent with its required current or future behaviour.
 * This may include the validation of internal instances of types.
 *
 * @param [in] instance A pointer to a `PARCIterator` instance.
 *
 * @return true The instance is valid.
 * @return false The instance is not valid.
 *
 * Example:
 * @code
 * {
 *     PARCIterator *instance = parcIterator_Create(...);
 *
 *     if (parcIterator_IsValid(instance)) {
 *         printf("Instance is valid.\n");
 *     }
 * }
 * @endcode
 */
bool parcIterator_IsValid(const PARCIterator *instance);

/**
 * Assert that the given `PARCIterator` instance is valid.
 *
 * @param [in] iterator A pointer to a valid PARCIterator instance.
 *
 * Example:
 * @code
 * {
 *     PARCIterator *a = parcIterator_Create(...);
 *
 *     parcIterator_AssertValid(a);
 *
 *     printf("Instance is valid.\n");
 *
 *     parcIterator_Release(&b);
 * }
 * @endcode
 */
#ifdef PARCLibrary_DISABLE_VALIDATION
#  define parcIterator_OptionalAssertValid(_instance_)
#else
#  define parcIterator_OptionalAssertValid(_instance_) parcIterator_AssertValid(_instance_)
#endif
void parcIterator_AssertValid(const PARCIterator *iterator);

/**
 * Increase the number of references to a `PARCIterator`.
 *
 * Note that new `PARCIterator` is not created,
 * only that the given `PARCIterator` reference count is incremented.
 * Discard the reference by invoking `parcIterator_Release`.
 *
 * @param [in] instance A pointer to a `PARCIterator` instance.
 *
 * @return The input `PARCIterator` pointer.
 *
 * Example:
 * @code
 * {
 *     PARCIterator *a = parcIterator_Create(...);
 *
 *     PARCIterator *b = parcIterator_Acquire(a);
 *
 *     parcIterator_Release(&a);
 *     parcIterator_Release(&b);
 * }
 * @endcode
 */
PARCIterator *parcIterator_Acquire(const PARCIterator *instance);

/**
 * Release a previously acquired reference to the specified instance,
 * decrementing the reference count for the instance.
 *
 * The pointer to the instance is set to NULL as a side-effect of this function.
 *
 * If the invocation causes the last reference to the instance to be released,
 * the instance is deallocated and the instance's implementation will perform
 * additional cleanup and release other privately held references.
 *
 * @param [in,out] iteratorPtr A pointer to a pointer to the instance to release, which will be set to NULL.
 *
 * Example:
 * @code
 * {
 *     PARCIterator *a = parcIterator_Create(...);
 *
 *     parcIterator_Release(&a);
 * }
 * @endcode
 */
void parcIterator_Release(PARCIterator **iteratorPtr);

/**
 * Return the next item in the iterated list
 *
 * @param [in] iterator A pointer to the instance of `PARCIterator`
 *
 * @return A pointer to the next item in the iterated list
 *
 * Example:
 * @code
 * {
 *      while (parcIterator_HasNext(iterator)) {
 *          void *element = parcIterator_Next(iterator);
 *      }
 * }
 * @endcode
 */
void *parcIterator_Next(PARCIterator *iterator);

/**
 * Return true if there are more items left over which to iterate.
 *
 * @param [in] iterator A pointer to the instance of `PARCIterator`
 *
 * @return True if there is are more items over which to iterate; false otherwise.
 *
 * Example:
 * @code
 * {
 *      while (parcIterator_HasNext(iterator)) {
 *          void *element = parcIterator_Next(iterator);
 *      }
 * }
 * @endcode
 */
bool parcIterator_HasNext(const PARCIterator *iterator);

/**
 * Removes from the underlying collection the last element returned by the iterator (optional operation).
 * This function can be called only once per call to `parcIterator_Next`.
 * The behavior of an iterator is unspecified if the underlying collection is
 * modified while the iteration is in progress in any way other than by calling this method.
 *
 * Pointers to the element after removing it via this function may point to invalid remnants of the object.
 * To avoid this, acquire a reference to the element before invoking this function.
 *
 * @param [in] iterator A pointer to the instance of `PARCIterator`
 *
 * Example:
 * @code
 * {
 *      while (parcIterator_HasNext(iterator)) {
 *          void *element = parcIterator_Next(iterator);
 *      }
 * }
 * @endcode
 */
void parcIterator_Remove(PARCIterator *iterator);
#endif // libparc_parc_Iterator_h
