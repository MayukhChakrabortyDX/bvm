/*
    Welcome to the logical section of the opcode execution.
    As the name suggest, this section covers the logical part
    of the VM, like jumps, calls, syscalls, equalities and so on.
*/

#pragma once
#include "../../fibre/fibre.h"
#include <stdio.h>
#include <stdlib.h>

//these derefences are too handful to write, so here's some
//definition so that I save some typings.
#define _R1(T) fibre->registers[R1].T
#define _R2(T) fibre->registers[R2].T
#define _RPC fibre->registers[RPC].u64
#define _RFLAG fibre->registers[RFLAG].u64

static inline void i64_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(i64) == _R2(i64) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void i64_not_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(i64) != _R2(i64) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void i64_greater_than_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(i64) >= _R2(i64) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void i64_less_than_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(i64) <= _R2(i64) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void i64_less_than(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(i64) < _R2(i64) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void i64_greater_than(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(i64) > _R2(i64) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void i32_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(i32) == _R2(i32) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void i32_not_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(i32) != _R2(i32) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void i32_greater_than_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(i32) >= _R2(i32) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void i32_less_than_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(i32) <= _R2(i32) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void i32_less_than(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(i32) < _R2(i32) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void i32_greater_than(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(i32) > _R2(i32) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void i16_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(i16) == _R2(i16) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void i16_not_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(i16) != _R2(i16) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void i16_greater_than_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(i16) >= _R2(i16) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void i16_less_than_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(i16) <= _R2(i16) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void i16_less_than(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(i16) < _R2(i16) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void i16_greater_than(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(i16) > _R2(i16) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void i8_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(i8) == _R2(i8) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void i8_not_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(i8) != _R2(i8) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void i8_greater_than_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(i8) >= _R2(i8) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void i8_less_than_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(i8) <= _R2(i8) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void i8_less_than(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(i8) < _R2(i8) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void i8_greater_than(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(i8) > _R2(i8) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void u64_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(u64) == _R2(u64) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void u64_not_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(u64) != _R2(u64) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void u64_greater_than_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(u64) >= _R2(u64) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void u64_less_than_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(u64) <= _R2(u64) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void u64_less_than(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(u64) < _R2(u64) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void u64_greater_than(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(u64) > _R2(u64) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void u32_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(u32) == _R2(u32) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void u32_not_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(u32) != _R2(u32) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void u32_greater_than_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(u32) >= _R2(u32) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void u32_less_than_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(u32) <= _R2(u32) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void u32_less_than(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(u32) < _R2(u32) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void u32_greater_than(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(u32) > _R2(u32) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void u16_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(u16) == _R2(u16) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void u16_not_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(u16) != _R2(u16) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void u16_greater_than_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(u16) >= _R2(u16) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void u16_less_than_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(u16) <= _R2(u16) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void u16_less_than(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(u16) < _R2(u16) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void u16_greater_than(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(u16) > _R2(u16) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void u8_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(u8) == _R2(u8) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void u8_not_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(u8) != _R2(u8) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void u8_greater_than_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(u8) >= _R2(u8) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void u8_less_than_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(u8) <= _R2(u8) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void u8_less_than(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(u8) < _R2(u8) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void u8_greater_than(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(u8) > _R2(u8) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void f32_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(f32) == _R2(f32) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void f32_not_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(f32) != _R2(f32) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void f32_greater_than_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(f32) >= _R2(f32) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void f32_less_than_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(f32) <= _R2(f32) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void f32_less_than(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(f32) < _R2(f32) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void f32_greater_than(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(f32) > _R2(f32) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void d64_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(d64) == _R2(d64) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void d64_not_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(d64) != _R2(d64) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void d64_greater_than_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(d64) >= _R2(d64) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void d64_less_than_equals(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(d64) <= _R2(d64) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void d64_less_than(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(d64) < _R2(d64) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void d64_greater_than(struct Fibre *fibre) {

    _RFLAG = 0;

    if ( _R1(d64) > _R2(d64) ) {

        _RFLAG = 1;
        _RPC++;
        return;

    }

    //else just jump twice
    _RPC += 2;
    return;

}

static inline void jump(struct Fibre *fibre) {

    fibre->registers[RPC].u64 = fibre->registers[R1].u64;

}

static inline void jump_inline(struct Fibre *fibre) {

    //take the jump address from the instruction only
    fibre->registers[RPC].u64 = fibre->instructions[ fibre->registers[RPC + 1].u64 ];

}

static inline void call_inline(struct Fibre *fibre) {

    if ( fibre->call_stack_head > fibre->call_stack_size ) {
        printf("Fatal Error: Call Stack Size exceeded\n");
        exit(-1);
    }

    fibre->call_stack[ fibre->call_stack_head ] = fibre->instructions[ fibre->registers[RPC + 1].u64 ];
    fibre->registers[RPC].u64 = fibre->instructions[ fibre->registers[ RPC ].u64 ];
    fibre->call_stack_head++;

}

static inline void call(struct Fibre *fibre) {

    if ( fibre->call_stack_head > fibre->call_stack_size ) {
        printf("Fatal Error: Call Stack Size exceeded\n");
        exit(-1);
    }

    fibre->call_stack[ fibre->call_stack_head ] = fibre->registers[RPC + 1].u64;
    fibre->registers[RPC].u64 = fibre->registers[R1].u64;
    fibre->call_stack_head++;

}

static inline void call_return(struct Fibre *fibre) {

    fibre->registers[RPC].u64 = fibre->call_stack[ fibre->call_stack_head - 1 ];
    fibre->call_stack_head--;

}