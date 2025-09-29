#pragma once

#include "../fibre/fibre.h"
#include "opcode.h"
#include <stdint.h>
#include <stdio.h>

#include "execution/arithmetic.c"
#include "execution/logical.c"
#include "execution/memory.c"

static inline void execute_fibre(struct Fibre *fibre) {

    static void *dispatch_table[OP_PROGRAM_END + 1] = {
        [OP_i64ADD] = &&OP_i64ADD,
        [OP_i64SUB] = &&OP_i64SUB,
        [OP_i64MUL] = &&OP_i64MUL,
        [OP_i64DIV] = &&OP_i64DIV,

        [OP_i32ADD] = &&OP_i32ADD,
        [OP_i32SUB] = &&OP_i32SUB,
        [OP_i32MUL] = &&OP_i32MUL,
        [OP_i32DIV] = &&OP_i32DIV,

        [OP_i16ADD] = &&OP_i16ADD,
        [OP_i16SUB] = &&OP_i16SUB,
        [OP_i16MUL] = &&OP_i16MUL,
        [OP_i16DIV] = &&OP_i16DIV,

        [OP_i8ADD] = &&OP_i8ADD,
        [OP_i8SUB] = &&OP_i8SUB,
        [OP_i8MUL] = &&OP_i8MUL,
        [OP_i8DIV] = &&OP_i8DIV,

        [OP_fADD] = &&OP_fADD,
        [OP_fSUB] = &&OP_fSUB,
        [OP_fMUL] = &&OP_fMUL,
        [OP_fDIV] = &&OP_fDIV,

        [OP_dADD] = &&OP_dADD,
        [OP_dSUB] = &&OP_dSUB,
        [OP_dMUL] = &&OP_dMUL,
        [OP_dDIV] = &&OP_dDIV,

        [OP_ui64ADD] = &&OP_ui64ADD,
        [OP_ui64SUB] = &&OP_ui64SUB,
        [OP_ui64MUL] = &&OP_ui64MUL,
        [OP_ui64DIV] = &&OP_ui64DIV,

        [OP_ui32ADD] = &&OP_ui32ADD,
        [OP_ui32SUB] = &&OP_ui32SUB,
        [OP_ui32MUL] = &&OP_ui32MUL,
        [OP_ui32DIV] = &&OP_ui32DIV,

        [OP_ui16ADD] = &&OP_ui16ADD,
        [OP_ui16SUB] = &&OP_ui16SUB,
        [OP_ui16MUL] = &&OP_ui16MUL,
        [OP_ui16DIV] = &&OP_ui16DIV,

        [OP_ui8ADD] = &&OP_ui8ADD,
        [OP_ui8SUB] = &&OP_ui8SUB,
        [OP_ui8MUL] = &&OP_ui8MUL,
        [OP_ui8DIV] = &&OP_ui8DIV,

        [OP_JUMP] = &&OP_JUMP,
        [OP_CALL] = &&OP_CALL,
        [OP_SYSCALL] = &&OP_SYSCALL,
        [OP_JUMP_INLINE] = &&OP_JUMP_INLINE,
        [OP_CALL_INLINE] = &&OP_CALL_INLINE,
        [OP_SYSCALL_INLINE] = &&OP_SYSCALL_INLINE,
        [OP_RETURN] = &&OP_RETURN,

        [OP_i64_EQ] = &&OP_i64_EQ,
        [OP_i64_NEQ] = &&OP_i64_NEQ,
        [OP_i64_GTEQ] = &&OP_i64_GTEQ,
        [OP_i64_LTEQ] = &&OP_i64_LTEQ,
        [OP_i64_GE] = &&OP_i64_GE,
        [OP_i64_LE] = &&OP_i64_LE,

        [OP_i32_EQ] = &&OP_i32_EQ,
        [OP_i32_NEQ] = &&OP_i32_NEQ,
        [OP_i32_GTEQ] = &&OP_i32_GTEQ,
        [OP_i32_LTEQ] = &&OP_i32_LTEQ,
        [OP_i32_GE] = &&OP_i32_GE,
        [OP_i32_LE] = &&OP_i32_LE,

        [OP_i16_EQ] = &&OP_i16_EQ,
        [OP_i16_NEQ] = &&OP_i16_NEQ,
        [OP_i16_GTEQ] = &&OP_i16_GTEQ,
        [OP_i16_LTEQ] = &&OP_i16_LTEQ,
        [OP_i16_GE] = &&OP_i16_GE,
        [OP_i16_LE] = &&OP_i16_LE,

        [OP_i8_EQ] = &&OP_i8_EQ,
        [OP_i8_NEQ] = &&OP_i8_NEQ,
        [OP_i8_GTEQ] = &&OP_i8_GTEQ,
        [OP_i8_LTEQ] = &&OP_i8_LTEQ,
        [OP_i8_GE] = &&OP_i8_GE,
        [OP_i8_LE] = &&OP_i8_LE,

        [OP_u64_EQ] = &&OP_u64_EQ,
        [OP_u64_NEQ] = &&OP_u64_NEQ,
        [OP_u64_GTEQ] = &&OP_u64_GTEQ,
        [OP_u64_LTEQ] = &&OP_u64_LTEQ,
        [OP_u64_GE] = &&OP_u64_GE,
        [OP_u64_LE] = &&OP_u64_LE,

        [OP_u32_EQ] = &&OP_u32_EQ,
        [OP_u32_NEQ] = &&OP_u32_NEQ,
        [OP_u32_GTEQ] = &&OP_u32_GTEQ,
        [OP_u32_LTEQ] = &&OP_u32_LTEQ,
        [OP_u32_GE] = &&OP_u32_GE,
        [OP_u32_LE] = &&OP_u32_LE,

        [OP_u16_EQ] = &&OP_u16_EQ,
        [OP_u16_NEQ] = &&OP_u16_NEQ,
        [OP_u16_GTEQ] = &&OP_u16_GTEQ,
        [OP_u16_LTEQ] = &&OP_u16_LTEQ,
        [OP_u16_GE] = &&OP_u16_GE,
        [OP_u16_LE] = &&OP_u16_LE,

        [OP_u8_EQ] = &&OP_u8_EQ,
        [OP_u8_NEQ] = &&OP_u8_NEQ,
        [OP_u8_GTEQ] = &&OP_u8_GTEQ,
        [OP_u8_LTEQ] = &&OP_u8_LTEQ,
        [OP_u8_GE] = &&OP_u8_GE,
        [OP_u8_LE] = &&OP_u8_LE,

        [OP_f_EQ] = &&OP_f_EQ,
        [OP_f_NEQ] = &&OP_f_NEQ,
        [OP_f_GTEQ] = &&OP_f_GTEQ,
        [OP_f_LTEQ] = &&OP_f_LTEQ,
        [OP_f_GE] = &&OP_f_GE,
        [OP_f_LE] = &&OP_f_LE,

        [OP_d_EQ] = &&OP_d_EQ,
        [OP_d_NEQ] = &&OP_d_NEQ,
        [OP_d_GTEQ] = &&OP_d_GTEQ,
        [OP_d_LTEQ] = &&OP_d_LTEQ,
        [OP_d_GE] = &&OP_d_GE,
        [OP_d_LE] = &&OP_d_LE,

        [OP_MOV] = &&OP_MOV,
        [OP_CLEAR] = &&OP_CLEAR,

        [OP_LOAD_8] = &&OP_LOAD_8,
        [OP_LOAD_16] = &&OP_LOAD_16,
        [OP_LOAD_32] = &&OP_LOAD_32,
        [OP_LOAD_64] = &&OP_LOAD_64,

        [OP_ALLOC] = &&OP_ALLOC,
        [OP_FREE] = &&OP_FREE,

        [OP_MEMSTORE_64] = &&OP_MEMSTORE_64,
        [OP_MEMSTORE_32] = &&OP_MEMSTORE_32,
        [OP_MEMSTORE_16] = &&OP_MEMSTORE_16,
        [OP_MEMSTORE_8] = &&OP_MEMSTORE_8,

        [OP_REGLOAD_64] = &&OP_REGLOAD_64,
        [OP_REGLOAD_32] = &&OP_REGLOAD_32,
        [OP_REGLOAD_16] = &&OP_REGLOAD_16,
        [OP_REGLOAD_8] = &&OP_REGLOAD_8,

        [OP_PROGRAM_END] = &&OP_PROGRAM_END
    };

    //in our enum, OP_PROGRAM_END denotes the last opcode, so there can't be something more than that
    if ( fibre->instructions[ fibre->registers[RPC].u64 ] > OP_PROGRAM_END ) {
        goto INVALID;
    }

    //MUCH FASTER than switch or if statements.
    goto *dispatch_table[fibre->instructions[ fibre->registers[RPC].u64 ]];

    OP_i64ADD: return i64_add(fibre);
    OP_i32ADD: return;
    OP_i16ADD: return;
    OP_i8ADD: return;

    OP_ui64ADD: return;
    OP_ui32ADD: return;
    OP_ui16ADD: return;
    OP_ui8ADD: return;

    OP_fADD: return;
    OP_dADD: return;

    OP_i64MUL: return;
    OP_i32MUL: return;
    OP_i16MUL: return;
    OP_i8MUL:  return;

    OP_ui64MUL: return;
    OP_ui32MUL: return;
    OP_ui16MUL: return;
    OP_ui8MUL:  return;

    OP_fMUL: return;
    OP_dMUL: return;

    OP_i64SUB: return;
    OP_i32SUB: return;
    OP_i16SUB: return;
    OP_i8SUB:  return;

    OP_ui64SUB: return;
    OP_ui32SUB: return;
    OP_ui16SUB: return;
    OP_ui8SUB:  return;

    OP_fSUB: return;
    OP_dSUB: return;

    OP_i64DIV: return;
    OP_i32DIV: return;
    OP_i16DIV: return;
    OP_i8DIV:  return;

    OP_ui64DIV: return;
    OP_ui32DIV: return;
    OP_ui16DIV: return;
    OP_ui8DIV:  return;

    OP_fDIV: return;
    OP_dDIV: return;

    OP_FREE: return free_heap(fibre);
    OP_ALLOC: return allocate_heap(fibre);

    OP_LOAD_8: return load_byte(fibre);
    OP_LOAD_16: return load_2bytes(fibre);
    OP_LOAD_32: return load_4bytes(fibre);
    OP_LOAD_64: return load_8bytes(fibre);

    OP_MEMSTORE_8: return store_byte(fibre);
    OP_MEMSTORE_16: return store_2bytes(fibre);
    OP_MEMSTORE_32: return store_4bytes(fibre);
    OP_MEMSTORE_64: return store_8bytes(fibre);

    OP_REGLOAD_8: return regload_byte(fibre);
    OP_REGLOAD_16: return regload_2bytes(fibre);
    OP_REGLOAD_32: return regload_4bytes(fibre);
    OP_REGLOAD_64: return regload_8bytes(fibre);

    OP_MOV: return move_data(fibre);
    OP_CLEAR: return clear_register(fibre);

    OP_JUMP: return jump(fibre);
    OP_CALL: return call(fibre);
    OP_SYSCALL: return;
    OP_JUMP_INLINE: return jump_inline(fibre);
    OP_CALL_INLINE: return call_inline(fibre);
    OP_SYSCALL_INLINE: return;
    OP_RETURN: return call_return(fibre);

    // equality flag operators
    OP_i64_EQ: return i64_equals(fibre);
    OP_i64_NEQ: return i64_not_equals(fibre);
    OP_i64_GTEQ: return i64_greater_than_equals(fibre);
    OP_i64_LTEQ: return i64_less_than_equals(fibre);
    OP_i64_GE: return i64_greater_than(fibre);
    OP_i64_LE: return i64_less_than(fibre);

    OP_i32_EQ: return i32_equals(fibre);
    OP_i32_NEQ: return i32_not_equals(fibre);
    OP_i32_GTEQ: return i32_greater_than_equals(fibre);
    OP_i32_LTEQ: return i32_less_than_equals(fibre);
    OP_i32_GE: return i32_greater_than(fibre);
    OP_i32_LE: return i32_less_than(fibre);

    OP_i16_EQ: return i16_equals(fibre);
    OP_i16_NEQ: return i16_not_equals(fibre);
    OP_i16_GTEQ: return i16_greater_than_equals(fibre);
    OP_i16_LTEQ: return i16_less_than_equals(fibre);
    OP_i16_GE: return i16_greater_than(fibre);
    OP_i16_LE: return i16_less_than(fibre);

    OP_i8_EQ: return i8_equals(fibre);
    OP_i8_NEQ: return i8_not_equals(fibre);
    OP_i8_GTEQ: return i8_greater_than_equals(fibre);
    OP_i8_LTEQ: return i8_less_than_equals(fibre);
    OP_i8_GE: return i8_greater_than(fibre);
    OP_i8_LE: return i8_less_than(fibre);

    OP_u64_EQ: return u64_equals(fibre);
    OP_u64_NEQ: return u64_not_equals(fibre);
    OP_u64_GTEQ: return u64_greater_than_equals(fibre);
    OP_u64_LTEQ: return u64_less_than_equals(fibre);
    OP_u64_GE: return u64_greater_than(fibre);
    OP_u64_LE: return u64_less_than(fibre);

    OP_u32_EQ: return u32_equals(fibre);
    OP_u32_NEQ: return u32_not_equals(fibre);
    OP_u32_GTEQ: return u32_greater_than_equals(fibre);
    OP_u32_LTEQ: return u32_less_than_equals(fibre);
    OP_u32_GE: return u32_greater_than(fibre);
    OP_u32_LE: return u32_less_than(fibre);

    OP_u16_EQ: return u16_equals(fibre);
    OP_u16_NEQ: return u16_not_equals(fibre);
    OP_u16_GTEQ: return u16_greater_than_equals(fibre);
    OP_u16_LTEQ: return u16_less_than_equals(fibre);
    OP_u16_GE: return u16_greater_than(fibre);
    OP_u16_LE: return u16_less_than(fibre);

    OP_u8_EQ: return u8_equals(fibre);
    OP_u8_NEQ: return u8_not_equals(fibre);
    OP_u8_GTEQ: return u8_greater_than_equals(fibre);
    OP_u8_LTEQ: return u8_less_than_equals(fibre);
    OP_u8_GE: return u8_greater_than(fibre);
    OP_u8_LE: return u8_less_than(fibre);

    OP_f_EQ: return f32_equals(fibre);
    OP_f_NEQ: return f32_not_equals(fibre);
    OP_f_GTEQ: return f32_greater_than_equals(fibre);
    OP_f_LTEQ: return f32_less_than_equals(fibre);
    OP_f_GE: return f32_greater_than(fibre);
    OP_f_LE: return f32_less_than(fibre);

    OP_d_EQ: return d64_equals(fibre);
    OP_d_NEQ: return d64_not_equals(fibre);
    OP_d_GTEQ: return d64_greater_than_equals(fibre);
    OP_d_LTEQ: return d64_less_than_equals(fibre);
    OP_d_GE: return d64_greater_than(fibre);
    OP_d_LE: return d64_less_than(fibre);

    //no like literally just return man.
    OP_PROGRAM_END: return;

    INVALID:
        printf("Internal error: Unrecognized Bytecode %X\n", (uint32_t)fibre->instructions[fibre->registers[RPC].u32]);

}