---
layout: post
title: Data Structures
categories: toplevel
---
The PARC Library provides implementation of common data structures.

<ul id="blog-posts" class="posts">
  {% for post in site.categories.datastructures %}
    <li>
      <h3><a href="{{site.baseurl}}{{ post.url }}">&raquo; {{ post.title }}</a></h3>        
        <p>{{ post.excerpt }}</p>
    </li>
  {% endfor %}
</ul>
    