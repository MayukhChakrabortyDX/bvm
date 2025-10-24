#pragma once
#include "../fibre/fibre.h"
#include "../memory/heap.h"
#include <stdint.h>

#define _R1(T) fibre->registers[R1].T
#define _R2(T) fibre->registers[R2].T
#define _RPC fibre->registers[RPC].u64
#define _RFLAG fibre->registers[RFLAG].u64
#define _ROUT(T) fibre->registers[ROUT].T

#define RESIZE(size) _ROUT(u64) = _R1(size); _RPC++;
//cast means (float) or (double) or (int32_t) etc.
//type from is simply f32/d64 etc and so is type_to
#define CONV(cast, type_from, type_to) _ROUT(type_from) = (cast)_R1(type_to); _RPC++;

//simplified arithmetics
#define _ADD(type, fibre) _ROUT(type) = _R1(type) + _R2(type); _RPC++
#define _SUB(type, fibre) _ROUT(type) = _R1(type) - _R2(type); _RPC++
#define _MUL(type, fibre) _ROUT(type) = _R1(type) * _R2(type); _RPC++
#define _DIV(type, fibre) _ROUT(type) = _R1(type) / _R2(type); _RPC++

#define EQUAL(type, fibre) _RPC += _R1(type) == _R2(type) ? 1 : 2
#define NOT_EQUAL(type, fibre) _RPC += _R1(type) != _R2(type) ? 1 : 2
#define LESS_THAN_EQUAL(type, fibre) _RPC += _R1(type) <= _R2(type) ? 1 : 2
#define LESS_THAN(type, fibre) _RPC += _R1(type) < _R2(type) ? 1 : 2

//defines a scheduler to work effectively.
//these are all green threads.
struct Scheduler {

    struct Fibre *ptr;
    struct Scheduler *next_fibre;
    struct Scheduler *previous_fibre;

};

typedef struct Scheduler Scheduler;
//the second argument is a function pointer to a syscall registry
//for now let's keep it single threaded.

//TODO: Unimplemented
void __new_scheduler__();
//TODO: Unimplemented
void __drop_scheduler__(Scheduler *pool);

void schedule_fibres(Scheduler *pool, uint64_t *instructions, uint8_t *heap, struct BlockUnit *heap_metadata, MethodTable *table);