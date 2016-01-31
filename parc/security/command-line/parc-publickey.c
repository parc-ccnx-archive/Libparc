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
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <LongBow/runtime.h>

#include <parc/algol/parc_ArrayList.h>
#include <parc/security/parc_Security.h>
#include <parc/security/parc_PublicKeySignerPkcs12Store.h>

void
parcPublicKey_Create(PARCArrayList *args)
{
    unsigned int keyLength = 1024;
    unsigned int validityDays = 30;

    char *fileName = parcArrayList_Get(args, 2);
    char *password = parcArrayList_Get(args, 3);
    char *subjectName = parcArrayList_Get(args, 4);

    if (parcArrayList_Size(args) > 5) {
        keyLength = (unsigned int) strtoul(parcArrayList_Get(args, 5), NULL, 10);
    }

    if (parcArrayList_Size(args) > 6) {
        validityDays = (unsigned int) strtoul(parcArrayList_Get(args, 6), NULL, 10);
    }
    bool result = parcPublicKeySignerPkcs12Store_CreateFile(fileName, password, subjectName, keyLength, validityDays);
    if (!result) {
        printf("Error: %s %s", fileName, strerror(errno));
        return;
    }
    printf("Created %s, key length %d valid for %d days.\n", fileName, keyLength, validityDays);
}

void
parcPublicKey_Validate(PARCArrayList *args)
{
    char *fileName = parcArrayList_Get(args, 2);
    char *password = parcArrayList_Get(args, 3);

    PARCSigningInterface *interface = parcPublicKeySignerPkcs12Store_Open(fileName, password, PARC_HASH_SHA256);
    if (interface == NULL) {
        printf("Invalid %s\n", fileName);
        return;
    }
    printf("Valid %s\n", fileName);
}

void
printUsage(char *progName)
{
    printf("usage: %s [-h | --help] [[-c | --create] fileName password subjectName [keyLength validityDays] | [-v | --validate] fileName password]\n", progName);
    printf("\n");
    printf("Copyright 2015 Palo Alto Research Center (PARC), a Xerox company.\n");
    printf("\n");
    printf("All Rights Reserved. Use is subject to license terms.\n");
    printf("\n");
    printf("Create and validate PKCS12 keystores that are used with the CCNx code.\n");
    printf("\n");
    printf("optional arguments:\n");
    printf("\t-h, --help\tShow this help message and exit\n");
    printf("\t-c, --create\tCreate a PKCS12 keystore with the given filename, password, subject name, and optional key length and validity length (in days)\n");
    printf("\n");
    printf("\t\t\texample: ./parc_publickey -c keyfile.pkcs12 <password> <subject name> 1024 365\n");
    printf("\n");
    printf("\t-v, --validate\tValidate a PKCS12 file with the given password\n");
    printf("\n");
    printf("\t\t\texample: ./parc_publickey -v keyfile.pkcs12 <password>");
    printf("\n");
}

int
main(int argc, char *argv[])
{
    char *programName = "parc_publickey";
    if (argc < 2) {
        printUsage(programName);
        exit(1);
    }

    PARCArrayList *args = parcArrayList_Create(NULL);
    parcArrayList_AddAll(args, (void **) argv, argc);

    parcSecurity_Init();

    char *arg = parcArrayList_Get(args, 1);
    if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
        printUsage(programName);
        return 0;
    } else if (strcmp(arg, "-c") == 0 || strcmp(arg, "--create") == 0) {
        parcPublicKey_Create(args);
    } else if (strcmp(arg, "-v") == 0 || strcmp(arg, "--validate") == 0) {
        parcPublicKey_Validate(args);
    } else {
        printUsage(programName);
        exit(1);
    }

    parcSecurity_Fini();
    return 0;
}
