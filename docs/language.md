# The bliss assembly language (BASM)

All languages that ultimately targets the bliss runtime
must adhere to the assembly language format as described
in this document.

## The syntax of BASM

BASM has three sections, `@data`, `@code` and `@func`
sections.

`@data` describes all the constants, including numbers
and strings that are being used. They are heap-allocated
static lifetime immutable blobs.

`@code` stores the actual instructions that are going
to be executed by the runtime itself

`@func` is the section that stores the function signature
and it's locals, kind of like `@data` section but for
local variables

## Basic syntax of BASM

```basm
@data
; all data are stored here

@code
; code parts

@func
; function signature part

```

The sections do not follow a strict order themselves, so
each section can be written in whichever order the developer
may want.

## Line nature of BASM

BASM is inherently a line-based syntax language. What
it means is that lines are generally the defining feature
for almost everything.

Each of the section consists of specific statement
that follows some rule.

```basm

@data
	statement1
	statement2 // the indentation is not mandatory
	
@code
	statement1
	statement2

```

But a statement depends on the context, and here context
means under which section it belongs.

## The @data section

The `@data` section consists of all the static variables.
Think of this like a place where you can define constant
globals.

A statement in the `@data` section is as follows:

statement -> name type value

Here, `name` is a string that follows [a-z][A-Z] only
`type` is the data types which we will discuss in a bit
`value` is again sensetive to the `type` as we discussed

- Primitive types

The following types are as follows:
[ i8, i16, i32, i64, u8, u64, f32, f64 ]

and their names are self-explainatory.

- Array types

Just use `*` after a type to denote the start of an array

- Values

Now, let's understand how the syntax itself works using
values

```basm

@data
	helloWorld char* "Hello World"
	a i32* [0, 1, 2, 3, 4]
	k char** [ "Hello World", "Second String!", "Third String" ] ; multi dimensional strings are also allowed!
	b i32: [0|25]; [x| y] <- x = initial value, y = total static size
	c i32 56
	d i32 -8
	e f32 2.368
	; etc..

```

That's pretty much it in regards to the `@data` section

## The @func section

This section consists of sub-sections that defines a
function itself.

The statement is again further divided into multiple parts
now.

statement -> 	$name (type, type) ::section
	sub_statement -> identical to @data statement
	
Let's unpack this a bit more:

```basm
	
@func
	$main (int, char*) ::main
		a int 10
		b int 20
		c char* "Hello World"
```

So the local variable definitions are identical to how we
would define the `@data` section itself.

What's new is the function signature itself, which is self
explainatory, it just means you are taking the said arguments
as variables.

## The @code section

This is the interesting part, because this contains the actual
code that needs to be executed.

Code part is also analyzed line wise, to make it easier to