/*
    Welcome to the logical section of the opcode execution.
    As the name suggest, this section covers the logical part
    of the VM, like jumps, calls, syscalls, equalities and so on.
*/


#pragma once
#include "../../fibre/fibre.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../syscall/registry.h"
#include "../types.h"
#include "../../memory/heap.h"

//these derefences are too handful to write, so here's some
//definition so that I save some typings.
//TODO: Convert this function to macros for true inlining, and also use scopes to not redefine tables.

#include "common.h"

#define EQUAL(type, fibre) _RPC += _R1(type) == _R2(type) ? 1 : 2
#define NOT_EQUAL(type, fibre) _RPC += _R1(type) != _R2(type) ? 1 : 2
#define LESS_THAN_EQUAL(type, fibre) _RPC += _R1(type) <= _R2(type) ? 1 : 2
#define LESS_THAN(type, fibre) _RPC += _R1(type) < _R2(type) ? 1 : 2

VMCALL jump_inline(struct Fibre *fibre) {

    //take the jump address from the instruction only
    fibre->registers[RPC].u64 = fibre->instructions[ fibre->registers[RPC + 1].u64 ];

}

/*

    TODO: Calls works on the callstack -> allocate the call registry block
    TODO: Provide the pointer to the allocated region (start) and push the value to the stack
    TODO: Move to the address pointer given in the registry itself.

*/
VMCALL call_inline(struct Fibre *fibre) {

    if ( fibre->call_stack_head > fibre->call_stack_size ) {
        printf("Fatal Error: Call Stack Size exceeded\n");
        exit(-1);
    }

    // TODO: When call, allocate the function's space to heap and also allocate that.
    fibre->CallStack[ fibre->call_stack_head ] = malloc(sizeof(struct CallStackItem));

    // TODO: allocate the function heap
    //let's discuss how the call_inline works.
    //so in the most basic sense, it looks like call X, x is an address on the heap.
    //the value on the heap (x) actually tells us how much the function we should allocate (the first 64 bits)
    //the second 64 bits tells us the address to actually jump to.
    
    //get the address to the heap - copy first 8 bytes which is the size for the function itself.
    uint64_t size_to_allocate = 0;
    memcpy(&size_to_allocate, &fibre->heap[fibre->instructions[ _RPC + 1 ]], 8);

    //get the address where we need to actually jump to.
    uint64_t jump_to = 0;
    memcpy(&jump_to, &fibre->heap[ fibre->instructions[ _RPC + 2 ] ], 8);

    //allocate the function memory and then hopefully jump.
    uint64_t dest = 0;
    vmalloc(size_to_allocate, &dest, fibre->heap_metadata, &fibre->registers[RERR].u64);

    //TODO: store the function heap [DONE]
    fibre->CallStack[ fibre->call_stack_head ]->heap_ptr = dest; //the destination, i.e addres of the heap allocated space for you!
    //store the return type.
    fibre->CallStack[ fibre->call_stack_head ]->return_ptr = fibre->instructions[ fibre->registers[RPC + 1].u64 ];
    fibre->registers[RPC].u64 = fibre->instructions[ fibre->registers[ RPC ].u64 ];
    fibre->call_stack_head++;

}

VMCALL call_return(struct Fibre *fibre) {

    fibre->registers[RPC].u64 = fibre->CallStack[ fibre->call_stack_head - 1 ]->return_ptr;
    vmfree(fibre->CallStack[ fibre->call_stack_head ]->heap_ptr, fibre->heap_metadata, &fibre->registers[RERR].u64);
    free( fibre->CallStack[ fibre->call_stack_head - 1] );
    fibre->call_stack_head--;

}

VMCALL _syscall_inline(struct Fibre *fibre) {

    //wait for the system call to finish
    fibre->status = WAITING;
    //BTW ownership is now transferred. The syscall MUST release (status = RUNNING)
    //TODO: Implement error mechanism to handle what happens when the index is out of bounds.
    fibre->table[ fibre->instructions[ _RPC + 1 ] ].ptr( fibre );
    //call_method( fibre, fibre->instructions[ fibre->registers[RPC].u64 + 1 ], fibre->table);
    _RPC += 2;

}