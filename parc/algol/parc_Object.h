/*
 * Copyright (c) 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @file parc_Object.h
 * @ingroup memory
 * @brief Reference Counted Object Memory
 *
 * An arbitrary C structure stored in allocated memory with a reference counter.
 *
 * When a PARC Object is created, via `parcObject_Create`, or `parcObject_Copy` it has reference count of 1.
 *
 * References to a PARC Object are acquired by calling `parcObject_Acquire`
 * and once a reference is no longer needed it is released via parcObject_Release`.
 * When the last reference is released, the memory storing the PARC Object is deallocated.
 * Any further reference to that object or its memory is undefined.
 *
 * When creating a PARCObject, the caller may supply an instance of `PARCObjectDescriptor` containing
 * configuration information used and pointers to functions that are invoked during the lifecycle of the PARC Object.
 * Notable functions that are a finalization cleanup function for an object that will be deallocated,
 * a clone function for an object that is being cloned,
 * and a string generator for an object that is implementing the `ToString` function.
 * Implementors of modules that use PARCObject supply a specification of callback
 * functions that implement specific behaviours for interfaces using PARC Object.
 *
 * @author Glenn Scott, Mike Slominski, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libparc_parc_Object_h
#define libparc_parc_Object_h

#include <stdint.h>
#include <time.h>

#include <LongBow/runtime.h>
#include <LongBow/longBow_Compiler.h>

#include <parc/algol/parc_CMacro.h>
#include <parc/algol/parc_JSON.h>
#include <parc/algol/parc_HashCode.h>

#ifdef PARCLibrary_DISABLE_VALIDATION
#  define parcObject_OptionalAssertValid(_instance_)
#else
#  define parcObject_OptionalAssertValid(_instance_) parcObject_AssertValid(_instance_)
#endif

typedef uint64_t PARCReferenceCount;

//Switch to strong type after subclasses are converted
//typedef struct {int unused;} PARCObject;
typedef void PARCObject;

/**
 * A Function that performs the final cleanup and resource deallocation when
 * a PARC Object has no more references.
 */
typedef void (PARCObjectDestroy)(PARCObject **);

/**
 * A Function that performs the final cleanup and resource deallocation when
 * a PARC Object has no more references.
 *
 * If the function returns `true` the object is automatically deallocated and destroyed.
 * If the function returns `false` the object is not automatically deallocated and destroyed,
 * and the responsibility for the object's state and memory are the responsibility of
 * the `PARCObjectDestructor` function.
 */
typedef bool (PARCObjectDestructor)(PARCObject **);

/**
 * A Function that releases one reference to the given PARC Object.
 * On the final release, where the number of references becomes 0,
 * the PARCObjectDestroy function is invoked.
 */
typedef void (PARCObjectRelease)(PARCObject **objectPointer);

/**
 * A function that produces a deep-copy of the given PARC Object instance.
 */
typedef PARCObject *(PARCObjectCopy)(const PARCObject *object);

/**
 * A function that invokes the proper _Equals() function for two PARC Object instances.
 */
typedef bool (PARCObjectEquals)(const PARCObject *objectX, const PARCObject *objectY);

/**
 * A function that invokes the proper _Compare() functions for two PARC Object instances.
 */
typedef int (PARCObjectCompare)(const PARCObject *, const PARCObject *);

/**
 * A function that computes the 32-bit hashcode of the given PARC Object instance.
 */
typedef PARCHashCode (PARCObjectHashCode)(const PARCObject *object);

/**
 * A function that produces a C style nul-terminated string representation of the given PARC Object instance.
 */
typedef char *(PARCObjectToString)(const PARCObject *object);

/**
 * A function that displays a human readable representation of the given PARCObject.
 */
typedef void (PARCObjectDisplay)(const PARCObject *object, const int indentation);

/**
 * A function that generates a JSON representation of the given PARC Object instance.
 */
typedef PARCJSON *(PARCObjectToJSON)(const PARCObject *);

typedef struct PARCObjectDescriptor {
    char name[32];
    PARCObjectDestroy *destroy;
    PARCObjectDestructor *destructor;
    PARCObjectRelease *release;
    PARCObjectCopy *copy;
    PARCObjectToString *toString;
    PARCObjectEquals *equals;
    PARCObjectCompare *compare;
    PARCObjectHashCode *hashCode;
    PARCObjectToJSON *toJSON;
    PARCObjectDisplay *display;
    struct PARCObjectDescriptor *super;
} PARCObjectDescriptor;

/*!
 * @define parcObject_DescriptorName(_type)
 * Creates a subtype specific name for a subtype's PARCObjectDescriptor
 * struct which is a parameter to parcObject_Create.
 */
#define parcObject_MetaName(_type) parcCMacro_Cat(_type, _Descriptor)
#define parcObject_DescriptorName(_type) parcCMacro_Cat(_type, _Descriptor)

extern PARCObjectDescriptor parcObject_DescriptorName(PARCObject);

/**
 * Assert that an instance of PARC Object is valid.
 *
 * If the instance is not valid, terminate via an assertion.
 *
 * Valid means the internal state of the type is consistent with its
 * required current or future behaviour.
 * This may include the validation of internal instances of types.
 *
 * @param [in] object A pointer to a PARC Object instance.
 */
void parcObject_AssertValid(const PARCObject *object);

