/*
 * Copyright (c) 2013-2016, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @file parc_ObjectDescriptor.h
 * @ingroup object
 * @brief PARC Object metadata.
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
 * @copyright 2013-2016, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libparc_parc_ObjectDescriptor_h
#define libparc_parc_ObjectDescriptor_h

#include <stdint.h>
#include <stdbool.h>
#include <parc/algol/parc_JSON.h>
#include <parc/algol/parc_HashCode.h>

/**
 * A Function that performs the final cleanup and resource deallocation when
 * a PARC Object has no more references.
 *
 * This is deprecated and will be removed.
 * Use `PARCObjectDestructor`
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

/**
 * Every PARC Object instance contains a pointer to an instance of this structure defining
 * the canonical meta-data for the object.
 */
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
    const struct PARCObjectDescriptor *super;
    size_t objectSize;
    unsigned objectAlignment;
    bool isLockable;
} PARCObjectDescriptor;


/**
 * @define parcObject_DescriptorName(_type)
 *
 * Creates a subtype specific name for a subtype's `PARCObjectDescriptor`
 * which is a parameter to `parcObject_Create`.
 */
#define parcObject_DescriptorName(_type) parcCMacro_Cat(_type, _Descriptor)

/**
 * @define parcObjectDescriptor_Declaration(_type_)
 *
 * Create a declaration of a `PARCObject` implementation.
 */
#define parcObjectDescriptor_Declaration(_type_) const PARCObjectDescriptor parcObject_DescriptorName(_type_)

/**
 * @define parcObject_Declare(_type_)
 *
 * Create a declaration of a `PARCObject` implementation.
 */
#define parcObject_Declare(_type_) \
struct _type_; \
typedef struct _type_ _type_; \
extern parcObjectDescriptor_Declaration(_type_)


/**
 * Create an allocated instance of `PARCObjectDescriptor`.
 *
 * @param [in] name A nul-terminated, C string containing the name of the object descriptor.
 * @param [in] objectSize The number of bytes necessary to contain the object.
 * @param [in] objectAlignment The alignment boundary necessary for the object, a power of 2 greater than or equal to `sizeof(void *)`
 * @param [in] isLockable True, if this object implementation supports locking.
 * @param [in] destructor The callback function to call when the last `parcObject_Release()` is invoked (replaces @p destroy).
 * @param [in] release The callback function to call when `parcObject_Release()` is invoked.
 * @param [in] copy The callback function to call when parcObject_Copy() is invoked.
 * @param [in] toString The callback function to call when `parcObject_ToString()` is invoked.
 * @param [in] equals The callback function to call when `parcObject_Equals()` is invoked.
 * @param [in] compare The callback function to call when `parcObject_Compare()` is invoked.
 * @param [in] hashCode The callback function to call when `parcObject_HashCode()` is invoked.
 * @param [in] toJSON The callback function to call when `parcObject_ToJSON()` is invoked.
 * @param [in] display The callback function to call when `parcObject_Display()` is invoked.
 * @param [in] super A pointer to a PARCObjectDescriptor for the supertype of created PARCObjectDescriptor
 *
 * @return NULL Memory could not be allocated to store the `PARCObjectDescriptor` instance.
 * @return non-NULL Successfully created the implementation
 */
const PARCObjectDescriptor *parcObjectDescriptor_Create(const char *name,
                                                        size_t objectSize,
                                                        unsigned int objectAlignment,
                                                        bool isLockable,
                                                        PARCObjectDestructor *destructor,
                                                        PARCObjectRelease *release,
                                                        PARCObjectCopy *copy,
                                                        PARCObjectToString *toString,
                                                        PARCObjectEquals *equals,
                                                        PARCObjectCompare *compare,
                                                        PARCObjectHashCode *hashCode,
                                                        PARCObjectToJSON *toJSON,
                                                        PARCObjectDisplay *display,
                                                        const PARCObjectDescriptor *super);

bool parcObjectDescriptor_Destroy(PARCObjectDescriptor **descriptorPointer);

#endif
