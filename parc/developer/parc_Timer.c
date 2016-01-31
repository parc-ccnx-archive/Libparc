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
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>

#include <sys/time.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_Memory.h>

#include <parc/developer/parc_Timer.h>

struct PARCTimer {
    struct timeval start;
    struct timeval stop;
};

static bool
_parcTimer_Destructor(PARCTimer **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCTimer pointer.");

    /* cleanup the instance fields here */
    return true;
}

parcObject_ImplementAcquire(parcTimer, PARCTimer);

parcObject_ImplementRelease(parcTimer, PARCTimer);

parcObject_Override(PARCTimer, PARCObject,
                    .destructor = (PARCObjectDestructor *) _parcTimer_Destructor,
                    .release  = (PARCObjectRelease *) parcTimer_Release,
                    .copy     = (PARCObjectCopy *) parcTimer_Copy,
                    .toString = (PARCObjectToString *) parcTimer_ToString,
                    .equals   = (PARCObjectEquals *) parcTimer_Equals,
                    .hashCode = (PARCObjectHashCode *) parcTimer_HashCode,
                    .toJSON   = (PARCObjectToJSON *) parcTimer_ToJSON,
                    .display  = (PARCObjectDisplay *) parcTimer_Display);

void
parcTimer_AssertValid(const PARCTimer *instance)
{
    assertTrue(parcTimer_IsValid(instance),
               "PARCTimer is not valid.");
}


PARCTimer *
parcTimer_Create(void)
{
    PARCTimer *result = parcObject_CreateInstance(PARCTimer);

    if (result != NULL) {
        result->start.tv_sec = 0;
        result->start.tv_usec = 0;
        result->stop.tv_sec = 0;
        result->stop.tv_usec = 0;
    }

    return result;

}

PARCTimer *
parcTimer_Copy(const PARCTimer *original)
{
    PARCTimer *result = parcTimer_Create();
    result->start = original->start;
    result->stop = original->stop;

    return result;
}

void
parcTimer_Display(const PARCTimer *instance, int indentation)
{
    parcDisplayIndented_PrintLine(indentation, "PARCTimer@%p {", instance);
    /* Call Display() functions for the fields here. */
    parcDisplayIndented_PrintLine(indentation, "}");
}

bool
parcTimer_Equals(const PARCTimer *x, const PARCTimer *y)
{
    bool result = false;

    if (x == y) {
        result = true;
    } else if (x == NULL || y == NULL) {
        result = false;
    } else {
        if (memcmp(&x->start, &y->start, sizeof(x->start)) == 0) {
            if (memcmp(&x->stop, &y->stop, sizeof(x->stop)) == 0) {
                result = true;
            }
        }
    }

    return result;
}

PARCHashCode
parcTimer_HashCode(const PARCTimer *timer)
{
    PARCHashCode result = parcHashCode_Hash((uint8_t *) timer, sizeof(PARCTimer));
    return result;
}

bool
parcTimer_IsValid(const PARCTimer *instance)
{
    bool result = false;

    if (instance != NULL) {
        result = true;
    }

    return result;
}

PARCJSON *
parcTimer_ToJSON(const PARCTimer *instance)
{
    PARCJSON *result = parcJSON_Create();

    if (result != NULL) {
        PARCJSON *start = parcJSON_Create();
        parcJSON_AddInteger(start, "seconds", instance->start.tv_sec);
        parcJSON_AddInteger(start, "microseconds", instance->start.tv_usec);

        PARCJSON *stop = parcJSON_Create();
        parcJSON_AddInteger(stop, "seconds", instance->stop.tv_sec);
        parcJSON_AddInteger(stop, "microseconds", instance->stop.tv_usec);

        parcJSON_AddObject(result, "start", start);
        parcJSON_AddObject(result, "stop", stop);
        parcJSON_Release(&start);
        parcJSON_Release(&stop);
    }

    return result;
}

char *
parcTimer_ToString(const PARCTimer *instance)
{
    char *result = parcMemory_Format("PARCTimer@%p\n", instance);

    return result;
}

void
parcTimer_Start(PARCTimer *timer)
{
    gettimeofday(&timer->start, NULL);
}

void
parcTimer_Stop(PARCTimer *timer)
{
    gettimeofday(&timer->stop, NULL);
}

uint64_t
parcTimer_ElapsedTime(const PARCTimer *timer)
{
    struct timeval difference;
    timersub(&timer->stop, &timer->start, &difference);

    uint64_t result = difference.tv_sec * 1000000 + difference.tv_usec;
    return result;
}

