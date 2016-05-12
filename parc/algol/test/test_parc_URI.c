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
/*
 *
 */
#include "../parc_URI.c"
#include <LongBow/unit-test.h>

#include <stdint.h>

#include <parc/algol/parc_Hash.h>
#include <parc/algol/parc_SafeMemory.h>
#include <parc/testing/parc_ObjectTesting.h>

#include <parc/algol/parc_Hash.h>

#include "_test_parc_URI.h"

LONGBOW_TEST_RUNNER(parcURI)
{
    LONGBOW_RUN_TEST_FIXTURE(parcURI);
}

LONGBOW_TEST_RUNNER_SETUP(parcURI)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_RUNNER_TEARDOWN(parcURI)
{
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDERR_FILENO);
    if (outstandingAllocations != 0) {
        printf("Tests leak memory by %d allocations\n", outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(parcURI)
{
    LONGBOW_RUN_TEST_CASE(parcURI, parseScheme);
    LONGBOW_RUN_TEST_CASE(parcURI, parseScheme_Only);
    LONGBOW_RUN_TEST_CASE(parcURI, parseScheme_BadScheme);
    LONGBOW_RUN_TEST_CASE(parcURI, parseScheme_EmptyScheme);

    LONGBOW_RUN_TEST_CASE(parcURI, parseAuthority);
    LONGBOW_RUN_TEST_CASE(parcURI, parseAuthority_NoAuthority);
    LONGBOW_RUN_TEST_CASE(parcURI, parseAuthority_NoPath);

    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_Acquire);

    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_AuthorityEquals_SamePointer);
    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_AuthorityEquals_NullPointers);

    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_QueryEquals_SamePointer);
    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_QueryEquals_NullPointers);

    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_FragmentEquals_SamePointer);
    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_FragmentEquals_NullPointers);

    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_SchemeEquals_SamePointer);
    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_SchemeEquals_NullPointers);

    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_Parse);
    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_Parse_NoScheme);
    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_Parse_SchemeOnly);

    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_SetScheme);
    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_SetScheme_Reset);
    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_SetScheme_Resetting);

    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_SetFragment);
    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_SetFragment_Reset);
    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_SetFragment_Resetting);

    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_SetQuery);
    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_SetQuery_Reset);
    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_SetQuery_Resetting);

    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_SetAuthority);
    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_SetAuthority_Reset);
    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_SetAuthority_Resetting);

    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_Equals_Contract);

    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_GetPath);
    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_GetQuery);
    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_GetFragment);
    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_Copy);
    LONGBOW_RUN_TEST_CASE(parcURI, parcURI_ToString_Full);

    LONGBOW_RUN_TEST_CASE(parcURI, PARCURI_ToString_SchemeOnly);
    LONGBOW_RUN_TEST_CASE(parcURI, PARCURI_ToString_NoAuthority);
}

LONGBOW_TEST_FIXTURE_SETUP(parcURI)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(parcURI)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(parcURI, parcURI_Acquire)
{
    char *uriString = URI_SCHEME "://" URI_AUTHORITY "/" URI_PATH_SEGMENT "/" URI_PATH_SEGMENT "?" URI_QUERY "#" URI_FRAGMENT;

    uriString = parcMemory_StringDuplicate(uriString, strlen(uriString));

    PARCURI *uri = parcURI_Parse(uriString);

    PARCURI *handle = parcURI_Acquire(uri);

    assertTrue(parcURI_Equals(uri, handle), "Expected URI and acquired handle to be equal");

    parcURI_Release(&handle);
    parcURI_Release(&uri);
    parcMemory_Deallocate((void **) &uriString);
}

LONGBOW_TEST_CASE(parcURI, parcURI_AuthorityEquals_SamePointer)
{
    char *authority = "authority@auth";
    assertTrue(_parcURI_AuthorityEquals(authority, authority),
               "Expected authorities to be equal since they're the same pointers: %p - %p",
               (void *)authority, (void *)authority);
}

LONGBOW_TEST_CASE(parcURI, parcURI_AuthorityEquals_NullPointers)
{
    char *authority = "authority@auth";
    assertFalse(_parcURI_AuthorityEquals(NULL, authority), "Expected authorities to not be equal since one is NULL");
    assertFalse(_parcURI_AuthorityEquals(authority, NULL), "Expected authorities to not be equal since one is NULL");
    assertTrue(_parcURI_AuthorityEquals(NULL, NULL), "Expected authorities to be equal since both are NULL");
}

