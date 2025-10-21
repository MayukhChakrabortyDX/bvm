#pragma once
// This is responsible for heap allocation strategy.

/**
 *
 * The algorithm is as follows: we use a contigious block to represent a memory block unit
 *
 */

#ifndef NUMBER
#define NUMBER uint64_t
#endif

#include <stdlib.h>
#include <stdint.h>

struct BlockUnit
{
    uint64_t size; //size in bytes
    uint64_t pointer;
    uint8_t is_allocated; // 0 = false, 1 = true
    uint8_t is_constant; // 0 = false, 1 = true
    struct BlockUnit *next_block;
    struct BlockUnit *previous_block;
};

struct BlockUnit *BlockUnit_factory(uint64_t size, uint64_t pointer, uint8_t is_allocated, uint8_t is_constant);

void vmalloc(uint64_t size, uint64_t *destination, struct BlockUnit *block, uint64_t *error);

void vmfree(uint64_t address, struct BlockUnit *block, uint64_t *error);