/**
 * Determine if an instance of PARCObject is valid.
 *
 * A valid PARCObject has non-NULL, it has a reference count > 0,
 * it is non-zero in length, and has a valid alignment.
 *
 * @param [in] object A pointer to a PARC Object instance.
 *
 * @return true The PARCObject is valid.
 * @return true The PARCObject is invalid.
 */
bool parcObject_IsValid(const PARCObject *object);

/**
 * Create a new reference counted object that is a deep copy of the specified object,
 * if possible, or, otherwise, a shallow copy of the object's total allocation memory.
 *
 * The reference count for the new object is 1.
 *
 * @param [in] object A pointer to the original object.
 *
 * @return NULL Memory could not be allocated.
 * @return non-NULL A pointer to a new reference counted object.
 *
 * Example:
 * @code
 * {
 *     struct timeval time = { .tv_sec = 1, .tv_usec = 2 };
 *
 *     struct timeval *t = parcObject_Copy(&time, sizeof(time), NULL);
 * }
 * @endcode
 *
 * @see parcObject_Release
 */
PARCObject *parcObject_Copy(const PARCObject *object);

/**
 * Compare two object instances.
 *
 * The comparison function in the first `PARCObjectDescriptor` parameter is used for comparison.
 * The objects are expected to be of the same type. Thus, if the comparison function
 * associated with the first `PARCObjectDescriptor` function is NULL, it is assumed the
 * same holds for the second parameter. In this case, the instance pointers are compared.
 *
 * @param [in] x An object.
 * @param [in] y An object.
 *
 * @return int The result of the comparison.
 *
 * Example:
 * @code
 * {
 *     struct timeval *t = parcObject_Create(sizeof(struct timeval), NULL);
 *
 *     int compareResult = parcObject_Compare(t, t);
 *     printf("0? %d\n", compareResult);
 *
 *     parcObject_Release(&t);
 * }
 * @endcode
 */
int parcObject_Compare(const PARCObject *x, const PARCObject *y);

/**
 * Determine if two PARCObject instances are equal.
 *
 * Two PARCObject instances are equal if, and only if, the instance pointers are equal.
 *
 * The following equivalence relations on non-null `PARCObject` instances are maintained:
 *
 *  * It is reflexive: for any non-null reference value x, `PARCObject_Equals(x, x)`
 *      must return true.
 *
 *  * It is symmetric: for any non-null reference values x and y,
 *    `parcObject_Equals(x, y)` must return true if and only if
 *        `parcObject_Equals(y, x)` returns true.
 *
 *  * It is transitive: for any non-null reference values x, y, and z, if
 *        `parcObject_Equals(x, y)` returns true and
 *        `parcObject_Equals(y, z)` returns true,
 *        then  `parcObject_Equals(x, z)` must return true.
 *
 *  * It is consistent: for any non-null reference values x and y, multiple
 *      invocations of `parcObject_Equals(x, y)` consistently return true or
 *      consistently return false.
 *
 *  * For any non-null reference value x, `parcObject_Equals(x, NULL)` must
 *      return false.
 *
 * @param x A pointer to a `PARCObject` instance.
 * @param y A pointer to a `PARCObject` instance.
 * @return true if the two `PARCObject` instances are equal.
 *
 * Example:
 * @code
 * {
 *    PARCObject *a = parcObject_Create();
 *    PARCObject *b = parcObject_Create();
 *
 *    if (parcObject_Equals(a, b)) {
 *        // true
 *    } else {
 *        // false
 *    }
 * }
 * @endcode
 */
bool parcObject_Equals(const PARCObject *x, const PARCObject *y);

/**
 * Retrieve the hashcode of the given object.
 *
 * If no object implementation is provided, the hashcode is the 32-bit address
 * of the base object pointer. Otherwise, the hashcode is computed by the
 * provided hashcode function.
 *
 * @param [in] object An object.
 *
 * @return uint32_t The object hashcode
 *
 * Example:
 * @code
 * {
 *     struct timeval *t = parcObject_Create(sizeof(struct timeval), NULL);
 *
 *     uint32_t hashcode = parcObject_HashCode(t);
 *     printf("Hashcode = %d\n", hashcode);
 *
 *     parcObject_Release(&t);
 * }
 * @endcode
 */
PARCHashCode parcObject_HashCode(const PARCObject *object);

/**
 * Create a C string containing a human readable representation of the given object.
 *
 * @param [in] object The object from which a string representation will be generated.
 *
 * @return NULL Memory could not be allocated to contain the C string.
 * @return non-NULL An allocated C string that must be deallocated via parcMemory_Deallocate().
 *
 * Example:
 * @code
 * {
 *     struct timeval *t = parcObject_Create(sizeof(struct timeval), NULL);
 *
 *     char *string = parcObject_ToString(t);
 *     printf("%s\n", string);
 *     parcMemory_Deallocate((void **)&string);
 *
 *     parcObject_Release(&t);
 * }
 * @endcode
 */
char *parcObject_ToString(const PARCObject *object);

/**
 * Create a `PARCJSON` instance (representation) of the given object.
 *
 * @param [in] object The object from which a JSON instance will be generated.
 *
 * @return NULL Memory could not be allocated to contain the `PARCJSON` instance.
 * @return non-NULL An allocated C string that must be deallocated via parcMemory_Deallocate().
 *
 * Example:
 * @code
 * {
 *     struct timeval *t = parcObject_Create(sizeof(struct timeval), NULL);
 *
 *     PARCJSON *json = parcObject_ToJSON(t);
 *     printf("JSON representation: %s\n", parcJSON_ToString(json));
 *     parcJSON_Release(&json);
 *
 *     parcObject_Release(&t);
 * }
 * @endcode
 */
