#ifndef SYSCALL_LOADER
#define SYSCALL_LOADER

#include "../fibre/fibre.h"
#include <stdint.h>

//method table is global, has a global lifetime that lives beyond main.
//okay so it basically loads the system call item here.
//although we are using globals that could cause issue I feel.
void load_system_call( uint8_t index, char* name, SystemMethodTable *table, char *syscall_registry);
#endif