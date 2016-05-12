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
 * @file parc_LogReporter.h
 * @brief An abstract representation of a PARC Log Reporter.
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef PARC_Library_parc_LogReporter_h
#define PARC_Library_parc_LogReporter_h

#include <parc/logging/parc_LogEntry.h>

typedef void (PARCLogReporterAcquire)(void *reporter);

/**
 * A Function that performs the final cleanup and resource deallocation when
 * a PARCLogReporter is no longer needed.
 */
typedef void (PARCLogReporterRelease)(void **reporterP);

/**
 */
typedef void (PARCLogReporterReport)(const PARCLogEntry *reporter);

struct PARCLogReporter;
typedef struct PARCLogReporter PARCLogReporter;

/**
 * Create a new instance of `PARCLogReporter` using the given the functions specified.
 *
 * @param [in] acquire A pointer to a function that performs the Aquire contract.
 * @param [in] release A pointer to a function that performs the Release contract.
 * @param [in] report A pointer to a function that performs the 'report' function.
 * @param [in] privateObject A pointer to a PARCObject that is supplied to the report function when invoked, or NULL.
 *
 * @return NULL Memory could not be allocated.
 * @return non-NULL A pointer to a valid `PARCLogReporter` instance.
 *
 * Example:
 * @code
 * {
 *     PARCLogReporter *result = parcLogReporter_Create(&parcLogReporterFile_Acquire,
 *         parcLogReporterFile_Release,
 *         parcLogReporterFile_Report,
 *         parcOutputStream_Acquire(output));
 *     return result;
 * }
 * @endcode
 */
PARCLogReporter *parcLogReporter_Create(PARCLogReporter *(*acquire)(const PARCLogReporter *),
                                        void (*release)(PARCLogReporter **),
                                        void (*report)(PARCLogReporter *, const PARCLogEntry *),
                                        void *privateObject);

/**
 * Increase the number of references to a `PARCLogReporter` instance.
 *
 * Note that new `PARCLogReporter` is not created,
 * only that the given `PARCLogReporter` reference count is incremented.
 * Discard the reference by invoking `parcLogReporter_Release`.
 *
 * @param [in] instance A pointer to a `PARCLogReporter` instance.
 *
 * @return The input `PARCLogReporter` pointer.
 *
 * Example:
 * @code
 * {
 *     PARCLogReporter *reporter = parcLogReporter_Create(&parcLogReporterFile_Acquire,
 *         parcLogReporterFile_Release,
 *         parcLogReporterFile_Report,
 *         parcOutputStream_Acquire(output));
 *
 *     PARCLogReporter *x_2 = parcLogReporter_Acquire(reporter);
 *
 *     parcLogReporter_Release(&reporter);
 *     parcLogReporter_Release(&x_2);
 * }
 * @endcode
 */
PARCLogReporter *parcLogReporter_Acquire(const PARCLogReporter *instance);

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
 * @param [in,out] instancePtr A pointer to a pointer to a `PARCLogReporter`.  The parameter is set to zero.
 *
 * Example:
 * @code
 * {
 *     PARCLogReporter *reporter = parcLogReporter_Create(&parcLogReporterFile_Acquire,
 *         parcLogReporterFile_Release,
 *         parcLogReporterFile_Report,
 *         parcOutputStream_Acquire(output));
 *
 *     parcLogReporter_Release(&reporter);
 * }
 * @endcode
 */
void parcLogReporter_Release(PARCLogReporter **instancePtr);

/**
 * Report the given PARCLogEntry
 *
 * @param [in] reporter A pointer to a valid PARCLogReporter instance.
 * @param [in] entry A pointer to a valid PARCLogEntry instance.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void parcLogReporter_Report(PARCLogReporter *reporter, const PARCLogEntry *entry);

/**
 * Get the private PARCObject supplied when the PARCLogReporter was created.
 *
 * @param [in] reporter A valid PARCLogReporter instance.
 *
 * @return A same pointer supplied when the PARCLogReporter was created.
 *
 */
void *parcLogReporter_GetPrivateObject(const PARCLogReporter *reporter);

#endif
