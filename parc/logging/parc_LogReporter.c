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

#include <parc/logging/parc_LogReporter.h>
#include <parc/algol/parc_Object.h>

struct PARCLogReporter {
    PARCLogReporter *(*acquire)(const PARCLogReporter *);
    void (*release)(PARCLogReporter **);
    void (*report)(PARCLogReporter *, const PARCLogEntry *);

    PARCObject *privateObject;
};

static void
_parcLogReporter_Destroy(PARCLogReporter **reporterPtr __attribute__((unused)))
{
    PARCLogReporter *result = *reporterPtr;
    if (result->privateObject != NULL) {
        parcObject_Release(&result->privateObject);
    }
}

parcObject_ExtendPARCObject(PARCLogReporter, _parcLogReporter_Destroy, NULL, NULL, NULL, NULL, NULL, NULL);

PARCLogReporter *
parcLogReporter_Create(PARCLogReporter *(*acquire)(const PARCLogReporter *),
                       void (*release)(PARCLogReporter **),
                       void (*report)(PARCLogReporter *, const PARCLogEntry *),
                       void *privateObject)
{
    PARCLogReporter *result = parcObject_CreateInstance(PARCLogReporter);
    result->acquire = acquire;
    result->release = release;
    result->report = report;
    result->privateObject = privateObject;

    return result;
}

parcObject_ImplementAcquire(parcLogReporter, PARCLogReporter);

parcObject_ImplementRelease(parcLogReporter, PARCLogReporter);

void
parcLogReporter_Report(PARCLogReporter *reporter, const PARCLogEntry *report)
{
    reporter->report(reporter, report);
}

void *
parcLogReporter_GetPrivateObject(const PARCLogReporter *reporter)
{
    return reporter->privateObject;
}
