#include "../engine/error.h"
#include "heap.h"

HeapHeader* __new_heap_header__(uint64_t size, uint64_t pointer, uint8_t is_allocated, uint8_t is_constant) {

    HeapHeader *unit = (struct BlockUnit *)malloc(sizeof(struct BlockUnit));

    unit->size = size; unit->pointer = pointer; unit->is_allocated = is_allocated; unit->is_constant = is_constant;
    unit->next_block = NULL; unit->previous_block = NULL;

    return unit;

};


// destination is generally the ROUT register
void vmalloc(uint64_t size, NUMBER *destination, HeapHeader *block, NUMBER *error) {

    *error = 0;
    // iterative unit!
    while (block) {

        if ((block->size > size) && (block->is_allocated == 0) && (block->is_constant == 0)) {
            
            HeapHeader *split = (HeapHeader *)malloc(sizeof(HeapHeader));

            split->size = block->size-size;
            split->is_allocated = 0;
            split->pointer = block->pointer + size;

            block->size = size;

            //rearrange the pointers
            split->next_block = block->next_block;
            split->previous_block = block;

            if ( block->next_block ) {
                block->next_block->previous_block = split;
            }

            block->next_block = split;

            block->is_allocated = 1;
            *destination = block->pointer;

            return;
            
        }

        else if ((block->size == size) && (block->is_allocated == 0) && (block->is_constant == 0)) {

            block->is_allocated = 1;
            *destination = (NUMBER)block->pointer;
            return;

        }

        block = block->next_block;
        
    }

    //no allocation found (size exceeded)
    *error = HEAP_MEMORY_FULL;
}

void vmfree(NUMBER address, HeapHeader *block, NUMBER *error) {

    *error = 0;
    HeapHeader *sub_block = block;
    unsigned char is_free = 0;

    while (sub_block) {
        //constants cannot be freed.
        if ( sub_block->pointer == address ) {
            if ( sub_block->is_allocated == 1 && sub_block->is_constant == 0 ) {

                sub_block->is_allocated = 0;
                is_free = 1; //free is succesful
                break;

            }
        }

        sub_block = sub_block->next_block;

    }
    
    if ( is_free == 0 ) {
        *error = INVALID_FREE;
    }

    //rejoin groups.
    while (block) {

        if ( block-> is_allocated == 0 ) {

            if ( block->next_block ) {

                if ( block -> next_block -> is_allocated == 0 ) {

                    block->size = block->size + block->next_block->size;
                    
                    HeapHeader *nextBlock = block->next_block;

                    block->next_block = block->next_block->next_block;

                    if ( block->next_block ) {
                        block->next_block->previous_block = block;
                    }

                    free(nextBlock);

                    block = block->next_block;
                    continue;

                }

            }

        }

        block = block->next_block;

    }

}