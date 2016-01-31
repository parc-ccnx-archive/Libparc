---
layout: article
title: Encapsulation and Separation of Concerns
categories: conventions
---

An imporant part of managing software complexity and dependencies are the concepts of separation of concerns,
and encapsulation.
C provides no rigor for implementing these concepts but convention can be applied to help implement both.

To that end, how structures are allocated and used is extremely important.
The structure pointer operation `->` is a powerful operator both in terms of
its expressive ability and in careless use its devastating effects on both separation
of concerns and encapsulation.
A common (and traditional) technique in C programming is to define a C structure in a header file
and allow unrestricted access to the structure members by client code.
This requires the correct use of the structure members by all potential client code
and inhibits (if not prohibits) future changes to the structure without affecting all existing client code.
As a result the structure becomes ossified and impervious to change. 

The convention used by the PARC Library is to not allow uncontrolled access to structure members,
but rather access and manipulate them via modules specifically designed to use the structure privately.

Structure members are not exposed outside of the module.
C structures are forward-declared in the Header file but not defined in the Header file.
The actual definition is in the corresponding Source file.
Functions in the module's C Source file implement operations on the members.

The C typedef construct is encouraged, but you must choose appropriate names.
See the guidline on naming.
Use typedefs to communicate a type as you would a class in an object-oriented langauge.

For example:

The C Header file `parcElasticString.h`
{% highlight c %}
struct parc_elastic_string;
typedef struct parc_elastic_string PARCElasticString;
{% endhighlight %}

The C Source file `parcElasticString.c`
{% highlight c %}
struct parc_elastic_string {
    PARCElasticByteBuffer *buffer;
};
{% endhighlight %}

> Pro-tip: For modules that are more sophisticated than providing simple data access (lists, maps, arrays, etc)
> 'getters' can be a sign that you have the wrong abstraction.

##### Data Transfer Objects
Sometimes you just an organized composite data structure to store and retrieve
data without operations to modify state or ensure invariants.
For example, a structure containing configuration flags.

In this case, and with good judgement, it is suitable to define that structure in a C Header
file.  Client code may then create instances of these structures via the struct keyword.
