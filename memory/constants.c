#pragma once

#include <stdlib.h>
#ifndef app_main
#include <stdint.h>
#endif

struct ConstantBlock {

    uint8_t *value; //sequence of bytes
    uint64_t size; //in bytes
    struct ConstantBlock *next_block;

};

struct ConstantBlock* ConstantBlock_factory( uint8_t* value, uint64_t size, struct ConstantBlock *next_block ) {

    struct ConstantBlock *factory = (struct ConstantBlock*)malloc(sizeof(struct ConstantBlock));
    factory->size = size;
    factory->value = value;
    factory->next_block = next_block;

    return factory;

}