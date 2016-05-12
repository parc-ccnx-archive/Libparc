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
 * @file parc_Map.h
 * @ingroup datastructures
 * @brief An object that maps keys to values.
 *
 * A map cannot contain duplicate keys; each key can map to at most one value.
 *
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#ifndef libparc_parc_Map_h
#define libparc_parc_Map_h
#include <stdbool.h>

struct parc_map;
typedef struct parc_map PARCMap;

typedef struct parc_map_interface {
    /**
     * Removes all of the mappings from this map.
     *
     * The map will be empty after this call returns.
     *
     * @param [in,out] map The instance of `PARCMap` to be cleared of mappings
     *
     *
     * Example:
     * @code
     * {
     *     <#example#>
     * }
     * @endcode
     *
     */
    void (*parcMap_Clear)(PARCMap *map);

    /**
     * Returns true if this map contains a mapping for the specified key.
     *
     * @param [in] map A pointer to the instance of `PARCMap` to check
     * @param [in] key A pointer to the key to check for in @p map
     *
     * @return True if the map cnatins a mapping for the specified key
     *
     * Example:
     * @code
     * {
     *     <#example#>
     * }
     * @endcode
     */
    bool (*parcMap_ContainsKey)(PARCMap *map, void *key);

    /**
     * Returns true if this map maps one or more keys to the specified value.
     *
     * @param [in] map A pointer to the instance of `PARCMap` to check
     * @param [in] value A pointer to the value to check for in @p map
     *
     * @return True if the map contains one or more keys that map to @p value.
     *
     * Example:
     * @code
     * {
     *     <#example#>
     * }
     * @endcode
     */
    bool (*parcMap_ContainsValue)(PARCMap *map, void *value);

    /**
     * Compares the specified object with this map for equality.
     *
     * @param [in] map A pointer to the instance of `PARCMap` to check
     * @param [in] other A pointer to the other instance of `PARCMap` to compare
     * @return  True is the two maps are equal.
     *
     * Example:
     * @code
     * {
     *     <#example#>
     * }
     * @endcode
     *
     */
    bool (*parcMap_Equals)(PARCMap *map, void *other);

    /**
     * Returns the value to which the specified key is mapped, or null if this map contains no mapping for the key.
     *
     * @param [in] map A pointer to the instance of `PARCMap` to check
     * @param [in] key A pointer to the key to check for in @p map
     *
     * @return NULL If the @p key is not present in @p map
     * @return NOT NULL The value to which the @p key is mapped.
     *
     * Example:
     * @code
     * {
     *     <#example#>
     * }
     * @endcode
     *
     */
    void *(*parcMap_Get)(PARCMap * map, void *key);

    /**
     * Returns the hash code value for this map.
     *
     * @param [in] map A pointer to the instance of `PARCMap` to hash
     *
     * @return The hash of the instance of `PARCMap`
     *
     * Example:
     * @code
     * {
     *     <#example#>
     * }
     * @endcode
     */
    int (*parcMap_HashCode)(PARCMap *map);

    /**
     * Returns true if this map contains no key-value mappings.
     *
     *
     * @param [in] map A pointer to the instance of `PARCMap` to check
     *
     * @return True if the map contains no mappings.
     *
     * Example:
     * @code
     * {
     *     <#example#>
     * }
     * @endcode
     */
    bool (*parcMap_IsEmpty)(PARCMap *map);

    /**
     * Associates the specified value with the specified key in this map (optional operation).
     *
     * @param [in,out] map A pointer to the instance of `PARCMap` in which to insert @p value at @p key.
     * @param [in] key A pointer to the key in @p map in which to insert @p value.
     * @param [in] value A pointer to the the value to insert at @p key in @p map.
     *
     * @return
     *
     * Example:
     * @code
     * {
     *     <#example#>
     * }
     * @endcode
     */
    void *(*parcMap_Put)(PARCMap * map, void *key, void *value);

    /**
     * Copies all of the mappings from the specified map to this map (optional operation).
     *
     * @param [in,out] map The instance of `PARCMap` to be modified.
     * @param [in] other The instance of `PARCMap` whose mappings should be copied to @p map.
     *
     * Example:
     * @code
     * {
     *     <#example#>
     * }
     * @endcode
     */
    void (*parcMap_PutAll)(PARCMap *map, PARCMap *other);

    /**
     * Removes the mapping for a key from this map if it is present (optional operation).
     *
     * @param [in,out] map The instance of `PARCMap` to be modified.
     * @param [in] key The key to the mapping to be removed
     *
     * @return
     *
     * Example:
     * @code
     * {
     *     <#example#>
     * }
     * @endcode
     */
    void *(*parcMap_Remove)(PARCMap * map, void *key);

    /**
     * Returns the number of key-value mappings in this map.
     *
     * @param [in,out] map The instance of `PARCMap` to be inspected.
     *
     * @return int The number of mappings in the map
     *
     * Example:
     * @code
     * {
     *     <#example#>
     * }
     * @endcode
     *
     */
    int (*parcMap_Size)(PARCMap *map);
} PARCMapInterface;

