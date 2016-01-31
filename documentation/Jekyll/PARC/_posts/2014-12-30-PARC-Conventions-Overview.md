---
layout: article
title: Overview
categories: conventions
---
There are a number of conventions in use to promote good API design and use.
Following these conventions will ensure that the code you write will integrate
well into these libraries and the way the libraries are implemented will be clear to you.

Most conventions are used to avoid some pitfall or antipattern.
Some conventions just _are_ and may be argued in one way or another.
That's for a different venue than this description.

The code is written in the [C99 Standard](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1124.pdf "Title")  dialect.
Compiling the code with pre-C99 standard compilers is not supported.

There are a number of API conventions in use that promote:

* Minimality

Have as few public functions per module and as few modules as possible.

This makes it easier to understand, remember, debug, and change the API.
Adding functions to perform operations that are not immediately necessary
just adds technical "weight" and contributes to an API that's hard to understand or remember.

* Completeness

A complete API means the expected functionality should be there.
The key word is "expected" by the user.
You must strike a reasonable boundary between completeness and being minimal.
For example, if you have a module or type with name "list" in it,
it should act like a list and support the expected list operations.
Otherwise, choose a better name.

* Simple

Do the least surprising thing.
Avoid side-effects.
If you must succumb to unavoidable side-effects, document them clearly.
Make common tasks straight-forward to implement.

* Intuitive

Use names that describe with precision what they do, as opposed, for example, how they do it.
Make it possible for a user who has no experience with the API to read the API and understand
what it does without having to understand the internal implementation.

* Memorizeable

Names and namespaces are well-chosen and meaningful.
Choose a consistent and precise naming convention.
Use recognizable patterns and concepts, and avoid abbreviations.

* Readable

Code is written once, but read, described, documented, debugged and changed many times by people other than the author.
Readable code may sometimes take longer to write, but saves time throughout the product’s life cycle.

