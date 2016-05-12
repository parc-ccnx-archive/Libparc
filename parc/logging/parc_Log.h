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
 * @file parc_Log.h
 * @brief Event logging.
 *
 * This is an logging mechanism patterned after the Syslog logging protocol (RFC 5424),
 * and influenced by `java.util.logging` and Apache Log4J.
 *
 * The lifecycle of a `PARCLog` starts with creating an instance via `parcLog_Create`
 * and calling the various functions to emit log messages.
 *
 * Finally the log is released via `parcLog_Release` which ensures
 * that any queued log messages are transmitted and resources are released.
 *
 * Every PARCLog instance has a logging level, a threshold that is set via `parcLog_SetLevel`
 * that determines what kind of PARCLogEntry instances are actually logged.
 * The PARCLogLevel PARCLogLevel_Emergency is always logged regardless of the current logging level.
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_Logger_h
#define libparc_parc_Logger_h

#include <stdarg.h>

#include <parc/logging/parc_LogReporter.h>
#include <parc/logging/parc_LogEntry.h>
#include <parc/logging/parc_LogLevel.h>

struct PARCLog;
typedef struct PARCLog PARCLog;

/**
 * Create a valid PARCLog instance.
 *
 * The initial instance's log level is set to `PARCLogLevel_Off`.
 *
 * @param [in] hostName A pointer to a nul-terminated C string, or NULL (See {@link PARCLogEntry}).
 * @param [in] applicationName A pointer to a nul-terminated C string, or NULL (See {@link PARCLogEntry}).
 * @param [in] processId A pointer to a nul-terminated C string, or NULL (See {@link PARCLogEntry}).
 * @param [in] reporter A pointer to a valid `PARCLogReporter` instance.
 *
 * @return non-NULL A valid PARCLog instance.
 * @return NULL An error occurred.
 *
 * Example:
 * @code
 * {
 *     PARCFileOutputStream *fileOutput = parcFileOutputStream_Create(1);
 *     PARCOutputStream *output = parcFileOutputStream_AsOutputStream(fileOutput);
 *
 *     PARCLogReporter *reporter = parcLogReporterFile_Create(output);
 *
 *     parcOutputStream_Release(&output);
 *     parcFileOutputStream_Release(&fileOutput);
 *
 *     PARCLog *log = parcLog_Create("localhost", "myApp", "daemon", reporter);
 *     parcLogReporter_Release(&reporter);
 * }
 * @endcode
 */
PARCLog *parcLog_Create(const char *hostName, const char *applicationName, const char *processId, PARCLogReporter *reporter);

/**
 * Increase the number of references to a `PARCLog`.
 *
 * Note that new `PARCLog` is not created,
 * only that the given `PARCLog` reference count is incremented.
 * Discard the reference by invoking `parcLog_Release`.
 *
 * @param [in] parcLog A pointer to a `PARCLog` instance.
 *
 * @return The input `PARCLog` pointer.
 *
 * Example:
 * @code
 * {
 *     PARCFileOutputStream *fileOutput = parcFileOutputStream_Create(1);
 *     PARCOutputStream *output = parcFileOutputStream_AsOutputStream(fileOutput);
 *
 *     PARCLogReporter *reporter = parcLogReporterFile_Create(output);
 *
 *     parcOutputStream_Release(&output);
 *     parcFileOutputStream_Release(&fileOutput);
 *
 *     PARCLog *log = parcLog_Create("localhost", "myApp", "daemon", reporter);
 *
 *     PARCLog *x_2 = parcLog_Acquire(log);
 *
 *     parcLog_Release(&log);
 *     parcLog_Release(&x_2);
 * }
 * @endcode
 */
PARCLog *parcLog_Acquire(const PARCLog *parcLog);

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
 * @param [in,out] logPtr A pointer to a PARCLog instance pointer, which will be set to zero on return.
 *
 * Example:
 * @code
 * {
 *     PARCFileOutputStream *fileOutput = parcFileOutputStream_Create(1);
 *     PARCOutputStream *output = parcFileOutputStream_AsOutputStream(fileOutput);
 *
 *     PARCLogReporter *reporter = parcLogReporterFile_Create(output);
 *
 *     parcOutputStream_Release(&output);
 *     parcFileOutputStream_Release(&fileOutput);
 *
 *     PARCLog *log = parcLog_Create("localhost", "myApp", "daemon", reporter);
 *     parcLogReporter_Release(&reporter);
 *
 *     parcLog_Release(&log);
 * }
 * @endcode
 */
void parcLog_Release(PARCLog **logPtr);

