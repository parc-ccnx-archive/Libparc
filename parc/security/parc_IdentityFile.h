/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
/**
 * @file parc_IdentityFile.h
 * @ingroup security
 * @brief This is a PARCIdentity represented as a PKCS12 keystore file.
 *
 * To create these files, use the `parc-publickey` command line tool. Or you could also
 * use `openssl` to create the same file.
 *
 * A `PARCIdentityFile` is a concrete instance of a `PARCIdentity`. To create a
 * `PARCIdentity` from a `PARCIdentityFile`, one would do the following:
 *
 * @code
 * {
 *     const char *keystoreName = "my_identity.p12";
 *     const char *keystorePassword = "my_password";
 *
 *     // Create the concrete identity instance from the PKCS12 file
 *     PARCIdentityFile *identityFile = parcIdentityFile_Create(keystoreName, keystorePassword);
 *
 *     // Create a generic `PARCIdentity` from the concrete identity instance
 *     PARCIdentity *identity = parcIdentity_Create(identityFile, PARCIdentityFileAsPARCIdentity);
 *
 *     // Now use the `PARCIdentity` for signing purposes (amongst other things)
 *     PARCSigner *signer = parcIdentity_GetSigner(identity);
 * }
 * @endcode
 *
 * @author Glenn Scott, Christopher A. Wood, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_IdentityFile_h
#define libparc_parc_IdentityFile_h

#include <parc/security/parc_Identity.h>

struct parc_identity_file;
typedef struct parc_identity_file PARCIdentityFile;

/**
 * The mapping of a PARCIdentityFile to the generic PARCIdentity.
 */
extern PARCIdentityInterface *PARCIdentityFileAsPARCIdentity;

/**
 * Create an instance of `PARCIdentityFile` from a given filename, and a password to unlock the stored information.
 *
 * The information is stored in PKCS 12 format.
 *
 * @param [in] fileName The name (relative path) to a file to be opened
 * @param [in] password The password to be used to open the identity file
 *
 * @return NULL A `PARCIdentityFile` could not be allocated.
 * @return PARCIdentityFile A newly allocated `PARCIdentityFile` that must be freed with `parcIdentityFile_Release()`
 *
 * Example:
 * @code
 * {
 *     PARCIdentityFile *file = parcIdentityFile_Create("./secret.p12", "1234");
 *     ...
 *     parcIdentityFile_Release(&file);
 * }
 * @endcode
 */
PARCIdentityFile *parcIdentityFile_Create(const char *fileName, const char *password);

/**
 * Increase the number of references to the given `PARCIdentityFile` instance.
 *
 * A new instance is not created,
 * only that the given instance's reference count is incremented.
 * Discard the acquired reference by invoking `parcIdentityFile_Release()`.
 *
 * @param [in] identity A pointer to a `PARCIdentityFile` instance.
 *
 * @return NULL An error occurred.
 * @return non-NULL A pointer to a PARCIdentityFile instance.
 *
 * Example:
 * @code
 * {
 *     PARCIdentityFile *file = parcIdentityFile_Create("./secret.p12", "1234");
 *     PARCIdentityFile *secondHandle = parcIdentityFile_Acquire(file);
 *     // use both handles as needed
 *     parcIdentityFile_Release(&file);
 *     parcIdentityFile_Release(&secondHandle);
 * }
 * @endcode
 */
PARCIdentityFile *parcIdentityFile_Acquire(const PARCIdentityFile *identity);

/**
 * Decrease the number of references to the given `PARCIdentityFile` instance.
 *
 * This only decrements the reference count so long as count >= 1. If the count
 * reaches zero, the object's memory is freed. The content pointer is always
 * NULLified after invocation.
 *
 * @param [in,out] identityPtr A pointer to a `PARCIdentityFile` instance.
 *
 * Example:
 * @code
 * {
 *     PARCIdentityFile *file = parcIdentityFile_Create("./secret.p12", "1234");
 *     // use handle as needed
 *     parcIdentityFile_Release(&file);
 * }
 * @endcode
 */
void parcIdentityFile_Release(PARCIdentityFile **identityPtr);

/**
 * Determine if the PARCIdentityFile exists.
 *
 * It must exist and be a regular file.
 *
 * @param [in] identity A pointer to a valid PARCIdentity instance.
 *
 * @return true The file exists and is a regular file.
 * @return false The file does not exist (see errno values for stat(2)) or is not a regular file.
 *
 * Example:
 * @code
 * {
 *     PARCIdentityFile *file = parcIdentityFile_Create("./secret.p12", "1234");
 *
 *     if (parcIdentityFile_Exists(file) == true) {
 *         printf("The file exists\n");
 *     } else {
 *         printf("The file does not exist fos\n");
 *     }
 * }
 * @endcode
 */
