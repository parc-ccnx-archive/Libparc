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
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2015, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include <LongBow/runtime.h>

#include <parc/algol/parc_Time.h>
#include <parc/algol/parc_Memory.h>

char *
parcTime_TimevalAsString(struct timeval timeval)
{
    char *string;
    int nwritten = asprintf(&string, "%ld.%06ld", timeval.tv_sec, (long) timeval.tv_usec);
    assertTrue(nwritten >= 0, "Error calling asprintf");

    char *result = parcMemory_StringDuplicate(string, strlen(string));
    free(string);
    return result;
}

char *
parcTime_TimevalAsRFC3339(const struct timeval *utcTime, char result[64])
{
    char tmbuf[64];
    struct tm theTime;

    struct tm *nowtm = gmtime_r(&utcTime->tv_sec, &theTime);
    strftime(tmbuf, sizeof tmbuf, "%Y-%m-%dT%H:%M:%S", nowtm);
    snprintf(result, 64, "%s.%06ldZ", tmbuf, (long) utcTime->tv_usec);
    return result;
}

char *
parcTime_TimevalAsISO8601(const struct timeval *utcTime, char result[64])
{
    char tmbuf[64];
    struct tm theTime;

    struct tm *nowtm = gmtime_r(&utcTime->tv_sec, &theTime);
    strftime(tmbuf, sizeof tmbuf, "%Y-%m-%d %H:%M:%S", nowtm);
    snprintf(result, 64, "%s.%06ldZ", tmbuf, (long) utcTime->tv_usec);
    return result;
}

char *
parcTime_TimeAsRFC3339(const time_t utcTime, char result[64])
{
    struct timeval theTime = { utcTime, 0 };

    return parcTime_TimevalAsRFC3339(&theTime, result);
}

char *
parcTime_NowAsRFC3339(char result[64])
{
    struct timeval theTime;
    gettimeofday(&theTime, NULL);

    return parcTime_TimevalAsRFC3339(&theTime, result);
}

char *
parcTime_TimeAsISO8601(const time_t utcTime, char result[64])
{
    struct timeval theTime = { utcTime, 0 };

    return parcTime_TimevalAsISO8601(&theTime, result);
}

char *
parcTime_NowAsISO8601(char result[64])
{
    struct timeval theTime;
    gettimeofday(&theTime, NULL);

    return parcTime_TimevalAsISO8601(&theTime, result);
}

struct timeval
parcTime_TimevalAdd(const struct timeval *addend1, const struct timeval *addend2)
{
    struct timeval sum;

    sum.tv_sec = addend1->tv_sec + addend2->tv_sec;
    sum.tv_usec = addend1->tv_usec + addend2->tv_usec;
    if (sum.tv_usec >= 1000000) {
        sum.tv_usec -= 1000000;
        sum.tv_sec++;
    }
    return sum;
}

struct timeval
parcTime_TimevalSubtract(const struct timeval *minuend, const struct timeval *subtrahend)
{
    struct timeval result;

    result.tv_sec = minuend->tv_sec - subtrahend->tv_sec;
    result.tv_usec = minuend->tv_usec - subtrahend->tv_usec;
    if (result.tv_usec < 0) {
        result.tv_sec--;
        result.tv_usec += 1000000;
    }
    return result;
}

struct timeval
parcTime_NowTimeval(void)
{
    struct timeval timeval;
    gettimeofday(&timeval, NULL);
    return timeval;
}

uint64_t
parcTime_NowMicroseconds(void)
{
    struct timeval timeval;
    gettimeofday(&timeval, NULL);

    uint64_t result = timeval.tv_sec * 1000000 + timeval.tv_usec;
    return result;
}

uint64_t
parcTime_NowNanoseconds(void)
{
    struct timeval timeval;
    gettimeofday(&timeval, NULL);

    uint64_t result = timeval.tv_sec * 1000000000 + timeval.tv_usec * 1000;
    return result;
}
