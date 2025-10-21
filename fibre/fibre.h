/*
    Welcome to Fibre! a threading system that can be virtual
    or real depending on use case.
*/



#pragma once
#include <stdint.h>


//ever since functions became first-class citizen, this is required.
struct FunctionTable {
    uint64_t address; //stores the address corresponding to in the instruction space.
    struct BlockUnit *allocation_strategy; //predefined meta-data to identify the structure of local variable in the heap space
};

//useful for multi-threading (M:N Model)
enum FibreStatus {
    RUNNING, WAITING, TERMINATED
};

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

struct CallStackItem {
    uint64_t heap_ptr;
    uint64_t return_ptr;
};

struct Fibre {

    
    struct CallStackItem **CallStack;
    uint8_t *heap;

    uint16_t call_stack_size;
    uint16_t call_stack_head; //points to the 'head', i.e current location

    uint64_t *instructions;

    struct MethodTable *table; //a fibre must know about this table before
    //stores information about allocations.
    struct BlockUnit *heap_metadata;
    struct FunctionTable *function_table;
    union RegisterStorage registers[14];
    enum FibreStatus status;

};

struct Fibre *fibre_factory(
    uint16_t call_stack_size, 
    struct CallStackItem **CallStack, 
    uint8_t* heap, 
    struct BlockUnit* heap_metadata, 
    uint64_t* instructions,
    struct MethodTable *table,
    struct FunctionTable *function_table,
    enum FibreStatus status
);

struct MethodTable {

    void (*ptr)(struct Fibre*);
    uint8_t is_loaded; //0 means yet to be loaded

};