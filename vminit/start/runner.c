#include "runner.h"
#include "../../fibre/fibre.h"
#include "../../memory/heap.h"
#include "../../memory/constants.h"
#include "../../engine/engine.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void start_vm(struct VMArgs *args) {

    //owner, always the owner.
    uint8_t *heap = (uint8_t *)malloc( args->min_heap_size );
    struct CallStackItem **call_stack = malloc(sizeof(struct CallStackItem) * 64);

    struct BlockUnit *metadata = 
        BlockUnit_factory(
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

    struct Fibre *fibre_main = 
        fibre_factory(
            64, 
            call_stack, 
            heap, 
            metadata, 
            args->instructions, 
            args->methodTable, 
            args->function_table,
            RUNNING //defaults to running. No assuption here. (only syscall changes the program to waiting)
        );

    //but we also need to prepare the scheduler.
    struct Scheduler *pool = malloc(sizeof(struct Scheduler));

    pool->previous_fibre = pool; //self reference
    pool->next_fibre = pool; //self reference
    pool->ptr = fibre_main;
    
    //run multiple fibres in a single thread, or use M:N model too.
    schedule_fibres(pool);

}