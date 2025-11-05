#pragma once
#include "../fibre/fibre.h"
#include "../memory/heap.h"
#include <stdint.h>

#define _R1(T) fibre->registers[R1].T
#define _R2(T) fibre->registers[R2].T
#define _RPC fibre->registers[RPC].u64
#define _RFX fibre->registers[RFX].u64
#define _ROUT(T) fibre->registers[ROUT].T
#define _RERR fibre->registers[RERR].u64
#define _RRET fibre->registers[RRET].u64

#define RESIZE(size) _ROUT(u64) = _R1(size); _RPC++;
//cast means (float) or (double) or (int32_t) etc.
//type from is simply f32/d64 etc and so is type_to
#define CONV(cast, type_from, type_to) _ROUT(type_from) = (cast)_R1(type_to); _RPC++;

//simplified arithmetics
#define _ADD(type, fibre) _ROUT(type) = _R1(type) + _R2(type); _RPC++
#define _SUB(type, fibre) _ROUT(type) = _R1(type) - _R2(type); _RPC++
#define _MUL(type, fibre) _ROUT(type) = _R1(type) * _R2(type); _RPC++
#define _DIV(type, fibre) \
    _ROUT(type) = _R2(type) == 0 ? DIV_BY_ZERO : _R1(type) / _R2(type); \
    _RERR = _R2(type) == 0 ? DIV_BY_ZERO : _R1(type) / _R2(type); _RPC++
    
//types are encoded as is in the bits itself. no number exeeds 64 bits anyway.
#define _XADD(type, fibre) fibre->registers[instructions[_RPC + 1]].type += instructions[_RPC + 2]; _RPC += 3;
#define _XSUB(type, fibre) fibre->registers[instructions[_RPC + 1]].type -= instructions[_RPC + 2]; _RPC += 3;
#define _XMUL(type, fibre) fibre->registers[instructions[_RPC + 1]].type *= instructions[_RPC + 2]; _RPC += 3;
#define _XDIV(type, fibre) fibre->registers[instructions[_RPC + 1]].type /= instructions[_RPC + 2]; _RPC += 3; //verified to be NON-ZERO

#define EQUAL(type, fibre) _RPC += _R1(type) == _R2(type) ? 1 : 2
#define NOT_EQUAL(type, fibre) _RPC += _R1(type) != _R2(type) ? 1 : 2
#define LESS_THAN_EQUAL(type, fibre) _RPC += _R1(type) <= _R2(type) ? 1 : 2
#define LESS_THAN(type, fibre) _RPC += _R1(type) < _R2(type) ? 1 : 2

void schedule_fibres(Fibre *pool, uint64_t *instructions, uint8_t *heap, struct BlockUnit *heap_metadata, SystemMethodTable *table, BytecodeMethodTable **fx_table);