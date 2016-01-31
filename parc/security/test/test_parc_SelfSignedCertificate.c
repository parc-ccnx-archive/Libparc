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
#include <config.h>
#include <LongBow/unit-test.h>

#include <parc/algol/parc_SafeMemory.h>

#ifdef __APPLE__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../parc_SelfSignedCertificate.c"

LONGBOW_TEST_RUNNER(parc_SelfSignedCertificate)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Local);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_SelfSignedCertificate)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_SelfSignedCertificate)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcSelfSignedCertificate_CreateRsaCertificate);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    if (parcSafeMemory_ReportAllocation(STDOUT_FILENO) != 0) {
        printf("('%s' leaks memory by %d (allocs - frees)) ", longBowTestCase_GetName(testCase), parcMemory_Outstanding());
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, parcSelfSignedCertificate_CreateRsaCertificate)
{
    RSA *rsa;
    EVP_PKEY *private_key;
    X509 *cert;
    int result;
    const char *subjectname = "alice";
    unsigned keylength = 2046;
    unsigned validityDays = 35;

    parcSecurity_Init();

    rsa = RSA_new();
    assertNotNull(rsa, "Got null from RSA_new()");

    private_key = EVP_PKEY_new();
    assertNotNull(private_key, "Got null from EVP_PKEY_new()");

    cert = X509_new();
    assertNotNull(cert, "Got null from X509_new()");

    result = parcSelfSignedCertificate_CreateRSACertificate(subjectname, keylength, validityDays, cert, rsa, private_key);

    assertTrue(result, "got error from CreateRsaCertificate: %d", result);

    // Now validate a few things about it
    X509_NAME *name = X509_get_subject_name(cert);
    assertNotNull(name, "got null for X509 subject name");

    {
        int idx = X509_NAME_get_index_by_NID(name, NID_commonName, -1);
        X509_NAME_ENTRY *name_entry = X509_NAME_get_entry(name, idx);
        ASN1_STRING *name_data = X509_NAME_ENTRY_get_data(name_entry);
        unsigned char *utf8;
        int length = ASN1_STRING_to_UTF8(&utf8, name_data);
        printf("CN value %s\n", utf8);
        printf("CN length %d\n", length);

        assertTrue(strncmp(subjectname, (char *) utf8, length) == 0, "got incorrect subject name");

        OPENSSL_free(utf8);
    }

    {
        int i;
        AUTHORITY_KEYID  *akid = X509_get_ext_d2i(cert, NID_authority_key_identifier, NULL, NULL);
        assertNotNull(akid, "got null authority key identifier");

        ASN1_OCTET_STRING *skid = X509_get_ext_d2i(cert, NID_subject_key_identifier, NULL, NULL);
        assertNotNull(skid, "got null subject key identifier");
        ASN1_OCTET_STRING_cmp(akid->keyid, skid);

        printf("SKID value: ");
        for (i = 0; i < skid->length; i++) {
            printf("%02X", skid->data[i]);
        }
        printf("\n");

        printf("SKID length %d\n", skid->length);

        AUTHORITY_KEYID_free(akid);
    }


    EVP_PKEY_free(private_key);
    RSA_free(rsa);
    X509_free(cert);
    void ERR_free_strings(void);
    parcSecurity_Fini();
}

LONGBOW_TEST_FIXTURE(Local)
{
    LONGBOW_RUN_TEST_CASE(Local, _addCertificateExtensionWithContextFail);
    LONGBOW_RUN_TEST_CASE(Local, _addCertificateExtensionFail);
}

LONGBOW_TEST_FIXTURE_SETUP(Local)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Local)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Local, _addCertificateExtensionWithContextFail)
{
    X509 *cert;
    int result;

    parcSecurity_Init();

    cert = X509_new();
    assertNotNull(cert, "Got null from X509_new()");

    result = _addCertificateExtensionWithContext(cert, -1, "keyid:always");

    assertFalse(result, "Expected false result from _addCertificateExtensionWithContextFail()");

    parcSecurity_Fini();
}

LONGBOW_TEST_CASE(Local, _addCertificateExtensionFail)
{
    X509 *cert;
    int result;

    parcSecurity_Init();

    cert = X509_new();
    assertNotNull(cert, "Got null from X509_new()");

    result = _addCertificateExtension(cert, -1, "keyid:always");

    assertFalse(result, "Expected false result from _addCertificateExtension()");

    parcSecurity_Fini();
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_SelfSignedCertificate);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}

#ifdef __APPLE__
#pragma clang diagnostic pop
#endif