LONGBOW_TEST_CASE(parcURI, parcURI_QueryEquals_SamePointer)
{
    char *query = "query";
    assertTrue(_parcURI_QueryEquals(query, query),
               "Expected queries to be equal since they're the same pointers: %p - %p",
               (void *)query, (void *)query);
}

LONGBOW_TEST_CASE(parcURI, parcURI_QueryEquals_NullPointers)
{
    char *query = "query";
    assertFalse(_parcURI_QueryEquals(NULL, query), "Expected queries to not be equal since one is NULL");
    assertFalse(_parcURI_QueryEquals(query, NULL), "Expected queries to not be equal since one is NULL");
    assertTrue(_parcURI_QueryEquals(NULL, NULL), "Expected queries to be equal since both are NULL");
}

LONGBOW_TEST_CASE(parcURI, parcURI_FragmentEquals_SamePointer)
{
    char *fragment = "fragment";
    assertTrue(_parcURI_FragmentEquals(fragment, fragment),
               "Expected fragments to be equal since they're the same pointers: %p - %p",
               (void *)fragment, (void *)fragment);
}

LONGBOW_TEST_CASE(parcURI, parcURI_FragmentEquals_NullPointers)
{
    char *fragment = "fragment";
    assertFalse(_parcURI_FragmentEquals(NULL, fragment), "Expected fragments to not be equal since one is NULL");
    assertFalse(_parcURI_FragmentEquals(fragment, NULL), "Expected fragments to not be equal since one is NULL");
    assertTrue(_parcURI_FragmentEquals(NULL, NULL), "Expected fragments to be equal since both are NULL");
}

LONGBOW_TEST_CASE(parcURI, parcURI_SchemeEquals_SamePointer)
{
    char *scheme = "scheme";
    assertTrue(_parcURI_SchemeEquals(scheme, scheme),
               "Expected schemes to be equal since they're the same pointers: %p - %p",
               (void *)scheme, (void *)scheme);
}

LONGBOW_TEST_CASE(parcURI, parcURI_SchemeEquals_NullPointers)
{
    char *scheme = "scheme";
    assertFalse(_parcURI_SchemeEquals(NULL, scheme), "Expected schemes to not be equal since one is NULL");
    assertFalse(_parcURI_SchemeEquals(scheme, NULL), "Expected schemes to not be equal since one is NULL");
    assertTrue(_parcURI_SchemeEquals(NULL, NULL), "Expected schemes to be equal since both are NULL");
}

LONGBOW_TEST_CASE(parcURI, parseScheme_EmptyScheme)
{
    const char *pointer;
    char *actual = _parseScheme(":", &pointer); // empty string
    assertNull(actual, "Parsed scheme should be NULL since the input string was empty.");
    actual = _parseScheme("", &pointer); // empty string
    assertNull(actual, "Parsed scheme should be NULL since the input string was empty.");
}

LONGBOW_TEST_CASE(parcURI, parcURI_Parse)
{
    char *uriString = URI_SCHEME "://" URI_AUTHORITY "/" URI_PATH_SEGMENT "/" URI_PATH_SEGMENT "?" URI_QUERY "#" URI_FRAGMENT;

    uriString = parcMemory_StringDuplicate(uriString, strlen(uriString));

    PARCURI *uri = parcURI_Parse(uriString);
    assertNotNull(uri, "Expected non-null result for '%s'", uriString);

    memset(uriString, 0, strlen(uriString));

    assertTrue(strcmp(URI_SCHEME, parcURI_GetScheme(uri)) == 0,
               "Expected '%s', actual '%s'", URI_SCHEME, parcURI_GetScheme(uri));
    assertTrue(strcmp(URI_AUTHORITY, parcURI_GetAuthority(uri)) == 0,
               "Expected '%s', actual '%s'", URI_AUTHORITY, parcURI_GetAuthority(uri));
    assertTrue(strcmp(URI_QUERY, parcURI_GetQuery(uri)) == 0,
               "Expected '%s', actual '%s'", URI_QUERY, parcURI_GetQuery(uri));
    assertTrue(strcmp(URI_FRAGMENT, parcURI_GetFragment(uri)) == 0,
               "Expected '%s', actual '%s'", URI_FRAGMENT, parcURI_GetFragment(uri));

    parcMemory_Deallocate((void **) &uriString);

    parcURI_Release(&uri);
    assertNull(uri, "Expected parcURI_Release to null the pointer.");
}

