/*
    Welcome to Fibre! a threading system that can be virtual
    or real depending on use case.
*/

#pragma once
#include <stdint.h>

//this union saves a lot of time when operating on arithmetic -> literally a life saver.
union RegisterStorage {

    uint64_t u64; //ulong
    int64_t  i64; //long

    uint32_t u32; //uint
    int32_t  i32; //int

    uint16_t u16; //ushort
    int16_t  i16; //short
 
    uint8_t  u8; //ubyte
    int8_t   i8; //byte (also acts as char)

    float    f32; //float
    double   d64; //double

};

/*
    The first version had registers as struct -> but encoding/decoding was two step
    slower. Rather use a dedicated memory allocated registers for better performance,
    even better if on stack itself.
*/
enum RegisterAccesor {
    R1, R2, //2 input registers
    GR1, GR2, GR3, GR4, GR5, GR6, //6 general purpose registers
    ROUT, //output register 
    RCARRY, RERR, RFLAG, RSYS, //system specific registers
    RPC //the program counter itself.
};

struct Fibre {

    uint64_t *call_stack;
    uint8_t *heap;

    uint16_t call_stack_size;
    uint16_t call_stack_head; //points to the 'head', i.e current location

    uint64_t *instructions;
    
    //stores information about allocations.
    struct BlockUnit *heap_metadata;
    union RegisterStorage registers[14];

};