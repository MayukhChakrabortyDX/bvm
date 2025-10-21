# System Call Specification - 3/10/2025

BlissVM syscall is a way to interupt the virtual machine
to call a function that lives outside the bytecode itself.

Generally, syscall is used to interact with interfaces
like standard io, networking, graphics and so on.

## System Call Proposal

A system call in BlissVM works in a very standard way. The
bytecode `syscall` or `syscall_inline` is used for this purpose.

A system call function gets access to the entire thread, as
the thread (called as fibre) is passed to the said function to
work on.

### System call convention

The convention is to first load a relevant data to the RSYS register
and then call a certain system function via `syscall` or `syscall_inline`.

System calls also have addresses. For example, `syscall_inline 0` will
invoke the function present at `0th` address.

System call functions are what we call stateless function in terms of the
VM itself. It may write data in heap, or in register, or certain ops,
that MUST be documented in the syscall only.

### Syscall registry

A syscall registry is built into the VM that points to the .dll/.so files
required for the said syscalls.

When bytecode is loaded, the bytecode provides information of the syscall
being used.

Each version of the VM might have different registry indices, different
implementation altogether, so it's advised to read the documentation
for that stable version and all information about it.

## Implementation

Some changes to the bytecode is to be done to allow pre-loading of the
linked libraries for optimal performance. Load times can always be
traded for runtime performance.

Firstly, to be a syscall function, the function must be:

- return void
- takes `Struct *Fibre` as argument

any and all changes must be done to the fibre itself.

Since syscall-specific dlls are known ahead of time,
they are stored in an array of structs, that also tells
us if they were loaded or not. They also contain information
about the name of the dynamically linked library.