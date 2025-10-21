#include <stdlib.h>
#ifndef app_main
#include <stdint.h>
#endif

#include "constants.h"

struct ConstantBlock* ConstantBlock_factory( uint8_t* value, uint64_t size, struct ConstantBlock *next_block ) {

    struct ConstantBlock *factory = (struct ConstantBlock*)malloc(sizeof(struct ConstantBlock));
    factory->size = size;
    factory->value = value;
    factory->next_block = next_block;

    return factory;

}