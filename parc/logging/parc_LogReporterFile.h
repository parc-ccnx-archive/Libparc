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
 * @file parc_LogReporterFile.h
 * @brief <#Brief Description#>
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef __PARC_Library__parc_LogReporterFile__
#define __PARC_Library__parc_LogReporterFile__

#include <parc/logging/parc_LogReporter.h>
#include <parc/algol/parc_OutputStream.h>

/**
 * Create a new instance of `PARCLogReporter` using the given {@link PARCOutputStream}.
 *
 * @param [in] output A pointer to a valid `PARCOutputStream` instance.
 *
 * @return NULL Memory could not be allocated.
 * @return non-NULL A pointer to a valid `PARCLogReporter` instance.
 *
 * Example:
 * @code
 * {
 *     PARCFileOutputStream *fileOutput = parcFileOutputStream_Create(dup(STDOUT_FILENO));
 *     PARCOutputStream *out = parcFileOutputStream_AsOutputStream(fileOutput);
 *     parcFileOutputStream_Release(&fileOutput);
 *
 *     PARCLogReporter *reporter = parcLogReporterFile_Create(out);
 *     parcOutputStream_Release(&out);
 *
 *     parcLogReporter_Release(&reporter);
 * }
 * <#example#>
 * @endcode
 */
PARCLogReporter *parcLogReporterFile_Create(PARCOutputStream *output);

/**
 * Increase the number of references to a `PARCLogReporter` instance.
 *
 * Note that new `PARCLogReporter` is not created,
 * only that the given `PARCLogReporter` reference count is incremented.
 * Discard the reference by invoking `parcLogEntry_Release`.
 *
 * @param [in] instance A pointer to a `PARCLogReporter` instance.
 *
 * @return The input `PARCLogReporter` pointer.
 *
 * Example:
 * @code
 * {
 *     PARCLogReporter *x = parcLogReporterFile_Create(...);
 *
 *     PARCLogReporter *x_2 = parcLogReporterFile_Acquire(x);
 *
 *     parcLogReporterFile_Release(&x);
 *     parcLogReporterFile_Release(&x_2);
 * }
 * @endcode
 */
PARCLogReporter *parcLogReporterFile_Acquire(const PARCLogReporter *instance);

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
 * @param [in,out] reporterP A pointer to a PARCLogReporter instance pointer, which will be set to zero on return.
 *
 * Example:
 * @code
 * {
 *     PARCLogReporter *x = parcLogReporterFile_Create(...);
 *
 *     parcLogReporterFile_Release(&x);
 * }
 * @endcode
 */
void parcLogReporterFile_Release(PARCLogReporter **reporterP);

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
void parcLogReporterFile_Report(PARCLogReporter *reporter, const PARCLogEntry *entry);
#endif /* defined(__PARC_Library__parc_LogReporterFile__) */
