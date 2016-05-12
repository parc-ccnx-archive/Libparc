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
 * @file parc_KeyedElement.h
 * @ingroup datastructures
 * @brief A Pointer and a Key
 *  The `PARCKeyedElement` is a simple pointer and key tuple.
 *
 * @author Nacho "Ignacio" Solis, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
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