PARCJSON *parcObject_ToJSON(const PARCObject *object);

/**
 * Acquire a new reference to an object.
 *
 * The reference count to the object is incremented.
 *
 * @param [in] object The object to which to refer.
 *
 * @return The same value as the input parameter @p object
 *
 * Example:
 * @code
 * {
 *     struct timeval *t = parcObject_Create(sizeof(struct timeval), NULL);
 *
 *     struct timeval *new = parcObject_Acquire(t);
 *
 *     parcObject_Release(&t);
 *     parcObject_Release(&new);
 * }
 * @endcode
 */
PARCObject *parcObject_Acquire(const PARCObject *object);

/**
 * Release a previously acquired reference to the specified instance,
 * decrementing the reference count for the instance.
 *
 * The pointer to the instance is set to NULL as a side-effect of this function.
 *
 * If an invocation of `parcObject_Release` causes the last reference to
 * the instance to be released, it calls the instance's `finalize` function
 * specified in the `PARCObjectDescriptor` structure supplied when the instance
 * was created (see `parcObject_Create`, and the instance is deallocated.
 *
 * The contents of the dealloced memory used for the PARC object are undefined.
 * Do not reference the object after the last release.
 *
 * @param [in] objectPointer A pointer to a pointer to the instance to release.
 *
 * @return The number of remaining references to the object.
 *
 * Example:
 * @code
 * {
 *     struct timeval *t = parcObject_Create(sizeof(struct timeval), NULL);
 *
 *     parcObject_Release(&t);
 * }
 * @endcode
 *
 * @see parcObject_Create
 * @see parcObject_Acquire
 */
PARCReferenceCount parcObject_Release(PARCObject **objectPointer);

/**
 * Get the current `PARCReferenceCount` for the specified object.
 *
 * The reference count must always be greater than zero.
 *
 * @param [in] object A pointer to a valid `PARCObject` instance.
 *
 * @return The current reference count for the specified object.
 *
 * Example:
 * @code
 * {
 *     struct timeval *t = parcObject_Create(sizeof(struct timeval), NULL);
 *
 *     PARCReferenceCount count = parcObject_GetReferenceCount(object);
 *
 *     parcObject_Release(&t);
 * }
 * @endcode
 *
 * @see parcObject_Acquire
 * @see parcObject_Release
 */
PARCReferenceCount parcObject_GetReferenceCount(const PARCObject *object);

/**
 * Print a human readable representation of the given `PARC Object.
 *
 * @param [in] object A pointer to the instance to display.
 * @param [in] indentation The level of indentation to use to pretty-print the output.
 *
 * Example:
 * @code
 * {
 *     struct timeval *t = parcObject_Create(sizeof(struct timeval), NULL);
 *
 *     parcObject_Display(t, 0);
 *
 *     parcObject_Release(&t);
 * }
 * @endcode
 *
 */
void parcObject_Display(const PARCObject *object, const int indentation);

/**
 * <#One Line Description#>
 *
 * <#Paragraphs Of Explanation#>
 *
 * @param [in] object A pointer to a valid PARCObject instance
 * @param [in] objectType A pointer to a valid PARCObjectDescriptor instance
 *
 * @return The previous value of the given PARCObject's PARCObjectDescriptor.
 */
PARCObjectDescriptor *parcObject_SetDescriptor(PARCObject *object, const PARCObjectDescriptor *objectType);

/**
 * @def parcObject_MetaInitialize
 * @deprecated Use parcObject_ExtendPARCObject instead;
 *
 * Initialize a PARCObjectDescriptor structure. Every function pointer is set to NULL.
 *
 * @param [in] objectType A pointer to the PARCObjectDescriptor structure to initialize.
 *
 * @return The pointer to the initialized PARCObjectDescriptor.
 */
#define parcObject_MetaInitialize(objectType) \
    (objectType)->destructor = NULL, \
    (objectType)->destroy = NULL, \
    (objectType)->release = NULL, \
    (objectType)->copy = NULL, \
    (objectType)->toString = NULL, \
    (objectType)->equals = NULL, \
    (objectType)->compare = NULL, \
    (objectType)->hashCode = NULL, \
    (objectType)->toJSON = NULL, \
    (objectType)->display = NULL, \
    (objectType)->super = NULL

/**
 * Create an allocated instance of `PARCObjectDescriptor`.
 *
 * @param [in] name    A nul-terminated, C string containing the name of the object descriptor.
 * @param [in] destructor The callback function to call when the last `parcObject_Release()` is invoked (replaces @p destroy).
 * @param [in] release The callback function to call when `parcObject_Release()` is invoked.
 * @param [in] copy The callback function to call when parcObject_Copy() is invoked.
 * @param [in] toString The callback function to call when `parcObject_ToString()` is invoked.
 * @param [in] equals The callback function to call when `parcObject_Equals()` is invoked.
 * @param [in] compare The callback function to call when `parcObject_Compare()` is invoked.
 * @param [in] hashCode The callback function to call when `parcObject_HashCode()` is invoked.
 * @param [in] toJSON The callback function to call when `parcObject_ToJSON()` is invoked.
 * @param [in] display The callback function to call when `parcObject_Display()` is invoked.
 * @param [in] descriptor A pointer to a PARCObjectDescriptor for the supertype of created PARCObjectDescriptor
 *
 * @return NULL Memory could not be allocated to store the `PARCObjectDescriptor` instance.
 * @return non-NULL Successfully created the implementation
 */
