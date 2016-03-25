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
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2016, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>

#include <LongBow/runtime.h>

#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#include <stdint.h>
#include <stdbool.h>

#include <parc/algol/parc_DisplayIndented.h>
#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_Hash.h>
#include <parc/concurrent/parc_AtomicUint64.h>

//#define PEROBJECTLOCKATTRIBUTES 1

typedef struct parc_object_locking {
    pthread_mutex_t lock;
#ifdef PEROBJECTLOCKATTRIBUTES
    pthread_mutexattr_t lockAttributes;
#endif
    pthread_cond_t notification;
    pthread_t locker;
} _PARCObjectLocking;

/*
 * This is the per-object header.
 */
typedef struct object_header {
#define PARCObject_HEADER_MAGIC_GUARD_NUMBER 0x12345678
    uint32_t magicGuardNumber;
    PARCReferenceCount references;
    PARCObjectDescriptor *descriptor;
    _PARCObjectLocking *locking;

    _PARCObjectLocking lock;
} _PARCObjectHeader;

/*
 * Increment/decrement a pointer by the given value.
 *
 * @param [in] base The base pointer.
 * @param [in] increment The value of the pointer increment.
 *
 * @return void* The updated pointer
 */
static inline void *
_pointerAdd(const void *base, const size_t increment)
{
    void *result = (void *) &((char *) base)[increment];
    return result;
}

/*
 * Compute the size of the prefix as the number of bytes necessary
 * to fit the object header (with padding) into allocated memory,
 * such that the first address after the header is aligned according to the value of the alignment parameter.
 *
 * For example, if the object header is 5 bytes long, and the alignment is 4 (bytes),
 * then the necessary number of bytes to fit the header
 * (and yet maintain the proper alignment for the first memory location after the header) is 8.
 *
 * @param [in] alignment Cache alignment
 *
 * @return The size of the object header as the number of bytes necessary to fit the header (with padding) into allocated memory.
 */
static inline size_t
_parcObject_PrefixLength(const size_t alignment)
{
    return (sizeof(_PARCObjectHeader) + (alignment - 1)) & - alignment;
}

/*
 * Given the memory address, return a pointer to the corresponding _PARCObjectHeader structure.
 *
 * @param [in] object The base pointer to the object.
 *
 * @return A _PARCObjectHeader struct encapsulating the object's header information.
 */
static inline _PARCObjectHeader *
_parcObject_Header(const PARCObject *object)
{
    return _pointerAdd(object, -sizeof(_PARCObjectHeader));
}

static inline PARCObjectDescriptor *
_parcObject_Descriptor(const PARCObject *object)
{
    return (_parcObject_Header(object)->descriptor);
}

static inline _PARCObjectLocking *
_parcObjectHeader_Locking(const PARCObject *object)
{
    return &(_parcObject_Header(object)->lock);
}

static inline bool
_parcObjectHeader_IsValid(const _PARCObjectHeader *header, const PARCObject *object)
{
    bool result = true;
    
    if ((intptr_t) header >= (intptr_t) object) {
        result = false;
    } else if (header->magicGuardNumber != PARCObject_HEADER_MAGIC_GUARD_NUMBER) {
        result = false;
    } else if (header->references == 0) {
        result = false;
    }
    
    return result;
}

/*
 * Compute the origin of the allocated memory.
 *
 * This will be greater than the start of the object header.
 * due to alignment requirements causing the object header to be offset from the origin.
 */
static inline void *
_parcObject_Origin(const PARCObject *object)
{
    _PARCObjectHeader *header = _parcObject_Header(object);

    return _pointerAdd(object, -_parcObject_PrefixLength(header->descriptor->objectAlignment));
}

static inline PARCObjectEquals *
_parcObject_ResolveEquals(const PARCObjectDescriptor *descriptor)
{
    while (descriptor->equals == NULL) {
        descriptor = descriptor->super;
    }
    return descriptor->equals;
}

static inline PARCObjectCopy *
_parcObject_ResolveCopy(const PARCObjectDescriptor *descriptor)
{
    while (descriptor->copy == NULL) {
        descriptor = descriptor->super;
    }
    return descriptor->copy;
}

static inline PARCObjectToString *
_parcObject_ResolveToString(const PARCObjectDescriptor *descriptor)
{
    while (descriptor->toString == NULL) {
        descriptor = descriptor->super;
    }
    return descriptor->toString;
}

