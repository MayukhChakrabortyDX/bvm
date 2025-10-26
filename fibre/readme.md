# Lightweight and High performance Fibres

A fibre in essence is a forward circular linked list
that contains it's own registers and status window.

It's extremely lightweight, just a few bytes and hence
can be spawned in millions, only if the heap-size
support it based on function calls.

## The concept

A fibre simply stores unique registers (14, 8 bytes)
alongside a next-> pointer and a status window.

The instruction space is immutable and exists ahead
of time, so is the heap, except the heap is mutable
and allocable.

A sub-allocator (aside from malloc/free) is used to
manage this pre-allocated heap.

Since blissRT is by nature concurrent, the heap itself
has mutex only for true-multi-threaded execution points.

A single 1:N model do not need mutex because it's concurrent
and hence access the heap one after another anyways.

## Function Calls

A function call reuses the internally allocated heap itself.
A function may use local variables that are stored in the
frame itself.

The `OP_CALL` simply allocates the size of the function in
the heap, and then allocates a bit more to store the following:

[ callee_ptr ][ function_info_ptr ][ this_heap_ptr ][ return_ptr ]

Now what happens is that the heap_ptr is also stored in the RFX
register, which is used by the function to get to the correct heap
address.