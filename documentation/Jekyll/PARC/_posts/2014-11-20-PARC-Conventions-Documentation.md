---
layout: article
title: Documentation
categories: conventions
---
This section describes what is expected of function and module documentation.

XCode users have a set of Code Snipppet templates for writing C Source and Header file documentation and function documentation.

Below is a portion of an article written by Christopher Diggins from Autodesk, which is a fine summary of what is expected of
API documentation.

---------------------------

# Writing Code Comments for Effective API Reference Documentation 
Reference documentation for the API of SDKs is often generated from code comments using tools such as Doxygen. This document has two sections.
 
The first section describes how to write code comments that can help generating documentation that is most useful for SDK users. It is based on the documenting guidelines used by development teams at several companies including Autodesk, Microsoft, and Sun.
 
The second section provides a set of guidelines and best practices specifically for using Doxygen.
## 11 Rules for Effective API Documentation 
1. Document all API elements
2. Avoid undocumentation - say more than just the obvious
3. Don't postpone - document before code is checked-in
4. Define your jargon
5. Prioritize when necessary 
6. Avoid assumptions
7. Write to the reader, not yourself.    
8. Be concise  
9. Use code examples when appropriate
10. Use cross-linking extensively
11. Be professional


### Document all API Elements
At a minimum all API elements in an SDK should have at least a sentence to describe them. This should be the minimum bar for documentation. Even things that seem trivial must be documented. Remember, when you are adding a function or class or enumeration, it seems completely obvious to you why this is here because you are looking directly at the header file where things are defined. However, the end-user often sees these things out of context, and needs the documentation to help them orient themselves. They don't want to have to navigate to the definitions of things all the time. Consider the simple example: 

{% highlight c %}
class FileEvent : Event
{
  enum type
  {
	FileOpened,
	FileClosed
  };
} 
{% endhighlight%}

So here it probably seems that there is no reason to document the "FileOpened" enumeration. Consider the context where an SDK user encounters code "int n = FileOpened". I'm not saying that this is good code, but it happens. So here the precise meaning of "FileOpened" is not clear. It may be a variable that was defined somewhere else indicating that the file was opened, or it may be part of some event handling logic. The user doesn't know . If they are using a modern IDE like Visual Studio, a single line of documentation for the enumeration would cause tool-tip text box to show up. In this case just a simple line of documentation can help reduce a developer's confusion signficantly:

{% highlight c %}
/// Used to signal that a file related event (Event) has occurred.  
/// For example a file has been opened (FileEvent::FileOpened) or
/// closed (FileEvent::FileClosed). These events will be
/// triggered automatically by the system whenever a file is opened 
/// or closed by the user or through the methods File::Open() 
/// and File::Close().      
class FileEvent : Event
{
  /// Identifies the kind of file event 
  enum type
  {
	/// Identifies a FileEvent as corresponding to the opening of a file. 
	/// Thrown by the system in response to a File::Open() call.
    FileOpened, 
	/// Identifies a FileEvent as corresponding to the closing of a file. 
	/// Thrown by the system in response to a File::Close() call.
	FileClosed
  };
}  
{% endhighlight%}

### Avoid Undocumentation
A very big problem with documentation, which is arguably worse than omitting it completely, is resorting to writing undocumentation.  
 
