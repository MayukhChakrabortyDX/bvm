#pragma once

/*
    Contains status codes, error flags and conditions.
    This also explains how error works in the VM.

    For example, say you do OP_i64DIV, but the operators are 0/0 or some undefined value.
    The VM will report two things:

    1. A flag to denote error has happend (RFLAG)
    2. Error code itself, specific to the bytecode (RERR)

    These status codes are enumerated numbers only. The VM silently fails the opcode
    that caused the error, and it's the responsibility of the language/compiler to
    effectively handle this error.

    Some popular approaches includes simply using a systemcall to print the error on
    terminal and exit the app with a -1 status code

    and you can tap into the language specific try {} catch {} or even some modern
    features like from Rust, enum matching. These details are specific to the language
    and the compiler itself.

*/

enum ERRORS {

    DIV_BY_ZERO, //ofc. not allowed lol
    INVALID_FREE,
    HEAP_MEMORY_FULL,

    STORE_INVALID_REGISTER,
    STORE_INVALID_HEAP_ADDRESS,

    LOAD_INVALID_HEAP_ADDRESS,

    REGLOAD_INVALID_REGISTER,
    REGLOAD_INVALID_HEAP_ADDRESS

};