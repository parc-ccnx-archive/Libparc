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
 * @file parc_Event.h
 * @ingroup events
 * @brief Event management
 *
 * Provides a facade implementing many regularly available event functions.
 * This is an interface that software implementors may use to substitute
 * different kinds of underlying implementations of these event management functions.
 * Notable examples are libevent and libev.
 *
 * @author Kevin Fox, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_Event_h
#define libparc_parc_Event_h

/**
 * Current implementation based on top of libevent2
 */

#include <parc/algol/parc_EventScheduler.h>

/**
 * @typedef PARCEventType
 * @brief An enumeration of event types, with an additional persist flag
 */
typedef enum {
    PARCEventType_None          = 0x00,
    PARCEventType_Timeout       = 0x01,
    PARCEventType_Read          = 0x02,
    PARCEventType_Write         = 0x04,
    PARCEventType_Signal        = 0x08,
    PARCEventType_Persist       = 0x10,
    PARCEventType_EdgeTriggered = 0x20
} PARCEventType;

/**
 * @typedef PARCEventPriority
 * @brief Priority flags for queue scheduling, these currently match the RTA_*_PRIORITY
 * this will eventually be replaced.
 */
typedef enum {
    PARCEventPriority_Maximum = 0,
    PARCEventPriority_Normal  = 1,
    PARCEventPriority_Minimum = 2,
    PARCEventPriority_NumberOfPriorities = 3
} PARCEventPriority;

/**
 * @typedef PARCEvent_Callback
 * @brief Event callback definition
 */
typedef void (PARCEvent_Callback)(int fileDescriptor, PARCEventType type, void *user_data);

/**
 * @typedef PARCEvent
 * @brief A structure containing private libevent state data variables
 */
typedef struct PARCEvent PARCEvent;

/**
 * Create a new PARCEvent instance.
 *
 * A new PARCEvent instance is returned.
 *
 * @param [in] parcEventScheduler base instance
 * @param [in] fileDescriptor file descriptor to monitor
 * @param [in] events to catch
 * @param [in] callback function
 * @param [in] callbackArgs function private arguments
 * @returns A pointer to the a PARCEvent instance.
 *
 * Example:
 * @code
 * static void
 * _read_callback(int fileDescriptor, PARCEventType type, void *args)
 * {
 * }
 *
 * {
 *     PARCEventScheduler *eventScheduler = parcEventScheduer_Create();
 *     PARCEvent *event = parcEvent_Create(eventScheduler, fileDescriptor, PARCEvent_ReadEvent, _read_callback, _read_callback_args);
 * }
 * @endcode
 *
 */
PARCEvent *parcEvent_Create(PARCEventScheduler *parcEventScheduler, int fileDescriptor, PARCEventType events, PARCEvent_Callback *callback, void *callbackArgs);

/**
 * Start an event instance.
 *
 * @param [in] parcEvent instance to start
 * @returns -1 on error, 0 on success if nothing changed in the parcEvent backend, and 1 on success if something did.
 *
 * Example:
 * @code
 * startEvent(PARCEvent *parcEvent)
 * {
 *     return parcEvent_Start(parcEvent);
 * }
 * @endcode
 *
 */
int parcEvent_Start(PARCEvent *parcEvent);

/**
 * Stop a parcEvent instance.
 *
 * @param [in] parcEvent instance to stop
 * @returns -1 on error, 0 on success.
 *
 * Example:
 * @code
 * removeEvent(PARCEvent *parcEvent)
 * {
 *     return parcEvent_Stop(parcEvent);
 * }
 * @endcode
 *
 */
int parcEvent_Stop(PARCEvent *parcEvent);

/**
 * Poll if an event is available to process
 *
 * @param [in] parcEvent instance to stop
 * @param [in] event type to poll for
 * @returns -1 on error, 0 on success.
 *
 * Example:
 * @code
 * pollReadEvent(PARCEvent *parcEvent)
 * {
 *     return parcEvent_Poll(parcEvent, PARCEvent_ReadEvent);
 * }
 * @endcode
 *
 */
int parcEvent_Poll(PARCEvent *parcEvent, PARCEventType event);

/**
 * Destroy a parcEvent instance.
 *
 * @param [in] parcEvent address of instance to destroy.
 *
 * Example:
 * @code
 * {
 *     parcEvent_Destroy(&parcEvent);
 * }
 * @endcode
 *
 */
void parcEvent_Destroy(PARCEvent **parcEvent);

/**
 * Set a parcEvent instances priority.
 *
 * @param [in] parcEvent instance to modify
 * @param [in] priority to set to
 * @returns -1 on error, 0 on success.
 *
 * Example:
 * @code
 * {
 *     return parcEvent_SetPriority(parcEvent, priority);
 * }
 * @endcode
 *
 */
int parcEvent_SetPriority(PARCEvent *parcEvent, PARCEventPriority priority);

/**
 * Turn on debugging flags and messages
 *
 * Example:
 * @code
 * {
 *     parcEvent_EnableDebug();
 * }
 * @endcode
 *
 */
void parcEvent_EnableDebug(void);

/**
 * Turn off debugging flags and messages
 *
 * Example:
 * @code
 * {
 *     parcEvent_DisableDebug();
 * }
 * @endcode
 *
 */
void parcEvent_DisableDebug(void);
#endif // libparc_parc_Event_h
