//#define DEBUG

#include "../syscall/loader.h"
#include "../engine/opcode.h"
#include "../fibre/fibre.h"
#include <stdint.h>
#include <stdlib.h>
#include "../memory/heap.h"
#include "../memory/constants.h"
#include "../vminit/start/runner.h"

#ifdef DEBUG
	#include <stdio.h>
#endif

int main() {

	#ifdef _WIN32
		static char *syscall_registry[1] = { "./println_dev.dll" };
	#else
		static char *syscall_registry[1] = { "./println_dev.so" };
	#endif
    SystemMethodTable table[1] = { NULL };
    HeapHeader *unit = __new_heap_header__(12000, 0, 0);

    //now let's create a hello world block.
    //basically create a constant is what I meant.
    
    //This is literally Hello World
    uint8_t first_block[] = { 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x0 };

    struct ConstantBlock *block = __new_constant_block__(
        first_block, 15, NULL
    );

    //load the system call for it to be able to execute.
    load_system_call(0, "println", table, syscall_registry[0]);
    #ifdef DEBUG
    	printf("%llu", table[0]);
    #endif

    //recursive function call test.
    uint64_t instructions[] = {
   		OP_i32XADD, R2, 20000,
    	OP_JUMP, 8,
    	OP_i32XSUB, R2, 1,
        OP_SYSCALL,0,
        OP_i32NEQ, 
        OP_JUMP_IF, 5,
        OP_PROGRAM_END
    };

    BytecodeMethodTable **_table = malloc( sizeof( BytecodeMethodTable* ) );
    _table[0] = __new_bytecode_method_table__(0, 100,0);
    
    struct VMArgs args = {
        instructions, 
        200000,
        1,
        block,
        table, 
        _table
    };
    
    uint8_t filedata[] = {2, 8};
    
    //struct VMArgs *test = parse_bytecode_v0(filedata, 2);
    start_vm(&args);
    
    return 0;

}