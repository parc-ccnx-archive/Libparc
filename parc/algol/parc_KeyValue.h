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
 * @file parc_KeyValue.h
 * @ingroup datastructures
 * @brief A key and value tuple.
 *
 * The `PARCKeyValue` is a simple key-value tuple.
 *
 * @author Nacho "Ignacio" Solis, Michael Slominski, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libparc_parc_KeyValue_h
#define libparc_parc_KeyValue_h

#include "parc_Object.h"

struct parc_key_value;

/**
 * @typedef `PARCKeyValue`
 * @brief A `PARCKeyValue` is a tuple consisting of a key and a value
 */
typedef struct parc_key_value PARCKeyValue;

/**
 * Create a `PARCKeyValue` Element. The key and value must be PARCObjects.
 *
 * Neither the data nor the key will be copied, but referenced.
 * The key may not be `NULL`.
 *
 * @param [in] key A pointer to the key data
 * @param [in] value A pointer to the value data
 *
 * Example:
 * @code
 * {
 *      ...
 *      PARCKeyValue *kv = parcKeyValue_Create(key, value);
 *      ...
 *      parcKeyValue_Release(&kv);
 * }
 * @endcode
 */
PARCKeyValue *parcKeyValue_Create(const PARCObject *key,
                                  const PARCObject *value);

/**
 * Create a copy of a `PARCKeyValue` Element.
 *
 * The source PARCKeyValue may not be `NULL`.
 *
 * @param [in] keyValue Source PARCKeyValue element
 *
 * Example:
 * @code
 * {
 *      ...
 *      PARCKeyValue *keyValueCopy = parcKeyValue_Copy(sourcKeyValue);
 *      ...
 *      parcKeyValue_Release(&keyValueCopy);
 * }
 * @endcode
 */
PARCKeyValue *parcKeyValue_Copy(const PARCKeyValue *keyValue);

/**
 * Acquire a reference to a `PARCKeyValue` Element.
 *
 * The source PARCKeyValue may not be `NULL`.
 *
 * @param [in] keyValue Source PARCKeyValue element
 *
 * Example:
 * @code
 * {
 *      ...
 *      PARCKeyValue *keyValue = parcKeyValue_Acquire(sourcKeyValue);
 *      ...
 *      parcKeyValue_Release(&keyValue);
 * }
 * @endcode
 */
PARCKeyValue *parcKeyValue_Acquire(const PARCKeyValue *keyValue);

/**
 * Release a `PARCKeyValue`.
 *
 * Releases a reference to a PARCKeyValue element, if it is the last reference then the
 * contained key & value objects are released and the elements memory is freed.
 *
 * @param [in,out] keyValuePointer A pointer to the pointer to the `PARCKeyValue` to be released
 *
 * Example:
 * @code
 *      ...
 *      PARCKeyValue *kv = parcKeyValue_Create(key, value);
 *      ...
 *      parcKeyValue_Release(&kv);
 * @endcode
 */
void parcKeyValue_Release(PARCKeyValue **keyValuePointer);

/**
 * Set the value for a `PARCKeyValue`.
 * The previous value will be released.
 *
 * @param [in,out] keyValue A pointer to the keyValue
 * @param [in] value A pointer to the new value
 *
 * Example:
 * @code
 *      ...
 *      PARCKeyValue *kv = parcKeyValue_Create(key, value);
 *      ...
 *      parcKeyValue_SetKey(kv, newKey);
 *      ...
 *      parcKeyValue_Release(&kv);
 * @endcode
 */
void parcKeyValue_SetValue(PARCKeyValue *keyValue, PARCObject *value);

/**
 * Set the key for a `PARCKeyValue`.
 * The previous key will be released.
 *
 * @param [in,out] keyValue A pointer to the `PARCKeyValue`
 * @param [in] key A pointer to the new key
 *
 * Example:
 * @code
 *      ...
 *      PARCKeyValue *kv = parcKeyValue_Create(key, value);
 *      ...
 *      parcKeyValue_SetValue(kv, newValue);
 *      ...
 *      parcKeyValue_Release(&kv);
 * @endcode
 */
void parcKeyValue_SetKey(PARCKeyValue *keyValue, PARCObject *key);