static inline PARCObjectToJSON *
_parcObject_ResolveToJSON(const PARCObjectDescriptor *descriptor)
{
    while (descriptor->toJSON == NULL) {
        descriptor = descriptor->super;
    }
    return descriptor->toJSON;
}

static bool
_parcObjectType_Destructor(const PARCObjectDescriptor *descriptor, PARCObject **object)
{
    if (descriptor != NULL) {
        if (descriptor->destructor != NULL) {
            return descriptor->destructor(object);
        } else if (descriptor->destroy != NULL) {
            descriptor->destroy(object);
        }
    }

    return true;
}

static int
_parcObject_Compare(const PARCObject *self, const PARCObject *other)
{
    _PARCObjectHeader *header = _parcObject_Header(self);
    size_t length = header->descriptor->objectSize;
    int result = memcmp(self, other, length);
    return result;
}

static PARCObject *
_parcObject_Copy(const PARCObject *object)
{
    _PARCObjectHeader *header = _parcObject_Header(object);
    size_t length = header->descriptor->objectSize;

    void *result = parcObject_CreateInstanceImpl(header->descriptor);
    memcpy(result, object, length);
    parcObject_OptionalAssertValid(result);
    return result;
}

static bool
_parcObject_Equals(const PARCObject *x, const PARCObject *y)
{
    _PARCObjectHeader *header = _parcObject_Header(x);

    bool result = memcmp(x, y, header->descriptor->objectSize) == 0;

    return result;
}

static char *
_parcObject_ToString(const PARCObject *object)
{
    _PARCObjectHeader *header = _parcObject_Header(object);

    char *string;
    int nwritten = asprintf(&string,
                            "Object@%p { .references=%" PRId64 ", .objectLength = %zd, .objectAlignment=%u } data %p\n",
                            (void *) header, header->references, header->descriptor->objectSize, header->descriptor->objectAlignment, object);
    assertTrue(nwritten >= 0, "Error calling asprintf");
    char *result = parcMemory_StringDuplicate(string, strlen(string));
    free(string);
    return result;
}

static PARCJSON *
_parcObject_ToJSON(const PARCObject *object)
{
    _PARCObjectHeader *prefix = _parcObject_Header(object);

    PARCJSON *json = parcJSON_Create();

    parcJSON_AddInteger(json, "references", prefix->references);
    parcJSON_AddInteger(json, "objectLength", prefix->descriptor->objectSize);
    parcJSON_AddInteger(json, "objectAlignment", prefix->descriptor->objectAlignment);

    char *addressString;
    int nwritten = asprintf(&addressString, "%p", object);
    assertTrue(nwritten >= 0, "Error calling asprintf");

    parcJSON_AddString(json, "address", addressString);

    return json;
}

static PARCHashCode
_parcObject_HashCode(const PARCObject *object)
{
    _PARCObjectHeader *header = _parcObject_Header(object);
    return parcHashCode_Hash(object, header->descriptor->objectSize);
}

static void
_parcObject_Display(const PARCObject *object, const int indentation)
{
    parcObject_OptionalAssertValid(object);

    _PARCObjectHeader *header = _parcObject_Header(object);

    parcDisplayIndented_PrintLine(indentation,
                                  "PARCObject@%p @%p={ .references=%zd .objectAlignment=%zd .objectLength=%zd }\n",
                                  object, header, header->references, header->descriptor->objectAlignment, header->descriptor->objectSize);
}

PARCObjectDescriptor parcObject_DescriptorName(PARCObject) = {
    .name       = "PARCObject",
    .destroy    = NULL,
    .destructor = NULL,
    .release    = NULL,
    .copy       = _parcObject_Copy,
    .toString   = _parcObject_ToString,
    .equals     = _parcObject_Equals,
    .compare    = _parcObject_Compare,
    .hashCode   = _parcObject_HashCode,
    .toJSON     = _parcObject_ToJSON,
    .display    = _parcObject_Display,
    .super      = NULL,
    .isLockable = true,
    .objectSize = 0,
    .objectAlignment = sizeof(void *)    
};

bool
parcObject_IsValid(const PARCObject *object)
{
    bool result = true;

    if (object == NULL) {
        result = false;
    } else {
        if (_parcObjectHeader_IsValid(_parcObject_Header(object), object) == false) {
            result = false;
        }
    }

    return result;
}

#ifdef PARCLibrary_DISABLE_VALIDATION
#  define parcObjectHeader_OptionalAssertValid(_instance_)
#else
#  define parcObjectHeader_OptionalAssertValid(_instance_) _parcObjectHeader_AssertValid(_instance_)
#endif

