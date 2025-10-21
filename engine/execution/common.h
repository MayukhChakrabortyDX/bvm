#pragma once
#define VMCALL __attribute__((always_inline)) inline void
#define ABS_64(x) (x ^ (x >> 63)) - (x >> 63)
#define ABS_32(x) (x ^ (x >> 31)) - (x >> 31)
#define ABS_16(x) (x ^ (x >> 15)) - (x >> 15)
#define ABS_8(x) (x ^ (x >> 7)) - (x >> 7)

#define _R1(T) fibre->registers[R1].T
#define _R2(T) fibre->registers[R2].T
#define _RPC fibre->registers[RPC].u64
#define _RFLAG fibre->registers[RFLAG].u64
#define _ROUT(T) fibre->registers[ROUT].T