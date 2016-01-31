/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @file parc_URIAuthority.h
 * @ingroup networking
 * @brief A Universal Resource Identifier (URI) Authority
 *
 * @author Christopher A. Wood, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libparc_parc_URIAuthority_h
#define libparc_parc_URIAuthority_h

#include <stdbool.h>

struct parc_uri_authority;
typedef struct parc_uri_authority PARCURIAuthority;

/**
 * Create a new `PARCURIAuthority` object.
 *
 * The new `PARCURIAuthority` object is empty.
 *
 * @return A pointer to a `PARCURIAuthority` instance.
 *
 * Example:
 * @code
 * {
 *     PARCURIAuthority *auth = parcURIAuthority_Create();
 *     ...
 *     parcURIAuthority_Release(&auth);
 * }
 * @endcode
 */
PARCURIAuthority *parcURIAuthority_Create(void);

/**
 * Increase the number of references to a `PARCURIAuthority` instance.
 *
 * Note that new `PARCURIAuthority` is not created,
 * only that the given `PARCURIAuthority` reference count is incremented.
 * Discard the reference by invoking `parcURIAuthority_Release`.
 *
 * @param auth A pointer to the original instance.
 * @return The value of the input parameter @p instance.
 *
 * Example:
 * @code
 * {
 *     PARCURIAuthority *x = parcURIAuthority_Create();
 *
 *     PARCURIAuthority *x2 = parcURIAuthority_Acquire(x);
 *
 *     parcURIAuthority_Release(&x);
 *     parcURIAuthority_Release(&x2);
 * }
 * @endcode
 *
 * @see parcURIAuthority_Release
 */
PARCURIAuthority *parcURIAuthority_Acquire(const PARCURIAuthority *auth);

/**
 * Release a previously acquired reference to the specified instance,
 * decrementing the reference count for the instance.
 *
 * The pointer to the instance is set to NULL as a side-effect of this function.
 *
 * If the invocation causes the last reference to the instance to be released,
 * the instance is deallocated and the instance's implementation will perform
 * additional cleanup and release other privately held references.
 *
 * @param [in,out] authPtr A pointer to a pointer to the instance to release.
 *
 * Example:
 * @code
 * {
 *     PARCURIAuthority *auth = parcURIAuthority_Create();
 *
 *     parcURIAuthority_Release(&auth);
 * }
 * @endcode
 */
void parcURIAuthority_Release(PARCURIAuthority **authPtr);

/**
 * Return true if two `PARCURIAuthority` instances are equal.
 *
 * The following equivalence relations on non-null `PARCURIAuthority` instances are maintained:
 *
 *   * It is reflexive: for any non-null reference value x, parcURIAuthority_Equals(x, x) must return true.
 *
 *   * It is symmetric: for any non-null reference values x and y, parcURIAuthority_Equals(x, y) must return true if and only if
 *        parcURIAuthority_Equals(y x) returns true.
 *
 *   * It is transitive: for any non-null reference values x, y, and z, if
 *        parcURIAuthority_Equals(x, y) returns true and
 *        parcURIAuthority_Equals(y, z) returns true,
 *        then  parcURIAuthority_Equals(x, z) must return true.
 *
 *   * It is consistent: for any non-null reference values x and y, multiple invocations of parcURIAuthority_Equals(x, y)
 *         consistently return true or consistently return false.
 *
 *   * For any non-null reference value x, parcURIAuthority_Equals(x, NULL)) must return false.
 *
 * @param [in] authA The first `PARCURIAuthority` instance.
 * @param [in] authB The second `PARCURIAuthority` instance.
 *
 * @return true the given `PARCURIAuthority` instances are equal.
 * @return false the given `PARCURIAuthority` instances are not equal.
 *
 * Example:
 * @code
 * {
 *     const char *uriString = "http://chris@parc.com:80";
 *     PARCURI *uri = parcURI_Parse(uriString);
 *     PARCURI_Authority *authority = parcURIAuthority_Parse(parcURI_GetAuthority(uri));
 *     PARCURI_Authority *handle = parcURIAuthority_Acquire(authority);
 *
 *     if (parcURIAuthority_Equals(authority, handle)) {
 *         printf("Authorities are equal.\n");
 *     } else {
 *         printf("Authorities are NOT equal.\n");
 *     }
 *
 *     parcURIAuthority_Release(&uri);
 *     parcURIAuthority_Release(&copy);
 * }
 * @endcode
 */
