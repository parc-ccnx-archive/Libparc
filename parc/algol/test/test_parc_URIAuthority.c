/*
 * Copyright (c) 2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
#include "../parc_URIAuthority.c"
#include <stdint.h>

#include <LongBow/unit-test.h>

#include <parc/algol/parc_URI.h>

#include "_test_parc_URI.h"

#include <parc/algol/parc_SafeMemory.h>
#include <parc/testing/parc_ObjectTesting.h>

LONGBOW_TEST_RUNNER(parcURIAuthority)
{
    LONGBOW_RUN_TEST_FIXTURE(parcURIAuthority);
}

LONGBOW_TEST_RUNNER_SETUP(parcURIAuthority)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_RUNNER_TEARDOWN(parcURIAuthority)
{
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDERR_FILENO);
    if (outstandingAllocations != 0) {
        printf("Tests leak memory by %d allocations\n", outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(parcURIAuthority)
{
    LONGBOW_RUN_TEST_CASE(parcURIAuthority, parcURIAuthority_Parse);

    LONGBOW_RUN_TEST_CASE(parcURIAuthority, parcURIAuthority_Acquire);

    LONGBOW_RUN_TEST_CASE(parcURIAuthority, parcURIAuthority_Equals);

    LONGBOW_RUN_TEST_CASE(parcURIAuthority, parcURIAuthority_GetUserInfo);
    LONGBOW_RUN_TEST_CASE(parcURIAuthority, parcURIAuthority_GetHostName);
    LONGBOW_RUN_TEST_CASE(parcURIAuthority, parcURIAuthority_GetPort);
}

LONGBOW_TEST_FIXTURE_SETUP(parcURIAuthority)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(parcURIAuthority)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(parcURIAuthority, parcURIAuthority_Parse)
{
    char *uriString = URI_SCHEME "://" URI_AUTHORITY "/" URI_PATH_SEGMENT "/" URI_PATH_SEGMENT "?" URI_QUERY "#" URI_FRAGMENT;

    uriString = parcMemory_StringDuplicate(uriString, strlen(uriString));

    PARCURI *uri = parcURI_Parse(uriString);

    PARCURIAuthority *authority = parcURIAuthority_Parse(parcURI_GetAuthority(uri));

    assertEqualStrings(parcURIAuthority_GetUserInfo(authority), URI_AUTHORITY_USERINFO);

    parcURIAuthority_Release(&authority);

    parcMemory_Deallocate((void **) &uriString);
    parcURI_Release(&uri);

    // URI without the port
    uriString = URI_SCHEME "://" URI_AUTHORITY_USERINFO "@" URI_AUTHORITY_HOSTNAME "/" URI_PATH_SEGMENT "/" URI_PATH_SEGMENT "?" URI_QUERY "#" URI_FRAGMENT;
    uriString = parcMemory_StringDuplicate(uriString, strlen(uriString));
    uri = parcURI_Parse(uriString);
    authority = parcURIAuthority_Parse(parcURI_GetAuthority(uri));

    assertEqualStrings(parcURIAuthority_GetUserInfo(authority), URI_AUTHORITY_USERINFO);

    parcURIAuthority_Release(&authority);
    parcURI_Release(&uri);

    // URI with literal V4 address
    uriString = URI_SCHEME "://" URI_AUTHORITY_LITERAL_HOST "/" URI_PATH_SEGMENT "/" URI_PATH_SEGMENT "?" URI_QUERY "#" URI_FRAGMENT;
    uriString = parcMemory_StringDuplicate(uriString, strlen(uriString));
    uri = parcURI_Parse(uriString);
    authority = parcURIAuthority_Parse(parcURI_GetAuthority(uri));

    assertEqualStrings(parcURIAuthority_GetHostName(authority), URI_AUTHORITY_LITERAL_HOSTNAME);

    parcURIAuthority_Release(&authority);
    parcURI_Release(&uri);

    // URI with literal V6 address
    uriString = URI_SCHEME "://" URI_AUTHORITY_LITERAL_HOST6 "/" URI_PATH_SEGMENT "/" URI_PATH_SEGMENT "?" URI_QUERY "#" URI_FRAGMENT;
    uriString = parcMemory_StringDuplicate(uriString, strlen(uriString));
    uri = parcURI_Parse(uriString);
    authority = parcURIAuthority_Parse(parcURI_GetAuthority(uri));

    assertEqualStrings(parcURIAuthority_GetHostName(authority), URI_AUTHORITY_LITERAL_HOSTNAME6);

    parcURIAuthority_Release(&authority);
    parcURI_Release(&uri);

    // URI with full literal V6 address
    uriString = URI_SCHEME "://" URI_AUTHORITY_LITERAL_HOST6_2 "/" URI_PATH_SEGMENT "/" URI_PATH_SEGMENT "?" URI_QUERY "#" URI_FRAGMENT;
    uriString = parcMemory_StringDuplicate(uriString, strlen(uriString));
    uri = parcURI_Parse(uriString);
    authority = parcURIAuthority_Parse(parcURI_GetAuthority(uri));

    assertEqualStrings(parcURIAuthority_GetHostName(authority), URI_AUTHORITY_LITERAL_HOSTNAME6_2);

    parcURIAuthority_Release(&authority);
    parcURI_Release(&uri);

    parcMemory_Deallocate((void **) &uriString);
}

LONGBOW_TEST_CASE(parcURIAuthority, parcURIAuthority_Acquire)
{
    char *uriString = URI_SCHEME "://" URI_AUTHORITY "/" URI_PATH_SEGMENT "/" URI_PATH_SEGMENT "?" URI_QUERY "#" URI_FRAGMENT;

    uriString = parcMemory_StringDuplicate(uriString, strlen(uriString));

    PARCURI *uri = parcURI_Parse(uriString);

    PARCURIAuthority *authority = parcURIAuthority_Parse(parcURI_GetAuthority(uri));
    PARCURIAuthority *handle = parcURIAuthority_Acquire(authority);

    assertTrue(parcURIAuthority_Equals(authority, handle), "URI Authorities should be equal since they refer to the same object.");

    parcURIAuthority_Release(&authority);
    parcURIAuthority_Release(&handle);

    parcMemory_Deallocate((void **) &uriString);
    parcURI_Release(&uri);
}

LONGBOW_TEST_CASE(parcURIAuthority, parcURIAuthority_Equals)
{
    char *uriString1 = URI_SCHEME "://" URI_AUTHORITY;
    uriString1 = parcMemory_StringDuplicate(uriString1, strlen(uriString1));
    PARCURI *uri1 = parcURI_Parse(uriString1);
    PARCURIAuthority *x = parcURIAuthority_Parse(parcURI_GetAuthority(uri1));

    char *uriString2 = URI_SCHEME "://" URI_AUTHORITY;
    uriString2 = parcMemory_StringDuplicate(uriString2, strlen(uriString2));
    PARCURI *uri2 = parcURI_Parse(uriString2);
    PARCURIAuthority *y = parcURIAuthority_Parse(parcURI_GetAuthority(uri2));

    char *uriString3 = URI_SCHEME "://" URI_AUTHORITY;
    uriString3 = parcMemory_StringDuplicate(uriString3, strlen(uriString3));
    PARCURI *uri3 = parcURI_Parse(uriString3);
    PARCURIAuthority *z = parcURIAuthority_Parse(parcURI_GetAuthority(uri3));

    char *differentUriString = URI_SCHEME "://" URI_AUTHORITY_USERINFO;
    differentUriString = parcMemory_StringDuplicate(differentUriString, strlen(differentUriString));
    PARCURI *unequalUri = parcURI_Parse(differentUriString);
    PARCURIAuthority *u = parcURIAuthority_Parse(parcURI_GetAuthority(unequalUri));

    char *uriString5 = URI_SCHEME "://" URI_AUTHORITY_DIFFERENT_PORT;
    uriString5 = parcMemory_StringDuplicate(uriString5, strlen(uriString5));
    PARCURI *unequalUri5 = parcURI_Parse(uriString5);
    PARCURIAuthority *u5 = parcURIAuthority_Parse(parcURI_GetAuthority(unequalUri5));

    char *uriString4 = URI_SCHEME "://" URI_AUTHORITY_DIFFERENT_USER;
    uriString4 = parcMemory_StringDuplicate(uriString4, strlen(uriString4));
    PARCURI *unequalUri4 = parcURI_Parse(uriString4);
    PARCURIAuthority *u4 = parcURIAuthority_Parse(parcURI_GetAuthority(unequalUri4));

    parcObjectTesting_AssertEqualsFunction(parcURIAuthority_Equals, x, y, z, u);

    assertFalse(parcURIAuthority_Equals(x, u4), "Expected URI authorities with different user info to be unequal");
    assertFalse(parcURIAuthority_Equals(x, u5), "Expected URI authorities with different hsot names to be unequal");

    parcURIAuthority_Release(&x);
    parcURIAuthority_Release(&y);
    parcURIAuthority_Release(&z);
    parcURIAuthority_Release(&u);
    parcURIAuthority_Release(&u4);
    parcURIAuthority_Release(&u5);

    parcMemory_Deallocate((void **) &uriString1);
    parcMemory_Deallocate((void **) &uriString2);
    parcMemory_Deallocate((void **) &uriString3);
    parcMemory_Deallocate((void **) &uriString4);
    parcMemory_Deallocate((void **) &uriString5);
    parcMemory_Deallocate((void **) &differentUriString);

    parcURI_Release(&uri1);
    parcURI_Release(&uri2);
    parcURI_Release(&uri3);
    parcURI_Release(&unequalUri);
    parcURI_Release(&unequalUri4);
    parcURI_Release(&unequalUri5);
}

LONGBOW_TEST_CASE(parcURIAuthority, parcURIAuthority_GetUserInfo)
{
    char *uriString = URI_SCHEME "://" URI_AUTHORITY "/" URI_PATH_SEGMENT "/" URI_PATH_SEGMENT "?" URI_QUERY "#" URI_FRAGMENT;
    uriString = parcMemory_StringDuplicate(uriString, strlen(uriString));
    PARCURI *uri = parcURI_Parse(uriString);
    PARCURIAuthority *authority = parcURIAuthority_Parse(parcURI_GetAuthority(uri));

    assertTrue(strcmp(URI_AUTHORITY_USERINFO, parcURIAuthority_GetUserInfo(authority)) == 0, "URI Authority user info should be equal: %s - %s", URI_AUTHORITY_USERINFO, parcURIAuthority_GetUserInfo(authority));

    parcURIAuthority_Release(&authority);

    parcMemory_Deallocate((void **) &uriString);
    parcURI_Release(&uri);
}

LONGBOW_TEST_CASE(parcURIAuthority, parcURIAuthority_GetHostName)
{
    char *uriString = URI_SCHEME "://" URI_AUTHORITY "/" URI_PATH_SEGMENT "/" URI_PATH_SEGMENT "?" URI_QUERY "#" URI_FRAGMENT;
    uriString = parcMemory_StringDuplicate(uriString, strlen(uriString));
    PARCURI *uri = parcURI_Parse(uriString);
    PARCURIAuthority *authority = parcURIAuthority_Parse(parcURI_GetAuthority(uri));

    assertTrue(strcmp(URI_AUTHORITY_HOSTNAME, parcURIAuthority_GetHostName(authority)) == 0, "URI Authority host name should be equal: %s - %s", URI_AUTHORITY_HOSTNAME, parcURIAuthority_GetHostName(authority));

    parcURIAuthority_Release(&authority);

    parcMemory_Deallocate((void **) &uriString);
    parcURI_Release(&uri);
}

LONGBOW_TEST_CASE(parcURIAuthority, parcURIAuthority_GetPort)
{
    char *uriString = URI_SCHEME "://" URI_AUTHORITY "/" URI_PATH_SEGMENT "/" URI_PATH_SEGMENT "?" URI_QUERY "#" URI_FRAGMENT;
    uriString = parcMemory_StringDuplicate(uriString, strlen(uriString));
    PARCURI *uri = parcURI_Parse(uriString);
    PARCURIAuthority *authority = parcURIAuthority_Parse(parcURI_GetAuthority(uri));

    assertTrue(atol(URI_AUTHORITY_PORT_1) == parcURIAuthority_GetPort(authority),
               "URI Authority host name should be equal: %ld - %ld", atol(URI_AUTHORITY_PORT_1), parcURIAuthority_GetPort(authority));

    parcURIAuthority_Release(&authority);

    parcMemory_Deallocate((void **) &uriString);
    parcURI_Release(&uri);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parcURIAuthority);
    int status = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(status);
}
