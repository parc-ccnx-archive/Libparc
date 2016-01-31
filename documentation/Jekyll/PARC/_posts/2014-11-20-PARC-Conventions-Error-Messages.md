---
layout: article
title: Error Messages
categories: conventions
---
There are essentially two kinds of error messages: Messages for a user and messages for a developer.

# User Messages

Messages that are intended to inform the user about state or conditions (whether errors or not)
must not be emitted by libraries.
Instead the library functions must convey unambiguous state information to higher application abstractions which
will decide how to communicate or deal with this information.

# Developer Messages

Error messages must indicate what the error was and contain enough information
to lead to actionable information for a remedy,
not just the state that caused the error.

For example:

{% highlight c %}
if (fp == NULL) {
    printf("fp == NULL\n");
    // This is a terrible error message.
    return NULL;
}

if (fp == NULL) {
    printf("Unable to open file.");
     // What file?  Why?
    return NULL;
}

if (fp == NULL) {
    printf("Unable to open %s  No such file.\n", fileName);    
    // Much better.
    return NULL;
}

{% endhighlight %}