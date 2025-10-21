/*

    Common header file for all arithmetic here.

*/

#pragma once
#include "../../fibre/fibre.h"
#include <stdint.h>
#include "../types.h"
#define XOR ^
#include "common.h"

VMCALL iRESIZE_8(struct Fibre *fibre) {

    fibre->registers[ROUT].u64 = fibre->registers[R1].i8;
    fibre->registers[RPC].u64++;

}

VMCALL iRESIZE_16(struct Fibre *fibre) {

    fibre->registers[ROUT].u64 = fibre->registers[R1].i16;
    fibre->registers[RPC].u64++;

}

VMCALL iRESIZE_32(struct Fibre *fibre) {

    fibre->registers[ROUT].u64 = fibre->registers[R1].i32;
    fibre->registers[RPC].u64++;

}

VMCALL uRESIZE_8(struct Fibre *fibre) {

    fibre->registers[ROUT].u64 = fibre->registers[R1].u8;
    fibre->registers[RPC].u64++;

}

VMCALL uRESIZE_16(struct Fibre *fibre) {

    fibre->registers[ROUT].u64 = fibre->registers[R1].u16;
    fibre->registers[RPC].u64++;

}

VMCALL uRESIZE_32(struct Fibre *fibre) {

    fibre->registers[ROUT].u64 = fibre->registers[R1].u32;
    fibre->registers[RPC].u64++;

}

VMCALL iCONVf(struct Fibre *fibre) {

    fibre->registers[ROUT].f32 = (float)fibre->registers[R1].i32;
    fibre->registers[RPC].u64++;

}

VMCALL lCONVd(struct Fibre *fibre) {

    fibre->registers[ROUT].d64 = (double)fibre->registers[R1].i64;
    fibre->registers[RPC].u64++;

}

VMCALL fCONVi(struct Fibre *fibre) {

    fibre->registers[ROUT].i32 = (int32_t)fibre->registers[R1].f32;
    fibre->registers[RPC].u64++;

}   

VMCALL dCONVl(struct Fibre *fibre) {

    fibre->registers[ROUT].i64 = (int64_t)fibre->registers[R1].d64;
    fibre->registers[RPC].u64++;

}

//Generic Functions starts here -> the best of it's kind
void ADD(struct Fibre *fibre) {

    //detect type and viola!
    const void *dispatch_type_table[TYPE_d64 + 1] = {
        [TYPE_i64] = &&TYPE_i64,
        [TYPE_i32] = &&TYPE_i32,
        [TYPE_i16] = &&TYPE_i16,
        [TYPE_i8] = &&TYPE_i8,

        [TYPE_u64] = &&TYPE_u64,
        [TYPE_u32] = &&TYPE_u32,
        [TYPE_u16] = &&TYPE_u16,
        [TYPE_u8] = &&TYPE_u8,

        [TYPE_f32] = &&TYPE_f32,
        [TYPE_d64] = &&TYPE_d64
    };

    //in our enum, OP_PROGRAM_END denotes the last opcode, so there can't be something more than that
    if ( fibre->instructions[ fibre->registers[RPC].u64 + 1] > TYPE_d64 ) {
        goto INVALID;
    }

    //MUCH FASTER than switch or if statements.
    goto *dispatch_type_table[fibre->instructions[ fibre->registers[RPC].u64 + 1 ]];


    TYPE_i64: _ROUT(i64) = _R1(i64) + _R2(i64); goto DONE;
    TYPE_i32:  _ROUT(i32) = _R1(i32) + _R2(i32); goto DONE;
    TYPE_i16:  _ROUT(i16) = _R1(i16) + _R2(i16); goto DONE;
    TYPE_i8:  _ROUT(i8) = _R1(i8) + _R2(i8); goto DONE;
    
    TYPE_u64:  _ROUT(u64) = _R1(u64) + _R2(u64); goto DONE;
    TYPE_u32:  _ROUT(u32) = _R1(u32) + _R2(u32); goto DONE;
    TYPE_u16:  _ROUT(u16) = _R1(u16) + _R2(u16); goto DONE;
    TYPE_u8:  _ROUT(u8) = _R1(u8) + _R2(u8); goto DONE;

    TYPE_f32:  _ROUT(f32) = _R1(f32) + _R2(f32); goto DONE;
    TYPE_d64:  _ROUT(d64) = _R1(d64) + _R2(d64); goto DONE;

    INVALID: ;

    DONE:
    fibre->registers[RPC].u64 += 2;

}

void SUB(struct Fibre *fibre) {

    //detect type and viola!
    const void *dispatch_type_table[TYPE_d64 + 1] = {
        [TYPE_i64] = &&TYPE_i64,
        [TYPE_i32] = &&TYPE_i32,
        [TYPE_i16] = &&TYPE_i16,
        [TYPE_i8] = &&TYPE_i8,

        [TYPE_u64] = &&TYPE_u64,
        [TYPE_u32] = &&TYPE_u32,
        [TYPE_u16] = &&TYPE_u16,
        [TYPE_u8] = &&TYPE_u8,

        [TYPE_f32] = &&TYPE_f32,
        [TYPE_d64] = &&TYPE_d64
    };

    //in our enum, OP_PROGRAM_END denotes the last opcode, so there can't be something more than that
    if ( fibre->instructions[ fibre->registers[RPC].u64 + 1] > TYPE_d64 ) {
        goto INVALID;
    }

    //MUCH FASTER than switch or if statements.
    goto *dispatch_type_table[fibre->instructions[ fibre->registers[RPC].u64 + 1 ]];


    TYPE_i64: _ROUT(i64) = _R1(i64) - _R2(i64); goto DONE;
    TYPE_i32:  _ROUT(i32) = _R1(i32) - _R2(i32); goto DONE;
    TYPE_i16:  _ROUT(i16) = _R1(i16) - _R2(i16); goto DONE;
    TYPE_i8:  _ROUT(i8) = _R1(i8) - _R2(i8); goto DONE;
    
    TYPE_u64:  _ROUT(u64) = _R1(u64) - _R2(u64); goto DONE;
    TYPE_u32:  _ROUT(u32) = _R1(u32) - _R2(u32); goto DONE;
    TYPE_u16:  _ROUT(u16) = _R1(u16) - _R2(u16); goto DONE;
    TYPE_u8:  _ROUT(u8) = _R1(u8) - _R2(u8); goto DONE;

    TYPE_f32:  _ROUT(f32) = _R1(f32) - _R2(f32); goto DONE;
    TYPE_d64:  _ROUT(d64) = _R1(d64) - _R2(d64); goto DONE;

    INVALID: ;

    DONE:
    fibre->registers[RPC].u64 += 2;

}

