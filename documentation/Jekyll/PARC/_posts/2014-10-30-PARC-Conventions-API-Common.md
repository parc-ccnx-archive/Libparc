---
layout: article
title: Common API Conventions
categories: conventions
---

The PARC library has conventions for certain function names.
Not all modules must implement these functions,
but if they do, they must conform to the specification here.

## Canonical Function Names ##

Many of the canonical function names are reminicient of similar Java method names.
This is intentional as Java programmers will find these names familiar and their
functionality equivalent to the corresponding Java method.

* `TypeName *typeName_Create(parameters...)`

  Create an instance of `TypeName`.
  The function takes any parameters necessary to create the instance of `TypeName`.
  The instance may be allocated by any means compatible with the corresponding instance's implementation of
  `typeName_Destroy()` or `typeName_Release()`.
  See the sections on reference counted and non-reference counted instances.

* `TypeName *typeName_Init(TypeName *instance, parameters...)`

  Initialize (or reinitialize) an existing instance of `typeName`.

* `TypeName *typeName_Wrap(parameters...)`

  Import or 'wrap' a data in one form into an instance of `TypeName` without copying the data.

* `bool typeName_Equals(const TypeName *a, const TypeName *b)`

   Perform the following equivalence relations on non-null `TypeName` instances:

    * Reflexive: for any non-null reference value _x_, `typeName_Equals(x, x)` must return true.
 
    * Symmetric: for any non-null reference values _x_ and _y_, `typeName_Equals(x, y)` must return true if and only if 
  `typeName_Equals(y, x)` returns true.

    * Transitive: for any non-null reference values _x_, _y_, and _z_, if `typeName_Equals(x, y)` returns true and
   `typeName_Equals(y, z)` returns true, then  `typeName_Equals(x, z)` must return true.

    * Consistent: for any non-null reference values _x_ and _y_, multiple invocations of `typeName_Equals(x, y)`
  consistently return true or consistently return false.

    * For any non-null reference value _x_, `typeName_Equals(x, NULL))` must return false.

* `int typeName_CompareTo(const TypeName *a, const TypeName *b)`
  Compares instance _`a`_ with instance _`b`_ for order.

  The comparison function that this evaluates is the Signum function required to return a negative integer,
  zero, or a positive integer as _`a`_ is less than, equal to, or greater than _`b`_.
  No requirements on the absolute values are imposed.

  The function must:
 
    * ensure `sgn(typeName_CompareTo(a, b)) == -sgn(b, a)` for all values of a and b.

    * ensure that the relation is transitive: `(typeName_CompareTo(x, y) > 0 && typeName_CompareTo(y, z) > 0)` implies `typeName_CompareTo(x, z) > 0`.

    * ensure that the condition `typeName_CompareTo(x, y)== 0` implies that `sgn(typeName_CompareTo(x, z)) == sgn(typeName_CompareTo(y, z))`,
  for all values of z.

  This also stipulates that

   * `typeName_CompareTo(` _null_`,` _null_`)  == 0`,
   * `typeName_CompareTo(` _non-null_`,` _null_`) > 0`,
   * `typeName_CompareTo(`_null_`,` _non-null_`) < 0`.

  It is strongly recommended, but not strictly required that `(typeName_CompareTo(x, y)==0) == typeName_Equals(x, y))`.
  Generally speaking, any module that implements the `CompareTo` function and violates this condition should clearly indicate this fact.
  For example, _"Note: this structure has a natural ordering that is inconsistent with equals."_

* `int typeName_HashCode(TypeName *instance)`

  Generate a digest of the instance into a single `int` value.
  The value is not intended to be unique, (it may be an imperfect hash) but must return the same value for the same instance at all times. 

  It must be consistent with the corresponding Equals(x, y) function:
  `typeName_Equals(x, y) && (typeName_HashCode(x) == typeName_HashCode(y))`

* `char *typeName_ToString(TypeName *instance)`
  Create an allocated C string containing a human readable representation of the value or state of the given instance of `TypeName`.
  The value must be explicitly freed via `parc_free()`.

* `PARCElasticString *typeName_BuildString(const TypeName *instance, PARCElasticString *string)`
  Extend the given `PARCElasticString` with a string representation of the value or state of the given instance of `TypeName`

* `size_t typeName_Size(const TypeName* instance)`
  Return the number of the elements in a multi-element instance.

* `TypeName *typeName_Clone(const TypeName* instance)`
  Return a new instance of `TypeName` such that modifications to the original `instance` or its subsidiary members
  are not reflected in the cloned instance.
