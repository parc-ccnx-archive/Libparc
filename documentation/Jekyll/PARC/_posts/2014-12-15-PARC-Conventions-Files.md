---
layout: article
title: Files
categories: conventions
---
This section describes what is expected of various files.

## Source Files

Source files have case-sensitive names.

Every source file contains a postive assertion of Copyright.
At the top of each file include a Copyright information header.
Do not use `(C)` as a substitute for the word `Copyright`

{% highlight c %}
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
{% endhighlight %}


Updated as appropriate.

## Make files

The primary makefile for all cases is `Makefile`

## Libraries
Every library contains an object file that contains these entry points (substituting the library name for `library` below).

The purpose of these entry points are to supply the name, version and copyright information suitable for display as required.
The `library_About()` function displays the library attribution including any required attribution from incorporated code.

{% highlight c %}
/**
 * Return the name as a C string.
 */
const char *library_Name(void);

/**
 * Return the version as a C string.
 */
const char *longBow_Version(void);

/**
 * Return the About text as a C string.
 */
const char *longBow_About(void);

/**
 * Return the minimum copyright notice as a C string.
 */
const char *longBow_MiniNotice(void);

/**
 * Return the short copyright notice as a C string.
 */
const char *longBow_ShortNotice(void);

/**
 * Return the long copyright notice as a C string.
 */
const char *longBow_LongNotice(void);

{% endhighlight %}
