/*
    Welcome to Fibre! a threading system that can be virtual
    or real depending on use case.
*/

#pragma once
#include <stdint.h>

//useful for multi-threading (M:N Model)
enum FibreStatus {
    RUNNING, WAITING, TERMINATED
}; typedef enum FibreStatus FibreStatus;

//this union saves a lot of time when operating on arithmetic -> literally a life saver.
union RegisterStorage {

    int64_t  i64; //long
    uint64_t u64;

    int32_t  i32; //int
    int16_t  i16; //shor
    int8_t   i8; //byte (also acts as char)

    float    f32; //float
    double   d64; //double

}; typedef union RegisterStorage RegisterStorage;

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

    //?: Intentional, Must be pointers of frames. Maximum of 1 MiB here.
    RegisterStorage *registers;
    FibreStatus status;

};

typedef struct Fibre Fibre;
Fibre *__new_fibre__();
void __drop_fibre__(Fibre *fibre);

typedef void (*MethodTable)(struct Fibre*, uint8_t*);
MethodTable *__new_method_table__();
void __drop_method_table__(MethodTable *table);