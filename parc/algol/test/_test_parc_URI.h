/*
 * Copyright (c) 2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
//
//  _test_parc_URI.h
//  PARC Algol
//
//  Created by Wood, Christopher <Christopher.Wood@parc.com> on 10/15/14.
//  Copyright (c) 2014 Computer Science Laboratory. All rights reserved.
//

#ifndef PARC_Algol__test_parc_URI_h
#define PARC_Algol__test_parc_URI_h

#define URI_SCHEME "lci"

#define URI_AUTHORITY_USERINFO   "user:pass"
#define URI_AUTHORITY_USERINFO_2 "user2:pass2"
#define URI_AUTHORITY_HOSTNAME   "parc.com"
#define URI_AUTHORITY_HOSTNAME_2 "xerox.com"
#define URI_AUTHORITY_PORT_1 "1234"
#define URI_AUTHORITY_PORT_2 "5678"

#define URI_AUTHORITY                URI_AUTHORITY_USERINFO "@" URI_AUTHORITY_HOSTNAME ":" URI_AUTHORITY_PORT_1
#define URI_AUTHORITY_DIFFERENT_PORT URI_AUTHORITY_USERINFO "@" URI_AUTHORITY_HOSTNAME ":" URI_AUTHORITY_PORT_2
#define URI_AUTHORITY_DIFFERENT_HOST URI_AUTHORITY_USERINFO "@" URI_AUTHORITY_HOSTNAME_2 ":" URI_AUTHORITY_PORT_1
#define URI_AUTHORITY_DIFFERENT_USER URI_AUTHORITY_USERINFO_2 "@" URI_AUTHORITY_HOSTNAME ":" URI_AUTHORITY_PORT_1

#define URI_PATH_SEGMENT "%00%01%02-.%03%04_%05%06%07%08%09abcdefghijklmnopqrstuvwxyz"
#define URI_PATH_SEGMENT_WITH_SLASHES URI_PATH_SEGMENT "//////"

#define URI_PATH "/" URI_PATH_SEGMENT "/" URI_PATH_SEGMENT

#define URI_QUERY "x=1&y=2&z=3"

#define URI_FRAGMENT "alphabetagamma"

#define URI_FULL URI_SCHEME "://" URI_AUTHORITY "/" URI_PATH "?" URI_QUERY "#" URI_FRAGMENT

char *TEST_URI_SCHEME = URI_SCHEME;
char *TEST_URI_AUTHORITY = URI_AUTHORITY;
#endif // PARC_Algol__test_parc_URI_h
