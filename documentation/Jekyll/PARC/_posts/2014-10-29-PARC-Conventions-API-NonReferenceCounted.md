---
layout: article
title: API Conventions for Non Reference Counted Memory.
categories: conventions
---

The PARC library supports both reference counted memory and non-reference counted memory.
In addition to the common conventions, here are the conventions for the APIs supporting non-reference counted memory.

## Canonical Function Names ##

* `void PREFIXName_Destroy(PREFIXName **instancePointer)`

Destroy an instance of `PREFIXName`.
The value of `instancePointer` is always set to NULL after the destroy

Typically an instance of `PREFIXName` is implemented to have pointers to other, potentially reference counted, objects.
When the reference count of the `PREFIXName` instances is `1`, the next invocation of
`PREFIXName_Destroy` will free the memory storing `PREFIXName` and the pointers to the other,
potentially reference counted objects will be lost.

<ul id="blog-posts" class="posts">
  {% for post in site.categories.conventions %}
    <li>
      <h3><a href="{{site.baseurl}}{{ post.url }}">&raquo; {{ post.title }}</a></h3>        
        <p>{{ post.excerpt }}</p>
    </li>
  {% endfor %}
</ul>
    