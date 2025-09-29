#pragma once
#include <stdint.h>

/*

    So welcome to the bytecode parser. This function is a single responsibility
    function that convers a byte stream (generally from file) to executable
    instruction for the VM.

    For that, let's understand the general architecure the bytecode is stored
    in bytes. For that, we need to understand the data types that's possible
    in the bytecode format.

    1. characters, which are generally just 8 bit unsigned.
    2. 64 bit integers, also suffixed as 'l' (think long)
    3. 32 bit integers, also suffixed as 'i' (think int)
    4. 16 bit integers, also suffixed as 's' (think short)
    5. 8 bit integers, also suffixed as 'b' (think byte)
    6. for unsigned, they are again suffixed with 'u', i.e 'ul', 'ub' etc.

    7. arrays, suffixed with 'a' followed by the type via a dot, i.e a.ul, a.s, a.ui etc.
    8. in constant pool, arrays are ALWAYS fixed size, so 4 bytes are allocated to represent the size of the array.
    9. in constant pool, the maximum possible array items is 2^32
    10. these are the rules of the constant pool, the detailed diagram of byte structure is also present in
        this folder. check the "vminit/structure" folder to view all the implementation details of each and every OPCODE,
        constant pool and such.
    
*/

uint64_t parse_bytecode(uint8_t* file_data) {

    //parses the bytecode to produce valid instruction.

}