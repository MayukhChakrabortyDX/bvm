#pragma once
#include "fibre.h"
#include <stdint.h>
#include <stdlib.h>

/*
    As always, since this factory can be reused for virtual thread,
    we cannot assume that heap to be allocated.

    Since heaps can be shared, it's best left to the main function
    to manage all threads and execution engine itself.
*/
struct Fibre *fibre_factory(
    uint16_t call_stack_size, 
    uint64_t* call_stack, 
    uint8_t* heap, 
    struct BlockUnit* heap_metadata, 
    uint64_t* instructions
) {

    struct Fibre *fibre = (struct Fibre*)malloc(sizeof(struct Fibre*));

    fibre->call_stack_head=0;
    fibre->call_stack = call_stack;
    fibre->call_stack_size = call_stack_size;
    
    fibre->heap = heap;
    fibre->heap_metadata = heap_metadata;

    fibre->instructions = instructions;
    
    return fibre;

}