PARCObjectDescriptor *parcObjectDescriptor_Create(const char *name,
                                                  PARCObjectDestructor *destructor,
                                                  PARCObjectRelease *release,
                                                  PARCObjectCopy *copy,
                                                  PARCObjectToString *toString,
                                                  PARCObjectEquals *equals,
                                                  PARCObjectCompare *compare,
                                                  PARCObjectHashCode *hashCode,
                                                  PARCObjectToJSON *toJSON,
                                                  PARCObjectDisplay *display,
                                                  PARCObjectDescriptor *descriptor);

void parcObjectDescriptor_Destroy(PARCObjectDescriptor **descriptorPointer);


/// Helper MACROS for PARCObject Normalization
/** \cond */
/**
 * parcObject_DestroyWrapper builds the boiler plate wrapper for PARCObject type conversion in the
 * destroy operation. Intended for internal use.
 */
#define parcObject_DestroyWrapper(_type, _fname)                \
    static void _autowrap_destroy_##_type(PARCObject **object)  \
    {                                                           \
        _fname((_type **) object);                              \
    }

/**
 * parcObject_DestructorWrapper builds the boiler plate wrapper for PARCObject type conversion in the
 * destroy operation. Intended for internal use.
 */
#define parcObject_DestructorWrapper(_type, _fname)               \
    static bool _autowrap_destructor_##_type(PARCObject **object) \
    {                                                             \
        return _fname((_type **) object);                         \
    }

/**
 * parcObject_CopyWrapper builds the boiler plate wrapper for PARCObject type conversion in the
 * copy operation. Intended for internal use.
 */
#define parcObject_CopyWrapper(_type, _fname)                           \
    static PARCObject *_autowrap_copy_##_type(const PARCObject *object) \
    {                                                                   \
        return (PARCObject *) _fname((const _type *) object);           \
    }

/**
 * parcObject_ToStringWrapper builds the boiler plate wrapper for PARCObject type conversion in the
 * ToString operation. Intended for internal use.
 */
#define parcObject_ToStringWrapper(_type, _fname) \
    static char *_autowrap_toString_##_type(const PARCObject * object)  \
    {                                                                   \
        return _fname((const _type *) object);                          \
    }

/**
 * parcObject_EqualsWrapper builds the boiler plate wrapper for PARCObject type conversion in the
 * equals operation. Intended for internal use.
 */
#define parcObject_EqualsWrapper(_type, _fname)                         \
    static bool _autowrap_equals_##_type(const PARCObject *a, const PARCObject *b) \
    {                                                                   \
        return _fname((const _type *) a, (const _type *) b);            \
    }

/**
 * parcObject_CompareWrapper builds the boiler plate wrapper for PARCObject type conversion in the
 * compare operation. Intended for internal use.
 */
#define parcObject_CompareWrapper(_type, _fname) \
    static int _autowrap_compare_##_type(const PARCObject *a, const PARCObject *b) \
    {                                                                   \
        return _fname((const _type *) a, (const _type *) b);                        \
    }

/**
 * parcObject_HashCodeWrapper builds the boiler plate wrapper for PARCObject type conversion in the
 * HashCode operation. Intended for internal use.
 */
#define parcObject_HashCodeWrapper(_type, _fname) \
    static PARCHashCode _autowrap_hashCode_##_type(const PARCObject *object) \
    {                                                                   \
        return _fname((const _type *) object);                                \
    }

/**
 * parcObject_CopyWrapper builds the boiler plate wrapper for PARCObject type conversion in the
 * ToJSON operation. Intended for internal use.
 */
#define parcObject_ToJSONWrapper(_type, _fname) \
    static PARCJSON *_autowrap_toJSON_##_type(const PARCObject *object) \
    {                                                                   \
        return _fname((const _type *) object);                          \
    }

/**
 * parcObject_DisplayyWrapper builds the boiler plate wrapper for PARCObject type conversion in the
 * Display operation. Intended for internal use.
 */
#define parcObject_DisplayWrapper(_type, _fname) \
    static void _autowrap_Display_##_type(const PARCObject *object, const int indentation) \
    {                                                                                       \
        _fname((const _type *) object, indentation);                                        \
    }

/**
 * _autowrap_NULL is a part of the c-macro trick for implement a macro If-Else switch.
 * If included as a macro parameter it inserts a comma into the parameter list for that macro.
 * This can be used by a switching macro that always resolves to the nth element and the
 * presence of a comma generating macro changes which element is the nth. When NULL is used
 * as a parameter in a call to "ExtendPARCObject", _autowrap_NULL will be the name generated which
 * expands to a comma.
 */
#define _autowrap_NULL(...) ,

/** \endcond */

