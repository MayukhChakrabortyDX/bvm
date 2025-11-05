# The bliss assembly language

The bliss assembly is the underlying code that
actually runs on the runtime.

The assembler compiles the assembly to useable
bytecode by the runtime.

## The requirement

The most basic requirement to start the runtime
is the following values:

- Instructions
- Constant Table
- System Method Table
- Function Table

So the bytecode must encode this in the file
for the runtime to be able to produce an executable
context.

A few more requirements, tho optional are

- Minimum heap size
- Heap Capacity Multiplier

These tells how the memory is managed, and are
passed via the command line arguments.

## The assembly format

The assembly takes inspirations from the real
x86_64 assembly, modifiying certain parts
to make it easier.

Here's a simple example:
```bsm
@data
  str char* "Hello World"
  a int 0
end

@function
  $main (int, int), ::start
    a int 10 ; these are local variable
    b int 20 ; the second local variable stored linearly
  endf
  
  $add (int, int), ::add
  endf
end

@code
  call start
  ::add
    fregload R1 24 4 ; copy 4 bytes from the local variable, (argument)
    fregload R2 28 4 ; copt 4 bytes again
    iadd ; add the two integers
    mov RRET ROUT
    return ;return from the function
    
  ::start
    ; prepare the call arguments in the heap first
    alloc 8 ; 8 bytes because I need to pass two integers
    mov R1 ROUT
    load R2 24 ; load the offset constant
    load GR1 4 ; load the first 4 bytes
    fheap_copy
    load R2 28 ; change the offset
    mov GR3 R2
    mov GR4 R1 ; keep a copy
    load R2 4 ; offset
    uladd
    mov R1 ROUT
    mov R1 GR3
    fheap_copy
    ; now our calling agent is ready!
    mov R1 GR4; and we are done
    ; we should save this address to free this call object here
    call add
end
```

We can use a lot of optimization details here, just the fact
that we can use heap-allocated objects as arguments, which is
so funny lol. (But easy to implement as well)

## Syntax (in depth)

1. Sections
All sections starts with '@'

There are only three sections, and they must be annotated to end.

@data section stores all the global constants
@function section stores all the function information itself
@code section stores all code that we are going to work with.

### @data section

Data section is straight forward, you define static runtime values

1. data types

char, byte, short, int, long, float, double and array

<name> <type> <data>

example:

str char 'a'

or array

str char* "Hello World"

or give fixed size array

str char[24] "Hello World"
data int[25] [0, 1, 2, 3, 4, 5]

by default, all empty spaces are initialized using 0 value

### @function section

This section stores metadata about the function itself.
Let's identify the syntax itself:

$name (arg1, arg2) ::label
endf

so we use $<name> to define the name of the Function.
the (arg1, arg2, ...) stores the argument list for the
Function.

The ::label is the function label, the ultimate jump that's
going to happen eventually.

The function can also define local variables ahead of time
(must be actually) for it to work.

$name (arg1, arg2) ::label
  <name> <type> <data>
endf

The syntax is same as how we define static variables, except
those statics are immutable, but function local variables
are mutable by certain function instance.

### @code section

This is where all the code actually lies. You have to write
the code here