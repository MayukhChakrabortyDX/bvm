#pragma once
#include <stdint.h>

struct ConstantBlock {

    uint8_t *value; //sequence of bytes
    uint64_t size; //in bytes
    struct ConstantBlock *next_block;

}; typedef struct ConstantBlock ConstantBlock;

ConstantBlock* __new_constant_block__( uint8_t* value, uint64_t size, ConstantBlock *next_block );
void __drop_constant_block__(ConstantBlock* ptr);