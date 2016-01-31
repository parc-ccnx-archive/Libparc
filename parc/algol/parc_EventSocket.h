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
 * @file parc_EventSocket.h
 * @ingroup events
 * @brief Socket events
 *
 * Provides a facade implementing many regularly available event functions.
 * This is an interface that software implementors may use to substitute
 * different kinds of underlying implementations of these event management functions.
 * Notable examples are libevent and libev.
 *
 * @author Kevin Fox, Palo Alto Research Center (Xerox PARC)
 * @copyright 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libparc_parc_EventSocket_h
#define libparc_parc_EventSocket_h

#include <sys/types.h>
#include <sys/socket.h>

/**
 * Current implementation based on top of libevent2
 */

#include <parc/algol/parc_EventScheduler.h>
#include <parc/algol/parc_Event.h>

typedef struct PARCEventSocket PARCEventSocket;

typedef void (PARCEventSocket_Callback)(int fd, struct sockaddr *address,
                                        int socklen, void *user_data);
typedef void (PARCEventSocket_ErrorCallback)(PARCEventScheduler *,
                                             int error, char *errorString,
                                             void *user_data);

/**
 * Create a socket event handler instance.
 *
 * The event instance is passed in.
 *
 * @param [in] parcEventScheduler the scheduler instance
 * @param [in] callback the callback function.
 * @param [in] errorCallback the error callback function.
 * @param [in] userData pointer to private arguments for instance callback function
 * @param [in] sa is the socket address to bind to (INET, INET6, LOCAL)
 * @param [in] socklen is the sizeof the actual sockaddr (e.g. sizeof(sockaddr_un))
 * @returns A pointer to a new PARCEventSocket instance.
 *
 * Example:
 * @code
 * {
 * }
 * @endcode
 *
 */
PARCEventSocket *parcEventSocket_Create(PARCEventScheduler *parcEventScheduler,
                                        PARCEventSocket_Callback *callback,
                                        PARCEventSocket_ErrorCallback *errorCallback,
                                        void *userData,
                                        const struct sockaddr *sa, int socklen);

/**
 * Destroy a socket event handler instance.
 *
 * The event instance is passed in.
 *
 * @param [in] parcEventSocket the address of the instance to destroy.
 *
 * Example:
 * @code
 * {
 *     PARCEvent *event = parcEventSocket_Destroy(&parcEventSocket);
 * }
 * @endcode
 *
 */
void parcEventSocket_Destroy(PARCEventSocket **parcEventSocket);

/**
 * Turn on debugging flags and messages
 *
 * Example:
 * @code
 * {
 *     parcEventSocket_EnableDebug();
 * }
 * @endcode
 *
 */
void parcEventSocket_EnableDebug(void);

/**
 * Turn off debugging flags and messages
 *
 * Example:
 * @code
 * {
 *     parcEventSocket_DisableDebug();
 * }
 * @endcode
 *
 */
void parcEventSocket_DisableDebug(void);
#endif // libparc_parc_EventSocket_h
