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
 * @file parc_KeyStore.h
 * @ingroup security
 * @brief A container of Key Store information.
 *
 * A Key Store is a repository of key information typically accessable
 * through some authentication and authorisation system.
 * The PARCKeyStore type contains the necessary information to successfully
 * gain access to a Key Store.
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libparc_parc_KeyStore_h
#define libparc_parc_KeyStore_h

struct parc_key_store;
typedef struct parc_key_store PARCKeyStore;

/**
 * Create a `PARCKeyStore` from a filename.
 *
 * @param [in] fileName The file name.
 * @param [in] passWord The password to be used.
 *
 * @return A pointer to the new `PARCKeyStore`
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
PARCKeyStore *parcKeyStore_CreateFile(const char *fileName, const char *passWord);

/**
 * Acquire a new reference to an instance of `PARCKeyStore`.
 *
 * The reference count to the instance is incremented.
 *
 * @param [in] keyStore The instance of `PARCKeyStore` to which to refer.
 *
 * @return The same value as the input parameter @p keyStore
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
PARCKeyStore *parcKeyStore_Acquire(const PARCKeyStore *keyStore);

/**
 * Release a `PARCKeyStore` reference.
 *
 * Only the last invocation where the reference count is decremented to zero,
 * will actually destroy the `PARCKeyStore`.
 *
 * @param [in,out] keyStorePtr A pointer to the pointer to the `PARCKeyStore` to release.
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
void parcKeyStore_Release(PARCKeyStore **keyStorePtr);

/**
 * Get the file name from a `PARCKeyStore`.
 *
 * @param [in] keyStore  An instance of `PARCKeyStore`.
 *
 * @return  A pointer to the filename from the @p keyStore.
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
const char *parcKeyStore_GetFileName(const PARCKeyStore *keyStore);

/**
 * Get the password from a `PARCKeyStore`.
 *
 * @param [in] keyStore An instance of `PARCKeyStore`.
 *
 * @return  A pointer to the password from the @p keyStore.
 *
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
const char *parcKeyStore_GetPassWord(const PARCKeyStore *keyStore);
#endif // libparc_parc_KeyStore_h
