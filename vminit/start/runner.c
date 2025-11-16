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
            0
        );

    //based on the constant table, we need to produce a heap metadata.
    struct ConstantBlock* block = args->constantTable;

    while (block) {
        uint64_t ptr = 0;
        uint64_t err = 0;
        vmalloc(block->size, &ptr, metadata, &err);
        memcpy(heap + ptr, args->constantTable->value, args->constantTable->size);
        block = block->next_block;
    }

    struct Fibre *fibre_main = __new_fibre__();
    //but we also need to prepare the scheduler.
    
    //run multiple fibres in a single thread, or use M:N model too.
    schedule_fibres(fibre_main, args->instructions, heap, metadata, args->methodTable, args->fx_table);
    //it's guaranteed to have atleast one fibre.
    __drop_fibre__(fibre_main);

}

VMArgs *__new_vmargs__(
    uint64_t *instructions, 
    uint64_t min_heap_size, 
    uint32_t heap_capacity_multipler, 
    ConstantBlock *constants,
    SystemMethodTable *methodTable,
    BytecodeMethodTable **fx_table
) {
	
	VMArgs *args = malloc(sizeof( VMArgs ));
	
	args->instructions = instructions;
	args->min_heap_size = min_heap_size;
	args->heap_capacity_multipler = heap_capacity_multipler;
	args->constantTable = constants;
	args->methodTable = methodTable;
	args->fx_table = fx_table;
	
	return args;
	
}

void __drop_vmargs__(VMArgs *ptr) {
	free(ptr->instructions); //this is indeed heap allocated
	__drop_constant_block__(ptr->constantTable);
	//__drop_system_method_table__(ptr->methodTable);
	
	BytecodeMethodTable *buffer = ptr->fx_table[0];
	while (buffer) {
		__drop_bytecode_method_table__(buffer);
		buffer = buffer + 1;
	}
	
	//finally free itself
	free(ptr);
	
}