Undocumentation is a term coined by Jeff Atwood and is used to describe code comments that are useless because they state the obvious:
Undocumentation ... looks like documentation. It shows up in the help alongside documentation, it's indexed like documentation, but it's not documentation. It doesn't actually tell anyone anything they didn't already know.  - Jeff Atwood (http://www.codinghorror.com/blog/archives/000451.html)
Here is an example of undocumentation:

{% highlight c %}
/// A file event class.
class FileEvent
{
  /// A type enumeration 
  enum type
  {
	/// The file was opened.
	FileOpened, 
	/// The file was closed
	FileClosed
  };
}   
{% endhighlight%}
Writing undocumentation is an easy mistake for anyone to make, especially when we write class or function descriptions that describe what something is, but overlook what it that a user really wants to know. To avoid writing undocumentation we have to put ourselves in the reader's shoes. What is it they need to know to use the SDK? 

To avoid undocumentation, your description should address the following points:

Why is this API element here?  
What other API elements are related? 
In what context would the SDK user need to use this API element? 
What should an SDK user avoid doing with the API element? 
What should an SDK user always do with the API element?    
What are the preconditions, or assumptions, that must hold when using the API element?
What other API element may be used as alternatives?
What concepts are relevant to the API element? (link to them) 
How does this differ from any similarly named API elements

### Don't Pospone writing Documentation (Strike while the Iron is Hot)  
Procrastination is a common reason for poor API documentation. If people don't write documentation before they check-in their code, deadlines and fire-fighting tend to keep pushing documentation to the bottom of the to-do list. Procrastination makes documentation harder, because the person documenting the code later will either not be the original author or will have forgot what it meant. 

Writing the documentation immediately not only saves time, it also improves the quality of code reviews. Code reviewers can more easily verify that code submissions conform to the author's intentions because they are clearly explained.  

### Define your Jargon
Jargon is any term whose definition is specific to the SDK, its host application, or the general problem domain. While the API documentation can't be expected to provide detailed definitions of everything, a short phrase clarifying what is meant by the term can go a long ways. 

Identifying what is jargon can be tricky, because usually things that a developer considers as common knowledge, has a special meaning or interpretation within the context of the SDK. Returning to our previous "FileEvent" example, the jargon here is "event". Many SDKs have a concept of event, but its precise meaning varies. 

Jargon should be defined in a single place, usually a class definition, where it makes the most sense. For example you wouldn't define "event" in the FileEvent class, but instead in the base class called "Event". Whenever you refer to the term in your documentation you should then include the API name in parentheses, events (Event) so that the documentation generator can link the class.  

### Prioritize when Neccessary
 When it is not possible to document everything correctly, within a certain time frame, it is advisable to try and focus on the most important API elements rather than do a little bit on everything. The most important thing is to focus first on class descriptions. The 80/20 rule of thumb can then be applied to decide which classes to focus on. This states that roughly 20% of the API is going to be consulted 80% of the time.

While there is no precise method to determine which classes are the most important, there are several different techniques/heuristics that can be used: 
 
Base classes are generally higher-priority than derived classes
The number of member functions a class has (not counting inherited members) is an indicator of its priority.
If a class is associated with an SDK-specific concept (e.g. Node, Event, Filter, ConnectionPoint, Plug) then it is a higher-priority.
The more API elements that reference a specific API element, the more important it is. You can measure this by running a link-checker (such as Xenu) on the generated HTML documentation.  
Classes which can be described as utility or helper classes are generally less important.
While not entirely reliable, the length of a class name is frequently inversely related to its priority. The shorter the name, the more important it generally is.
SDK user feedback (e.g. via forums, and support questions)
Gathering SDK usage statistics
 
One approach to applying these heuristics in a methodical manner is to create an excel sheet, with a list of all the classes. For each heuristic use a column to assign a value of 1 to 3 representing whether it is high, medium or low priority according to that heuristic. In an additional column, sum the values of the row. You can then sort the list by the final column from low to high.          
6. Avoid Assumptions
The most important rule of communication may be to avoid assumptions as much as possible. Some assumptions almost seem inescapable, but we still have a tendency to make more assumptions than necessary. Some important assumptions to avoid are: 
The reader speaks English as a first language - To take this into consideration means that you have to try and write clearly, consistently, and avoid informal or idiomatic language.
The reader has the same learning style as yourself - There are many different learning styles that a user has, and which must be taken into account.
The reader will consult the headers/samples/programmer's guide/Google/a book/etc. - Even if you tell the reader to consult a specific resource, it doesn't guarantee that they will. People don't like being told what to do, are very busy, and have different learning styles. 
Topic X was read before the current one - Most documentation is consulted in a non-linear manner. If you have a suggested order of reading documentation, link topics to their prerequisite topics.

While this may feel like it ties your hands when writing documentation, but it doesn't have to. You just have to try and say what the reader needs in as simple a manner as possible.   

### Write to the Reader
An important guideline for any technical communication is to make sure that you write what the reader wants or needs, not just what you feel like writing. It is often tempting to explain things that we find interesting, or that we understand well. The user has their own agenda, and needs documentation that helps them get their job done as efficiently as possible. 

### Be Concise

### Use Inline Code Examples when Appropriate
Inline code examples are useful for illustrating and reinforcing concepts. Don't bother writing code example that illustrate nothing more than the parameter types. Do not substitute code examples for actual definitions, or explanations. This is because generally a code example can only illustrate particular use cases, and cannot easily explain all of the preconditions, postconditions, and invariants of an API element.  

### Cross-Linking
Links to related material is a useful clue to users as to the role of an API element even in the absence of documentation. It is quick and easy to do, and it helps users to navigate the documentation quickly and easily. Realize that many users land on random topics because of a search on a specific term, and need to understand how API elements are related to each other. This is also very important when key concepts are explained in other API element documentation, that you want the user to read first. If this is the case, be explicit that the user should read one piece of documentation before another. 

### Be Professional
Keep in mind that documentation generated from code comments will become public facing, and has an impact on the public perception of the company. Some basic guidelines:

Use correct product names should be used 
Write complete and grammatically correct sentences that end with a period.
Use correct company branding.
Use correct spelling and capitalization.
Use the appropriate attribution and copyright messages.   