bool parcURIAuthority_Equals(const PARCURIAuthority *authA, const PARCURIAuthority *authB);

/**
 * Produce a `PARCURI_Authority` type from the authority string.
 *
 *   The returned value must be destroyed via {@link parcMemory_Deallocate}.
 *
 * @param [in] authority The authority string to parse.
 *
 * @return A newly allocated `PARCURI_Authority` string.
 *
 * Example:
 * @code
 * {
 *     const char *uriString = "http://chris@parc.com:80";
 *     PARCURI *uri = parcURI_Parse(uriString);
 *
 *     PARCURI_Authority *authority = parcURIAuthority_Parse(parcURI_GetAuthority(uri));
 *     // use the authority
 *
 *     parcURIAuthority_Release(&authority);
 *     parcURI_Release(&uri);
 * }
 * @endcode
 */
PARCURIAuthority *parcURIAuthority_Parse(const char *authority);

/**
 * Retrieve the user information from the given `PARCURI_Authority` instance.
 *
 * @param [in] authority The `PARCURI_Authority` instance being queried.
 *
 * @return The user info string component of the `PARCURI_Authority` instance. Copy the string prior to modification.
 *
 * Example:
 * @code
 * {
 *     const char *uriString = "http://chris@parc.com:80";
 *     PARCURI *uri = parcURI_Parse(uriString);
 *
 *     PARCURI_Authority *authority = parcURIAuthority_Parse(parcURI_GetAuthority(uri));
 *     printf("User info: "%s\n", parcURIAuthority_GetUserInfo(authority));
 *
 *     parcURIAuthority_Release(&authority);
 *     parcURI_Release(&uri);
 * }
 * @endcode
 */
const char *parcURIAuthority_GetUserInfo(const PARCURIAuthority *authority);

/**
 * Retrieve the host name from the given `PARCURI_Authority` instance.
 *
 * @param [in] authority The `PARCURI_Authority` instance being queried.
 *
 * @return The host name string component of the `PARCURI_Authority` instance. Copy the string prior to modification.
 *
 * Example:
 * @code
 * {
 *     const char *uriString = "http://chris@parc.com:80";
 *     PARCURI *uri = parcURI_Parse(uriString);
 *
 *     PARCURI_Authority *authority = parcURIAuthority_Parse(parcURI_GetAuthority(uri));
 *     printf("Host name: "%s\n", parcURIAuthority_GetHostName(authority));
 *
 *     parcURIAuthority_Release(&authority);
 *     parcURI_Release(&uri);
 * }
 * @endcode
 */
const char *parcURIAuthority_GetHostName(const PARCURIAuthority *authority);

/**
 * Retrieve the host port from the given `PARCURI_Authority` instance.
 *
 * @param [in] authority The `PARCURI_Authority` instance being queried.
 *
 * @return The host port string component of the `PARCURI_Authority` instance. Copy the string prior to modification.
 *
 * Example:
 * @code
 * {
 *     const char *uriString = "http://chris@parc.com:80";
 *     PARCURI *uri = parcURI_Parse(uriString);
 *
 *     PARCURI_Authority *authority = parcURIAuthority_Parse(parcURI_GetAuthority(uri));
 *     printf("Host port: "%s\n", parcURIAuthority_GetPort(authority));
 *
 *     parcURIAuthority_Release(&authority);
 *     parcURI_Release(&uri);
 * }
 * @endcode
 */
long parcURIAuthority_GetPort(const PARCURIAuthority *authority);
#endif /* defined(libparc_parc_URIAuthority_h) */
