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
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>

#include <LongBow/runtime.h>

#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/queue.h>
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

typedef enum {
    _PARCObjectLock_Unlocked = 0,
    _PARCObjectLock_Locked = 1
} _PARCObjectLock;

typedef struct parc_object_locking {
    pthread_mutex_t lock;
    pthread_mutexattr_t lockAttributes;
    pthread_cond_t notification;
    pthread_t locker;
    bool notified;
} _PARCObjectLocking;

/**
 * This is the per-object header.
 */
typedef struct object_header {
    PARCReferenceCount references;
    PARCObjectDescriptor *descriptor;
    size_t objectLength;               // The number of bytes which is >= the length to store the object.

    _PARCObjectLocking locking;

    unsigned char objectAlignment;    // The required aligment.  Must be a power of 2 and >= sizeof(void *).
} _PARCObjectHeader;

/**
 * Return true if the given alignment value is greater than or equal to
 * <code>sizeof(void *)</code> and is a power of 2.
 *
 * @param [in] alignment Cache alignment.
 *
 * @return true If the alignment is valid.
 * @return false If the alignment is is less than <code>sizeof(void *)</code> or is not a power of 2.
 */
static inline bool
_alignmentIsValid(const size_t alignment)
{
    return alignment >= sizeof(void *) && (alignment & (~alignment + 1)) == alignment;
}

/**
 * Increment/decrement a pointer by the given value.
 *
 * @param [in] base The base pointer to be modified.
 * @param [in] increment The size of the pointer increment.
 *
 * @return void* The updated pointer
 */
static inline void *
_pointerAdd(const void *base, const size_t increment)
{
    void *result = (void *) &((char *) base)[increment];
    return result;
}

/**
 * Compute the size of the preamble as the number of bytes necessary
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

/**
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
_objectHeader_Descriptor(const PARCObject *object)
{
    return (_parcObject_Header(object)->descriptor);
}

static inline _PARCObjectLocking *
_objectHeader_Locking(const PARCObject *object)
{
    return &(_parcObject_Header(object)->locking);
}

static inline bool
_objectHeaderIsValid(const _PARCObjectHeader *header)
{
    bool result = true;

    if (header->references == 0) {
        result = false;
    } else if (_alignmentIsValid(header->objectAlignment) == false) {
        result = false;
    } else if (header->objectLength == 0) {
        result = false;
    }

    return result;
}

/**
 * Compute the origin of the allocated memory.
 *
 * This may be different than the address of the object header,
 * due to alignment requirements causing the object header to be offset from the origin.
 */
static inline void *
_parcObject_Origin(const void *object)
{
    _PARCObjectHeader *header = _parcObject_Header(object);

    return _pointerAdd(object, -_parcObject_PrefixLength(header->objectAlignment));
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
    size_t length = header->objectLength;
    int result = memcmp(self, other, length);
    return result;
}

static PARCObject *
_parcObject_Copy(const PARCObject *object)
{
    _PARCObjectHeader *header = _parcObject_Header(object);
    size_t length = header->objectLength;

    void *result = parcObject_CreateInstanceImpl(length, header->descriptor);
    memcpy(result, object, length);
    parcObject_OptionalAssertValid(result);
    return result;
}

static bool
_parcObject_Equals(const PARCObject *x, const PARCObject *y)
{
    bool result = false;
    _PARCObjectHeader *header = _parcObject_Header(x);

    result = memcmp(x, y, header->objectLength) == 0;

    return result;
}

static char *
_parcObject_ToString(const PARCObject *object)
{
    _PARCObjectHeader *header = _parcObject_Header(object);

    char *string;
    int nwritten = asprintf(&string,
                            "Object@%p { .references=%" PRId64 ", .objectLength = %zd, .objectAlignment=%u } data %p\n",
                            (void *) header, header->references, header->objectLength, header->objectAlignment, object);
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
    parcJSON_AddInteger(json, "objectLength", prefix->objectLength);
    parcJSON_AddInteger(json, "objectAlignment", prefix->objectAlignment);
    parcJSON_AddInteger(json, "address", prefix->objectAlignment);

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
    return parcHashCode_Hash(object, header->objectLength);
}

static void
_parcObject_Display(const PARCObject *object, const int indentation)
{
    parcObject_OptionalAssertValid(object);

    _PARCObjectHeader *header = _parcObject_Header(object);

    parcDisplayIndented_PrintLine(indentation,
                                  "PARCObject@%p @%p={ .references=%zd .objectAlignment=%zd .objectLength=%zd }\n",
                                  object, header, header->references, header->objectAlignment, header->objectLength);
}