void MUL(struct Fibre *fibre) {

    //detect type and viola!
    const void *dispatch_type_table[TYPE_d64 + 1] = {
        [TYPE_i64] = &&TYPE_i64,
        [TYPE_i32] = &&TYPE_i32,
        [TYPE_i16] = &&TYPE_i16,
        [TYPE_i8] = &&TYPE_i8,

        [TYPE_u64] = &&TYPE_u64,
        [TYPE_u32] = &&TYPE_u32,
        [TYPE_u16] = &&TYPE_u16,
        [TYPE_u8] = &&TYPE_u8,

        [TYPE_f32] = &&TYPE_f32,
        [TYPE_d64] = &&TYPE_d64
    };

    //in our enum, OP_PROGRAM_END denotes the last opcode, so there can't be something more than that
    if ( fibre->instructions[ fibre->registers[RPC].u64 + 1] > TYPE_d64 ) {
        goto INVALID;
    }

    //MUCH FASTER than switch or if statements.
    goto *dispatch_type_table[fibre->instructions[ fibre->registers[RPC].u64 + 1 ]];


    TYPE_i64: _ROUT(i64) = _R1(i64) * _R2(i64); goto DONE;
    TYPE_i32:  _ROUT(i32) = _R1(i32) * _R2(i32); goto DONE;
    TYPE_i16:  _ROUT(i16) = _R1(i16) * _R2(i16); goto DONE;
    TYPE_i8:  _ROUT(i8) = _R1(i8) * _R2(i8); goto DONE;
    
    TYPE_u64:  _ROUT(u64) = _R1(u64) * _R2(u64); goto DONE;
    TYPE_u32:  _ROUT(u32) = _R1(u32) * _R2(u32); goto DONE;
    TYPE_u16:  _ROUT(u16) = _R1(u16) * _R2(u16); goto DONE;
    TYPE_u8:  _ROUT(u8) = _R1(u8) * _R2(u8); goto DONE;

    TYPE_f32:  _ROUT(f32) = _R1(f32) * _R2(f32); goto DONE;
    TYPE_d64:  _ROUT(d64) = _R1(d64) * _R2(d64); goto DONE;

    INVALID: ;

    DONE:
    fibre->registers[RPC].u64 += 2;

}

void DIV(struct Fibre *fibre) {

    //detect type and viola!
    const void *dispatch_type_table[TYPE_d64 + 1] = {
        [TYPE_i64] = &&TYPE_i64,
        [TYPE_i32] = &&TYPE_i32,
        [TYPE_i16] = &&TYPE_i16,
        [TYPE_i8] = &&TYPE_i8,

        [TYPE_u64] = &&TYPE_u64,
        [TYPE_u32] = &&TYPE_u32,
        [TYPE_u16] = &&TYPE_u16,
        [TYPE_u8] = &&TYPE_u8,

        [TYPE_f32] = &&TYPE_f32,
        [TYPE_d64] = &&TYPE_d64
    };

    //in our enum, OP_PROGRAM_END denotes the last opcode, so there can't be something more than that
    if ( fibre->instructions[ fibre->registers[RPC].u64 + 1] > TYPE_d64 ) {
        goto INVALID;
    }

    //MUCH FASTER than switch or if statements.
    goto *dispatch_type_table[fibre->instructions[ fibre->registers[RPC].u64 + 1 ]];

    TYPE_i64: _ROUT(i64) = _R1(i64) / _R2(i64); goto DONE;
    TYPE_i32:  _ROUT(i32) = _R1(i32) / _R2(i32); goto DONE;
    TYPE_i16:  _ROUT(i16) = _R1(i16) / _R2(i16); goto DONE;
    TYPE_i8:  _ROUT(i8) = _R1(i8) / _R2(i8); goto DONE;
    
    TYPE_u64:  _ROUT(u64) = _R1(u64) / _R2(u64); goto DONE;
    TYPE_u32:  _ROUT(u32) = _R1(u32) / _R2(u32); goto DONE;
    TYPE_u16:  _ROUT(u16) = _R1(u16) / _R2(u16); goto DONE;
    TYPE_u8:  _ROUT(u8) = _R1(u8) / _R2(u8); goto DONE;

    TYPE_f32:  _ROUT(f32) = _R1(f32) / _R2(f32); goto DONE;
    TYPE_d64:  _ROUT(d64) = _R1(d64) / _R2(d64); goto DONE;

    INVALID: ;

    DONE:
    fibre->registers[RPC].u64 += 2;

}