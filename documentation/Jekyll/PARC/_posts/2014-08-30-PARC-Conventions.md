---
layout: article
title: Style Guide and Coding Conventions
categories: toplevel
---
There is a guiding philosophy behind the motivations and implementations of the PARC Library and there
are a number of conventions in use to promote good API design, readbility and use.
Following these conventions will make it more likely that the code you write will integrate well into these libraries.

A style guide is an important aid to help a group of developers write code that looks the same across the code base.
The style imposes some patterns on how or when things are used,
how related things can be grouped together.
A good style will help developers and non-developers alike read the code easily.

But a style guide is only a tool, not a goal.
Code that confroms to the style guide is not guaranteed to be good code,
and writing code that conforms to the style guide doesn't make a good programmer.
From architecture, design, organization and implementation,
the goal is readable software that exemplifies the best in modern
software engineeering and the style guide is an aid to that end.

This set of articles is a work-in-progress and undergoes updates and reorganization as needed.

------------------------

The code is written in the  C99 dialect.
Compiling the code with pre-C99 standard compilers is not supported.

The best reference for the coding standard is the existing code.
There may be some deviations, but a general theme is found throughout.
Please write your code to fit with this theme.

References:

* [CERT C Coding Standard](https://www.securecoding.cert.org/confluence/display/seccode/CERT+C+Coding+Standard)
* [C99 Standard](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1124.pdf "Title")

The references above are additional information on the coding conventions.

<ul id="blog-posts" class="posts">
  {% for post in site.categories.conventions %}
    <li>
      <h3><a href="{{site.baseurl}}{{ post.url }}">&raquo; {{ post.title }}</a></h3>        
        <p>{{ post.excerpt }}</p>
    </li>
  {% endfor %}
</ul>
    