LONGBOW_TEST_CASE(parcURI, parcURI_Parse_NoScheme)
{
    char *uriString = "/" URI_PATH_SEGMENT;

    uriString = parcMemory_StringDuplicate(uriString, strlen(uriString));

    PARCURI *uri = parcURI_Parse(uriString);
    assertNull(uri,
               "Expected null result for '%s'", uriString);
    parcMemory_Deallocate((void **) &uriString);
}

LONGBOW_TEST_CASE(parcURI, parcURI_Parse_SchemeOnly)
{
    char *uriString = URI_SCHEME ":";

    uriString = parcMemory_StringDuplicate(uriString, strlen(uriString));

    PARCURI *uri = parcURI_Parse(uriString);
    assertNotNull(uri,
                  "Expected non-null result for '%s'", uriString);

    memset(uriString, 0, strlen(uriString));

    assertTrue(strcmp(URI_SCHEME, parcURI_GetScheme(uri)) == 0,
               "Expected '%s', actual '%s'", URI_SCHEME, parcURI_GetScheme(uri));
    assertNull(parcURI_GetAuthority(uri),
               "Expected NULL, actual '%s'", parcURI_GetAuthority(uri));
    assertNull(parcURI_GetQuery(uri),
               "Expected NULL, actual '%s'", parcURI_GetQuery(uri));
    assertNull(parcURI_GetFragment(uri),
               "Expected NULL, actual '%s'", parcURI_GetFragment(uri));

    parcMemory_Deallocate((void **) &uriString);
    parcURI_Release(&uri);
    assertNull(uri,
               "Expected parcURI_Release to null the pointer.");
}

LONGBOW_TEST_CASE(parcURI, parseScheme)
{
    const char *pointer;
    char *actual = _parseScheme(URI_FULL, &pointer);
    assertTrue(strcmp(URI_SCHEME, actual) == 0,
               "Expected '%s' actual '%s'", URI_SCHEME, actual);
    parcMemory_Deallocate((void **) &actual);
}

LONGBOW_TEST_CASE(parcURI, parseScheme_Only)
{
    const char *pointer;
    char *actual = _parseScheme(URI_SCHEME ":", &pointer);
    assertTrue(strcmp(URI_SCHEME, actual) == 0,
               "Expected '%s' actual '%s'", URI_SCHEME, actual);
    parcMemory_Deallocate((void **) &actual);
}

LONGBOW_TEST_CASE(parcURI, parseScheme_BadScheme)
{
    const char *pointer;
    char *actual = _parseScheme("", &pointer);
    assertNull(actual,
               "Expected NULL actual '%s'", actual);
}

LONGBOW_TEST_CASE(parcURI, PARCURI_GetAuthority)
{
    PARCURI *uri = parcURI_Parse(URI_SCHEME "://" URI_AUTHORITY "/" URI_PATH_SEGMENT "/" URI_PATH_SEGMENT "?" URI_QUERY "#" URI_FRAGMENT);
    const char *actual = parcURI_GetAuthority(uri);
    assertTrue(strcmp(TEST_URI_AUTHORITY, actual) == 0,
               "Expected '%s' actual '%s'", URI_AUTHORITY, actual);
    parcURI_Release(&uri);
}

LONGBOW_TEST_CASE(parcURI, parcURI_GetQuery)
{
    PARCURI *uri = parcURI_Parse(URI_SCHEME "://" URI_AUTHORITY "/" URI_PATH_SEGMENT "/" URI_PATH_SEGMENT "?" URI_QUERY "#" URI_FRAGMENT);
    const char *actual = parcURI_GetQuery(uri);
    assertTrue(strcmp(URI_QUERY, actual) == 0,
               "Expected '%s' actual '%s'", URI_FRAGMENT, actual);

    parcURI_Release(&uri);
}

LONGBOW_TEST_CASE(parcURI, parcURI_GetFragment)
{
    PARCURI *uri = parcURI_Parse(URI_SCHEME "://" URI_AUTHORITY "/" URI_PATH_SEGMENT "/" URI_PATH_SEGMENT "?" URI_QUERY "#" URI_FRAGMENT);
    const char *actual = parcURI_GetFragment(uri);
    assertTrue(strcmp(URI_FRAGMENT, actual) == 0,
               "Expected '%s' actual '%s'", URI_FRAGMENT, actual);
    parcURI_Release(&uri);
}

