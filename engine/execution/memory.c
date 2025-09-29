/*
    Welcome to the memory section of the opcode execution.
    As the name suggest, this section covers the memory part
    like data moving in registers, allocation of heap and anything
    related to direct memory operations.
*/
#pragma once
#include "../../fibre/fibre.h"
#include "../../memory/heap.c"
#include <string.h>

static inline void move_data(struct Fibre *fibre) {

    fibre->registers[ fibre->instructions[RPC + 1] ].u64 = fibre->registers[ fibre->instructions[RPC + 2] ].u64;
    fibre->registers[RPC].u64+=3;

}

static inline void clear_register(struct Fibre *fibre) {

    fibre->registers[ fibre->instructions[RPC + 1] ].u64 = 0;
    fibre->registers[RPC].u64 += 2;

}

static inline void allocate_heap(struct Fibre *fibre) {

    vmalloc(fibre->registers[R1].u64, &fibre->registers[ROUT].u64, fibre->heap_metadata, &fibre->registers[RERR].u64);
    fibre->registers[RPC].u64++;

}

static inline void free_heap(struct Fibre *fibre) {

    vmfree(fibre->registers[R1].u64, fibre->heap_metadata, &fibre->registers[RERR].u64);
    fibre->registers[RPC].u64++;

}

/*
TODO: all the loaders must also verify the existence of the block before reference
* Search the blocks and see if the block has enough bytes to even load from.
* If not, we can produce a segmentation fault -> accesing data in cross blocks could cause
* undefined behavior.
*/

static inline void load_byte(struct Fibre *fibre) {

    //loads 1 byte (in essence just copies.)
    //load_8 register offset address

    memcpy(

        //this looks a bit complicated so let me clarify
        //the destination is a register, so we need to pass pointe
        //to the destination register.

        //but the address to the register is stored in the instructions space, just next to the 
        //current one.

        &fibre->registers[
            fibre->instructions[
                fibre->registers[RPC + 1].u64
            ]

        ].u64, 
        
        &fibre->heap[
            fibre->instructions[
                //offset and addresses represents a single byte.
                fibre->registers[RPC + 2].u64 + //offset
                fibre->registers[RPC + 3].u64 //address
            ]
        ], 
        
        1
    );

    fibre->registers[RPC].u64 += 4;

}

static inline void load_2bytes(struct Fibre *fibre) {

    //loads 1 byte (in essence just copies.)
    //load_8 register offset address

    memcpy(

        //this looks a bit complicated so let me clarify
        //the destination is a register, so we need to pass pointe
        //to the destination register.

        //but the address to the register is stored in the instructions space, just next to the 
        //current one.

        &fibre->registers[
            fibre->instructions[
                fibre->registers[RPC + 1].u64
            ]

        ].u64, 
        
        &fibre->heap[
            fibre->instructions[
                //offset and addresses represents a single byte.
                fibre->registers[RPC + 2].u64 + //offset
                fibre->registers[RPC + 3].u64 //address
            ]
        ], 
        
        2
    );

    fibre->registers[RPC].u64 += 4;

}

static inline void load_4bytes(struct Fibre *fibre) {

    //loads 1 byte (in essence just copies.)
    //load_8 register offset address

    memcpy(

        //this looks a bit complicated so let me clarify
        //the destination is a register, so we need to pass pointe
        //to the destination register.

        //but the address to the register is stored in the instructions space, just next to the 
        //current one.

        &fibre->registers[
            fibre->instructions[
                fibre->registers[RPC + 1].u64
            ]

        ].u64, 
        
        &fibre->heap[
            fibre->instructions[
                //offset and addresses represents a single byte.
                fibre->registers[RPC + 2].u64 + //offset
                fibre->registers[RPC + 3].u64 //address
            ]
        ], 
        
        4
    );

    fibre->registers[RPC].u64 += 4;

}

static inline void load_8bytes(struct Fibre *fibre) {

    //loads 1 byte (in essence just copies.)
    //load_8 register offset address

    memcpy(

        //this looks a bit complicated so let me clarify
        //the destination is a register, so we need to pass pointe
        //to the destination register.

        //but the address to the register is stored in the instructions space, just next to the 
        //current one.

        &fibre->registers[
            fibre->instructions[
                fibre->registers[RPC + 1].u64
            ]

        ].u64, 
        
        &fibre->heap[
            fibre->instructions[
                //offset and addresses represents a single byte.
                fibre->registers[RPC + 2].u64 + //offset
                fibre->registers[RPC + 3].u64 //address
            ]
        ], 
        
        8
    );

    fibre->registers[RPC].u64 += 4;

}

static inline void store_byte(struct Fibre *fibre) {

    memcpy(
        &fibre->heap[
            fibre->registers[R1].u64 + //address
            fibre->registers[GR1].u64 //offset
        ], 
        &fibre->registers[R2].u64, 
        1
    );

    fibre->registers[RPC].u64++;

}

static inline void store_2bytes(struct Fibre *fibre) {

    memcpy(
        &fibre->heap[
            fibre->registers[R1].u64 + //address
            fibre->registers[GR1].u64 //offset
        ], 
        &fibre->registers[R2].u64, 
        2
    );

    fibre->registers[RPC].u64++;

}

static inline void store_4bytes(struct Fibre *fibre) {

    memcpy(
        &fibre->heap[
            fibre->registers[R1].u64 + //address
            fibre->registers[GR1].u64 //offset
        ], 
        &fibre->registers[R2].u64, 
        4
    );
    
    fibre->registers[RPC].u64++;

}

static inline void store_8bytes(struct Fibre *fibre) {

    memcpy(
        &fibre->heap[
            fibre->registers[R1].u64 + //address
            fibre->registers[GR1].u64 //offset
        ], 
        &fibre->registers[R2].u64, 
        8
    );
    
    fibre->registers[RPC].u64++;

}

static inline void regload_byte(struct Fibre *fibre) {

    //address to heap is stored in R1, offset in R2
    memcpy(
        &fibre->registers[ROUT].u64, 
        &fibre->heap[
            fibre->registers[R1].u64 +//address
            fibre->registers[R2].u64 //offset
        ], 
        1
    );

    fibre->registers[RPC].u64++;

}

static inline void regload_2bytes(struct Fibre *fibre) {

    //address to heap is stored in R1, offset in R2
    memcpy(
        &fibre->registers[ROUT].u64, 
        &fibre->heap[
            fibre->registers[R1].u64 +//address
            fibre->registers[R2].u64 //offset
        ], 
        2
    );

    fibre->registers[RPC].u64++;

}static inline void regload_4bytes(struct Fibre *fibre) {

    //address to heap is stored in R1, offset in R2
    memcpy(
        &fibre->registers[ROUT].u64, 
        &fibre->heap[
            fibre->registers[R1].u64 +//address
            fibre->registers[R2].u64 //offset
        ], 
        4
    );

    fibre->registers[RPC].u64++;

}static inline void regload_8bytes(struct Fibre *fibre) {

    //address to heap is stored in R1, offset in R2
    memcpy(
        &fibre->registers[ROUT].u64, 
        &fibre->heap[
            fibre->registers[R1].u64 +//address
            fibre->registers[R2].u64 //offset
        ], 
        8
    );

    fibre->registers[RPC].u64++;

}