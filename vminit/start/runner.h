#pragma once

/*

    Welcome to the true brain! This part controls the following:
    1. Creating fibres.
    2. Executing fibres.

*/

#include <stdint.h>
#include "../../fibre/fibre.h"
#include "../../memory/constants.h"

struct VMArgs {

    uint64_t *instructions;
    //both sizes are in bytes only.
    //BTW the minimum heap size is given by the user, but in case it's not, it's 256 MB default
    uint64_t min_heap_size;

    //this is a ceiling for the max heap size;
    //it simply means: the min heap size can be multiplied till N times
    //easy for dumb allocator to allocate in chunks that are multiples of min_heap_size
    uint32_t heap_capacity_multipler;
    ConstantBlock *constantTable;
    MethodTable *methodTable;
    BytecodeMethodTable **fx_table;

}; typedef struct VMArgs VMArgs;

VMArgs *__new_vmargs__(
    uint64_t *instructions, 
    uint64_t min_heap_size, 
    uint32_t heap_capacity_multipler, 
    ConstantBlock *contants,
    MethodTable *methodTable,
    BytecodeMethodTable **fx_table
);
void __drop_vmargs__(VMArgs *ptr);

void start_vm(struct VMArgs *args);