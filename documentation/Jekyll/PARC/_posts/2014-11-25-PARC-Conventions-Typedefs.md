---
layout: article
title: Typedefs
categories: conventions
---
The use of C typedef definitions are to be used to promote
precision in expressing the domain or range of a value, to improve readability, and to enforce better type checking.

As an example of a good use of a typedef, consider:
{% highlight c %}
typedef int boolean;
{% endhighlight %}
While the standard `int` type is sufficient to convey a boolean value,
the type `boolean` makes it clear exactly the intent of the programmer who says:

{% highlight c %}
boolean flag;
{% endhighlight %}

New typedefs must be named carefully to convey meaning, and never be used for syntactic shorthand.