#define parcObject_Override(_subtype, _superType, ...) \
    LongBowCompiler_IgnoreInitializerOverrides \
    static const PARCObjectDescriptor parcObject_DescriptorName(_subtype) = {          \
        .destroy = NULL,    \
        .destructor = NULL, \
        .release  = NULL,   \
        .copy     = NULL,   \
        .toString = NULL,   \
        .equals   = NULL,   \
        .compare  = NULL,   \
        .hashCode = NULL,   \
        .toJSON   = NULL,   \
        .display  = NULL,   \
        .super = &parcObject_DescriptorName(_superType),    \
        .name = #_subtype,     \
        __VA_ARGS__         \
    }; \
    LongBowCompiler_WarnInitializerOverrides \
    static const PARCObjectDescriptor parcObject_DescriptorName(_subtype)

/**
 * @define parcObject_ExtendPARCObject
 *
 * @discussion parcObject_ExtendPARCObject is a helper macro for constructing a PARCObjectDescriptor Structure of
 * function pointers pointing to a subtype's overriding functions. This struct serves the same
 * purpose as a vTable in c++ and provides for simple polymorphism. The functions used as parameters
 * should NOT call through to the parcObject function they override as this will result in an infinite loop.
 * NULL should be used for functions where PARCObject's default implementation is desired.
 *
 * Note: It uses the parcCMacro_IfElse trickery to handle NULL parameters.
 *
 * @param [in] _destroy A pointer to the Destroy callback function.
 * @param [in] _copy A pointer to the Copy callback function.
 * @param [in] _toString A pointer to the ToString callback function.
 * @param [in] _equals A pointer to the Equals callback function.
 * @param [in] _compare A pointer to the Compare callback function.
 * @param [in] _hashCode A pointer to the HashCode callback function.
 * @param [in] _toJSON A pointer to the ToJSON callback function.
 */
