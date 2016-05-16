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
 * @file parc_LogLevel.h
 * @brief Logging levels for PARCLog
 *
 * A PARCLogLevel represents both the severity level of a particular PARCLogEntry,
 * and the threshold of a particular PARCLog instance.
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef PARC_Library_parc_LogLevel_h
#define PARC_Library_parc_LogLevel_h

typedef unsigned char PARCLogLevel;

/** OFF is a special level that can be used to turn off logging. */
extern const PARCLogLevel PARCLogLevel_Off;

/** ALL indicates that all messages should be logged. */
extern const PARCLogLevel PARCLogLevel_All;

/** PARCLogLevel_Emergency is a message level indicating the system is unusable. */
extern const PARCLogLevel PARCLogLevel_Emergency;

/** PARCLogLevel_Alert is a message level indicating action must be taken immediately to ensure correctness. */
extern const PARCLogLevel PARCLogLevel_Alert;

/** PARCLogLevel_Critical is a message level for critical conditions. */
extern const PARCLogLevel PARCLogLevel_Critical;

/** PARCLogLevel_Error is a message level reporting error conditions. */
extern const PARCLogLevel PARCLogLevel_Error;

/** PARCLogLevel_Warning indicates a fairly detailed tracing message. */
extern const PARCLogLevel PARCLogLevel_Warning;

/** FINEST indicates a normal but significant condition. */
extern const PARCLogLevel PARCLogLevel_Notice;

/** INFO is a message level for informational messages. */
extern const PARCLogLevel PARCLogLevel_Info;

/** INFO is a message level for debug-level messages. */
extern const PARCLogLevel PARCLogLevel_Debug;

/**
 * Compare `PARCLogLevel` instances @p levelA and @p levelA for order.
 *
 * @param [in] levelA An instance of PARCLogLevel.
 * @param [in] levelB An instance of PARCLogLevel.
 *
 * @return < 0 levelA is less than levelB
 * @return == 0 levelA is equal to levelB
 * @return > 0 levelA is greater than levelB
 *
 * Example:
 * @code
 * {
 *     int comparison = parcLogLevel_Compare(PARCLogLevel_Notice, PARCLogLevel_Notice);
 * }
 * @endcode
 */
#define parcLogLevel_Compare(_levelA_, _levelB_) (_levelA_ - _levelB_)
//int parcLogLevel_Compare(const PARCLogLevel levelA, const PARCLogLevel levelB);

/**
 * Determine if two instances of PARCLogLevel are equal.
 *
 * @param [in] levelA An instance of PARCLogLevel.
 * @param [in] levelB An instance of PARCLogLevel.
 *
 * @return true The instances are equal.
 * @return false The instances are not equal.
 *
 * Example:
 * @code
 * {
 *     bool equal = parcLogLevel_Equals(PARCLogLevel_Notice, PARCLogLevel_Notice);
 * }
 * @endcode
 */
int parcLogLevel_Equals(const PARCLogLevel levelA, const PARCLogLevel levelB);

/**
 * Get the string representation of the PARCLogLevel;
 *
 * @param [in] level A valid PARCLogLevel
 *
 * @return A pointer to a constant, nul-terminated C string.
 *
 * Example:
 * @code
 * {
 *     const char * logLevelString = parcLogLevel_ToString(PARCLogLevel_Emergency);
 * }
 * @endcode
 */
const char *parcLogLevel_ToString(const PARCLogLevel level);

/**
 * Given a string representation of a logging level, return the corresponding PARCLogLevel value.
 *
 * The string is case insensitive.
 *
 * Unknown or uninterpretable strings return PARCLogLevel_All.
 *
 * @param [in] levelAsString A nul-terminated C string representation of the logging level.
 *
 * @return A valid PARCLogLevel.
 *
 * Example:
 * @code
 * {
 *     PARCLogLevel level = parcLogLevel_FromString("Emergency");
 * }
 * @endcode
 */
PARCLogLevel parcLogLevel_FromString(const char *levelAsString);
#endif
