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
 * @file parc_KeyedElement.h
 * @ingroup datastructures
 * @brief A Pointer and a Key
 *  The `PARCKeyedElement` is a simple pointer and key tuple.
 *
 * @author Nacho "Ignacio" Solis, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
#ifndef libparc_parc_KeyedElement_h
#define libparc_parc_KeyedElement_h

#include <stdint.h>
#include <stdlib.h>

struct parc_keyed_element;

/**
 * A `PARCKeyedElement` is a tuple consisting of an address and a key+len.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
typedef struct parc_keyed_element PARCKeyedElement;

/**
 * Create a `PARCKeyedElement`.  Note that the key will be copied (size keylen) while the data will just be
 * referenced.  The key copy will be released when the KeyedElement is destroyed.
 *
 * @param [in] data The data we want to associate with a key.
 * @param [in] key A pointer to the key data (will be copied).
 * @param [in] keylen The length of the keydata.
 * @return The new instance of `PARCKeyedElement`.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCKeyedElement *parcKeyedElement_Create(void *data, const void *key, size_t keylen);

/**
 * Destroy a `PARCKeyedElement`.
 *
 * If the Free functions were passed to the constructor they will be called if
 * not NULL. *
 * @param [in,out] keyedElementPointer A pointer to the pointer to the `PARCKeyedElement` to be destroyed.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void parcKeyedElement_Destroy(PARCKeyedElement **keyedElementPointer);
/**
 * Set the data of a `PARCKeyedElement`.
 *
 * @param [in,out] keyedElement The pointer to the `PARCKeyedElement` whose data will be reset to @p data.
 * @param [in] data The data to put be into @p keyedElement
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void parcKeyedElement_SetData(PARCKeyedElement *keyedElement, void *data);

/**
 * Get the data of a `PARCKeyedElement`.
 *
 * @param [in] keyedElement The pointer to the `PARCKeyedElement` whose data will be retrieved.
 * @return A pointer to the retrieved data.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void *parcKeyedElement_GetData(PARCKeyedElement *keyedElement);

/**
 * Retrieve the key of a `PARCKeyedElement`.
 *
 * @param [in] keyedElement A pointer to the `PARCKeyedElement` whose key should be retreieved.
 *
 * @return A pointer to the `PARCKeyedElement`'s key.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void *parcKeyedElement_GetKey(PARCKeyedElement *keyedElement);

/**
 * Return the size of the `PARCKeyedElement`'s key.
 *
 * @param [in] keyedElement A pointer to the `PARCKeyedElement` whose key length should be returned.
 *
 * @return The length of the retrieved key.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
size_t parcKeyedElement_GetKeyLen(PARCKeyedElement *keyedElement);
#endif // libparc_parc_KeyedElement_h
