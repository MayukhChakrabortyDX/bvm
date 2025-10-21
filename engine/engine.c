#include "engine.h"

#include "../fibre/fibre.h"
#include <stdint.h>
#include "opcode.h"
#include <stdio.h>
#include "execution/arithmetic.h"
#include "execution/memory.h"
#include "execution/logical.h"

void schedule_fibres(struct Scheduler *pool) {

    struct Fibre *fibre = pool->ptr;

    static void *dispatch_table[OP_PROGRAM_END + 1] = {
        [OP_ADD] = &&OP_ADD,
        [OP_SUB] = &&OP_SUB,
        [OP_MUL] = &&OP_MUL,
        [OP_DIV] = &&OP_DIV,

        [OP_iRESIZE_8] = &&OP_iRESIZE_8,
        [OP_iRESIZE_16] = &&OP_iRESIZE_16,
        [OP_iRESIZE_32] = &&OP_iRESIZE_32,

        [OP_uRESIZE_8] = &&OP_uRESIZE_8,
        [OP_uRESIZE_16] = &&OP_uRESIZE_16,
        [OP_uRESIZE_32] = &&OP_uRESIZE_32,

        [OP_iCONVf] = &&OP_iCONVf, 
        [OP_lCONVd] = &&OP_lCONVd, 
        [OP_fCONVi] = &&OP_fCONVi, 
        [OP_dCONVl] = &&OP_dCONVl,

        [OP_JUMP] = &&OP_JUMP,
        [OP_CALL] = &&OP_CALL,
        [OP_SYSCALL] = &&OP_SYSCALL,
        [OP_RETURN] = &&OP_RETURN,

        [OP_EQ] = &&OP_EQ,
        [OP_NEQ] = &&OP_NEQ,
        [OP_LTEQ] = &&OP_LTEQ,
        [OP_LE] = &&OP_LE,

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

        if ( pool->next_fibre ) {
            fibre = pool->next_fibre->ptr;
            pool = pool->next_fibre;
        }

    __MAIN:
        //in our enum, OP_PROGRAM_END denotes the last opcode, so there can't be something more than that
        if ( fibre->status == WAITING ) {
            goto FETCH;
        } //if a fibre is terminated it will automatically go to the terminated opcode.

        if ( fibre->instructions[ fibre->registers[RPC].u64 ] > OP_PROGRAM_END ) {
            goto INVALID;
        }

        //MUCH FASTER than switch or if statements.
        goto *dispatch_table[fibre->instructions[ fibre->registers[RPC].u64 ]];

    OP_ADD:   ADD(fibre);          goto FETCH;
    OP_SUB:   SUB(fibre);          goto FETCH;
    OP_MUL:   MUL(fibre);          goto FETCH;
    OP_DIV:   DIV(fibre);          goto FETCH;

    OP_iRESIZE_8:  iRESIZE_8(fibre);  goto FETCH;
    OP_iRESIZE_16: iRESIZE_16(fibre); goto FETCH;
    OP_iRESIZE_32: iRESIZE_32(fibre); goto FETCH;

    OP_uRESIZE_8:  uRESIZE_8(fibre);  goto FETCH;
    OP_uRESIZE_16: uRESIZE_16(fibre); goto FETCH;
    OP_uRESIZE_32: uRESIZE_32(fibre); goto FETCH;

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

    OP_EQ:   EQUAL(fibre);           goto FETCH;
    OP_NEQ:  NOT_EQUAL(fibre);       goto FETCH;
    OP_LTEQ: LESS_THAN_EQUAL(fibre);   goto FETCH;
    OP_LE:   LESS_THAN(fibre);       goto FETCH;

    OP_PROGRAM_END: {
        
        fibre->status = TERMINATED;

        if ( !pool->next_fibre ) {
            return;
        }

        //? rewires the pool for next/previous pointers
        pool->previous_fibre->next_fibre = pool->next_fibre;
        pool->next_fibre->previous_fibre = pool->previous_fibre;
        //? There MUST exist a next fibre, so switch there
        pool = pool->next_fibre;

        //? No need for the scheduler to run again, jump directly to __MAIN.
        goto __MAIN;

    }  // naturally ends VM

    INVALID:
        printf("Internal error: Unrecognized Bytecode %X\n",
            (uint32_t)fibre->instructions[fibre->registers[RPC].u64]);
        return;

}