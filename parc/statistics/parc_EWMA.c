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

#include <parc/statistics/parc_EWMA.h>

struct PARCEWMA {
    bool initialized;
    int64_t value;
    double coefficient;
    double coefficient_r;
};

static inline bool
_parcEWMA_FloatEquals(double x, double y, double e)
{
    return fabs(x-y) < e;
}

static bool
_parcEWMA_Destructor(PARCEWMA **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCEWMA pointer.");

    return true;
}

parcObject_ImplementAcquire(parcEWMA, PARCEWMA);

parcObject_ImplementRelease(parcEWMA, PARCEWMA);

parcObject_Override(
    PARCEWMA, PARCObject,
    .destructor = (PARCObjectDestructor *) _parcEWMA_Destructor,
    .copy = (PARCObjectCopy *) parcEWMA_Copy,
    .toString = (PARCObjectToString *)  parcEWMA_ToString,
    .equals = (PARCObjectEquals *)  parcEWMA_Equals,
    .compare = (PARCObjectCompare *)  parcEWMA_Compare,
    .hashCode = (PARCObjectHashCode *)  parcEWMA_HashCode,
    .toJSON = (PARCObjectToJSON *)  parcEWMA_ToJSON);

void
parcEWMA_AssertValid(const PARCEWMA *instance)
{
    assertTrue(parcEWMA_IsValid(instance),
               "PARCEWMA is not valid.");
}

PARCEWMA *
parcEWMA_Create(double coefficient)
{
    PARCEWMA *result = parcObject_CreateInstance(PARCEWMA);
    if (result != NULL) {
        result->initialized = false;
        result->value = 0;
        result->coefficient = coefficient;
        result->coefficient_r = 1.0 - coefficient;
    }

    return result;
}

int
parcEWMA_Compare(const PARCEWMA *instance, const PARCEWMA *other)
{
    int result = 0;

    if (instance == other) {
        result = 0;
    } else if (instance == NULL) {
        result = -1;
    } else if (other == NULL) {
        result = 1;
    } else {
        result = instance->value - other->value;
    }

    return result;
}

PARCEWMA *
parcEWMA_Copy(const PARCEWMA *original)
{
    PARCEWMA *result = parcEWMA_Create(original->coefficient);
    result->initialized = original->initialized;
    result->value = original->value;

    return result;
}

void
parcEWMA_Display(const PARCEWMA *ewma, int indentation)
{
    parcDisplayIndented_PrintLine(indentation,
                                  "PARCEWMA@%p { .initialized=%s .coefficient=%lf, .value=%" PRId64 " }",
                                  ewma,
                                  ewma->initialized ? "true" : "false",
                                  ewma->coefficient,
                                  ewma->value);
}

bool
parcEWMA_Equals(const PARCEWMA *x, const PARCEWMA *y)
{
    bool result = false;

    if (x == y) {
        result = true;
    } else if (x == NULL || y == NULL) {
        result = false;
    } else {
        if (x->initialized == y->initialized) {
            if (_parcEWMA_FloatEquals(x->coefficient, y->coefficient, 0.00001)) {
                if (_parcEWMA_FloatEquals(x->value, y->value, 0.00001)) {
                    result = true;
                }
            }
        }
    }

    return result;
}

PARCHashCode
parcEWMA_HashCode(const PARCEWMA *instance)
{
    PARCHashCode result = 0;

    return result;
}

bool
parcEWMA_IsValid(const PARCEWMA *instance)
{
    bool result = false;

    if (instance != NULL) {
        result = true;
    }

    return result;
}

PARCJSON *
parcEWMA_ToJSON(const PARCEWMA *instance)
{
    PARCJSON *result = parcJSON_Create();

    if (result != NULL) {
    }

    return result;
}

char *
parcEWMA_ToString(const PARCEWMA *ewma)
{
    char *result = parcMemory_Format("PARCEWMA@%p { .initialized=%s .coefficient=%lf, .value=%" PRId64 " }",
                                     ewma,
                                     ewma->initialized ? "true" : "false",
                                     ewma->coefficient,
                                     ewma->value);
    return result;
}

int64_t
parcEWMA_Update(PARCEWMA *ewma, const int64_t value)
{
    if (ewma->initialized) {
        // E_t = a * V + (1 - a) * E_(t-1)
        double x = (ewma->coefficient * value);
        double y = (ewma->coefficient_r * ewma->value);

        ewma->value = x + y;
    } else {
        ewma->value = value;
        ewma->initialized = true;
    }
    return ewma->value;
}

int64_t
parcEWMA_GetValue(const PARCEWMA *ewma)
{
    return ewma->value;
}