LONGBOW_TEST_CASE(parcURI, parcURI_GetPath)
{
    char *uriString = URI_SCHEME "://" URI_AUTHORITY "/" URI_PATH_SEGMENT "/" URI_PATH_SEGMENT "?" URI_QUERY "#" URI_FRAGMENT;
    PARCURI *uri = parcURI_Parse(uriString);
    PARCURIPath *actual = parcURI_GetPath(uri);

    char *string = parcURIPath_ToString(actual);

    char *expected = URI_PATH_SEGMENT "/" URI_PATH_SEGMENT;
    assertTrue(strcmp(expected, string) == 0, "Expected '%s' actual '%s'", expected, string);

    parcMemory_Deallocate((void **) &string);
    parcURI_Release(&uri);
}

LONGBOW_TEST_CASE(parcURI, parcURI_Copy)
{
    PARCURI *uri = parcURI_Parse(URI_SCHEME "://" URI_AUTHORITY "/" URI_PATH_SEGMENT "/" URI_PATH_SEGMENT "?" URI_QUERY "#" URI_FRAGMENT);
    PARCURI *uri2 = parcURI_Copy(uri);

    char *expected = parcURI_ToString(uri);
    char *actual = parcURI_ToString(uri2);

    assertTrue(strcmp(expected, actual) == 0,
               "Expected '%s' actual '%s'", expected, actual);
    parcMemory_Deallocate((void **) &expected);
    parcMemory_Deallocate((void **) &actual);
    parcURI_Release(&uri);
    parcURI_Release(&uri2);
}

LONGBOW_TEST_CASE(parcURI, parcURI_Equals_Contract)
{
    PARCURI *x = parcURI_Parse(URI_SCHEME "://" URI_AUTHORITY "/" URI_PATH_SEGMENT "?" URI_QUERY "#" URI_FRAGMENT);
    PARCURI *y = parcURI_Parse(URI_SCHEME "://" URI_AUTHORITY "/" URI_PATH_SEGMENT "?" URI_QUERY "#" URI_FRAGMENT);
    PARCURI *z = parcURI_Parse(URI_SCHEME "://" URI_AUTHORITY "/" URI_PATH_SEGMENT "?" URI_QUERY "#" URI_FRAGMENT);
    PARCURI *u = parcURI_Parse(URI_SCHEME "://" URI_AUTHORITY "/" URI_PATH_SEGMENT "?" URI_QUERY);

    parcObjectTesting_AssertEqualsFunction(parcURI_Equals, x, y, z, u);

    parcURI_Release(&x);
    parcURI_Release(&y);
    parcURI_Release(&z);
    parcURI_Release(&u);
}

LONGBOW_TEST_CASE(parcURI, PARCURI_GetScheme)
{
    PARCURI *uri = parcURI_Parse(URI_SCHEME "://" URI_AUTHORITY "/" URI_PATH_SEGMENT "/" URI_PATH_SEGMENT "?" URI_QUERY "#" URI_FRAGMENT);
    const char *actual = parcURI_GetScheme(uri);
    assertTrue(strcmp(TEST_URI_SCHEME, actual) == 0,
               "Expected '%s' actual '%s'", TEST_URI_SCHEME, actual);
    parcURI_Release(&uri);
}

LONGBOW_TEST_CASE(parcURI, PARCURI_Parse)
{
    PARCURI *uri = parcURI_Parse(URI_SCHEME "://" URI_AUTHORITY "/" URI_PATH_SEGMENT "/" URI_PATH_SEGMENT "?" URI_QUERY "#" URI_FRAGMENT);
    assertNotNull(uri,
                  "Expected a non-null result.");

    assertTrue(strcmp(URI_SCHEME, parcURI_GetScheme(uri)) == 0,
               "Expected '%s', actual '%s'", URI_SCHEME, parcURI_GetScheme(uri));
    assertTrue(strcmp(URI_AUTHORITY, parcURI_GetAuthority(uri)) == 0,
               "Expected '%s', actual '%s'", URI_AUTHORITY, parcURI_GetAuthority(uri));
    assertTrue(strcmp(URI_QUERY, parcURI_GetQuery(uri)) == 0,
               "Expected '%s', actual '%s'", URI_QUERY, parcURI_GetQuery(uri));
    assertTrue(strcmp(URI_FRAGMENT, parcURI_GetFragment(uri)) == 0,
               "Expected '%s', actual '%s'", URI_FRAGMENT, parcURI_GetFragment(uri));

    parcURI_Release(&uri);
}