static inline void
_parcObjectHeader_AssertValid(const _PARCObjectHeader *header, const PARCObject *object)
{
    trapIllegalValueIf(header->references == 0, "PARCObject@%p references must be > 0", object);
}

static inline void
_parcObject_AssertValid(const PARCObject *object)
{
    trapIllegalValueIf(object == NULL, "PARCObject must be a non-null pointer.");

    _PARCObjectHeader *header = _parcObject_Header(object);
    _parcObjectHeader_AssertValid(header, object);
}

void
parcObject_AssertValid(const PARCObject *object)
{
    _parcObject_AssertValid(object);
}

PARCObject *
parcObject_Acquire(const PARCObject *object)
{
    parcObject_OptionalAssertValid(object);

    _PARCObjectHeader *header = _parcObject_Header(object);

    parcAtomicUint64_Increment(&header->references);

    return (PARCObject *) object;
}

static inline PARCObjectCompare *
_parcObject_ResolveCompare(const PARCObjectDescriptor *descriptor)
{
    while (descriptor->compare == NULL) {
        descriptor = descriptor->super;
    }
    return descriptor->compare;
}

int
parcObject_Compare(const PARCObject *x, const PARCObject *y)
{
    int result = 0;
    if ((x == NULL) || (y == NULL)) {
        result = 0;
        if (x != NULL) {
            result = 1;
        } else if (y != NULL) {
            result = -1;
        }
        return result;
    }

    parcObject_OptionalAssertValid(x);
    parcObject_OptionalAssertValid(y);

    PARCObjectCompare *compare = _parcObject_ResolveCompare(_parcObject_Descriptor(x));
    result = compare(x, y);

    return result;
}

bool
parcObject_IsInstanceOf(const PARCObject *object, const PARCObjectDescriptor *descriptor)
{
    bool result = false;
    
    if (object != NULL) {
        _PARCObjectHeader *header = _parcObject_Header(object);
        
        if (_parcObjectHeader_IsValid(header, object)) {
            PARCObjectDescriptor *d = _parcObject_Descriptor(object);
            
            while (result == false) {
                if (d == descriptor) {
                    result = true;
                }
                d = d->super;
            }
        }
    }
    
    return result;
}

bool
parcObject_Equals(const PARCObject *x, const PARCObject *y)
{
    bool result = false;

    if (x == y) {
        result = true;
    } else if (x != NULL && y != NULL) {
        _PARCObjectHeader *xHeader = _parcObject_Header(x);
        _PARCObjectHeader *yHeader = _parcObject_Header(y);

        if (xHeader->descriptor == yHeader->descriptor) {
            PARCObjectEquals *equals = _parcObject_ResolveEquals(xHeader->descriptor);
            result = equals(x, y);
        }
    }

    return result;
}

static inline PARCObjectHashCode *
_parcObject_ResolveHashCode(const PARCObjectDescriptor *descriptor)
{
    while (descriptor->hashCode == NULL) {
        descriptor = descriptor->super;
    }
    return descriptor->hashCode;
}

PARCHashCode
parcObject_HashCode(const PARCObject *object)
{
    parcObject_OptionalAssertValid(object);

    PARCObjectHashCode *hashCode = _parcObject_ResolveHashCode(_parcObject_Descriptor(object));

    return hashCode(object);
}

static inline PARCObjectDisplay *
_parcObject_ResolveDisplay(const PARCObjectDescriptor *descriptor)
{
    while (descriptor->display == NULL) {
        descriptor = descriptor->super;
    }
    return descriptor->display;
}

void
parcObject_Display(const PARCObject *object, const int indentation)
{
    parcObject_OptionalAssertValid(object);

    PARCObjectDisplay *display = _parcObject_ResolveDisplay(_parcObject_Descriptor(object));

    display(object, indentation);
}

char *
parcObject_ToString(const PARCObject *object)
{
    parcObject_OptionalAssertValid(object);

    PARCObjectToString *toString = _parcObject_ResolveToString(_parcObject_Descriptor(object));

    return toString(object);
}

PARCJSON *
parcObject_ToJSON(const PARCObject *object)
{
    parcObject_OptionalAssertValid(object);

    PARCObjectToJSON *toJSON = _parcObject_ResolveToJSON(_parcObject_Descriptor(object));
    return toJSON(object);
}

