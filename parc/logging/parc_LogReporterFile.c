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

#include <stdio.h>
#include <inttypes.h>

#include <parc/logging/parc_LogReporterFile.h>
#include <parc/logging/parc_LogFormatSyslog.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_Time.h>

#include <parc/algol/parc_FileOutputStream.h>

PARCLogReporter *
parcLogReporterFile_Create(PARCOutputStream *output)
{
    PARCLogReporter *result = parcLogReporter_Create(&parcLogReporterFile_Acquire,
                                                     parcLogReporterFile_Release,
                                                     parcLogReporterFile_Report,
                                                     parcOutputStream_Acquire(output));
    return result;
}

PARCLogReporter *
parcLogReporterFile_Acquire(const PARCLogReporter *reporter)
{
    return parcObject_Acquire(reporter);
}

void
parcLogReporterFile_Release(PARCLogReporter **reporterP)
{
    parcObject_Release((void **) reporterP);
}

void
parcLogReporterFile_Report(PARCLogReporter *reporter, const PARCLogEntry *entry)
{
    PARCOutputStream *output = parcLogReporter_GetPrivateObject(reporter);

    PARCBuffer *formatted = parcLogFormatSyslog_FormatEntry(entry);
    parcOutputStream_Write(output, formatted);
    parcBuffer_Release(&formatted);
}
