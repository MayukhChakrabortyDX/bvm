#pragma once
#include "types.h"

enum OPCODES {

    //this is bliss vm assembly or ISA. 
    //general purpose arithmetic operation
    //OP_ADD TYPE (check the types.h for all the type ENUMs)

    TYPES(ADD), TYPES(SUB), TYPES(MUL), TYPES(DIV),
    TYPES(XADD), TYPES(XSUB), TYPES(XMUL), TYPES(XDIV), //XADD and XSUB means that they add and subtract using inline values presented to them.
    //btw this does the operation on the register directly. so u64XADD R1 8 ; adds to the R1 the 64bit unsigned 8
    //these are unsigned and useful for iterators on addresses.

    //resizes the value in R1. stores the resized value in ROUT
    //Note: resizing will cause bit loss.
    OP_iRESIZE_8, OP_iRESIZE_16, OP_iRESIZE_32,

    //convert (literally) bits from integers and float, vice versa
    //and long with double and vice versa.
    //stores the output in ROUT.
    OP_iCONVf, OP_lCONVd, OP_fCONVi, OP_dCONVl,

    //jump call and syscall with conditional jump, address independent
    //store the 'to-be' jump address at R1, same for OP_CALL, and for syscall use the RSYS and R1
    OP_JUMP, OP_CALL, OP_SYSCALL, OP_RETURN,

    //? OP_CALL _id: The <id> is looked up in the hash-table that gives us the instruction space address.
    //? The arguments are guaranteed to be in a continous space (as per compiler)
    //? It takes two more argument: from R1 it takes the argument pointer address.
    //? Since the heap address of the function is already stored in RFX, it is taken from there.
    
    //? OP_FCALL does the exact same operands, except it launches a new fibre and the register there is used for
    //? the function execution.

    //? OP_RETURN: if you want to return some value, store it in RRET register, the standard way to communicate between functions.

    //TODO: Implement Fibre Call, and increments.
    //! Fibre calls are not stored in call stacks -> they are independent.
    //equality flag operators //binary operator over R1 and R2, address independent
    //if true -> goes to the immediate next instruction, which is always a jump instruction
    //if false, skips the immediate next instruction.
    //again, equality operators OPS TYPE (check types.h for detailed type description.)
    TYPES(EQ), TYPES(NEQ), TYPES(LTEQ), TYPES(LE),

    //static move between registers, address dependent but easily encodable below
    OP_MOV, OP_CLEAR, 
    
    //copies data between two heap addresses. takes the heap addresses from R1 (source) and R2 (destination) and GR1 (size to copy)
    OP_HEAP_COPY,
    OP_fHEAP_COPY,

    //the only address dependent non-encodable OP-CODE. this loads an addresble value to some register
    //given by R1 or R2
    //format: OP_LOAD register_address heap_address size
    OP_LOAD, //load only 8 byts, best used for inline values/constants.
    OP_fLOAD, //loads data to fixed register that are inlined, and instead of heap address uses heap offset, adding from RFX.

    //memory allocator //address independent, store in R1
    OP_ALLOC, OP_FREE,

    //load to heap memory from R1 at address present in R2
    //basically register->heap. Mention the size immediatly after
    OP_MEMSTORE,
    OP_fMEMSTORE, //memory store for a function, uses the relative offset present in R1 and the heap address in RFX

    //load specific heap values present in address R1 to ROUT
    //heap->register (only register address is inline.)
    //to use it, store the address at R1, store the offset at R2, and call any of the loader option, you will get the data at ROUT.
    OP_REGLOAD, //mention the size immedieatly after.
    OP_fREGLOAD, //register load, but uses the offset heap address instead of absolute heap address.

    //denotes program has ended.
    OP_PROGRAM_END

};