#define parcObject_ExtendPARCObject(_type, _destroy, _copy, _toString, _equals, _compare, _hashCode, _toJSON) \
    parcCMacro_IfElse(, parcObject_DestroyWrapper(_type, _destroy), _autowrap_##_destroy()) \
    parcCMacro_IfElse(, parcObject_CopyWrapper(_type, _copy), _autowrap_##_copy()) \
    parcCMacro_IfElse(, parcObject_ToStringWrapper(_type, _toString), _autowrap_##_toString()) \
    parcCMacro_IfElse(, parcObject_EqualsWrapper(_type, _equals), _autowrap_##_equals()) \
    parcCMacro_IfElse(, parcObject_CompareWrapper(_type, _compare), _autowrap_##_compare()) \
    parcCMacro_IfElse(, parcObject_HashCodeWrapper(_type, _hashCode), _autowrap_##_hashCode()) \
    parcCMacro_IfElse(, parcObject_ToJSONWrapper(_type, _toJSON), _autowrap_##_toJSON()) \
    parcObject_Override(_type, PARCObject, \
        .destroy = parcCMacro_IfElse(NULL, _autowrap_destroy_##_type, _autowrap_##_destroy()), \
        .destructor = NULL, \
        .release  = NULL,                                               \
        .copy     = parcCMacro_IfElse(NULL, _autowrap_copy_##_type, _autowrap_##_copy()), \
        .toString = parcCMacro_IfElse(NULL, _autowrap_toString_##_type, _autowrap_##_toString()), \
        .equals   = parcCMacro_IfElse(NULL, _autowrap_equals_##_type, _autowrap_##_equals()), \
        .compare  = parcCMacro_IfElse(NULL, _autowrap_compare_##_type, _autowrap_##_compare()), \
        .hashCode = parcCMacro_IfElse(NULL, _autowrap_hashCode_##_type, _autowrap_##_hashCode()), \
        .toJSON   = parcCMacro_IfElse(NULL, _autowrap_toJSON_##_type, _autowrap_##_toJSON()), \
        .display  = NULL)

#define parcObject_Create(_subtype) \
    (_subtype *) parcObject_CreateImpl(sizeof(_subtype))

/**
 * Create a reference counted segment of memory of at least @p objectLength bytes long with
 * the default functions provided by the `PARCObject` implementation.
 *
 * The allocated memory is such that the memory's base address is aligned on a `sizeof(void *)` boundary.
 *
 * If memory cannot be allocated, `errno` is set to `ENOMEM`.
 *
 * @param [in] objectLength The length, in bytes, of the memory to allocate.
 *
 * @return NULL The memory could not be allocated.
 * @return non-NULL A pointer to a valid `PARCObject` instance providing reference counted memory of at least length bytes.
 *
 * Example:
 * @code
 * {
 *     struct timeval *t = parcObject_CreateImpl(sizeof(struct timeval));
 * }
 * @endcode
 *
 * @see parcObject_CreateAndClear
 * @see parcObject_CreateAndClearImpl
 */
PARCObject *parcObject_CreateImpl(const size_t objectLength);

#define parcObject_CreateAndClear(_subtype) \
    (_subtype *) parcObject_CreateAndClearImpl(sizeof(_subtype))

PARCObject *parcObject_CreateAndClearImpl(const size_t objectLength);

/**
 * @define parcObject_CreateInstance
 *
 * `parcObject_CreateInstance` is a helper C-macro that creates an instance of a PARCObject subtype
 * using `parcObject_CreateInstanceImpl` that is based on the PARCObjectDescriptor.
 *
 * @param [in] _subtype A subtype's type string (e.g. PARCBuffer)
 */
#define parcObject_CreateInstance(_subtype) \
    parcObject_CreateInstanceImpl(sizeof(_subtype), &parcObject_DescriptorName(_subtype))

PARCObject *parcObject_CreateInstanceImpl(const size_t objectLength, const PARCObjectDescriptor *descriptor);

/**
 * @define parcObject_CreateAndClearInstance
 *
 * parcObject_CreateAndClearInstance is a helper C-macro that creates an instance of a PARCObject subtype
 * using parcObject_CreateAndClear that is based on the PARCObjectDescriptor struct created by the
 * parcObject_ExtendPARCObject macro.
 *
 * @param [in] _subtype A subtype's type string (e.g. PARCBuffer)
 */
#define parcObject_CreateAndClearInstance(_subtype) \
    (_subtype *) parcObject_CreateAndClearInstanceImpl(sizeof(_subtype), &parcObject_DescriptorName(_subtype))
/**
 * Create a reference counted segment of memory of at least @p objectLength long.
 *
 * The implementation pointer, is either NULL or points to a valid `PARCObjectDescriptor` structure
 * containing the callback functions that implement the object's life-cycle operations.
 *
 * The allocated memory is such that the memory's base address is aligned on a sizeof(void *) boundary,
 * and filled with zero bytes.
 *
 * If memory cannot be allocated, `errno` is set to ENOMEM.
 *
 * @param [in] objectLength The length, in bytes, of the memory to allocate.
 * @param [in] descriptor Either NULL, or a pointer to a `PARCObjectDescriptor` structure.
 *
 * @return NULL The memory could not be allocated.
 * @return non-NULL A pointer to reference counted memory of at least length bytes.
 *
 * Example:
 * @code
 * {
 *     struct timeval *t = parcObject_CreateAndClearInstanceImpl(sizeof(struct timeval), &PARCObject_Descriptor);
 * }
 * @endcode
 *
 * @see PARCObjectDescriptor
 * @see parcObject_Create
 */
PARCObject *parcObject_CreateAndClearInstanceImpl(const size_t objectLength, const PARCObjectDescriptor *descriptor);

/**
 * @def parcObject_ImplementAcquire
 *
 * parcObject_ImplementAcquire is a helper C-macro that creates a canonical subtype specific
 * Acquire function.
 *
 * @param [in] _namespace A subtype's namespace string (e.g. parcBuffer)
 * @param [in] _type A subtype's type string (e.g. PARCBuffer)
 */
#define parcObject_ImplementAcquire(_namespace, _type)               \
    _type *_namespace##_Acquire(const _type *pObject) {              \
        return (_type *) parcObject_Acquire((PARCObject *) pObject); \
    } extern _type *_namespace##_Acquire(const _type *pObject)

/**
 * @def parcObject_ImplementRelease
 *
 * parcObject_ImplementRelease is a helper C-macro that creates a canonical subtype specific
 * Release function.
 *
 * @param [in] _namespace A subtype's namespace string (e.g. parcBuffer)
 * @param [in] _type A subtype's type string (e.g. PARCBuffer)
 */
#define parcObject_ImplementRelease(_namespace, _type) \
    inline void _namespace##_Release(_type **pObject) { \
        assertNotNull(pObject, "NULL pointer passed to Release.  Double free?"); \
        parcObject_Release((PARCObject **) pObject);     \
    } extern void _namespace##_Release(_type **pObject)

/**
 * parcObject_ImplementationCheck is a helper macro that will generate compile time warnings for
 * missing canonical functions or canonical functions with faulty signatures.
 *
 * @param _namespace A subtype's namespace string (e.g. parcBuffer)
 * @param _type A subtype's type string (e.g. PARCBuffer)
 */
#define parcObject_ImplementationCheck(_namespace, _type) \
    static void                                           \
    _impl_check() {                                       \
        _type *po;                                        \
        const _type co;                                   \
        const _type *pco;                                 \
        pco = _namespace##_Copy(&co);                     \
        pco = _namespace##_Acquire(&co);                  \
        pco = pco;                                        \
        _namespace##_Release(&po);                        \
        bool b = _namespace##_Equals(&co, &co);           \
        b = b;                                            \
        int i = _namespace##_Compare(&co, &co);           \
        i = i;                                            \
        char *pc = _namespace##_ToString(&co);            \
        pc = pc;                                          \
        uint32_t ui = _namespace##_HashCode(&co);         \
        ui = ui;                                          \
        PARCJSON *pj = _namespace##_ToJSON(&co);          \
        pj = pj;                                          \
    } typedef void parcCMacro_Cat(_type, _IC_NOOP)

/**
 * Obtain the lock on the given `PARCObject` instance.
 *
 * If the lock is already held by another thread, this function will block.
 * If the lock is aleady held by the current thread, this function will return `false`.
 *
 * Implementors must avoid deadlock by attempting to lock the object a second time within the same calling thread.
 *
 * @param [in] object A pointer to a valid `PARCObject` instance.
 *
 * @return true The lock was obtained successfully.
 * @return false The lock is already held by the current thread, or the `PARCObject` is invalid.
 *
 * Example:
 * @code
 * {
 *     if (parcObject_Lock(object)) {
 *
 *     }
 * }
 * @endcode
 */
bool parcObject_Lock(const PARCObject *object);

/**
 * @def parcObject_ImplementLock
 *
 * parcObject_ImplementLock is a helper C-macro that defines a static, inline facade for the `parcObject_Lock` function.
 *
 * @param [in] _namespace A subtype's namespace string (e.g. parcBuffer)
 * @param [in] _type A subtype's type string (e.g. PARCBuffer)
 */
#define parcObject_ImplementLock(_namespace, _type)              \
    static inline bool _namespace##_Lock(const _type *pObject) { \
        return parcObject_Lock((PARCObject *) pObject);          \
    } typedef void parcCMacro_Cat(_type, _Lock_NOOP)

/**
 * Try to obtain the advisory lock on the given `PARCObject` instance.
 *
 * Once the lock is obtained, the caller must release the lock via `parcObject_Unlock`.
 *
 * @param [in] object A pointer to a valid `PARCObject` instance.
 *
 * @return true The `PARCObject` is locked.
 * @return false The `PARCObject` is unlocked.
 *
 * Example:
 * @code
 * {
 *     while (parcObject_TryLock(object))
 *         ;
 * }
 * @endcode
 */
bool parcObject_TryLock(const PARCObject *object);

/**
 * @def parcObject_ImplementTryLock
 *
 * parcObject_ImplementTryLock is a helper C-macro that defines a static, inline facade for the `parcObject_TryLock` function.
 *
 * @param [in] _namespace A subtype's namespace string (e.g. parcBuffer)
 * @param [in] _type A subtype's type string (e.g. PARCBuffer)
 */
#define parcObject_ImplementTryLock(_namespace, _type)              \
    static inline bool _namespace##_TryLock(const _type *pObject) { \
        return parcObject_TryLock((PARCObject *) pObject);          \
    } typedef void parcCMacro_Cat(_type, _TryLock_NOOP)

/**
 * Try to unlock the advisory lock on the given PARCObject instance.
 *
 * @param [in] object A pointer to a valid PARCObject instance.
 *
 * @return true The PARCObject was locked and now is unlocked.
 * @return false The PARCObject was not locked and remains unlocked.
 *
 * Example:
 * @code
 * {
 *     parcObject_Unlock(object);
 * }
 * @endcode
 */
bool parcObject_Unlock(const PARCObject *object);

/**
 * @def parcObject_ImplementUnlock
 *
 * parcObject_ImplementUnlock is a helper C-macro that defines a static, inline facade for the `parcObject_Unlock` function.
 *
 * @param [in] _namespace A subtype's namespace string (e.g. parcBuffer)
 * @param [in] _type A subtype's type string (e.g. PARCBuffer)
 */
#define parcObject_ImplementUnlock(_namespace, _type)              \
    static inline bool _namespace##_Unlock(const _type *pObject) { \
        return parcObject_Unlock((PARCObject *) pObject);          \
    } typedef void parcCMacro_Cat(_type, _Unlock_NOOP)

/**
 * Determine if the advisory lock on the given PARCObject instance is locked.
 *
 * @param [in] object A pointer to a valid PARCObject instance.
 *
 * @return true The PARCObject is locked.
 * @return false The PARCObject is unlocked.
 * Example:
 * @code
 * {
 *     <#example#>
 * }
 * @endcode
 */
bool parcObject_IsLocked(const PARCObject *object);

/**
 * @def parcObject_ImplementIsLocked
 *
 * parcObject_ImplementIsLocked is a helper C-macro that defines a static, inline facade for the `parcObject_IsLocked` function.
 *
 * @param [in] _namespace A subtype's namespace string (e.g. parcBuffer)
 * @param [in] _type A subtype's type string (e.g. PARCBuffer)
 */
#define parcObject_ImplementIsLocked(_namespace, _type)              \
    static inline bool _namespace##_IsLocked(const _type *pObject) { \
        return parcObject_IsLocked((const PARCObject *) pObject);    \
    } typedef void parcCMacro_Cat(_type, _IsLocked_NOOP)

/**
 * Causes the calling thread to wait until another thread invokes the `parcObject_Notify()` function on the same object.
 *
 * The calling thread must own this object's lock.
 * The calling thread will release ownership of this lock and wait until another thread invokes `parcObject_Notify`
 * on the same object. The original calling thread then re-obtains ownership of the lock and resumes execution.
 *
 * This function must only be called by a thread that is the owner of this object's lock.
 *
 * @param [in] object A pointer to a valid PARCObject instance.
 *
 * Example:
 * @code
 * {
 *
 *     parcObject_Wait(object);
 * }
 * @endcode
 */
void parcObject_Wait(const PARCObject *object);

/**
 * @def parcObject_ImplementWait
 *
 * parcObject_ImplementWait is a helper C-macro that defines a static, inline facade for the `parcObject_Wait` function.
 *
 * @param [in] _namespace A subtype's namespace string (e.g. `parcBuffer`)
 * @param [in] _type A subtype's type string (e.g. `PARCBuffer`)
 * @see parcObject_Wait
 */
#define parcObject_ImplementWait(_namespace, _type)              \
    static inline void _namespace##_Wait(const _type *pObject) { \
        parcObject_Wait((const PARCObject *) pObject);           \
    }  typedef void parcCMacro_Cat(_type, _Wait_NOOP)


/**
 * Causes the calling thread to wait until either another thread invokes the `parcObject_Notify()`
 * function on the same object or the system time equals or exceeds the specified time.
 *
 * The calling thread must own the object's lock.
 * The calling thread will release ownership of this lock and wait until another thread invokes
 * `parcObject_Notify` or the computer's system time equals or exceeds that specified by @p time.
 * on the same object.
 * The original calling thread then re-obtains ownership of the lock and resumes execution.
 *
 * This function must only be called by a thread that is the owner of this object's lock.
 *
 * @param [in] object A pointer to a valid PARCObject instance.
 
 * @returns false if the alloted time was exceeded.
 * @returns true if another thread invoked the `parcObject_Notify()` function
 *
 * Example:
 * @code
 * {
 *     struct timeval tv;
 *     gettimeofday(&tv, NULL);
 *
 *     struct timespec absoluteTime;
 *     absoluteTime.tv_sec = tv.tv_sec + 0;
 *     absoluteTime.tv_nsec = 0;
 *
 *     parcObject_WaitUntil(object, &absoluteTime);
 * }
 * @endcode
 */
bool parcObject_WaitUntil(const PARCObject *object, const struct timespec *time);

/**
 * @def parcObject_ImplementWaitUntil
 *
 * parcObject_ImplementWaitUntil is a helper C-macro that defines a static, inline facade for the
 * `parcObject_WaitUntil` function.
 *
 * @param [in] _namespace A subtype's namespace string (e.g. `parcBuffer`)
 * @param [in] _type A subtype's type string (e.g. `PARCBuffer`)
 * @see parcObject_WaitUntil
 */
#define parcObject_ImplementWaitUntil(_namespace, _type)                                           \
    static inline bool _namespace##_WaitUntil(const _type *pObject, const struct timespec *time) { \
        return parcObject_WaitUntil((const PARCObject *) pObject, time);                           \
}  typedef void parcCMacro_Cat(_type, _WaitUntil_NOOP)

/**
 * Causes the calling thread to wait until either another thread invokes the `parcObject_Notify()`
 * function on the same object or the given number of nanoseconds elapses.
 *
 * The calling thread must own the object's lock.
 *
 * The calling thread will release ownership of its lock and wait until another thread invokes
 * `parcObject_Notify` on the same object,
 * or the computer's system time equals or exceeds  the time specified by the
 * time of invocation plus nanoSeconds.
 * The original calling thread then re-obtains ownership of the lock and resumes execution.
 *
 * This function must only be called by a thread that is the owner of this object's lock.
 *
 * @param [in] object A pointer to a valid PARCObject instance.
 * @param [in] nanoSeconds The number of nanoseconds to wait.
 *
 * @returns false if the allotted time was exceeded.
 * @returns true if another thread invoked the `parcObject_Notify()` function
 *
 * Example:
 * @code
 * {
 *     parcObject_WaitFor(object, 1000000000UL);
 * }
 * @endcode
 */
bool parcObject_WaitFor(const PARCObject *object, const uint64_t nanoSeconds);

/**
 * @def parcObject_ImplementWaitUntil
 *
 * parcObject_ImplementWaitUntil is a helper C-macro that defines a static, inline facade for the
 * `parcObject_WaitUntil` function.
 *
 * @param [in] _namespace A subtype's namespace string (e.g. `parcBuffer`)
 * @param [in] _type A subtype's type string (e.g. `PARCBuffer`)
 * @see parcObject_WaitUntil
 */
#define parcObject_ImplementWaitFor(_namespace, _type)                                               \
    static inline bool _namespace##_WaitFor(const _type *pObject, const unsigned long nanoSeconds) { \
        return parcObject_WaitFor((const PARCObject *) pObject, nanoSeconds);                        \
    }  typedef void parcCMacro_Cat(_type, _WaitFor_NOOP)

/**
 * Wakes up a single thread that is waiting on this object (see `parcObject_Wait)`.
 * If any threads are waiting on this object, one of them is chosen to be awakened.
 * The choice is arbitrary and occurs at the discretion of the underlying implementation.
 *
 * The awakened thread will not be able to proceed until the current thread relinquishes the lock on this object.
 * The awakened thread will compete in the usual manner with any other threads that might be actively
 * competing to synchronize on this object;
 * for example,
 * the awakened thread enjoys no reliable privilege or disadvantage in being the next thread to lock this object.
 *
 * @param [in] object A pointer to a valid `PARCObject` instance.
 *
 * Example:
 * @code
 * {
 *
 *     parcObject_Notify(object);
 * }
 * @endcode
 */
void parcObject_Notify(const PARCObject *object);

/**
 * @def parcObject_ImplementNotify
 *
 * parcObject_ImplementNotify is a helper C-macro that defines a static, inline facade for the `parcObject_Notify` function.
 *
 * @param [in] _namespace A subtype's namespace string (e.g. parcBuffer)
 * @param [in] _type A subtype's type string (e.g. PARCBuffer)
 */
#define parcObject_ImplementNotify(_namespace, _type)              \
    static inline void _namespace##_Notify(const _type *pObject) { \
        parcObject_Notify((const PARCObject *) pObject);           \
    } typedef void parcCMacro_Cat(_type, _Notify_NOOP)

/**
 * Determine if a given `PARCObject` is and instance of the specified `PARCObjectDescriptor`.
 *
 * @param [in] object A pointer to a valid PARCObject instance.
 * @param [in] descriptor A pointer to a valid PARCObjectDescriptor instance.
 *
 * @return true @p object is an instance of @p descriptor.
 * @return false @p object is not an instance of @p descriptor.
 */
bool parcObject_IsInstanceOf(const PARCObject *object, const PARCObjectDescriptor *descriptor);

#endif // libparc_parc_Object_h
