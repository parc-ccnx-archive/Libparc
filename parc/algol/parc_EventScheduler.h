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
 * @file parc_EventScheduler.h
 * @ingroup events
 * @brief Event scheduler
 *
 * Provides a facade implementing many regularly available event functions.
 * This is an interface that software implementors may use to substitute
 * different kinds of underlying implementations of these event management functions.
 * Notable examples are libevent and libev.
 *
 * @author Kevin Fox, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_EventScheduler_h
#define libparc_parc_EventScheduler_h

#include <stdlib.h>
#include <stdint.h>

/*
 * Currently implemented using libevent
 */

#include <parc/algol/parc_Memory.h>
#include <parc/logging/parc_Log.h>

/**
 * @typedef PARCEventScheduler
 * @brief A structure containing private event state
 */
struct PARCEventScheduler;
typedef struct PARCEventScheduler PARCEventScheduler;

typedef enum {
    PARCEventSchedulerDispatchType_Blocking     = 0x00,
    PARCEventSchedulerDispatchType_LoopOnce     = 0x01,
    PARCEventSchedulerDispatchType_NonBlocking  = 0x02,
} PARCEventSchedulerDispatchType;

/**
 * Create a new parcEventScheduler instance.
 *
 * A new parcEventScheduler instance is returned.
 *
 * @returns A pointer to the a PARCEvent instance.
 *
 * Example:
 * @code
 * {
 *     PARCEvent *parcEventScheduler = parcEvent_Create();
 * }
 * @endcode
 *
 */
PARCEventScheduler *parcEventScheduler_Create(void);

/**
 * Start the event eventScheduler
 *
 * @param [in] parcEventScheduler - The parcEventScheduler instance to start.
 * @param type - The mode of dispatch for this `PARCEventScheduler` instance.
 * @returns 0 on success, -1 on failure
 *
 * Example:
 * @code
 * {
 *     result = parcEventScheduler_Start(parcEventScheduler, PARCEventScheduler_Blocking);
 * }
 * @endcode
 *
 */
int parcEventScheduler_Start(PARCEventScheduler *parcEventScheduler, PARCEventSchedulerDispatchType type);

/**
 * Dispatch the event scheduler to process any pending events, blocking until
 * some events have been triggered and then processed.
 *
 * @param [in] parcEventScheduler - The parcEventScheduler instance to run.
 * @returns 0 on success, -1 on failure
 *
 * Example:
 * @code
 * {
 *     result = parcEventScheduler_DispatchBlocking(parcEventScheduler, PARCEventScheduler_Blocking);
 * }
 * @endcode
 */
int parcEventScheduler_DispatchBlocking(PARCEventScheduler *parcEventScheduler);

/**
 * Dispatch the event scheduler to process any pending events.
 *
 * If there are no pending events then the function will immediately return.
 *
 * @param [in] parcEventScheduler - The parcEventScheduler instance to run.
 * @returns 0 on success, -1 on failure
 *
 * Example:
 * @code
 * {
 *     result = parcEventScheduler_DispatchNonBlocking(parcEventScheduler, PARCEventScheduler_Blocking);
 * }
 * @endcode
 *
 */
int parcEventScheduler_DispatchNonBlocking(PARCEventScheduler *parcEventScheduler);

/**
 * Stop the event parcEventScheduler
 *
 * @param [in] parcEventScheduler instance to stop scheduling.
 * @param [in] delay time to wait before stopping, 0 for stop now
 * @returns 0 on success, -1 on failure
 *
 * Example:
 * @code
 * {
 *     result = parcEventScheduler_Stop(parcEventScheduler, timeout);
 * }
 * @endcode
 *
 */
int parcEventScheduler_Stop(PARCEventScheduler *parcEventScheduler, struct timeval *delay);

/**
 * Immediately abort the event parcEventScheduler
 *
 * @param [in] parcEventScheduler instance to abort scheduling.
 * @returns 0 on success, -1 on failure
 *
 * Example:
 * @code
 * {
 *     result = parcEventScheduler_Abort(parcEventScheduler);
 * }
 * @endcode
 *
 */
int parcEventScheduler_Abort(PARCEventScheduler *parcEventScheduler);

/**
 * Destroy a parcEventScheduler instance.
 *
 * The address of the parcEventScheduler instance is passed in.
 *
 * @param [in] parcEventScheduler address of instance to destroy.
 *
 * Example:
 * @code
 * {
 *     parcEventScheduler_Destroy(&parcEventScheduler);
 * }
 * @endcode
 *
 */
void parcEventScheduler_Destroy(PARCEventScheduler **parcEventScheduler);

/**
 * Turn on debugging flags and messages
 *
 * Example:
 * @code
 * {
 *     parcEventScheduler_EnableDebug();
 * }
 * @endcode
 *
 */
void parcEventScheduler_EnableDebug(void);

/**
 * Turn off debugging flags and messages
 *
 * Example:
 * @code
 * {
 *     parcEventScheduler_DisableDebug();
 * }
 * @endcode
 *
 */
void parcEventScheduler_DisableDebug(void);

/**
 * Internal libevent data accessor function.
 *
 * THIS IS FOR INTERNAL USE ONLY. USE WITH CAUTION.
 *
 * Example:
 * @code
 * {
 *     parcEventScheduler_GetEvBase(parcEventScheduler);
 * }
 * @endcode
 *
 */
void *parcEventScheduler_GetEvBase(PARCEventScheduler *parcEventScheduler);

/**
 * Logger accessor function
 *
 * Example:
 * @code
 * {
 *     PARCLog *logger = parcEventScheduler_GetLogger(parcEventScheduler);
 * }
 * @endcode
 *
 */
PARCLog *parcEventScheduler_GetLogger(PARCEventScheduler *parcEventScheduler);
#endif // libparc_parc_EventScheduler_h
