#pragma once

/*

    Phase 2 - Optimization. Introducing native types is much easier
    than to have explicit type annotated opcodes.

    Implications:
        - Easily upgradable
        - modular
        - JIT implementation becomes easier

    Also helps in reducing the opcode size & bytecode size significantly.

*/

enum types {

    TYPE_i64, TYPE_i32, TYPE_i16, TYPE_i8,
    TYPE_u64, TYPE_u32, TYPE_u16, TYPE_u8,
    TYPE_f32, TYPE_d64 //last type to check for invalidity.

};
