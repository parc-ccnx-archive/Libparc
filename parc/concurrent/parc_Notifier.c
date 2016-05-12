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
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include <LongBow/runtime.h>

#include <parc/concurrent/parc_Notifier.h>
#include <parc/algol/parc_Object.h>

#ifdef __GNUC__
#define ATOMIC_ADD_AND_FETCH(ptr, increment)      __sync_add_and_fetch(ptr, increment)
#define ATOMIC_BOOL_CAS(ptr, oldvalue, newvalue)  __sync_bool_compare_and_swap(ptr, oldvalue, newvalue)
#define ATOMIC_FETCH(ptr)                         ATOMIC_ADD_AND_FETCH(ptr, 0)
#define ATOMIC_SET(ptr, oldvalue, newvalue)       ATOMIC_BOOL_CAS(ptr, oldvalue, newvalue)
#else
#error "Only GNUC supported, we need atomic operations"
#endif

struct parc_notifier {
    volatile int paused;

    // If the notifications are paused and there is an event,
    // we indicate that we skipped a notify
    volatile int skippedNotify;

#define PARCNotifierWriteFd 1
#define PARCNotifierReadFd 0
    int fds[2];
};

static void
_parcNotifier_Finalize(PARCNotifier **notifierPtr)
{
    PARCNotifier *notifier = *notifierPtr;

    close(notifier->fds[0]);
    close(notifier->fds[1]);
}

parcObject_ExtendPARCObject(PARCNotifier, _parcNotifier_Finalize, NULL, NULL, NULL, NULL, NULL, NULL);

static bool
_parcNotifier_MakeNonblocking(PARCNotifier *notifier)
{
    // set the read side to be non-blocking
    int flags = fcntl(notifier->fds[PARCNotifierReadFd], F_GETFL, 0);
    if (flags == 0) {
        if (fcntl(notifier->fds[PARCNotifierReadFd], F_SETFL, flags | O_NONBLOCK) == 0) {
            return true;
        }
    }
    perror("fcntl error");
    return false;
}

PARCNotifier *
parcNotifier_Create(void)
{
    PARCNotifier *notifier = parcObject_CreateInstance(PARCNotifier);
    if (notifier) {
        notifier->paused = false;
        notifier->skippedNotify = false;

        int failure = pipe(notifier->fds);
        assertFalse(failure, "Error on pipe: %s", strerror(errno));

        if (!_parcNotifier_MakeNonblocking(notifier)) {
            parcObject_Release((void **) &notifier);
        }
    }

    return notifier;
}

parcObject_ImplementAcquire(parcNotifier, PARCNotifier);

parcObject_ImplementRelease(parcNotifier, PARCNotifier);


int
parcNotifier_Socket(PARCNotifier *notifier)
{
    return notifier->fds[PARCNotifierReadFd];
}

bool
parcNotifier_Notify(PARCNotifier *notifier)
{
    if (ATOMIC_BOOL_CAS(&notifier->paused, 0, 1)) {
        // old value was "0" so we need to send a notification
        uint8_t one = 1;
        ssize_t written;
        do {
            written = write(notifier->fds[PARCNotifierWriteFd], &one, 1);
            assertTrue(written >= 0, "Error writing to socket %d: %s", notifier->fds[PARCNotifierWriteFd], strerror(errno));
        } while (written == 0);

        return true;
    } else {
        // we're paused, so count up the pauses
        ATOMIC_ADD_AND_FETCH(&notifier->skippedNotify, 1);
        return false;
    }
}

void
parcNotifier_PauseEvents(PARCNotifier *notifier)
{
    // reset the skipped counter so we count from now until the StartEvents call
    notifier->skippedNotify = 0;
    ATOMIC_BOOL_CAS(&notifier->paused, 0, 1);

    // now clear out the socket
    uint8_t buffer[16];
    while (read(notifier->fds[PARCNotifierReadFd], &buffer, 16) > 0) {
        ;
    }
}

void
parcNotifier_StartEvents(PARCNotifier *notifier)
{
    ATOMIC_BOOL_CAS(&notifier->paused, 1, 0);
    if (notifier->skippedNotify) {
        // we missed some notifications, so re-signal ourself
        parcNotifier_Notify(notifier);
    }
}
