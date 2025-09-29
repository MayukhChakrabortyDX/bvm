enum OPCODES {

    // 64 bit, address independent
    OP_i64ADD, OP_i64SUB, OP_i64MUL, OP_i64DIV,

    // 32 bit, address independent
    OP_i32ADD, OP_i32SUB, OP_i32MUL, OP_i32DIV,

    // 16 bit address independent
    OP_i16ADD, OP_i16SUB, OP_i16MUL, OP_i16DIV,

    // 8 bit address independent
    OP_i8ADD, OP_i8SUB, OP_i8MUL, OP_i8DIV,

    // IEEE 745 Floating Point,  address independent
    OP_fADD, OP_fSUB, OP_fMUL, OP_fDIV,

    // IEEE 745 Double,  address independent
    OP_dADD, OP_dSUB, OP_dMUL, OP_dDIV,

    // unsigned
    // 64 bit,  address independent
    OP_ui64ADD, OP_ui64SUB, OP_ui64MUL, OP_ui64DIV,

    // 32 bit,  address independent
    OP_ui32ADD, OP_ui32SUB, OP_ui32MUL, OP_ui32DIV,

    // 16 bit, address independent
    OP_ui16ADD, OP_ui16SUB, OP_ui16MUL, OP_ui16DIV,

    // 8 bit, address independent
    OP_ui8ADD, OP_ui8SUB, OP_ui8MUL, OP_ui8DIV,

    //jump call and syscall with conditional jump, address independent
    //store the 'to-be' jump address at R1, same for OP_CALL, and for syscall use the RSYS and R1
    OP_JUMP, OP_CALL, OP_SYSCALL, OP_JUMP_INLINE, OP_CALL_INLINE, OP_SYSCALL_INLINE, OP_RETURN, //takes the address of the jump from constant table (heap)

    //equality flag operators //binary operator over R1 and R2, address independent
    //if true -> goes to the immediate next instruction, which is always a jump instruction
    //if false, skips the immediate next instruction.
    OP_i64_EQ, OP_i64_NEQ, OP_i64_GTEQ, OP_i64_LTEQ, OP_i64_GE, OP_i64_LE,
    OP_i32_EQ, OP_i32_NEQ, OP_i32_GTEQ, OP_i32_LTEQ, OP_i32_GE, OP_i32_LE,
    OP_i16_EQ, OP_i16_NEQ, OP_i16_GTEQ, OP_i16_LTEQ, OP_i16_GE, OP_i16_LE,
    OP_i8_EQ, OP_i8_NEQ, OP_i8_GTEQ, OP_i8_LTEQ, OP_i8_GE, OP_i8_LE,

    OP_u64_EQ, OP_u64_NEQ, OP_u64_GTEQ, OP_u64_LTEQ, OP_u64_GE, OP_u64_LE,
    OP_u32_EQ, OP_u32_NEQ, OP_u32_GTEQ, OP_u32_LTEQ, OP_u32_GE, OP_u32_LE,
    OP_u16_EQ, OP_u16_NEQ, OP_u16_GTEQ, OP_u16_LTEQ, OP_u16_GE, OP_u16_LE,
    OP_u8_EQ, OP_u8_NEQ, OP_u8_GTEQ, OP_u8_LTEQ, OP_u8_GE, OP_u8_LE,
    
    OP_f_EQ, OP_f_NEQ, OP_f_GTEQ, OP_f_LTEQ, OP_f_GE, OP_f_LE,
    OP_d_EQ, OP_d_NEQ, OP_d_GTEQ, OP_d_LTEQ, OP_d_GE, OP_d_LE,

    //static move between registers, address dependent but easily encodable below
    OP_MOV, OP_CLEAR,

    //the only address dependent non-encodable OP-CODE. this loads an addresble value to some register
    //given by R1 or R2
    OP_LOAD_8, OP_LOAD_16, OP_LOAD_32, OP_LOAD_64, //load only 8 bits.

    //memory allocator //address independent, store in R1
    OP_ALLOC, OP_FREE,

    //load to heap memory from R1 at address present in R2
    //basically register->heap
    OP_MEMSTORE_64, OP_MEMSTORE_32, OP_MEMSTORE_16, OP_MEMSTORE_8,

    //load specific heap values present in address R1 to ROUT
    //heap->register (only register address is inline.)
    //to use it, store the address at R1, store the offset at R2, and call any of the loader option, you will get the data at ROUT.
    OP_REGLOAD_64, OP_REGLOAD_32, OP_REGLOAD_16, OP_REGLOAD_8,

    //denotes program has ended.
    OP_PROGRAM_END

};