PARCObject *
parcObject_CreateAndClearInstanceImpl(const PARCObjectDescriptor *descriptor)
{
    PARCObject *result = parcObject_CreateInstanceImpl(descriptor);
    memset(result, 0, descriptor->objectSize);
    return result;
}

static inline void
_parcObject_InitializeLocking(_PARCObjectLocking *locking)
{
    if (locking != NULL) {
#ifdef PEROBJECTLOCKATTRIBUTES
        pthread_mutexattr_init(&locking->lockAttributes);
        pthread_mutexattr_settype(&locking->lockAttributes, PTHREAD_MUTEX_NORMAL);
        pthread_mutex_init(&locking->lock, &locking->lockAttributes);
#else
        //        locking->lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
        locking->lock = (pthread_mutex_t) PTHREAD_ERRORCHECK_MUTEX_INITIALIZER;
#endif
        
        locking->locker = (pthread_t) NULL;
        //        pthread_cond_init(&locking->notification, NULL);
        locking->notification = (pthread_cond_t) PTHREAD_COND_INITIALIZER;
    }
}

PARCObject *
parcObject_CreateInstanceImpl(const PARCObjectDescriptor *descriptor)
{
    size_t prefixLength = _parcObject_PrefixLength(sizeof(void *));
    size_t totalMemoryLength = prefixLength + descriptor->objectSize;

    void *origin = NULL;
    parcMemory_MemAlign(&origin, sizeof(void *), totalMemoryLength);

    if (origin == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    // This abuts the prefix to the user accessible memory,
    // it does not start at the beginning of the aligned prefix region.
    _PARCObjectHeader *header = (_PARCObjectHeader *) &((char *) origin)[prefixLength - sizeof(_PARCObjectHeader)];
    header->magicGuardNumber = PARCObject_HEADER_MAGIC_GUARD_NUMBER;
    header->references = 1;
    header->descriptor = (PARCObjectDescriptor *) descriptor;

    if (header->descriptor->isLockable) {
        header->locking = &header->lock;
        _parcObject_InitializeLocking(header->locking);
    }

    errno = 0;
    void *result = _pointerAdd(origin, prefixLength);
    return result;
}

PARCObject *
parcObject_Copy(const PARCObject *object)
{
    parcObject_OptionalAssertValid(object);

    PARCObjectCopy *copy = _parcObject_ResolveCopy(_parcObject_Descriptor(object));
    return copy(object);
}

PARCReferenceCount
parcObject_Release(PARCObject **objectPointer)
{
    PARCObject *object = *objectPointer;
    parcObject_OptionalAssertValid(object);

    _PARCObjectHeader *header = _parcObject_Header(object);

    trapIllegalValueIf(header->references == 0, "PARCObject@%p references must be > 0", object);

    PARCReferenceCount result = parcAtomicUint64_Decrement(&header->references);

    if (result == 0) {
        if (_parcObjectType_Destructor(header->descriptor, objectPointer)) {
            if (header->locking != NULL) {
                pthread_cond_destroy(&header->locking->notification);
//                pthread_cond_destroy(&header->lock.notification);
            }
            void *origin = _parcObject_Origin(object);
            parcMemory_Deallocate(&origin);
            assertNotNull(*objectPointer, "Class implementation unnecessarily clears the object pointer.");
        } else {
            assertNull(*objectPointer, "Class implementation must clear the object pointer.");
        }
    }

    *objectPointer = NULL;
    return result;
}

PARCReferenceCount
parcObject_GetReferenceCount(const PARCObject *object)
{
    parcObject_OptionalAssertValid(object);

    _PARCObjectHeader *header = _parcObject_Header(object);

    return header->references;
}

PARCObjectDescriptor *
parcObject_SetDescriptor(PARCObject *object, const PARCObjectDescriptor *descriptor)
{
    parcObject_OptionalAssertValid(object);

    _PARCObjectHeader *header = _parcObject_Header(object);

    PARCObjectDescriptor *result = header->descriptor;
    header->descriptor = (PARCObjectDescriptor *) descriptor;

    return result;
}

PARCObjectDescriptor *
parcObjectDescriptor_Create(const char *name,
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
                            PARCObjectDescriptor *super)
{
    assertNotNull(super, "Supertype descriptor cannot be NULL.");

    PARCObjectDescriptor *result = parcMemory_AllocateAndClear(sizeof(PARCObjectDescriptor));
    if (result != NULL) {
        strncpy(result->name, name, sizeof(result->name));
        result->destroy = NULL;
        result->destructor = destructor;
        result->release = release;
        result->copy = copy;
        result->toString = toString;
        result->equals = equals;
        result->compare = compare;
        result->hashCode = hashCode;
        result->toJSON = toJSON;
        result->display = display;
        result->super = super;
        result->objectSize = objectSize;
        result->objectAlignment = objectAlignment;
        result->isLockable = isLockable;
    }
    return result;
}

bool
parcObjectDescriptor_Destroy(PARCObjectDescriptor **descriptorPointer)
{
    parcMemory_Deallocate(descriptorPointer);
    return true;
}

bool
parcObject_Unlock(const PARCObject *object)
{
    bool result = false;
    _PARCObjectHeader *header = _parcObject_Header(object);
    if (header->references > 0) {
        _parcObjectHeader_AssertValid(header, object);
        
        if (object != NULL) {            
//            _PARCObjectLocking *locking = &header->lock;
            header->locking->locker = (pthread_t) NULL;
            result = (pthread_mutex_unlock(&header->locking->lock) == 0);
            
            assertTrue(result, "Attempted to unlock an unowned lock.");
        }
    }
    return result;
}

bool
parcObject_Lock(const PARCObject *object)
{
    extern int errno;
    
    bool result = false;
    
    if (object != NULL) {        
        parcObject_OptionalAssertValid(object);
        
        _PARCObjectLocking *locking = _parcObjectHeader_Locking(object);
        trapCannotObtainLockIf(locking->locker == pthread_self(), "Recursive locks are not supported.");
        
        errno = pthread_mutex_lock(&locking->lock);
        if (errno == 0) {
            locking->locker = pthread_self();
            result = true;
        }
    }
    
    return result;
}

bool
parcObject_TryLock(const PARCObject *object)
{
    bool result = false;
    
    if (object != NULL) {
        parcObject_OptionalAssertValid(object);
        
        _PARCObjectLocking *locking = _parcObjectHeader_Locking(object);
        
        trapCannotObtainLockIf(locking->locker == pthread_self(), "Recursive locks are not supported.");
        
        result = (pthread_mutex_trylock(&locking->lock) == 0);
        if (result == true) {
            locking->locker = pthread_self();
        }
    }
    return result;
}

bool
parcObject_IsLocked(const PARCObject *object)
{
    parcObject_OptionalAssertValid(object);
    _PARCObjectLocking *locking = _parcObjectHeader_Locking(object);
    return locking->locker != (pthread_t) NULL;
}

void
parcObject_Wait(const PARCObject *object)
{
    parcObject_OptionalAssertValid(object);

    _PARCObjectLocking *locking = _parcObjectHeader_Locking(object);

    pthread_cond_wait(&locking->notification, &locking->lock);
}

bool
parcObject_WaitUntil(const PARCObject *object, const struct timespec *time)
{
    parcObject_OptionalAssertValid(object);
    
    bool result = true;
    
    _PARCObjectLocking *locking = _parcObjectHeader_Locking(object);
    int waitResult = pthread_cond_timedwait(&locking->notification, &locking->lock, time);
    
    if (waitResult == ETIMEDOUT) {
        result = false;
    }
    
    return result;
}

bool
parcObject_WaitFor(const PARCObject *object, const uint64_t nanoSeconds)
{
    parcObject_OptionalAssertValid(object);
    
    bool result = true;
    
    struct timeval now;
    gettimeofday(&now, NULL);

    // Convert timeval to timespec.
    struct timespec time = {
        .tv_sec  = now.tv_sec,
        .tv_nsec = (now.tv_usec * 1000)
    };
    time.tv_nsec += nanoSeconds;
    time.tv_sec += time.tv_nsec / 1000000000;
    time.tv_nsec = time.tv_nsec % 1000000000;
    
    _PARCObjectLocking *locking = _parcObjectHeader_Locking(object);
    
    int waitResult = pthread_cond_timedwait(&locking->notification, &locking->lock, &time);
    
    if (waitResult == ETIMEDOUT) {
        result = false;
    }
    
    return result;
}

void
parcObject_Notify(const PARCObject *object)
{
    parcObject_OptionalAssertValid(object);

    _PARCObjectLocking *locking = _parcObjectHeader_Locking(object);

    pthread_cond_signal(&locking->notification);
}

void
parcObject_NotifyAll(const PARCObject *object)
{
    parcObject_OptionalAssertValid(object);
    
    _PARCObjectLocking *locking = _parcObjectHeader_Locking(object);
        
    pthread_cond_broadcast(&locking->notification);
}