/**
 * Set the log severity level to the given value.
 *
 * The level is the maximum severity that will be logged via the PARCLogReporter.
 * The log severity PARCLogLevel_Emergency cannot be blocked.
 *
 * @param [in] log A pointer to valid instance of PARCLog.
 * @param [in] level A pointer to valid instance of PARCLogLevel.
 *
 * @return The previous value of the threshold.
 *
 * Example:
 * @code
 * {
 *     PARCFileOutputStream *fileOutput = parcFileOutputStream_Create(1);
 *     PARCOutputStream *output = parcFileOutputStream_AsOutputStream(fileOutput);
 *
 *     PARCLogReporter *reporter = parcLogReporterFile_Create(output);
 *
 *     parcOutputStream_Release(&output);
 *     parcFileOutputStream_Release(&fileOutput);
 *
 *     PARCLog *log = parcLog_Create("localhost", "myApp", "daemon", reporter);
 *     parcLogReporter_Release(&reporter);
 *
 *     PARCLogLevel old = parcLog_SetLevel(log, PARCLogLevel_Warning);
 *
 *     parcLog_SetLevel(log, old);
 *
 *     parcLog_Release(&log);
 * }
 * @endcode
 */
PARCLogLevel parcLog_SetLevel(PARCLog *log, const PARCLogLevel level);

/**
 * Get the severity level of the given PARCLog instance.
 *
 * The level is the maximum severity that will be logged via the PARCLogReporter.
 * The log severity PARCLogLevel_Emergency cannot be blocked.
 *
 * @param [in] log A pointer to valid instance of PARCLog.
 *
 * @return The severity level of the given PARCLog instance.
 *
 * Example:
 * @code
 * {
 *     PARCFileOutputStream *fileOutput = parcFileOutputStream_Create(1);
 *     PARCOutputStream *output = parcFileOutputStream_AsOutputStream(fileOutput);
 *
 *     PARCLogReporter *reporter = parcLogReporterFile_Create(output);
 *
 *     parcOutputStream_Release(&output);
 *     parcFileOutputStream_Release(&fileOutput);
 *
 *     PARCLog *log = parcLog_Create("localhost", "myApp", "daemon", reporter);
 *     parcLogReporter_Release(&reporter);
 *
 *     PARCLogLevel level = parcLog_GetLevel(log, PARCLogLevel_Warning);
 *
 *     parcLog_Release(&log);
 * }
 * @endcode
 */
PARCLogLevel parcLog_GetLevel(const PARCLog *log);

/**
 * Test if a PARCLogLevel would be logged by the current state of the given PARCLog instance.
 *
 * @param [in] log A pointer to valid instance of PARCLog.
 * @param [in] level An instance of PARCLogLevel.
 *
 * @return true A PARCLogEntry of the given level would be logged.
 * @return false A PARCLogEntry of the given level would be logged.
 *
 * Example:
 * @code
 * {
 *     PARCFileOutputStream *fileOutput = parcFileOutputStream_Create(1);
 *     PARCOutputStream *output = parcFileOutputStream_AsOutputStream(fileOutput);
 *
 *     PARCLogReporter *reporter = parcLogReporterFile_Create(output);
 *
 *     parcOutputStream_Release(&output);
 *     parcFileOutputStream_Release(&fileOutput);
 *
 *     PARCLog *log = parcLog_Create("localhost", "myApp", "daemon", reporter);
 *     parcLogReporter_Release(&reporter);
 *
 *     if (parcLog_IsLoggable(log, PARCLogLevel_Warning)) {
 *         printf("Logging is set to Warning severity level\n");
 *     }
 *
 *     parcLog_Release(&log);
 * }
 * @endcode
 */

#define parcLog_IsLoggable(_log_, _level_) \
    (_level_ == PARCLogLevel_Emergency) || (parcLogLevel_Compare(parcLog_GetLevel(_log_), _level_) >= 0)
//bool parcLog_IsLoggable(const PARCLog *log, const PARCLogLevel level);

/**
 * Compose and emit a log message.
 *
 * @param [in] log A pointer to a valid PARCLog instance.
 * @param [in] level An instance of PARCLogLevel.
 * @param [in] messageId A value for the message identifier.
 * @param [in] format A pointer to a nul-terminated C string containing a printf format specification.
 * @param [in] ap A `va_list` representing the parameters for the format specification.
 *
 * @return true The message was logged.
 * @return false The message was not logged because the log severity threshold level is lower than the specified PARCLogLevel.
 *
 * Example:
 * @code
 *     parcLog_MessageVaList(log, PARCLogLevel_Warning, 123, "This is a warning message.");
 * @endcode
 */
bool parcLog_MessageVaList(PARCLog *log, PARCLogLevel level, uint64_t messageId, const char *format, va_list ap);

/**
 * Compose and emit a log message.
 *
 * @param [in] log A pointer to a valid PARCLog instance.
 * @param [in] level An instance of PARCLogLevel.
 * @param [in] messageId A value for the message identifier.
 * @param [in] format A pointer to a nul-terminated C string containing a printf format specification.
 * @param [in] ... Zero or more parameters as input for the format specification).
 *
 * @return true The message was logged.
 * @return false The message was not logged because the log severity threshold level is lower than the specified PARCLogLevel.
 *
 * Example:
 * @code
 *     parcLog_Message(log, PARCLogLevel_Warning, "This is a warning message.");
 * @endcode
 */
