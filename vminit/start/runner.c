#include "runner.h"
#include "../../memory/heap.h"
#include "../../engine/engine.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void start_vm(struct VMArgs *args) {

    //owner, always the owner.
    uint8_t *heap = (uint8_t *)malloc( args->min_heap_size );

    struct BlockUnit *metadata = 
        __new_heap_header__(
            args->min_heap_size, 
            0, 
            0, 
            0
        );

    //based on the constant table, we need to produce a heap metadata.
    {

        struct ConstantBlock* block = args->constantTable;

        while (block) {
        
            uint64_t ptr = 0;
            uint64_t err = 0;
            vmalloc(block->size, &ptr, metadata, &err);
            memcpy(heap + ptr, args->constantTable->value, args->constantTable->size);
            block = block->next_block;

        }

    }

    struct Fibre *fibre_main = __new_fibre__();
    //but we also need to prepare the scheduler.
    
    //run multiple fibres in a single thread, or use M:N model too.
    schedule_fibres(fibre_main, args->instructions, heap, metadata, args->methodTable, args->fx_table);
    //it's guaranteed to have atleast one fibre.
    __drop_fibre__(fibre_main);

}