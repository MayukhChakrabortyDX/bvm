#pragma once
#include <stdint.h>

struct ConstantBlock {

    uint8_t *value; //sequence of bytes
    uint64_t size; //in bytes
    struct ConstantBlock *next_block;

};

struct ConstantBlock* ConstantBlock_factory( uint8_t* value, uint64_t size, struct ConstantBlock *next_block );