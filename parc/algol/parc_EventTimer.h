/*
 * Copyright (c) 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @file parc_EventTimer.h
 * @ingroup events
 * @brief Timer events
 *
 * Provides a facade implementing many regularly available event functions.
 * This is an interface that software implementors may use to substitute
 * different kinds of underlying implementations of these event management functions.
 * Notable examples are libevent and libev.
 *
 * @author Kevin Fox, Palo Alto Research Center (Xerox PARC)
 * @copyright 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libparc_parc_EventTimer_h
#define libparc_parc_EventTimer_h

#include <parc/algol/parc_Event.h>

typedef void (PARCEventTimer_Callback)(int fd, PARCEventType type, void *user_data);

typedef struct PARCEventTimer PARCEventTimer;

/**
 * Create a new timer event instance.
 *
 * The new event instance is returned.
 *
 * @param [in] parcEventScheduler - scheduler instance to attach to
 * @param [in] flags - event flags
 * @param [in] callback - timer callback function
 * @param [in] callbackArguments - private arguments passed to callback
 * @returns A pointer to the a PARCEventTimer instance.
 *
 * Example:
 * @code
 * {
 *     PARCEventTimer *event = parcEvent_Create(parcEventScheduler, PARCEvent_None, callback, callbackArguments);
 * }
 * @endcode
 *
 */
PARCEventTimer *parcEventTimer_Create(PARCEventScheduler *parcEventScheduler,
                                      PARCEventType flags,
                                      PARCEventTimer_Callback *callback,
                                      void *callbackArguments);

/**
 * Position a timer event instance to be scheduled.
 *
 * @param [in] parcEventTimer - The newly created event instance and a timeout value.
 * @param [in] timeout - time to wait for event, or NULL to wait forever.
 * @returns -1 on error, 0 on success if nothing changed in the event backend, and 1 on success if something did.
 *
 * Example:
 * @code
 * addEvent(PARCEventTimer *parcEventTimer)
 * {
 *     struct timeval timeout = {5, 0};
 *     int result = parcEventTimer_Start(parcEventTimer, &timeout);
 * }
 * @endcode
 *
 */
int parcEventTimer_Start(PARCEventTimer *parcEventTimer, struct timeval *timeout);

/**
 * Stop a timer event instance.
 *
 * @param [in] parcEventTimer - The newly created event instance and a timeout value.
 * @returns 0 on success
 *
 * Example:
 * @code
 * removeEvent(PARCEventTimer *parcEventTimer)
 * {
 *     parcEventTimer_Stop(parcEventTimer);
 * }
 * @endcode
 *
 */
int parcEventTimer_Stop(PARCEventTimer *parcEventTimer);

/**
 * Destroy an event instance.
 *
 * The event instance is passed in.
 *
 * @param [in] parcEventTimer the instance to destroy.
 *
 * Example:
 * @code
 * {
 *     parcEventTimer_Destroy(&parcEventTimer);
 * }
 * @endcode
 *
 */
void parcEventTimer_Destroy(PARCEventTimer **parcEventTimer);

/**
 * Turn on debugging flags and messages
 *
 * Example:
 * @code
 * {
 *     parcEventTimer_EnableDebug();
 * }
 * @endcode
 *
 */
void parcEventTimer_EnableDebug(void);

/**
 * Turn off debugging flags and messages
 *
 * Example:
 * @code
 * {
 *     parcEventTimer_DisableDebug();
 * }
 * @endcode
 *
 */
void parcEventTimer_DisableDebug(void);
#endif // libparc_parc_EventTimer_h
