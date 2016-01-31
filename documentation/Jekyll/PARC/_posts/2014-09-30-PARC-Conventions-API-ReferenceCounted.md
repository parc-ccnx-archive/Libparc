---
layout: article
title: API Conventions for Reference Counted Memory.
categories: conventions
---

The PARC library supports both reference counted memory and non-reference counted memory.
In addition to the common conventions, here are the conventions for the APIs supporting reference counted memory.

* `PREFIXName * PREFIXName_Acquire(PREFIXName *instancePointer)`
Acquire a new reference to the given instance.

* `void PREFIXName_Release(PREFIXName **instancePointer)`

Release a reference to an instance of `PREFIXName`.
The value of `instancePointer` is always set to NULL after the release.

If `instancePointer` is NULL, this does nothing.

When the reference count of the `PREFIXName` instances is `1`,
the next invocation of `PREFIXName_Release` will free the memory storing `PREFIXName`.

Typically, however, an instance of `PREFIXName` is implemented to have pointers to other,
allocated, or reference counted, objects.
Implementors of the release function must ensure that the last release does not simply deallocate
the instance and leak memory or object references.

* HashCode
 Returns a hash code value for the given instance.
 
The general contract of `HashCode` is:

Whenever it is invoked on the same instance more than once during an execution of an application,
the {@link HashCode} function must consistently return the same value,
provided no information used in a corresponding {@link Equals}
comparisons on the instance is modified.

This value need not remain consistent from one execution of an application to another execution of the same application.
If two instances are equal according to the {@link Equals} method,
then calling the {@link HashCode} method on each of the two instances must produce the same integer result.

It is not required that if two instances are unequal according to the {@link Equals} function,
then calling the {@link HashCode}
method on each of the two objects must produce distinct integer results.