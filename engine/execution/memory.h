/*
    Welcome to the memory section of the opcode execution.
    As the name suggest, this section covers the memory part
    like data moving in registers, allocation of heap and anything
    related to direct memory operations.
*/
#pragma once
#include "../../fibre/fibre.h"
#include "../../memory/heap.h"
#include <string.h>
#include "common.h"

VMCALL move_data(struct Fibre *fibre) {

    fibre->registers[ fibre->instructions[RPC + 1] ].u64 = fibre->registers[ fibre->instructions[RPC + 2] ].u64;
    fibre->registers[RPC].u64+=3;

}

VMCALL clear_register(struct Fibre *fibre) {

    fibre->registers[ fibre->instructions[RPC + 1] ].u64 = 0;
    fibre->registers[RPC].u64 += 2;

}

VMCALL allocate_heap(struct Fibre *fibre) {

    vmalloc(fibre->registers[R1].u64, &fibre->registers[ROUT].u64, fibre->heap_metadata, &fibre->registers[RERR].u64);
    fibre->registers[RPC].u64++;

}

VMCALL free_heap(struct Fibre *fibre) {

    vmfree(fibre->registers[R1].u64, fibre->heap_metadata, &fibre->registers[RERR].u64);
    fibre->registers[RPC].u64++;

}

/*
TODO: all the loaders must also verify the existence of the block before reference
* Search the blocks and see if the block has enough bytes to even load from.
* If not, we can produce a segmentation fault -> accesing data in cross blocks could cause
* undefined behavior.
*/

VMCALL load(struct Fibre *fibre) {

    //loads 1 byte (in essence just copies.)
    //load_8 register offset address

    memcpy(

        //this looks a bit complicated so let me clarify
        //the destination is a register, so we need to pass pointe
        //to the destination register.

        //but the address to the register is stored in the instructions space, just next to the 
        //current one.

        //locate the register itself -> the first operand is the address to the register
        &fibre->registers[
            fibre->instructions[
                fibre->registers[RPC + 1].u64
            ]
        ].u64, 
        
        //locate the heap -> the second operand is the address to the heap
        &fibre->heap[
            fibre->instructions[_RPC + 2]
        ], 

        //size to copy -> copy this many bytes from heap to the register.
        fibre->instructions[ _RPC + 3 ]
    );

    _RPC += 4;

}

VMCALL store(struct Fibre *fibre) {

    memcpy(
        &fibre->heap[
            fibre->registers[R1].u64 + //address
            fibre->registers[GR1].u64 //offset
        ], 
        &fibre->registers[R2].u64, 
        fibre->instructions[ _RPC + 1 ]
    );

    _RPC+=2;

}

VMCALL regload(struct Fibre *fibre) {

    //address to heap is stored in R1, offset in R2
    memcpy(
        &fibre->registers[ROUT].u64, 
        &fibre->heap[
            fibre->registers[R1].u64 +//address
            fibre->registers[R2].u64 //offset
        ], 
        fibre->instructions[_RPC + 1]
    );

    _RPC += 2;

}