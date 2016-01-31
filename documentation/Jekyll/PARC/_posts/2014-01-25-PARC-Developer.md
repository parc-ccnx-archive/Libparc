---
layout: post
title: Developer Aids
categories: toplevel
---
There are a number of conventions in use to promote good API design and use.
Following these conventions will ensure that the code you write will integrate well into these libraries.

The code is written in the [C99 Standard](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1124.pdf "Title")  dialect.
Compiling the code with pre-C99 standard compilers is not supported.

There are a number of API conventions in use that promote:

<ul id="blog-posts" class="posts">
  {% for post in site.categories.developer %}
    <li>
      <h3><a href="{{site.baseurl}}{{ post.url }}">&raquo; {{ post.title }}</a></h3>        
        <p>{{ post.excerpt }}</p>
    </li>
  {% endfor %}
</ul>
    