/**
 * Create a PARCMap instance.
 *
 * Create an instance of `PARCMap` wrapping the given pointer to a base map
 * interface and the {@ link PARCMapInterface} structure containing pointers
 * to functions performing the actual Map operations.
 *
 * @param [in] map A pointer to the structure for the new instance of `PARCMap`
 * @param [in] interface A pointer to the instance of `PARCMapInterface`
 * @return A new instance of `PARCMap`
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
PARCMap *parcMap_Create(void *map, PARCMapInterface *interface);

/**
 * Removes all of the mappings from this map.
 *
 * The map will be empty after this call returns.
 *
 * @param [in,out] map A pointer to the instance of `PARCMap` to be cleared.
 *
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
void parcMap_Clear(PARCMap *map);

/**
 * Returns true if this map contains a mapping for the specified key.
 *
 * @param [in] map A pointer to the instance of `PARCMap` to be checked.
 * @param [in] key A pointer to the key to be checked for in @p map
 *
 * @return True if the specified key is found in the map.
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
bool parcMap_ContainsKey(PARCMap *map, void *key);

/**
 * Returns true if this map maps one or more keys to the specified value.
 *
 * @param [in] map A pointer to the instance of `PARCMap` to be checked.
 * @param [in] value A pointer to the value to be checked for in @p map
 *
 * @return True if the specified value has one or more keys pointing to it.
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
bool parcMap_ContainsValue(PARCMap *map, void *value);

/**
 * Determine if two `PARCMap` instances are equal.
 *
 * Two `PARCMap` instances are equal if, and only if, the maps have the same
 * number of elements, all of the keys are equal and the values to which they point are equal
 *
 * The following equivalence relations on non-null `PARCMap` instances are maintained:
 *
 *  * It is reflexive: for any non-null reference value x, `PARCMap_Equals(x, x)`
 *      must return true.
 *
 *  * It is symmetric: for any non-null reference values x and y,
 *    `parcMap_Equals(x, y)` must return true if and only if
 *        `parcMap_Equals(y, x)` returns true.
 *
 *  * It is transitive: for any non-null reference values x, y, and z, if
 *        `parcMap_Equals(x, y)` returns true and
 *        `parcMap_Equals(y, z)` returns true,
 *        then  `parcMap_Equals(x, z)` must return true.
 *
 *  * It is consistent: for any non-null reference values x and y, multiple
 *      invocations of `parcMap_Equals(x, y)` consistently return true or
 *      consistently return false.
 *
 *  * For any non-null reference value x, `parcMap_Equals(x, NULL)` must
 *      return false.
 *
 * @param [in] map A pointer to a `PARCMap` instance.
 * @param [in] other A pointer to a `PARCMap` instance.
 * @return true if the two `PARCMap` instances are equal.
 *
 * Example:
 * @code
 * {
 *    PARCMap *a = parcMap_Create();
 *    PARCMap *b = parcMap_Create();
 *
 *    if (parcMap_Equals(a, b)) {
 *        // true
 *    } else {
 *        // false
 *    }
 * }
 * @endcode
 */
bool parcMap_Equals(PARCMap *map, void *other);

/**
 * Returns the value to which the specified key is mapped, or null if this map contains no mapping for the key.
 *
 * @param [in] map A pointer to the instance of `PARCMap` to be checked.
 * @param [in] key A pointer to the key to be checked for which the value is to be returned.
 *
 * @return Null if no mapping for @p key exists
 * @return Non Null A pointer to the value for @p key
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
void *parcMap_Get(PARCMap *map, void *key);

/**
 * Returns the hash code value for this map.
 *
 * @param [in] map A pointer to the instance of `PARCMap` to be hashed.
 *
 * @return The hash value for the @p map
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
int parcMap_HashCode(PARCMap *map);

/**
 * Returns true if this map contains no key-value mappings.
 *
 * @param [in] map A pointer to the instance of `PARCMap` to be checked.
 *
 * @return True if the @p map is empty. else false.
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
bool parcMap_IsEmpty(PARCMap *map);

/**
 * Associates the specified value with the specified key in this map (optional operation).
 *
 * @param [in,out] map A pointer to the instance of `PARCMap` into which the key,value pair should be inserted.
 * @param [in] key A pointer to the key to be inserted in @p map
 * @param [in] value A pointer to the value to be inserted in @p map at @p key
 *
 * @return The previous value at @p key if one exists, else NULL
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
void *parcMap_Put(PARCMap *map, void *key, void *value);

/**
 * Copies all of the mappings from the specified map to this map (optional operation).
 *
 * @param [in,out] map The map into which all the mappings from @p other should be copied.
 * @param [in] other The instance of `PARCMap` whose mappings should be copied into @p map
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
void parcMap_PutAll(PARCMap *map, PARCMap *other);

/**
 * Removes the mapping for a key from this map if it is present (optional operation).
 *
 *
 * @param [in,out] map The instance of `PARCMap` in which @p key should be removed if present.
 * @param [in] key The pointer to the key representing the mapping that should be removed from @p map.
 *
 * @return A pointer to the value previously mapped to @p key, if @p key exists.
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
void *parcMap_Remove(PARCMap *map, void *key);

/**
 * Returns the number of key-value mappings in this map.
 *
 * @param [in,out] map The instance of `PARCMap` to be measured
 *
 * @return  The number of mappings in @p map.
 *
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
int parcMap_Size(PARCMap *map);
#endif // libparc_parc_Map_h
