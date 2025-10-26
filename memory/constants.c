#include <stdlib.h>
#include <stdint.h>
#include "constants.h"

ConstantBlock* __new_constant_block__( uint8_t* value, uint64_t size, ConstantBlock *next_block ) {

    ConstantBlock *factory = (struct ConstantBlock*)malloc(sizeof(struct ConstantBlock));
    factory->size = size;
    factory->value = value;
    factory->next_block = next_block;

    return factory;

}