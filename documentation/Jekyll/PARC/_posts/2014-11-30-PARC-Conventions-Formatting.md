---
layout: article
title: Formatting
categories: conventions
---

The following is a list of rules surrounding the basics of C code style involving space and braces.

## Do Not Use The Tab Character

Raw tab characters in the source files make the source code difficult
to read because different programs treat the tabs differently.
As a result the same code can look very different depending on whether it's printed or displayed,
or even displayed with different programs.in different views.

Code that is aligned vertically is indented or aligned on columns that are multiples of 4.


## K&R Bracing Style

The K&R Bracing Style was first introduced in The C Programming Language by Brian Kernighan and Dennis Ritchie.
Except for function definitions, 
the opening brace is placed at the end of the enclosing statement and the closing brace is on a line on its own
lined up with the enclosing statement.
Statements and declaration between the braces are indented relative to the enclosing statement.

For function definitions, the brace is placed on a line on its own lined up with the function declaration.

Except for function definitions, opening braces are placed on the same line as the enclosing statement.
Statements within the enclosed body are indented 1 tab space.
The closing brace is indented to the same level as the start of the enclosing statement.

For example:

{% highlight c %}
void
f(int a)
{
    int i;
    
    if (a > 0) {
        i = a;
    } else {
        i = a;
    }
}
{% endhighlight %}

## K&R Parenthesis Style

The K&R style for parenthesis for `if`, `while`, `switch` and `sizeof` inserted a space between the keyword and the opening parenthesis.

{% highlight c %}
for (int i = 0; i < 10; i++) {
    // ...
}

while (true) {
    // ...
}

switch (myEnum) {
    // ...
}
{% endhighlight %}

It isn't necessary to surround a return value with parenthesis,
but if you do place a space between `return` keyword and the opening paranthesis.
Essentially the same style as an `if`, `while`, `switch`  statement.
{% highlight c %}
    return true;
    
    return (a + 7);
{% endhighlight %}

## K&R Operator Style

The K&R style is complicated to describe, but basically the style places a space on both sides of the operator.
    
{% highlight c %}
if (i == 0) {

}

bool result = (success == true) && (bits & COMPLETE);

a = b + c;

{% endhighlight %}

Exceptions are the operators `!`, `~` and `-` when used for negation,
and in theses cases the operator has a space on the left, but no space on the right.

Here are some examples.
{% highlight c %}
if (!success) {

}

x = -a;

bool result = (success == true) && ~(bits & WARNED);

{% endhighlight %}

## 4 Spaces per Tab
One tab is 4 spaces.

## Sub-Statements Are Always Brace Enclosed

Loop and conditional statements always have brace enclosed sub-statements.

Correct:
{% highlight c %}
if (true) {
    ...
}
{% endhighlight %}

Incorrect:
{% highlight c %}
if (true)
    ...
{% endhighlight %}

The code reads consistently if all conditional and loop statements have braces.
The braces clearly and at a glance disambiguate the scope of the conditional.
Even if there is only a single statement after the condition or loop statement today,
there might be a need for more code in the future.

## Empty Braces Must Contain a Comment

Conditionals or loops with no instructions must contain a comment indicating there is specifically no content.

{% highlight c %}
while (...) {
    /* nothing to do */
}
{% endhighlight %}

## One Statement Per Line

Each statement shall be placed on a line on its own.

Putting several statements on the same line only makes the code hard to read.

## Separate Variable Declarations

Declare each variable on it's own line near to where it is used.
Variables declared at the beginning of a function, like old-timey C code,
looses context.

Avoid side-effects when initializing a variable.

This makes it easier to see all variables.

All binary arithmetic, bitwise and assignment operators and the ternary conditional operator (?:) shall be surrounded by spaces;
the comma operator shall be followed by a space but not preceded;
all other operators shall not be used with spaces.

## Function Return Types on a Separate Line
This allows the actual function name to always appear at column 1 of a line.

Like this:
{% highlight c %}
PARCPathName *
parcPathName_Parse(const char *path)
{
}
{% endhighlight %}


## Do Not Use Goto
Using the goto keyword should be avoided.

For all the arguments you have heard about situations where you need to use goto,
just realise they are all red-herrings.
Well designed and properly factored code does not need to use goto.

## Multi-line Statements
Consider breaking up complex expressions that span multiple lines.
If that's impossible, then divide the expression into sub-expressions 
and break lines between the sub-expressions, start a new line indent by 2 spaces and the connecting operator.

For example:
{% highlight c %}
int a = 1 + 2 + 3 + 4;

int a = 1 + 2
  + 3 + 4;
  
if ((function(a) || function(b)) && (function(c) || function(d))) {
    // ...
}

if ((function(a) || function(b)) 
  && (function(c) || function(d))) {
    // ...
 }

{% endhighlight %}

    