PARCObjectDescriptor
parcObject_DescriptorName(PARCObject) =
{
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
    .super      = NULL
};

bool
parcObject_IsValid(const PARCObject *object)
{
    bool result = true;

    if (object == NULL) {
        result = false;
    } else {
        if (_objectHeaderIsValid(_parcObject_Header(object)) == false) {
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
    trapIllegalValueIf(_alignmentIsValid(header->objectAlignment) == false,
                       "PARCObject@%p is corrupt. The alignment %d is not a power of 2 >= sizeof(void *)",
                       (void *) object, header->objectAlignment);
    trapIllegalValueIf(header->objectLength == 0, "PARCObject@%p length must be > 0", object);
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

    _PARCObjectHeader *header = _parcObject_Header(x);
    PARCObjectCompare *compare = _parcObject_ResolveCompare(header->descriptor);
    result = compare(x, y);

    return result;
}

bool
parcObject_IsInstanceOf(const PARCObject *object, const PARCObjectDescriptor *descriptor)
{
    PARCObjectDescriptor *d = _objectHeader_Descriptor(object);

    while (true) {
        if (d == descriptor) {
            return true;
        }
        d = d->super;
    }

    return false;
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

    _PARCObjectHeader *header = _parcObject_Header(object);
    PARCObjectHashCode *hashCode = _parcObject_ResolveHashCode(header->descriptor);

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

    _PARCObjectHeader *header = _parcObject_Header(object);
    PARCObjectDisplay *display = _parcObject_ResolveDisplay(header->descriptor);

    display(object, indentation);
}

char *
parcObject_ToString(const PARCObject *object)
{
    parcObject_OptionalAssertValid(object);

    _PARCObjectHeader *header = _parcObject_Header(object);
    PARCObjectToString *toString = _parcObject_ResolveToString(header->descriptor);

    return toString(object);
}

PARCJSON *
parcObject_ToJSON(const PARCObject *object)
{
    parcObject_OptionalAssertValid(object);

    _PARCObjectHeader *header = _parcObject_Header(object);
    PARCObjectToJSON *toJSON = _parcObject_ResolveToJSON(header->descriptor);
    return toJSON(object);
}

PARCObject *
parcObject_CreateImpl(const size_t objectLength)
{
    PARCObject *result = parcObject_CreateInstanceImpl(objectLength, &PARCObject_Descriptor);
    return result;
}

PARCObject *
parcObject_CreateAndClearImpl(const size_t objectLength)
{
    PARCObject *result = parcObject_CreateAndClearInstanceImpl(objectLength, &PARCObject_Descriptor);
    return result;
}

PARCObject *
parcObject_CreateAndClearInstanceImpl(const size_t objectLength, const PARCObjectDescriptor *descriptor)
{
    PARCObject *result = parcObject_CreateInstanceImpl(objectLength, descriptor);
    memset(result, 0, objectLength);
    return result;
}

PARCObject *
parcObject_CreateInstanceImpl(const size_t objectLength, const PARCObjectDescriptor *descriptor)
{
    if (objectLength == 0) {
        errno = EINVAL;
        return NULL;
    }

    size_t prefixLength = _parcObject_PrefixLength(sizeof(void *));
    size_t totalMemoryLength = prefixLength + objectLength;

    void *origin = NULL;
    parcMemory_MemAlign(&origin, sizeof(void *), totalMemoryLength);

    if (origin == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    // This abuts the prefix to the user accessible memory, it does not start at the beginning
    // of the aligned prefix region.
    _PARCObjectHeader *header = (_PARCObjectHeader *) &((char *) origin)[prefixLength - sizeof(_PARCObjectHeader)];
    header->references = 1;
    header->objectLength = objectLength;
    header->objectAlignment = sizeof(void *);
    header->descriptor = (PARCObjectDescriptor *) descriptor;

    _PARCObjectLocking *locking = &header->locking;
    if (locking != NULL) {
        pthread_mutexattr_init(&locking->lockAttributes);
        pthread_mutexattr_settype(&locking->lockAttributes, PTHREAD_MUTEX_NORMAL);

        pthread_mutex_init(&locking->lock, &locking->lockAttributes);

        locking->locker = (pthread_t) NULL;
        pthread_cond_init(&locking->notification, NULL);
        locking->notified = false;
    }

    errno = 0;
    void *result = _pointerAdd(origin, prefixLength);
    return result;
}

PARCObject *
parcObject_Copy(const PARCObject *object)
{
    parcObject_OptionalAssertValid(object);

    PARCObjectCopy *copy = _parcObject_ResolveCopy(_objectHeader_Descriptor(object));
    return copy(object);
}

PARCReferenceCount
parcObject_Release(PARCObject **objectPointer)
{
    PARCObject *object = *objectPointer;

    _PARCObjectHeader *header = _parcObject_Header(object);

    trapIllegalValueIf(header->references == 0, "PARCObject@%p references must be > 0", object);

    parcObject_OptionalAssertValid(object);


    PARCReferenceCount result = parcAtomicUint64_Decrement(&header->references);

    if (result == 0) {
        if (_parcObjectType_Destructor(header->descriptor, objectPointer)) {
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
    }
    return result;
}

void
parcObjectDescriptor_Destroy(PARCObjectDescriptor **descriptorPointer)
{
    parcMemory_Deallocate(descriptorPointer);
}

bool
parcObject_Unlock(const PARCObject *object)
{
    parcObject_OptionalAssertValid(object);

    bool result = false;

    _PARCObjectLocking *locking = _objectHeader_Locking(object);
    locking->locker = (pthread_t) NULL;
    result = (pthread_mutex_unlock(&locking->lock) == 0);

    return result;
}

bool
parcObject_Lock(const PARCObject *object)
{
    extern int errno;

    bool result = false;

    parcObject_OptionalAssertValid(object);

    _PARCObjectLocking *locking = _objectHeader_Locking(object);
    trapCannotObtainLockIf(locking->locker == pthread_self(), "Recursive locks are not supported.");

    errno = pthread_mutex_lock(&locking->lock);
    if (errno == 0) {
        locking->locker = pthread_self();
        result = true;
    }

    return result;
}

bool
parcObject_TryLock(const PARCObject *object)
{
    bool result = false;

    parcObject_OptionalAssertValid(object);

    _PARCObjectLocking *locking = _objectHeader_Locking(object);

    trapCannotObtainLockIf(locking->locker == pthread_self(), "Recursive locks are not supported.");

    result = (pthread_mutex_trylock(&locking->lock) == 0);
    if (result == true) {
        locking->locker = pthread_self();
    }
    return result;
}

bool
parcObject_IsLocked(const PARCObject *object)
{
    parcObject_OptionalAssertValid(object);
    _PARCObjectLocking *locking = _objectHeader_Locking(object);
    return locking->locker != (pthread_t) NULL;
}

void
parcObject_Wait(const PARCObject *object)
{
    parcObject_OptionalAssertValid(object);

    _PARCObjectLocking *locking = _objectHeader_Locking(object);

    trapUnexpectedStateIf(locking->locker == (pthread_t) NULL,
                          "You must Lock the object %p before calling parcObject_Wait", (void *) object);

    locking->notified = false;
    while (locking->notified == false) {
        pthread_cond_wait(&locking->notification, &locking->lock);
    }
}

void
parcObject_WaitUntil(const PARCObject *object, const struct timespec *time)
{
    parcObject_OptionalAssertValid(object);

    _PARCObjectHeader *header = _parcObject_Header(object);

    trapUnexpectedStateIf(header->locking.locker == (pthread_t) NULL,
                          "You must Lock the object %p before calling parcObject_Wait", (void *) object);

    header->locking.notified = false;
    pthread_cond_timedwait(&header->locking.notification, &header->locking.lock, time);
}

void
parcObject_WaitFor(const PARCObject *object, const uint64_t nanoSeconds)
{
    parcObject_OptionalAssertValid(object);

    _PARCObjectHeader *header = _parcObject_Header(object);

    trapUnexpectedStateIf(header->locking.locker == (pthread_t) NULL,
                          "You must Lock the object %p before calling parcObject_Wait", (void *) object);
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

    pthread_cond_timedwait(&header->locking.notification, &header->locking.lock, &time);
}

void
parcObject_Notify(const PARCObject *object)
{
    parcObject_OptionalAssertValid(object);

    _PARCObjectHeader *header = _parcObject_Header(object);

    trapUnexpectedStateIf(header->locking.locker == (pthread_t) NULL,
                          "You must Lock the object %p before calling parcObject_Notify", (void *) object);

    header->locking.notified = true;
    pthread_cond_signal(&header->locking.notification);
}
