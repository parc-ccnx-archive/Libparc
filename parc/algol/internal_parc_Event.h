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
 * @file parc_EventQueue.h
 * @ingroup events
 * @brief Queue buffer events
 *
 * Provides a facade implementing many regularly available event functions.
 * This is an interface that software implementors may use to substitute
 * different kinds of underlying implementations of these event management functions.
 * Notable examples are libevent and libev.
 *
 * @author Kevin Fox, Palo Alto Research Center (Xerox PARC)
 * @copyright 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libparc_internal_parc_Event_h
#define libparc_internal_parc_Event_h

#include <parc/algol/parc_EventScheduler.h>
#include <parc/algol/parc_EventQueue.h>

/**
 * Map alloc() method call to a PARC internal memory method
 *
 * @param [in] size of memory to allocate
 * @returns NULL on error, memory pointer on success.
 *
 * Example:
 * @code
 * {
 *     return internal_parc_alloc(1024);
 * }
 * @endcode
 *
 */
void *internal_parc_alloc(size_t size);

/**
 * Map realloc() method call to a PARC internal memory method
 *
 * @param [in] pointer to memory to reallocate
 * @param [in] newSize of memory to allocate
 * @returns NULL on error, new memory pointer on success.
 *
 * Example:
 * @code
 * {
 *     return internal_parc_realloc(ptr, 2048);
 * }
 * @endcode
 *
 */
void *internal_parc_realloc(void *pointer, size_t newSize);

/**
 * Map free() method call to a PARC internal memory method
 *
 * @param [in] pointer to memory to free
 *
 * Example:
 * @code
 * {
 *     internal_parc_free(ptr);
 * }
 * @endcode
 *
 */
void internal_parc_free(void *ptr);

/**
 * Verify and initialize libevent
 *
 * Example:
 * @code
 * {
 *     internal_parc_initializeLibevent();
 * }
 * @endcode
 *
 */
void internal_parc_initializeLibevent(void);

/**
 * Convert from/to libevent dispatcher options and PARCEventSchedulerDispatcherType options.
 *
 * Example:
 * @code
 * {
 *     PARCEventSchedulerDispatchType type = internal_eventloop_options_to_PARCEventSchedulerDispatchType(EVLOOP_ONCE);
 *     short evtype = internal_PARCEventSchedulerDispatchType_to_eventloop_options(PARCEventSchedulerDispatchType_LoopOnce);
 * }
 * @endcode
 *
 */
PARCEventSchedulerDispatchType internal_eventloop_options_to_PARCEventSchedulerDispatchType(short evoptions);
short internal_PARCEventSchedulerDispatchType_to_eventloop_options(PARCEventSchedulerDispatchType options);

/**
 * Convert from/to libevent bufferevent options and PARCEventQueueOption.
 *
 * Example:
 * @code
 * {
 *     PARCEventQueueOption parcEventQueueOption = internal_bufferevent_options_to_PARCEventQueueOption(BEV_OPT_CLOSE_ON_FREE);
 *     short buffereventOption = internal_PARCEventQueueOption_to_bufferevent_options(PARCEventQueueOption_CloseOnFree);
 * }
 * @endcode
 *
 */
PARCEventQueueOption internal_bufferevent_options_to_PARCEventQueueOption(short evflags);
short internal_PARCEventQueueOption_to_bufferevent_options(PARCEventQueueOption flags);

/**
 * Convert from/to libevent bufferevent types and PARCEventQueueEventType.
 *
 * Example:
 * @code
 * {
 *     PARCEventQueueEventType parcEventQueueEventType = internal_bufferevent_type_to_PARCEventQueueEventType(BEV_EVENT_READING);
 *     short buffereventOptions = internal_PARCEventQueueEventType_to_bufferevent_type(PARCEventQueueEventType_Reading);
 * }
 * @endcode
 *
 */
PARCEventQueueEventType internal_bufferevent_type_to_PARCEventQueueEventType(short evtypes);
short internal_PARCEventQueueEventType_to_bufferevent_type(PARCEventQueueEventType types);

/**
 * Convert from/to libevent event types and PARCEventType.
 *
 * Example:
 * @code
 * {
 *     PARCEventType parcEventType = internal_libevent_type_to_PARCEventType(EV_READ);
 *     short buffereventOptions = internal_PARCEventType_to_libevent_type(PARCEventType_Read);
 * }
 * @endcode
 *
 */
PARCEventType internal_libevent_type_to_PARCEventType(short evtypes);
short internal_PARCEventType_to_libevent_type(PARCEventType types);

/**
 * Convert from/to libevent priority types and PARCEventPriority.
 *
 * Example:
 * @code
 * {
 *     PARCEventPriority parcEventPriority = internal_libevent_priority_to_PARCEventPriority(0);
 *     short priority = internal_PARCEventPriority_to_libevent_priority(PARCEventPriority_Normal);
 * }
 * @endcode
 *
 */
short internal_PARCEventPriority_to_libevent_priority(PARCEventPriority priority);
PARCEventPriority internal_libevent_priority_to_PARCEventPriority(short evpriority);
#endif // libparc_internal_parc_Event_h
