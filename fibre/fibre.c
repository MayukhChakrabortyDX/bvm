#include "fibre.h"
#include <stdint.h>
#include <stdlib.h>

/*
    As always, since this factory can be reused for virtual thread,
    we cannot assume that heap to be allocated.

    Since heaps can be shared, it's best left to the main function
    to manage all threads and execution engine itself.
*/

//NOTE: Moves Ownership! MUST be used to create new stuff.
Fibre *__new_fibre__() {

    Fibre* fibre = (Fibre *) malloc( sizeof(Fibre) );
    fibre->registers = ( RegisterStorage* ) malloc( sizeof( RegisterStorage ) * REGISTER_COUNT );
    
    fibre->registers[R1].u64 = 0;
    fibre->registers[R2].u64 = 0;
    fibre->registers[GR1].u64 = 0;
    fibre->registers[GR2].u64 = 0;
    fibre->registers[GR3].u64 = 0;
    fibre->registers[GR4].u64 = 0;
    fibre->registers[GR5].u64 = 0;
    fibre->registers[GR6].u64 = 0;
    fibre->registers[ROUT].u64 = 0;
    fibre->registers[RERR].u64 = 0;
    fibre->registers[RFX].u64 = 0;
    fibre->registers[RRET].u64 = 0;
    fibre->registers[RSYS].u64 = 0;
    fibre->registers[RPC].u64 = 0;
    
    fibre->next = fibre;
    fibre->before = fibre;
    fibre->flag = 0;
    return fibre;

}

void __drop_fibre__(Fibre *fibre) {

    free(fibre->registers);
    free(fibre);

}

BytecodeMethodTable *__new_bytecode_method_table__(uint64_t instruction_ptr, uint64_t function_size, uint64_t arg_size) {

    BytecodeMethodTable *table = malloc(sizeof(BytecodeMethodTable));
    table->arg_size = arg_size;
    table->instruction_ptr = instruction_ptr;
    table->funtion_size = function_size;

    return table;

}

void __drop_bytecode_method_table__(BytecodeMethodTable *table) {
    free(table);
}