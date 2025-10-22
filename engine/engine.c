#include "engine.h"

#include "../fibre/fibre.h"
#include <stdint.h>
#include "opcode.h"
#include "execution/arithmetic.h"
#include "execution/memory.h"
#include "execution/logical.h"

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

void schedule_fibres(struct Scheduler *pool) {

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
        goto *dispatch_table[fibre->instructions[ fibre->registers[RPC].u64 ]];

    COMPOSE(ADD, _ADD, fibre, FETCH)
    COMPOSE(MUL, _MUL, fibre, FETCH)
    COMPOSE(SUB, _SUB, fibre, FETCH)
    COMPOSE(DIV, _DIV, fibre, FETCH)

    OP_iRESIZE_8:  iRESIZE_8(fibre);  goto FETCH;
    OP_iRESIZE_16: iRESIZE_16(fibre); goto FETCH;
    OP_iRESIZE_32: iRESIZE_32(fibre); goto FETCH;

    OP_iCONVf: iCONVf(fibre); goto FETCH;
    OP_lCONVd: lCONVd(fibre); goto FETCH;
    OP_fCONVi: fCONVi(fibre); goto FETCH;
    OP_dCONVl: dCONVl(fibre); goto FETCH;

    OP_FREE:   free_heap(fibre);     goto FETCH;
    OP_ALLOC:  allocate_heap(fibre); goto FETCH;

    OP_LOAD:  load(fibre);   goto FETCH;
    OP_MEMSTORE:  store(fibre);   goto FETCH;
    OP_REGLOAD:  regload(fibre);   goto FETCH;

    OP_MOV:   move_data(fibre);   goto FETCH;
    OP_CLEAR: clear_register(fibre); goto FETCH;

    OP_JUMP:    jump_inline(fibre);    goto FETCH;
    OP_CALL:    call_inline(fibre);    goto FETCH;
    OP_SYSCALL: _syscall_inline(fibre); goto FETCH;
    OP_RETURN:  call_return(fibre);    goto FETCH;

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