bool parcLog_Message(PARCLog *log, PARCLogLevel level, uint64_t messageId, const char * restrict format, ...);

/**
 * Compose and emit a PARCLogLevel_Warning message.
 *
 * @param [in] log A pointer to a valid PARCLog instance.
 * @param [in] format A pointer to a nul-terminated C string containing a printf format specification.
 * @param [in] ... Zero or more parameters as input for the format specification).
 *
 * @return true The message was logged.
 * @return false The message was not logged because the log severity threshold level is lower.
 *
 * Example:
 * @code
 *     parcLog_Warning(log, "This is a warning message.");
 * @endcode
 */
bool parcLog_Warning(PARCLog *log, const char * restrict format, ...);

/**
 * Compose and emit a PARCLogLevel_Message level message.
 *
 * @param [in] log A pointer to a valid PARCLog instance.
 * @param [in] format A pointer to a nul-terminated C string containing a printf format specification.
 * @param [in] ... Zero or more parameters as input for the format specification).
 *
 * @return true The message was logged.
 * @return false The message was not logged because the log severity threshold level is lower.
 *
 * Example:
 * @code
 *     parcLog_Info(log, "This is an info message.");
 * @endcode
 */
bool parcLog_Info(PARCLog *log, const char * restrict format, ...);

/**
 * Compose and emit a PARCLogLevel_Notice level message.
 *
 * @param [in] log A pointer to a valid PARCLog instance.
 * @param [in] format A pointer to a nul-terminated C string containing a printf format specification.
 * @param [in] ... Zero or more parameters as input for the format specification).
 *
 * @return true The message was logged.
 * @return false The message was not logged because the log severity threshold level is lower.
 *
 * Example:
 * @code
 *     parcLog_Notice(log, "This is a notice message.");
 * @endcode
 */
bool parcLog_Notice(PARCLog *log, const char * restrict format, ...);

/**
 * Compose and emit a PARCLogLevel_Debug level message.
 *
 * @param [in] log A pointer to a valid PARCLog instance.
 * @param [in] format A pointer to a nul-terminated C string containing a printf format specification.
 * @param [in] ... Zero or more parameters as input for the format specification).
 *
 * @return true The message was logged.
 * @return false The message was not logged because the log severity threshold level is lower.
 *
 * Example:
 * @code
 *     parcLog_DebugMessage(log, "This is a debug message.");
 * @endcode
 */
bool parcLog_Debug(PARCLog *log, const char * restrict format, ...);

/**
 * Compose and emit a PARCLogLevel_Error level message.
 *
 * @param [in] log A pointer to a valid PARCLog instance.
 * @param [in] format A pointer to a nul-terminated C string containing a printf format specification.
 * @param [in] ... Zero or more parameters as input for the format specification).
 *
 * @return true The message was logged.
 * @return false The message was not logged because the log severity threshold level is lower.
 *
 * Example:
 * @code
 *     parcLog_ErrorMessage(log, "This is an error message.");
 * @endcode
 */
bool parcLog_Error(PARCLog *log, const char * restrict format, ...);

/**
 * Compose and emit a PARCLogLevel_Critical level message.
 *
 * @param [in] log A pointer to a valid PARCLog instance.
 * @param [in] format A pointer to a nul-terminated C string containing a printf format specification.
 * @param [in] ... Zero or more parameters as input for the format specification).
 *
 * @return true The message was logged.
 * @return false The message was not logged because the log severity threshold level is lower.
 *
 * Example:
 * @code
 *     parcLog_CriticalMessage(log, "This is a critical message.");
 * @endcode
 */
bool parcLog_Critical(PARCLog *log, const char * restrict format, ...);

/**
 * Compose and emit a PARCLogLevel_Alert level message.
 *
 * @param [in] log A pointer to a valid PARCLog instance.
 * @param [in] format A pointer to a nul-terminated C string containing a printf format specification.
 * @param [in] ... Zero or more parameters as input for the format specification).
 *
 * @return true The message was logged.
 * @return false The message was not logged because the log severity threshold level is lower.
 *
 * Example:
 * @code
 *     parcLog_AlertMessage(log, "This is an alert message.");
 * @endcode
 */
bool parcLog_Alert(PARCLog *log, const char * restrict format, ...);

/**
 * Compose and emit a PARCLogLevel_Emergency level message.
 *
 * @param [in] log A pointer to a valid PARCLog instance.
 * @param [in] format A pointer to a nul-terminated C string containing a printf format specification.
 * @param [in] ... Zero or more parameters as input for the format specification).
 *
 * @return true The message was logged.
 * @return false The message was not logged because the log severity threshold level is lower.
 *
 * Example:
 * @code
 *     parcLog_EmergencyMessage(log, "This is an emergency message.");
 * @endcode
 */
bool parcLog_Emergency(PARCLog *log, const char * restrict format, ...);
#endif // libparc_parc_Logger_h
