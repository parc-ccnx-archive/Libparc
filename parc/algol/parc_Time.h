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
 * @file parc_Time.h
 * @ingroup inputoutput
 * @brief Time Manipulation
 *
 * Different platforms have different ways to express time-of-day, elapsed-time, clock-time.
 * In some cases multiple ways to express the same semantic value have evolved over time,
 * for example `struct timeval` and `struct timespec`.
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libparc_parc_Time_h
#define libparc_parc_Time_h

#include <sys/time.h>
#include <stdint.h>

/**
 * Create a nul-terminated C string containing the formatted representation of a `struct timeval`.
 *
 * The minimum length of the result is 8 characters consisting of the `struct timeval`
 * formatted as a decimal string consisting of the number of seconds since midnight (0 hour), January 1, 1970.
 *
 * @param [in] timeval The instance of the struct `timeval` to convert to a C string.
 * @return An allocated, null-terminated C string that must be freed via {@link parcMemory_Deallocate()}.
 *
 * Example:
 * @code
 * {
 *     struct timeval timeval;
 *     gettimeofday(&timeval, NULL);
 *
 *     char *string = parcTime_TimevalAsString(timeval);
 *
 *     parcMemory_Deallocate(&string);
 * }
 * @endcode
 */
char *parcTime_TimevalAsString(struct timeval timeval);

/**
 * Format an ISO8601 date from the given struct timeval into the given character array, @p result.
 *
 * @param [in] utcTime A pointer to a valid struct timeval instance with the time in UTC.
 * @param [out] result A pointer to the 64 element nul-terminated character array, @p result.
 *
 * @return The same value as @p result.
 *
 * Example:
 * @code
 * {
 *     struct timeval theTime;
 *     gettimeofday(&theTime, NULL);
 *
 *     char result[64];
 *     parcTime_TimevalAsISO8601(&theTime, result);
 *
 * }
 * @endcode
 */
char *parcTime_TimevalAsISO8601(const struct timeval *utcTime, char result[64]);

/**
 * Format an RFC3339 compliant date from the given struct timeval into the given character array.
 *
 * @param [in] utcTime A pointer to a valid struct timeval instance with the time in UTC.
 * @param [out] result A pointer to the 64 element nul-terminated character array, @p result.
 *
 * @return The same value as @p result.
 *
 * Example:
 * @code
 * {
 *     struct timeval theTime;
 *     gettimeofday(&theTime, NULL);
 *
 *     char result[64];
 *     parcTime_TimevalAsRFC3339(&theTime, result);
 *
 * }
 * @endcode
 */
char *parcTime_TimevalAsRFC3339(const struct timeval *utcTime, char result[64]);

/**
 * Format an ISO8601 date from the given `time_t` value into the given character array, @p result.
 *
 * @param [in] utcTime `time_t` value representing the time in UTC.
 * @param [out] result A pointer to the 64 element nul-terminated character array, @p result.
 *
 * @return The same value as @p result.
 *
 * Example:
 * @code
 * {
 *     time_t theTime = time(0);
 *
 *     char result[64];
 *     parcTime_TimeAsISO8601(theTime, result);
 *
 * }
 * @endcode
 */
char *parcTime_TimeAsISO8601(const time_t utcTime, char result[64]);

/**
 * Format the current time as an ISO8601 date into the given character array, @p result.
 *
 * @param [out] result A pointer to the 64 element nul-terminated character array, @p result.
 *
 * @return The same value as @p result.
 *
 * Example:
 * @code
 * {
 *     char result[64];
 *     parcTime_NowAsISO8601(theTime, result);
 *
 * }
 * @endcode
 */
char *parcTime_NowAsISO8601(char result[64]);

/**
 * Format an RFC3339 compliant date from the given `time_t` value into the given character array.
 *
 * @param [in] utcTime `time_t` value of the time in UTC.
 * @param [out] result A pointer to the 64 element nul-terminated character array, @p result.
 *
 * @return The same value as @p result.
 *
 * Example:
 * @code
 * {
 *     struct timeval theTime;
 *     time_t theTime = time(0);
 *
 *     char result[64];
 *     parcTime_TimeAsRFC3339(&theTime, result);
 *
 * }
 * @endcode
 */
char *parcTime_TimeAsRFC3339(const time_t utcTime, char result[64]);

/**
 * Format the current time as an RFC3339 compliant date into the given character array.
 *
 * @param [out] result A pointer to the 64 element nul-terminated character array, @p result.
 *
 * @return The same value as @p result.
 *
 * Example:
 * @code
 * {
 *     char result[64];
 *     parcTime_NowAsRFC3339(&theTime, result);
 *
 * }
 * @endcode
 */
char *parcTime_NowAsRFC3339(char result[64]);

/**
 * Add two `struct timeval` values together.
 *
 * @param [in] addend1 The first value.
 * @param [in] addend2 The second value.
 *
 * @return The sum of the first and second values.
 *
 * Example:
 * @code
 * {
 *     struct timeval now;
 *     gettimeofday(&now, NULL);
 *
 *     struct timeval theEnd = parcTime_TimevalAdd(&now, timeout);
 * }
 * @endcode
 */
struct timeval parcTime_TimevalAdd(const struct timeval *addend1, const struct timeval *addend2);

/**
 * Subtract two `struct timeval` values.
 *
 * @param [in] minuend The number from which the subtrahend is to be subtracted.
 * @param [in] subtrahend The subtrahend.
 *
 * @return The difference between the first and second values.
 *
 * Example:
 * @code
 * {
 *     struct timeval now;
 *     gettimeofday(&now, NULL);
 *
 *     struct timeval result = parcTime_TimevalSubtract(&now, timeout);
 * }
 * @endcode
 */
struct timeval parcTime_TimevalSubtract(const struct timeval *minuend, const struct timeval *subtrahend);

/**
 * The current time as a `struct timeval`.
 *
 * @return The current time as a `struct timeval`.
 *
 * Example:
 * @code
 * {
 *
 *     struct timeval now = parcTime_NowTimeval();
 * }
 * @endcode
 */
struct timeval parcTime_NowTimeval(void);

/**
 * The current time in microseconds since midnight (0 hour), January 1, 1970 as a `uint64_t`.
 *
 * @return The current time in microseconds since midnight (0 hour), January 1, 1970 as a `uint64_t`.
 *
 * Example:
 * @code
 * {
 *
 *     uint64_t now = parcTime_NowMicroseconds();
 * }
 * @endcode
 */
uint64_t parcTime_NowMicroseconds(void);

/**
 * The current time in nanoseconds since midnight (0 hour), January 1, 1970 as a `uint64_t`.
 *
 * @return The current time in nanoseconds since midnight (0 hour), January 1, 1970 as a `uint64_t`.
 *
 * Example:
 * @code
 * {
 *
 *     uint64_t now = parcTime_NowNanoseconds();
 * }
 * @endcode
 */
uint64_t parcTime_NowNanoseconds(void);
#endif // libparc_parc_Time_h
