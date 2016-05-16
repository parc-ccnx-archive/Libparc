/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 *
 * June 9, 2014 add openssl multithreaded support.
 *              See http://www.openssl.org/docs/crypto/threads.html.
 *              We need to implement locking_function() and threadid_func().
 */
#include <config.h>

#include <stdio.h>
#include <stdbool.h>

#include <pthread.h>

#include <LongBow/runtime.h>

#include <parc/security/parc_Security.h>
#include <parc/algol/parc_Memory.h>

#include <openssl/opensslv.h>
#include <openssl/err.h>
#include <openssl/crypto.h>

#define OPENSSL_THREAD_DEFINES
#include <openssl/opensslconf.h>
#if !defined(OPENSSL_THREADS)
#error OpenSSL must support threads
#endif

// OPENSSL_VERSION_NUMBER = MMNNFFPPS
// MM = major NN = minor FF = fix PP = patch S = 0 (dev), 1-E (betas), F (release)
// This will require 1.0.1e release minimum version
//#if OPENSSL_VERSION_NUMBER < 0x1000105fL
#if OPENSSL_VERSION_NUMBER < 0x0009081fL
#pragma message(OPENSSL_VERSION_TEXT)
#error OpenSSL version must be at least 0.9.8 release
#endif

// Use the LongBow aids for this (case 999)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include <openssl/evp.h>
#include <openssl/err.h>

/*
 * OpenSSL requires one lock per thread, so we have a global lock array
 */
static pthread_mutex_t *perThreadLocks;

static volatile bool parcSecurity_initialized = false;
static unsigned long parcSecurity_count = 0;

pthread_mutex_t parcSecurity_mutex = PTHREAD_MUTEX_INITIALIZER;

static void
_lockingCallback(int mode, int type, const char *file __attribute__((unused)), int line __attribute__((unused)))
{
    int error = 0;
    if (mode & CRYPTO_LOCK) {
        error = pthread_mutex_lock(&(perThreadLocks[type]));
    } else {
        error = pthread_mutex_unlock(&(perThreadLocks[type]));
    }

    assertTrue(error == 0, "Error in pthreads: (%d) %s", errno, strerror(errno));
}

#if OPENSSL_VERSION_NUMBER < 0x1000005fL

// for older pre-1.0 versions of openssl
static unsigned long
_getThreadId(void)
{
    // Now, do the "right thing" with it.  Some systems return a pointer to a struct
    // and other systems return a native type.  OpenSSL (1.0 or later) provide two
    // mapping functions to convert these types of representations to CRYPTO_THREADID.
#if defined(__APPLE__)
    uint64_t threadid = 0;
    int error = pthread_threadid_np(pthread_self(), &threadid);
    assertTrue(error == 0, "Error getting threadid");
    return (unsigned long) threadid;

#elif defined(__linux__)
    // linux (at least ubuntu and redhat) uses unsigned long int
    pthread_t threadid = pthread_self();
    return (unsigned long) threadid;
#else
#error Unsupported platform, only __APPLE__ and __linux__ supported
#endif
}

#else

// For new 1.0 or later versions of openssl
static void
_getThreadId(CRYPTO_THREADID *id)
{
    pthread_t threadid = pthread_self();

    // Now, do the "right thing" with it.  Some systems return a pointer to a struct
    // and other systems return a native type.  OpenSSL (1.0 or later) provide two
    // mapping functions to convert these types of representations to CRYPTO_THREADID.

#if defined(__APPLE__)
    // The Apple Mac OS X pthreads uses a pointer to a struct (struct _opaque_pthread_t)
    CRYPTO_THREADID_set_pointer(id, threadid);
#elif defined(__linux__)
    // linux (at least ubuntu and redhat) uses unsigned long int
    CRYPTO_THREADID_set_numeric(id, threadid);
#else
#error Unsupported platform, only __APPLE__ and __linux__ supported
#endif
}
#endif

static void
_initLocks(void)
{
    perThreadLocks = parcMemory_AllocateAndClear(CRYPTO_num_locks() * sizeof(pthread_mutex_t));
    assertNotNull(perThreadLocks, "parcMemory_AllocateAndClear(%zu) returned NULL",
                  CRYPTO_num_locks() * sizeof(pthread_mutex_t));

    for (int i = 0; i < CRYPTO_num_locks(); i++) {
        pthread_mutex_init(&(perThreadLocks[i]), NULL);
    }

#if OPENSSL_VERSION_NUMBER < 0x1000005fL
    // for older pre-1.0 versions
    CRYPTO_set_id_callback(_getThreadId);
#else
    CRYPTO_THREADID_set_callback(_getThreadId);
#endif

    CRYPTO_set_locking_callback(_lockingCallback);
}

static void
_finiLocks(void)
{
    CRYPTO_set_locking_callback(NULL);
    for (int i = 0; i < CRYPTO_num_locks(); i++) {
        pthread_mutex_destroy(&(perThreadLocks[i]));
    }
    parcMemory_Deallocate((void **) &perThreadLocks);
}

void
parcSecurity_AssertIsInitialized(void)
{
    trapUnexpectedStateIf(parcSecurity_IsInitialized() == false, "PARC Security framework is not initialized.  See parcSecurity_Init()");
}

void
parcSecurity_Init(void)
{
    int lockSuccessful = pthread_mutex_lock(&parcSecurity_mutex) == 0;
    assertTrue(lockSuccessful, "Unable to lock the PARC Security framework.");

    if (!parcSecurity_initialized) {
        _initLocks();
        OpenSSL_add_all_algorithms();
        ERR_load_crypto_strings();

        parcSecurity_initialized = true;
    }
    parcSecurity_count++;

    int unlockSuccessful = pthread_mutex_unlock(&parcSecurity_mutex) == 0;
    assertTrue(unlockSuccessful, "Unable to unlock the PARC Security framework.");
}

bool
parcSecurity_IsInitialized(void)
{
    return parcSecurity_initialized;
}

void
parcSecurity_Fini(void)
{
    int lockSuccessful = pthread_mutex_lock(&parcSecurity_mutex) == 0;
    assertTrue(lockSuccessful, "Unable to lock the PARC Security framework.");

    parcSecurity_count--;
    if (parcSecurity_count == 0) {
        EVP_cleanup();
        ERR_free_strings();
        parcSecurity_initialized = false;
        _finiLocks();
    }

    int unlockSuccessful = pthread_mutex_unlock(&parcSecurity_mutex) == 0;
    assertTrue(unlockSuccessful, "Unable to unlock the PARC Security framework.");
}

#pragma GCC diagnostic pop
