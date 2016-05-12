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
 * @header parc_CryptoCache.c
 * <#Abstract#>
 *
 *     <#Discussion#>
 *
 * @author Mosko, Marc
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
/*
 * This should be updated to make a reference counted copy of PARCKey and store that (along with its KeyId)
 * instead of using a direct copy of the user data.  That way, there's no issues about destroying the entry
 * but the user retaining a (now invalid) reference to it.
 *
 */

#include <config.h>
#include <stdio.h>
#include <string.h>

#include <LongBow/runtime.h>

#include <parc/algol/parc_Memory.h>
#include <parc/security/parc_CryptoCache.h>
#include <parc/algol/parc_HashCodeTable.h>

struct parc_crypto_cache {
    PARCHashCodeTable *keyid_table;
};

// =====================================================================
// Translations from void* to typed pointer for use in HashCodeTable

static bool
_keyidEquals(const void *ptrA, const void *ptrB)
{
    return parcKeyId_Equals((const PARCKeyId *) ptrA, (const PARCKeyId *) ptrB);
}

static void
_dataDestroy(void **voidPtr)
{
    PARCKey **keyPtr = (PARCKey **) voidPtr;
    parcKey_Release(keyPtr);
}

// =====================================================================

PARCCryptoCache *
parcCryptoCache_Create()
{
    PARCCryptoCache *cache = parcMemory_AllocateAndClear(sizeof(PARCCryptoCache));
    assertNotNull(cache, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(PARCCryptoCache));

    // KeyIdDestroyer is NULL because we get the keyid out of the key, and it will be destroyed
    // when the key is destroyed.
    cache->keyid_table = parcHashCodeTable_Create(_keyidEquals, parcKeyId_HashCodeFromVoid, NULL, _dataDestroy);

    return cache;
}

/**
 * Destroys the cache and all internal buffers.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void
parcCryptoCache_Destroy(PARCCryptoCache **cryptoCachePtr)
{
    assertNotNull(cryptoCachePtr, "Parameter must be non-null double pointer");
    assertNotNull(*cryptoCachePtr, "Parameter must dereference to non-null pointer");

    PARCCryptoCache *cache = *cryptoCachePtr;
    parcHashCodeTable_Destroy(&cache->keyid_table);
    parcMemory_Deallocate((void **) cryptoCachePtr);
    *cryptoCachePtr = NULL;
}

/**
 * Adds the specified key to the keycache.
 *
 * Parameters must be non-null
 * Returns true if added or false if keyid alredy existed and was a different than <code>key</code>
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
bool
parcCryptoCache_AddKey(PARCCryptoCache *cache, PARCKey *original_key)
{
    assertNotNull(cache, "Parameter cache must be non-null");
    assertNotNull(original_key, "Parameter key must be non-null");

    PARCKey *key = parcKey_Copy(original_key);
    PARCKeyId *keyid = parcKey_GetKeyId(key);

    return parcHashCodeTable_Add(cache->keyid_table, keyid, key);
}

/**
 * Fetches the Key.  The user must node modify or destroy the key.
 *
 * Returns NULL if the keyid is not found.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
const PARCKey *
parcCryptoCache_GetKey(PARCCryptoCache *cache, const PARCKeyId *keyid)
{
    assertNotNull(cache, "Parameter cache must be non-null");
    assertNotNull(keyid, "Parameter keyid must be non-null");

    return parcHashCodeTable_Get(cache->keyid_table, keyid);
}

/**
 * Removes the keyid and key.  The internal buffers are destroyed.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void
parcCryptoCache_RemoveKey(PARCCryptoCache *cache, const PARCKeyId *keyid)
{
    assertNotNull(cache, "Parameter cache must be non-null");
    assertNotNull(keyid, "Parameter keyid must be non-null");

    parcHashCodeTable_Del(cache->keyid_table, keyid);
}
