#pragma once
#include <stdint.h>
#include "parser.h"

/*

    Please refer to the docs/bytecode.md for structure and parsing algorithm
    for bytecode
    
*/

//bytecode parser version v0
uint64_t parse_bytecode_v0(uint8_t* file_data) {

    //parses the bytecode to produce valid instruction.
    //already assumed that the version number is 0.

}