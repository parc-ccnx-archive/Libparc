---
layout: article
title: Comments
categories: conventions
---

Well written source code has well written comments.
See the section on Documentation as well.

* Comments shall be written in English.

* Use JavaDoc style comments for documentation.

{% highlight c %}
/**
 *
 */
{% endhighlight %}

* Use `/* */` style comments for multi line comments.

{% highlight c %}
/*
 *
 */
{% endhighlight %}

* Use `//` style comments to comment out code blocks.

Normally do not comment out code and then leave it in the source file as it tends to remain there forever.
But sometimes you need to temporarily comment out a section of code when collaborating, testing, or adding new features.

The technique of surrounding code with C preprocessor macros is discouraged because
it's not immediately clear that a section of code is ignored, particularly if the section is long.

A technique that makes every ignored line obvious and will work regardless of preprocessor conditionals and embedded
`/* ... */` comments is to simply use the `//` comment.
 
{% highlight c %}
// size_t
// parcBuffer_Position(const PARCBuffer *buffer)
// {
//     return buffer->position;
// }
{% endhighlight %}

* All comments shall be placed above the line(s) the comment describes, indented identically.

Being consistent on placement of comments removes any question on what the comment refers to.

>> Pro Tip!
>> If you're writing comments to explain your code, you might first try to see
>> if you can write the code in a better, more clear way.
>> You may find that you won't need to write the comment in the first place.

