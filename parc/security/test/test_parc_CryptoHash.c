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

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../parc_CryptoHash.c"
#include "../parc_CryptoHasher.h"

#include <LongBow/unit-test.h>
#include <parc/testing/parc_ObjectTesting.h>
#include <fcntl.h>
#include <errno.h>

const int bufferLength = 1024;

LONGBOW_TEST_RUNNER(parc_CryptoHash)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_CryptoHash)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_CryptoHash)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcCryptoHash_CreateFromArray);
    LONGBOW_RUN_TEST_CASE(Global, parcCryptoHash_Release);
    LONGBOW_RUN_TEST_CASE(Global, parcCryptoHash_Equals);
    LONGBOW_RUN_TEST_CASE(Global, parcCryptoHash_GetDigest);
    LONGBOW_RUN_TEST_CASE(Global, parcCryptoHash_GetDigestType);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, parcCryptoHash_CreateFromArray)
{
    int fd = open("test_digest_bytes_128.sha256", O_RDONLY);
    assertFalse(fd < 0, "Could not open %s: %s", "test_digest_bytes_128.sha256", strerror(errno));

    uint8_t scratch[bufferLength];
    ssize_t read_length = read(fd, scratch, bufferLength);

    PARCCryptoHash *hash = parcCryptoHash_CreateFromArray(PARCCryptoHashType_SHA256, scratch, read_length);
    assertNotNull(hash, "Expected to be non null");

    parcCryptoHash_Release(&hash);
    close(fd);
}

LONGBOW_TEST_CASE(Global, parcCryptoHash_Release)
{
    int fd = open("test_digest_bytes_128.sha256", O_RDONLY);
    assertFalse(fd < 0, "Could not open %s: %s", "test_digest_bytes_128.sha256", strerror(errno));

    uint8_t scratch[bufferLength];
    ssize_t read_length = read(fd, scratch, bufferLength);

    PARCCryptoHash *hash = parcCryptoHash_CreateFromArray(PARCCryptoHashType_SHA256, scratch, read_length);
    assertNotNull(hash, "Expected to be non null");

    parcCryptoHash_Release(&hash);
    assertNull(hash, "Expected to be null");
    close(fd);
}

LONGBOW_TEST_CASE(Global, parcCryptoHash_Equals)
{
    int fd1 = open("test_digest_bytes_128.sha256", O_RDONLY);
    assertFalse(fd1 < 0, "Could not open %s: %s", "test_digest_bytes_128.sha256", strerror(errno));

    uint8_t scratch1[bufferLength];
    ssize_t read_length = read(fd1, scratch1, bufferLength);

    PARCCryptoHash *hash1 = parcCryptoHash_CreateFromArray(PARCCryptoHashType_SHA256, scratch1, read_length);
    PARCCryptoHash *hash2 = parcCryptoHash_CreateFromArray(PARCCryptoHashType_SHA256, scratch1, read_length);
    PARCCryptoHash *hash3 = parcCryptoHash_CreateFromArray(PARCCryptoHashType_SHA256, scratch1, read_length);

    int fd2 = open("test_digest_bytes_128.sha512", O_RDONLY);
    assertFalse(fd2 < 0, "Could not open %s: %s", "test_digest_bytes_128.sha512", strerror(errno));

    uint8_t scratch2[bufferLength];
    read_length = read(fd2, scratch2, bufferLength);

    PARCCryptoHash *unequalhash = parcCryptoHash_CreateFromArray(PARCCryptoHashType_SHA256, scratch2, read_length);

    parcObjectTesting_AssertEqualsFunction(parcObject_Equals, hash1, hash2, hash3, unequalhash);

    parcCryptoHash_Release(&hash1);
    parcCryptoHash_Release(&hash2);
    parcCryptoHash_Release(&hash3);
    parcCryptoHash_Release(&unequalhash);

    close(fd1);
    close(fd2);
}

LONGBOW_TEST_CASE(Global, parcCryptoHash_GetDigest)
{
    int fd_buffer = open("test_digest_bytes_128.bin", O_RDONLY);
    int fd_truth = open("test_digest_bytes_128.sha256", O_RDONLY);
    assertFalse(fd_buffer < 0, "Could not open %s: %s", "test_digest_bytes_128.bin", strerror(errno));
    assertFalse(fd_truth < 0, "Could not open %s: %s", "test_digest_bytes_128.sha256", strerror(errno));

    uint8_t scratch[bufferLength];
    ssize_t read_length = read(fd_truth, scratch, bufferLength);

    PARCCryptoHash *hashTruth = parcCryptoHash_CreateFromArray(PARCCryptoHashType_SHA256, scratch, read_length);

    read_length = read(fd_buffer, scratch, bufferLength);

    PARCCryptoHasher *hasher = parcCryptoHasher_Create(PARCCryptoHashType_SHA256);
    parcCryptoHasher_Init(hasher);
    parcCryptoHasher_UpdateBytes(hasher, scratch, read_length);

    PARCCryptoHash *hashTest = parcCryptoHasher_Finalize(hasher);

    assertTrue(parcBuffer_Equals(parcCryptoHash_GetDigest(hashTruth), parcCryptoHash_GetDigest(hashTest)), "Expected to be true");

    parcCryptoHasher_Release(&hasher);
    parcCryptoHash_Release(&hashTruth);
    parcCryptoHash_Release(&hashTest);

    close(fd_buffer);
    close(fd_truth);
}

LONGBOW_TEST_CASE(Global, parcCryptoHash_GetDigestType)
{
    int fd = open("test_digest_bytes_128.sha256", O_RDONLY);
    assertFalse(fd < 0, "Could not open %s: %s", "test_digest_bytes_128.sha256", strerror(errno));

    uint8_t scratch[bufferLength];
    ssize_t read_length = read(fd, scratch, bufferLength);

    PARCCryptoHash *hash = parcCryptoHash_CreateFromArray(PARCCryptoHashType_SHA256, scratch, read_length);
    assertNotNull(hash, "Expected to be non null");

    assertTrue(PARCCryptoHashType_SHA256 == parcCryptoHash_GetDigestType(hash), "Expected to be true");

    parcCryptoHash_Release(&hash);
    close(fd);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_CryptoHash);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