LONGBOW_TEST_CASE(parcURI, parcURI_ToString_Full)
{
    char *expected = URI_SCHEME "://" URI_AUTHORITY "/" URI_PATH_SEGMENT "/" URI_PATH_SEGMENT "?" URI_QUERY "#" URI_FRAGMENT;

    PARCURI *uri = parcURI_Parse(expected);
    char *actual = parcURI_ToString(uri);

    assertTrue(strcmp(expected, actual) == 0,
               "Expected '%s' actual '%s'", expected, actual);
    parcMemory_Deallocate((void **) &actual);
    parcURI_Release(&uri);
}

LONGBOW_TEST_CASE(parcURI, PARCURI_ToString_SchemeOnly)
{
    char *expected = URI_SCHEME ":" "/";
    PARCURI *uri = parcURI_Parse(expected);
    char *actual = parcURI_ToString(uri);

    assertTrue(strcmp(expected, actual) == 0,
               "Expected '%s' actual '%s'", expected, actual);
    parcURI_Release(&uri);
    parcMemory_Deallocate((void **) &actual);
}

LONGBOW_TEST_CASE(parcURI, PARCURI_ToString_NoAuthority)
{
    char *expected = URI_SCHEME ":" "/" URI_PATH_SEGMENT "/" URI_PATH_SEGMENT "?" URI_QUERY "#" URI_FRAGMENT;
    PARCURI *uri = parcURI_Parse(expected);
    char *actual = parcURI_ToString(uri);

    assertTrue(strcmp(expected, actual) == 0,
               "Expected '%s' actual '%s'", expected, actual);

    parcURI_Release(&uri);
    parcMemory_Deallocate((void **) &actual);
}

LONGBOW_TEST_CASE(parcURI, parseAuthority)
{
    char *authority = "//" URI_AUTHORITY "/" URI_PATH_SEGMENT "/" URI_PATH_SEGMENT;
    char *expected = URI_AUTHORITY;

    const char *pointer;
    char *actual = _parseAuthority(authority, &pointer);
    assertTrue(strcmp(expected, actual) == 0,
               "Expected '%s' actual '%s'", expected, actual);
    assertTrue(*pointer == '/',
               "Expected '/' actual '%c'", *pointer);
    parcMemory_Deallocate((void **) &actual);
}

LONGBOW_TEST_CASE(parcURI, parseAuthority_NoAuthority)
{
    char *string = "/" URI_PATH_SEGMENT "/" URI_PATH_SEGMENT;

    const char *pointer;
    char *actual = _parseAuthority(string, &pointer);
    assertTrue(actual == NULL,
               "Expected NULL actual '%s'", actual);
    assertTrue(*pointer == '/',
               "Expected '/' actual '%c'", *pointer);
}

LONGBOW_TEST_CASE(parcURI, parcURI_SetScheme)
{
    char *scheme = "scheme";
    PARCURI *uri = parcURI_Create();
    _parcURI_SetScheme(uri, scheme);

    assertTrue(strcmp(scheme, parcURI_GetScheme(uri)) == 0,
               "Expected %s actual %s", scheme, parcURI_GetScheme(uri));
    parcURI_Release(&uri);
}

LONGBOW_TEST_CASE(parcURI, parcURI_SetScheme_Resetting)
{
    char *scheme = "scheme";
    PARCURI *uri = parcURI_Create();
    _parcURI_SetScheme(uri, scheme);
    _parcURI_SetScheme(uri, scheme);

    assertTrue(strcmp(scheme, parcURI_GetScheme(uri)) == 0,
               "Expected %s actual %s", scheme, parcURI_GetScheme(uri));
    parcURI_Release(&uri);
}

LONGBOW_TEST_CASE(parcURI, parcURI_SetScheme_Reset)
{
    char *scheme = "scheme";
    PARCURI *uri = parcURI_Create();
    _parcURI_SetScheme(uri, scheme);
    _parcURI_SetScheme(uri, NULL);

    assertNull(parcURI_GetScheme(uri),
               "Expected NULL actual %s", parcURI_GetScheme(uri));
    parcURI_Release(&uri);
}

LONGBOW_TEST_CASE(parcURI, parcURI_SetFragment)
{
    char *fragment = "fragment";
    PARCURI *uri = parcURI_Create();
    _parcURI_SetFragment(uri, fragment);

    assertTrue(strcmp(fragment, parcURI_GetFragment(uri)) == 0,
               "Expected %s actual %s", fragment, parcURI_GetFragment(uri));
    parcURI_Release(&uri);
}

