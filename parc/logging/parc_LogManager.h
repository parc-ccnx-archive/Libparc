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
 * @file parc_LogManager.h
 * @brief <#Brief Description#>
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef PARC_Library_parc_LogManager_h
#define PARC_Library_parc_LogManager_h

struct PARCLogManager;
typedef struct PARCLogManager PARCLogManager;

/**
 * Create a new PARCLogManager
 *
 * @return non-NULL A pointer to a valid PARCLogManager
 * @return NULL Out of memory.
 *
 * Example:
 * @code
 * {
 *     PARCLogManager *manager = parcLogManager_Create();
 *
 *     parcLogManager_Release(&manager);
 * }
 * @endcode
 */
PARCLogManager *parcLogManager_Create(void);

/**
 * Increase the number of references to a `PARCLogManager` instance.
 *
 * Note that new `PARCLogManager` is not created,
 * only that the given `PARCLogManager` reference count is incremented.
 * Discard the reference by invoking `parcLogManager_Release`.
 *
 * @param [in] instance A pointer to a `PARCLogManager` instance.
 *
 * @return The input `PARCLogManager` pointer.
 *
 * Example:
 * @code
 * {
 *     PARCLogManager *manager = parcLogManager_Create();
 *
 *     PARCLogReporter *x_2 = parcLogManager_Acquire(reporter);
 *
 *     parcLogManager_Release(&manager);
 *     parcLogManager_Release(&x_2);
 * }
 * @endcode
 */
PARCLogManager *parcLogManager_Acquire(const PARCLogManager *instance);

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
 * @param [in,out] instancePtr A pointer to a pointer to a `PARCLogManager`.  The parameter is set to zero.
 *
 * Example:
 * @code
 * {
 *     PARCLogManager *manager = parcLogManager_Create();
 *
 *     parcLogManager_Release(&manager);
 * }
 * @endcode
 */
void parcLogManager_Release(PARCLogManager **instancePtr);
#endif
