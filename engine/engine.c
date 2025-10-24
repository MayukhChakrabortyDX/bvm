#include "engine.h"
#include "opcode.h"
#include <stdio.h>
#include <string.h>

//Okay so this macro simply helps me avoid repetitive code in COMPILE TIME!!
#define DISPATCH_TABLE_TYPES(NAME) \
    [OP_i64##NAME] = &&OP_i64##NAME, \
    [OP_i32##NAME] = &&OP_i32##NAME, \
    [OP_i16##NAME] = &&OP_i16##NAME, \
    [OP_i8##NAME] = &&OP_i8##NAME, \
    [OP_f32##NAME] = &&OP_f32##NAME, \
    [OP_d64##NAME] = &&OP_d64##NAME

#define COMPOSE(name, T, fibre, L) \
    OP_i64##name: T(i64, fibre); goto L; \
    OP_i32##name: T(i32, fibre); goto L; \
    OP_i16##name: T(i16, fibre); goto L ;\
    OP_i8##name: T(i8, fibre); goto L; \
    OP_f32##name: T(f32, fibre); goto L; \
    OP_d64##name: T(d64, fibre); goto L;

void schedule_fibres(Scheduler *pool, uint64_t *instructions, uint8_t *heap, struct BlockUnit *heap_metadata, MethodTable *table) {

    //use a doubly-linked list for faster dispatch algorithm.

    struct Fibre *fibre = pool->ptr;
    static void *dispatch_fetch_table[2] = { &&FETCH, &&__NEXT };
    static void *dispatch_table[OP_PROGRAM_END + 1] = {
        DISPATCH_TABLE_TYPES(ADD),
        DISPATCH_TABLE_TYPES(SUB),
        DISPATCH_TABLE_TYPES(MUL),
        DISPATCH_TABLE_TYPES(DIV),

        [OP_iRESIZE_8] = &&OP_iRESIZE_8,
        [OP_iRESIZE_16] = &&OP_iRESIZE_16,
        [OP_iRESIZE_32] = &&OP_iRESIZE_32,

        [OP_iCONVf] = &&OP_iCONVf, 
        [OP_lCONVd] = &&OP_lCONVd, 
        [OP_fCONVi] = &&OP_fCONVi, 
        [OP_dCONVl] = &&OP_dCONVl,

        [OP_JUMP] = &&OP_JUMP,
        [OP_CALL] = &&OP_CALL,
        [OP_SYSCALL] = &&OP_SYSCALL,
        [OP_RETURN] = &&OP_RETURN,

        DISPATCH_TABLE_TYPES(EQ),
        DISPATCH_TABLE_TYPES(NEQ),
        DISPATCH_TABLE_TYPES(LTEQ),
        DISPATCH_TABLE_TYPES(LE),

        [OP_MOV] = &&OP_MOV,
        [OP_CLEAR] = &&OP_CLEAR,

        [OP_LOAD] = &&OP_LOAD,

        [OP_ALLOC] = &&OP_ALLOC,
        [OP_FREE] = &&OP_FREE,

        [OP_MEMSTORE] = &&OP_MEMSTORE,
        [OP_REGLOAD] = &&OP_REGLOAD,

        [OP_PROGRAM_END] = &&OP_PROGRAM_END
    };

    goto __MAIN;

    FETCH:
        //this part is for the scheduler's logic of moving
        //through the fibre pointers.
        //?: Using doubly linked list, so that's that.
        fibre = pool->next_fibre->ptr;
        pool = pool->next_fibre;

    __MAIN:
        
        //TODO: Minimize these waiting stuff, maybe minimize it someway.
        //?: Todo is done
        //in our enum, OP_PROGRAM_END denotes the last opcode, so there can't be something more than that
        goto *dispatch_fetch_table[fibre->status == WAITING ? 0 : 1];
        //TODO: Verify the bytecode beforehand.
        // if ( fibre->instructions[ fibre->registers[RPC].u64 ] > OP_PROGRAM_END ) {
        //     goto INVALID;
        // }

    __NEXT:
        //MUCH FASTER than switch or if statements.
        goto *dispatch_table[instructions[ fibre->registers[RPC].u64 ]];

    COMPOSE(ADD, _ADD, fibre, FETCH)
    COMPOSE(MUL, _MUL, fibre, FETCH)
    COMPOSE(SUB, _SUB, fibre, FETCH)
    COMPOSE(DIV, _DIV, fibre, FETCH)

    OP_iRESIZE_8:  RESIZE(i8);  goto FETCH;
    OP_iRESIZE_16: RESIZE(i16); goto FETCH;
    OP_iRESIZE_32: RESIZE(i32); goto FETCH;

    OP_iCONVf: CONV(float, i32, f32) goto FETCH;
    OP_lCONVd: CONV(double, i64, d64) goto FETCH;
    OP_fCONVi: CONV(int32_t, f32, i32) goto FETCH;
    OP_dCONVl: CONV(int64_t, d64, i64) goto FETCH;

    OP_FREE:       
        vmfree(fibre->registers[R1].u64, heap_metadata, &fibre->registers[RERR].u64);
        fibre->registers[RPC].u64++; goto FETCH;
    
    OP_ALLOC:  
        vmalloc(fibre->registers[R1].u64, &fibre->registers[ROUT].u64,heap_metadata, &fibre->registers[RERR].u64);
        fibre->registers[RPC].u64++; goto FETCH;

    OP_LOAD:  
        memcpy(
            //this looks a bit complicated so let me clarify
            //the destination is a register, so we need to pass pointe
            //to the destination register.
            //but the address to the register is stored in the instructions space, just next to the 
            //current one.
            //locate the register itself -> the first operand is the address to the register
            &fibre->registers[
                instructions[
                    fibre->registers[RPC + 1].u64
                ]
            ].u64, 
            //locate the heap -> the second operand is the address to the heap
            &heap[
                instructions[_RPC + 2]
            ], 
            //size to copy -> copy this many bytes from heap to the register.
            instructions[ _RPC + 3 ]
        ); _RPC += 4; goto FETCH;

    OP_MEMSTORE:      
        memcpy(
            &heap[
                fibre->registers[R1].u64 + //address
                fibre->registers[GR1].u64 //offset
            ], 
            &fibre->registers[R2].u64, 
            instructions[ _RPC + 1 ]
        ); _RPC+=2; goto FETCH;

    OP_REGLOAD:      //address to heap is stored in R1, offset in R2
        memcpy(
            &fibre->registers[ROUT].u64, 
            &heap[
                fibre->registers[R1].u64 +//address
                fibre->registers[R2].u64 //offset
            ], 
            instructions[_RPC + 1]
        ); _RPC += 2; goto FETCH;

    OP_MOV:       
        fibre->registers[ instructions[RPC + 1] ].u64 = fibre->registers[ instructions[RPC + 2] ].u64;
        fibre->registers[RPC].u64+=3; goto FETCH;

    OP_CLEAR:     
        fibre->registers[ instructions[RPC + 1] ].u64 = 0;
        fibre->registers[RPC].u64 += 2; goto FETCH;

    OP_JUMP: _RPC = instructions[ fibre->registers[RPC + 1].u64 ]; goto FETCH;

    OP_CALL: //TODO

    OP_SYSCALL:
        fibre->status = WAITING;
        //a system call can ONLY manipulate registers and heap.
        table[instructions[_RPC + 1]](fibre, heap); _RPC+=2;
        goto FETCH;

    OP_RETURN: //TODO

    COMPOSE(EQ, EQUAL, fibre, FETCH)
    COMPOSE(NEQ, EQUAL, fibre, FETCH)
    COMPOSE(LE, EQUAL, fibre, FETCH)
    COMPOSE(LTEQ, EQUAL, fibre, FETCH)

    // naturally ends VM
    OP_PROGRAM_END: 
        fibre->status = TERMINATED;
        //?: the reference is itself, so that means no more fibre left to execute.
        if ( pool->next_fibre == pool ) {
            return;
        }
        //? rewires the pool for next/previous pointers
        pool->previous_fibre->next_fibre = pool->next_fibre;
        pool->next_fibre->previous_fibre = pool->previous_fibre;
        //? There MUST exist a next fibre, so switch there
        pool = pool->next_fibre;

        //? No need for the scheduler to run again, jump directly to __MAIN.
        goto __MAIN;

    return;

}