#include "../syscall/registry.h"
#include "../engine/opcode.h"
#include "../fibre/fibre.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../memory/heap.h"
#include "../memory/constants.h"
#include "../vminit/start/runner.h"

int main() {

    static char *syscall_registry[MAX_REGISTRY_SIZE] = { "./println_dev.so" };
    MethodTable table[MAX_REGISTRY_SIZE] = { NULL };
    HeapHeader *unit = __new_heap_header__(50*1024*1024, 0, 0, 0);

    //now let's create a hello world block.
    //basically create a constant is what I meant.

    uint8_t first_block[] = { 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x0 };

    struct ConstantBlock *block = __new_constant_block__(
        first_block, 12, NULL
    );

    uint8_t *heap = malloc(12000);

    //load the system call for it to be able to execute.
    load_system_call(0, "println", table, syscall_registry[0]);
    //printf("%X", table->ptr);

    uint64_t instructions[] = {
        OP_SYSCALL,0, 
        OP_PROGRAM_END
    };

    struct VMArgs args = {
        instructions, 
        20,
        1,
        block,
        table, 
        NULL
    };

    start_vm(&args);

    return 0;

}