bool parcIdentityFile_Exists(const PARCIdentityFile *identity);

/**
 * Retrieve the name of the file associated with this `PARCIdentityFile` instance.
 *
 * @param [in] identity A pointer to a `PARCIdentityFile` instance.
 *
 * @return NULL An error occurred.
 * @return non-NULL A pointer to a PARCIdentityFile instance.
 *
 * Example:
 * @code
 * {
 *     PARCIdentityFile *file = parcIdentityFile_Create("./secret.p12", "1234");
 *     const char *fileName = parcIdentityFile_GetFileName(file);
 *     // use handle and/or file name as needed
 *     parcIdentityFile_Release(&file);
 * }
 * @endcode
 */
const char *parcIdentityFile_GetFileName(const PARCIdentityFile *identity);

/**
 * Retrieve the file password associated with this `PARCIdentityFile` instance.
 *
 * @param [in] identity A pointer to a `PARCIdentityFile` instance.
 *
 * @return NULL An error occurred.
 * @return non-NULL A pointer to a PARCIdentityFile instance.
 *
 * Example:
 * @code
 * {
 *     PARCIdentityFile *file = parcIdentityFile_Create("./secret.p12", "1234");
 *     const char *password = parcIdentityFile_GetPassWord(file);
 *     // use handle and/or file name as needed
 *     parcIdentityFile_Release(&file);
 * }
 * @endcode
 */
const char *parcIdentityFile_GetPassWord(const PARCIdentityFile *identity);

/**
 * Create an instance of `PARCSigner` from the given `PARCIdentity`
 *
 * @param [in] identity A pointer to a PARCIdentity instance.
 *
 * @return NULL An error occurred
 * @return PARCSigner A new `PARCSigner` instance created using the identity file for the public/private signing keys
 *
 * Example:
 * @code
 * {
 *     PARCIdentityFile *file = parcIdentityFile_Create("./secret.p12", "1234");
 *     PARCSigner *signer = parcIdentityFile_GetSigner(file);
 *     parcIdentityFile_Release(&file);
 *     // use the signer
 *     parcSigner_Release(&signer);
 * }
 * @endcode
 */
PARCSigner *parcIdentityFile_CreateSigner(const PARCIdentityFile *identity);

/**
 * Determine if two PARCIdentityFiles are equal.
 *
 * The following equivalence relations on non-null `PARCIdentityFile` instances are maintained:
 *
 *   * It is reflexive: for any non-null reference value x, parcIdentityFile_Equals(x, x) must return true.
 *
 *   * It is symmetric: for any non-null reference values x and y, parcIdentityFile_Equals(x, y) must return true if and only if
 *        parcIdentityFile_Equals(y x) returns true.
 *
 *   * It is transitive: for any non-null reference values x, y, and z, if
 *        parcIdentityFile_Equals(x, y) returns true and
 *        parcIdentityFile_Equals(y, z) returns true,
 *        then  parcIdentityFile_Equals(x, z) must return true.
 *
 *   * It is consistent: for any non-null reference values x and y, multiple invocations of parcIdentityFile_Equals(x, y)
 *         consistently return true or consistently return false.
 *
 *   * For any non-null reference value x, parcIdentityFile_Equals(x, NULL)) must return false.
 *
 * @param a A pointer to a PARCIdentityFile instance.
 * @param b A pointer to a PARCIdentityFile instance.
 * @return True if the referenced PARCIdentityFiles are equal.
 *
 * Example:
 * @code
 * {
 *     PARCIdentityFile *a = parcIdentityFile_Create(...);
 *     PARCIdentityFile *b = parcIdentityFile_Create(...);
 *     if (parcIdentityFile_Equals(a, b)) {
 *         // this is expected
 *     } else {
 *         // this is not expected
 *     }
 *     parcIdentityFile_Release(&a);
 *     parcIdentityFile_Release(&b);
 * }

 * }
 * @endcode
 */
bool parcIdentityFile_Equals(const PARCIdentityFile *a, const PARCIdentityFile *b);

/**
 * Print a human readable representation of the given `PARCIdentityFile`.
 *
 * @param [in] indentation The level of indentation to use to pretty-print the output.
 * @param [in] instance A pointer to the instance to display.
 *
 * Example:
 * @code
 * {
 *     PARCIdentityFile *instance = parcIdentityFile_Create("./secret.p12", "1234");
 *
 *     parcIdentityFile_Display(instance, 0);
 *
 *     parcIdentityFile_Release(&instance);
 * }
 * @endcode
 */
void parcIdentityFile_Display(const PARCIdentityFile *instance, int indentation);
#endif // libparc_parc_IdentityFile_h
