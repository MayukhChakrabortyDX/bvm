#include "fibre.h"
#include <stdlib.h>

/*
    As always, since this factory can be reused for virtual thread,
    we cannot assume that heap to be allocated.

    Since heaps can be shared, it's best left to the main function
    to manage all threads and execution engine itself.
*/

struct Fibre *fibre_factory(
    uint16_t call_stack_size, 
    struct CallStackItem **CallStack, 
    uint8_t* heap, 
    struct BlockUnit* heap_metadata, 
    uint64_t* instructions,
    struct MethodTable *table,
    struct FunctionTable *function_table,
    enum FibreStatus status

) {

    struct Fibre *fibre = (struct Fibre*)malloc(sizeof(struct Fibre));

    fibre->call_stack_head=0;
    fibre->CallStack = CallStack;
    fibre->call_stack_size = call_stack_size;
    
    fibre->heap = heap;
    fibre->heap_metadata = heap_metadata;
    fibre->table = table;
    fibre->function_table = function_table;
    fibre->status = status;

    fibre->instructions = instructions;
    
    return fibre;

}