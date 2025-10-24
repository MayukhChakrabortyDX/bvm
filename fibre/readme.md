# Fibre Model V1

This is the newer model for a concurrent and high-performance
stackless 'fibre'

## The specification

A fibre consists of the following

```c

typedef struct {
    Registers registers[14]; //all 14 of the registers.
} Fibre;

```
## How HEAP works?

A heap is a shared area of storage across multiple concurrent
fibres. Since a scheduler function takes a pool of this fibres,
it also instinctly takes up the pointer to the heap.

The execution engine itself manages the heap and the instructions.

## How function calls works?

Function calls are simple! The VM utilizes it's wide array of
registers for this purpose!

When we call a function, a area in the heap is allocated to
store all it's variables. The first 16 bytes in the same
heap region are extra metadata and stores the following:

[ heap_prt ][ return_address ]

When we use the `call` opcode, the VM do is:

1. allocate the heap size as expected
2. Populate the heap_prt and return address long values
3. store the heap_prt in the RFLAG register.
4. jumps to the address of the function itself.

And then the function starts to execute as is, without
any issue.

When it's time to return, it simply returns to the return
address already stored to it's heap.

This help us to:
1. avoid a stack completely and use the heap instead
2. technically unbounded function call size if the heap is big enough.