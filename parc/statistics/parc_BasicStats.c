/*
 * Copyright (c) 2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
 * @author Glenn Scott, Palo Alto Research Center (PARC)
 * @copyright (c) 2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include <config.h>

#include <math.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_Memory.h>

#include <parc/statistics/parc_BasicStats.h>

struct PARCBasicStats {
    int64_t count;
    double maximum;
    double minimum;
    double mean;
    double variance;
};

static inline bool
_parcBasicStats_FloatEquals(double x, double y, double e)
{
    return fabs(x-y) < e;
}

static bool
_parcBasicStats_Destructor(PARCBasicStats **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCBasicStats pointer.");

    return true;
}

parcObject_ImplementAcquire(parcBasicStats, PARCBasicStats);

parcObject_ImplementRelease(parcBasicStats, PARCBasicStats);

parcObject_Override(
    PARCBasicStats, PARCObject,
    .destructor = (PARCObjectDestructor *) _parcBasicStats_Destructor,
    .copy = (PARCObjectCopy *) parcBasicStats_Copy,
    .toString = (PARCObjectToString *)  parcBasicStats_ToString,
    .equals = (PARCObjectEquals *)  parcBasicStats_Equals,
    .compare = (PARCObjectCompare *)  parcBasicStats_Compare,
    .hashCode = (PARCObjectHashCode *)  parcBasicStats_HashCode,
    .toJSON = (PARCObjectToJSON *)  parcBasicStats_ToJSON);


void
parcBasicStats_AssertValid(const PARCBasicStats *instance)
{
    assertTrue(parcBasicStats_IsValid(instance),
               "PARCBasicStats is not valid.");
}


PARCBasicStats *
parcBasicStats_Create(void)
{
    PARCBasicStats *result = parcObject_CreateInstance(PARCBasicStats);

    if (result != NULL) {
        result->count = 0;
        result->mean = 0;
        result->variance = 0;
        result->maximum = 0;
        result->minimum = 0;
    }

    return result;
}

int
parcBasicStats_Compare(const PARCBasicStats *instance, const PARCBasicStats *other)
{
    int result = 0;

    return result;
}

PARCBasicStats *
parcBasicStats_Copy(const PARCBasicStats *original)
{
    PARCBasicStats *result = parcBasicStats_Create();
    result->count = original->count;
    result->mean = original->mean;
    result->variance = original->variance;
    result->maximum = original->maximum;
    result->minimum = original->minimum;

    return result;
}

void
parcBasicStats_Display(const PARCBasicStats *stats, int indentation)
{
    parcDisplayIndented_PrintLine(indentation,
                                  "PARCBasicStats@%p { .count=%" PRId64 " .minimum=%llf .maximum=%llf .mean=%llf }",
                                  stats, stats->count, stats->minimum, stats->maximum, stats->mean);
}

bool
parcBasicStats_Equals(const PARCBasicStats *x, const PARCBasicStats *y)
{
    bool result = false;
   
    if (x == y) {
        result = true;
    } else if (x == NULL || y == NULL) {
        result = false;
    } else {
        if (x->count == y->count) {
            if (_parcBasicStats_FloatEquals(x->maximum, y->maximum, 0.00001)) {
                if (_parcBasicStats_FloatEquals(x->minimum, y->minimum, 0.00001)) {
                    if (_parcBasicStats_FloatEquals(x->mean, y->mean, 0.00001)) {
                        result = true;
                    }
                }
            }
        }
    }
   
    return result;
}

PARCHashCode
parcBasicStats_HashCode(const PARCBasicStats *instance)
{
    PARCHashCode result = 0;

    return result;
}

bool
parcBasicStats_IsValid(const PARCBasicStats *stats)
{
    bool result = false;

    if (stats != NULL) {
        result = true;
    }

    return result;
}

PARCJSON *
parcBasicStats_ToJSON(const PARCBasicStats *stats)
{
    PARCJSON *result = parcJSON_Create();

    if (result != NULL) {
        PARCJSONPair *pair = parcJSONPair_CreateFromDouble("maximum", stats->maximum);
        parcJSON_AddPair(result, pair);
        parcJSONPair_Release(&pair);
       
        pair = parcJSONPair_CreateFromDouble("minimum", stats->minimum);
        parcJSON_AddPair(result, pair);
        parcJSONPair_Release(&pair);

        pair = parcJSONPair_CreateFromDouble("mean", stats->mean);
        parcJSON_AddPair(result, pair);
        parcJSONPair_Release(&pair);
       
        pair = parcJSONPair_CreateFromDouble("variance", stats->variance);
        parcJSON_AddPair(result, pair);
        parcJSONPair_Release(&pair);

        pair = parcJSONPair_CreateFromInteger("count", stats->count);
        parcJSON_AddPair(result, pair);
        parcJSONPair_Release(&pair);
    }

    return result;
}

char *
parcBasicStats_ToString(const PARCBasicStats *stats)
{
    char *result = parcMemory_Format("PARCBasicStats@%p { .count=%" PRId64 " .minimum=%llf .maximum=%llf .mean=%llf }",
                                     stats, stats->count, stats->minimum, stats->maximum, stats->mean);

    return result;
}

void
parcBasicStats_Update(PARCBasicStats *stats, double value)
{
    stats->count++;

    if (value > stats->maximum) {
        stats->maximum = value;
    }

    if (value < stats->minimum) {
        stats->minimum = value;
    }

    double mean_ = stats->mean;

    double xMinusOldMean = value - mean_;

    stats->mean = mean_ + xMinusOldMean / stats->count;

    double xMinusCurrentMean = value - stats->mean;

    stats->variance = ((stats->variance * (stats->count - 1)) + xMinusOldMean * xMinusCurrentMean) / stats->count;
}

double
parcBasicStats_Mean(const PARCBasicStats *stats)
{
    return stats->mean;
}

double
parcBasicStats_Variance(const PARCBasicStats *stats)
{
    return stats->variance;
}

double
parcBasicStats_StandardDeviation(const PARCBasicStats *stats)
{
    return sqrt(stats->variance);
}

double
parcBasicStats_Maximum(const PARCBasicStats *stats)
{
    return stats->maximum;
}

double
parcBasicStats_Minimum(const PARCBasicStats *stats)
{
    return stats->minimum;
}

double
parcBasicStats_Range(const PARCBasicStats *stats)
{
    return stats->maximum - stats->minimum;
}