LONGBOW_TEST_CASE(parcURI, parcURI_SetFragment_Resetting)
{
    char *fragment = "fragment";
    PARCURI *uri = parcURI_Create();
    _parcURI_SetFragment(uri, fragment);
    _parcURI_SetFragment(uri, fragment);

    assertTrue(strcmp(fragment, parcURI_GetFragment(uri)) == 0,
               "Expected %s actual %s", fragment, parcURI_GetFragment(uri));
    parcURI_Release(&uri);
}

LONGBOW_TEST_CASE(parcURI, parcURI_SetFragment_Reset)
{
    char *fragment = "query";
    PARCURI *uri = parcURI_Create();
    _parcURI_SetFragment(uri, fragment);
    _parcURI_SetFragment(uri, NULL);

    assertNull(parcURI_GetFragment(uri),
               "Expected NULL actual %s", parcURI_GetFragment(uri));
    parcURI_Release(&uri);
}

LONGBOW_TEST_CASE(parcURI, parcURI_SetQuery)
{
    char *query = "query";
    PARCURI *uri = parcURI_Create();
    _parcURI_SetQuery(uri, query);

    assertTrue(strcmp(query, parcURI_GetQuery(uri)) == 0,
               "Expected %s actual %s", query, parcURI_GetQuery(uri));
    parcURI_Release(&uri);
}

LONGBOW_TEST_CASE(parcURI, parcURI_SetQuery_Resetting)
{
    char *query = "query";
    PARCURI *uri = parcURI_Create();
    _parcURI_SetQuery(uri, query);
    _parcURI_SetQuery(uri, query);

    assertTrue(strcmp(query, parcURI_GetQuery(uri)) == 0,
               "Expected %s actual %s", query, parcURI_GetQuery(uri));
    parcURI_Release(&uri);
}

LONGBOW_TEST_CASE(parcURI, parcURI_SetQuery_Reset)
{
    char *query = "query";
    PARCURI *uri = parcURI_Create();
    _parcURI_SetQuery(uri, query);
    _parcURI_SetQuery(uri, NULL);

    assertNull(parcURI_GetQuery(uri),
               "Expected NULL actual %s", parcURI_GetQuery(uri));
    parcURI_Release(&uri);
}

LONGBOW_TEST_CASE(parcURI, parcURI_SetAuthority)
{
    char *authority = "authority@auth";
    PARCURI *uri = parcURI_Create();
    _parcURI_SetAuthority(uri, authority);

    assertTrue(strcmp(authority, parcURI_GetAuthority(uri)) == 0,
               "Expected %s actual %s", authority, parcURI_GetAuthority(uri));
    parcURI_Release(&uri);
}

LONGBOW_TEST_CASE(parcURI, parcURI_SetAuthority_Resetting)
{
    char *authority = "query";
    PARCURI *uri = parcURI_Create();
    _parcURI_SetAuthority(uri, authority);
    _parcURI_SetAuthority(uri, authority);

    assertTrue(strcmp(authority, parcURI_GetAuthority(uri)) == 0,
               "Expected %s actual %s", authority, parcURI_GetAuthority(uri));
    parcURI_Release(&uri);
}

LONGBOW_TEST_CASE(parcURI, parcURI_SetAuthority_Reset)
{
    char *query = "query";
    PARCURI *uri = parcURI_Create();
    _parcURI_SetAuthority(uri, query);
    _parcURI_SetAuthority(uri, NULL);

    assertNull(parcURI_GetAuthority(uri),
               "Expected NULL actual %s", parcURI_GetAuthority(uri));
    parcURI_Release(&uri);
}

LONGBOW_TEST_CASE(parcURI, parseAuthority_NoPath)
{
    char *authority = "//" URI_AUTHORITY;
    char *expected = URI_AUTHORITY;

    const char *pointer;
    char *actual = _parseAuthority(authority, &pointer);

    assertTrue(strcmp(expected, actual) == 0,
               "Expected '%s' actual '%s'", authority, actual);
    assertTrue(*pointer == 0,
               "Expected null actual '%c'", *pointer);
    parcMemory_Deallocate((void **) &actual);
}

LONGBOW_TEST_CASE(parcURI, parseFragment)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_CASE(parcURI, parsePath)
{
    testUnimplemented("This test is unimplemented");
}

LONGBOW_TEST_CASE(parcURI, parseQuery)
{
    testUnimplemented("This test is unimplemented");
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parcURI);
    int status = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(status);
}
