/*

    Common header file for all arithmetic here.

*/

#pragma once
#include "../../fibre/fibre.h"
#include <stdint.h>
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

#define _ADD(type, fibre) _ROUT(type) = _R1(type) + _R2(type); _RPC++
#define _SUB(type, fibre) _ROUT(type) = _R1(type) - _R2(type); _RPC++
#define _MUL(type, fibre) _ROUT(type) = _R1(type) * _R2(type); _RPC++
#define _DIV(type, fibre) _ROUT(type) = _R1(type) / _R2(type); _RPC++