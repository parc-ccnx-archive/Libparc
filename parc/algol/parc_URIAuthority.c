/*
 * Copyright (c) 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @author Christopher A. Wood, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>

#include <LongBow/runtime.h>

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdarg.h>

#include <parc/algol/parc_URIAuthority.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_SafeMemory.h>

struct parc_uri_authority {
    char *userinfo;
    char *hostName;
    long port;
};

static void
_parcURIAuthority_Finalize(PARCURIAuthority **authorityPtr)
{
    PARCURIAuthority *authority = *authorityPtr;
    parcMemory_SafeFree(authority->userinfo);
    parcMemory_SafeFree(authority->hostName);
}

parcObject_ExtendPARCObject(PARCURIAuthority, _parcURIAuthority_Finalize, NULL, NULL, parcURIAuthority_Equals, NULL, NULL, NULL);

PARCURIAuthority *
parcURIAuthority_Create(void)
{
    PARCURIAuthority *result = parcObject_CreateInstance(PARCURIAuthority);
    result->userinfo = 0;
    result->hostName = 0;
    result->port = 0;
    return result;
}

parcObject_ImplementAcquire(parcURIAuthority, PARCURIAuthority);

parcObject_ImplementRelease(parcURIAuthority, PARCURIAuthority);

PARCURIAuthority *
parcURIAuthority_Parse(const char *authority)
{
    PARCURIAuthority *result = parcURIAuthority_Create();

    char *atSign = strchr(authority, '@');
    if (atSign != NULL) {
        result->userinfo = parcMemory_StringDuplicate(authority, atSign - authority);
        authority = ++atSign;
    }
    char *colon = strchr(authority, ':');
    if (colon != NULL) {
        result->hostName = parcMemory_StringDuplicate(authority, colon - authority);
        result->port = (short) strtol(++colon, NULL, 10);
    } else {
        result->hostName = parcMemory_StringDuplicate(authority, strlen(authority));
    }

    return result;
}

const char *
parcURIAuthority_GetUserInfo(const PARCURIAuthority *authority)
{
    return authority->userinfo;
}

const char *
parcURIAuthority_GetHostName(const PARCURIAuthority *authority)
{
    return authority->hostName;
}

long
parcURIAuthority_GetPort(const PARCURIAuthority *authority)
{
    return authority->port;
}

bool
parcURIAuthority_Equals(const PARCURIAuthority *authA, const PARCURIAuthority *authB)
{
    if (authA == authB) {
        return true;
    }
    if (authA == NULL || authB == NULL) {
        return false;
    }

    if (strcmp(authA->hostName, authB->hostName) == 0) {
        if (strcmp(authA->userinfo, authB->userinfo) == 0) {
            if (authA->port == authB->port) {
                return true;
            }
            return false;
        }
        return false;
    }

    return false;
}
