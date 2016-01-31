---
layout: article
title: Organization and Naming
categories: conventions
---
This section describes what is expected of things that have names: files, functions, types.

## Names, Namespaces and Scope

Choose sensible, descriptive, English language names.
Do not use short cryptic names, names based on slang,
puns,
uncommon acronyms,
or require the user to learn a new way to spell a word.
For example, if something is a list, then say `list` and not `lst`

Take time choosing a good name, being mindful of using a name adequate to the abstraction.
As an example of the wrong abstraction, consider array lists and linked lists. Both are certainly 'lists.'
But if you implement an array list, don't call it `PARCList` call it `PARCArrayList`.

If you're stuck for a name, try to use names to promote specific concepts that important to the problem or solution domain.
Be prepared to change a name to a better name, should inspiration strike.

### Global Names
All globally visible names, such as filenames, function names, macro names, and types,
must have a prefix indicating their origin.

For example, globally visible names in the PARC library begin with `parc` or `PARC`
(depending upon whether they are types or functions, see below).

#### File Names
Every C source file that has globally visible functions or data must
also have a corresponding C Header file that declares the type information for all functions and data.
The name of C source file and C header file differ only in the suffix `.c` and `.h` respectively.

Modules with names of the form `prefix_Name` manage a type with the corresponding name `PREFIXName`.
The source location of a global function or type must be obvious from its name.
If you have a type called `PARCArrayList`, then the corresponding include file is named `parc_ArrayList.h`
and source file is `parc_ArrayList.c`.


#### Function Names
Function names contain the stem of their module or file name.
For example, a function named `parcBuffer_Create()` is declared in the file `parc_Buffer.h` and defined in the file `parc_Buffer.c` 


### Static Names.h
Static variables are not exposed in a C Header file.

### Local Names
Function variables and loop variables with a small scope
may have short names if the purpose of that variable is obvious,
or if the name has a traditional purpose.

For example:

{% highlight c %}

for (int i = 0; i < 10; i++) {
    // ...
}
{% endhighlight %}

### Managing Scope ##

A C Header file specifies all global functions and types to expose in a corresponding C Source file.
For example `parc_ArrayList.h` specifies all of the global functions implemented in the file `parc_ArrayList.c`.
Nothing specific about the implementation in the source file should be exposed in the header file.