/**
 * Get the value pointer for the `PARCKeyValue`. No extra reference is created.
 *
 * @param [in] keyValue A pointer to the `PARCKeyValue`
 *
 * Example:
 * @code
 *      ...
 *      PARCKeyValue *kv = parcKeyValue_Create(key, value);
 *      ...
 *      PARCObject *value = parcKeyValue_GetValue(kv);
 *      ...
 *      parcKeyValue_Release(&kv);
 * @endcode
 */
PARCObject *parcKeyValue_GetValue(PARCKeyValue *keyValue);

/**
 * Get the key pointer for the `PARCKeyValue`. No extra reference is created.
 *
 * @param [in] keyValue A pointer to the `PARCKeyValue`
 *
 * Example:
 * @code
 *      ...
 *      PARCKeyValue *kv = parcKeyValue_Create(key, value);
 *      ...
 *      PARCObject *key = parcKeyValue_GetKey(kv);
 *      ...
 *      parcKeyValue_Release(&kv);
 * @endcode
 */
PARCObject *parcKeyValue_GetKey(PARCKeyValue *keyValue);

/**
 * Check for Key equality.  Return true if equal.
 *
 * @param [in] keyValue1 A pointer to the first `PARCKeyValue`
 * @param [in] keyValue2 A pointer to the second `PARCKeyValue`
 * @return true if the keyValues have the same key
 *
 * Example:
 * @code
 *      ...
 *      PARCKeyValue *kv1 = parcKeyValue_Create(key1, value);
 *      PARCKeyValue *kv2 = parcKeyValue_Create(key2, value);
 *      ...
 *      if (parcKeyValue_EqualKeys(kv1, kv2)) {
 *         ...
 *      }
 *      ...
 *      parcKeyValue_Release(&kv1);
 *      parcKeyValue_Release(&kv2);
 * @endcode
 */
bool parcKeyValue_EqualKeys(const PARCKeyValue *keyValue1, const PARCKeyValue *keyValue2);

/**
 * Check for element equality.  Return true if both key & value are equal.
 *
 * @param [in] keyValue1 A pointer to the first `PARCKeyValue`
 * @param [in] keyValue2 A pointer to the second `PARCKeyValue`
 * @return true if the keys & values both have the same value
 *
 * Example:
 * @code
 *      ...
 *      PARCKeyValue *kv1 = parcKeyValue_Create(key1, value);
 *      PARCKeyValue *kv2 = parcKeyValue_Create(key2, value);
 *      ...
 *      if (parcKeyValue_Equals(kv1, kv2)) {
 *         ...
 *      }
 *      ...
 *      parcKeyValue_Release(&kv1);
 *      parcKeyValue_Release(&kv2);
 * @endcode
 */
bool parcKeyValue_Equals(const PARCKeyValue *keyValue1, const PARCKeyValue *keyValue2);

/**
 * Compare PARCKeyValue elements.  Return an int result based on key compare only.
 *
 * @param [in] keyValue1 A pointer to the first `PARCKeyValue`
 * @param [in] keyValue2 A pointer to the second `PARCKeyValue`
 * @return parcObject_Compare(keyValue1->key, keyValue2->key)
 *
 * Example:
 * @code
 *      ...
 *      PARCKeyValue *kv1 = parcKeyValue_Create(key1, value);
 *      PARCKeyValue *kv2 = parcKeyValue_Create(key2, value);
 *      ...
 *      if (parcKeyValue_Compare(kv1, kv2) > 0) {
 *         ...
 *      }
 *      ...
 *      parcKeyValue_Release(&kv1);
 *      parcKeyValue_Release(&kv2);
 * @endcode
 */
int parcKeyValue_Compare(const PARCKeyValue *keyValue1, const PARCKeyValue *keyValue2);

/**
 * Return the HashCode of the PARCKeyValue key.
 *
 * @param [in] keyValue A pointer to the first `PARCKeyValue`
 * @return parcObject_HashCode(keyValue->key);
 *
 * Example:
 * @code
 *      ...
 *      PARCKeyValue *keyValue = parcKeyValue_Create(key1, value);
 *      ...
 *      PARCHashCode *hashCode = parcKeyValue_HashCode(keyValue);
 *      ...
 *      parcKeyValue_Release(&keyValue);
 * @endcode
 */
PARCHashCode parcKeyValue_HashCode(const PARCKeyValue *keyValue);
#endif // libparc_parc_KeyValue_h
