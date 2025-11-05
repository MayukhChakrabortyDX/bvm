#include "../fibre/fibre.h"
#include <stdint.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
//used by dlopen.

//used by dlopen.

//method table is global, has a global lifetime that lives beyond main.
//okay so it basically loads the system call item here.
//although we are using globals that could cause issue I feel.
__attribute__((always_inline)) inline void load_system_call( uint8_t index, char* name, SystemMethodTable *table, char *syscall_registry) {

    void *handle = dlopen( syscall_registry, RTLD_NOW );

    if (!handle) {
        printf("invalid program handle");
        exit(-1);
    }

    table[index] = (void (*)(struct Fibre*, uint8_t* heap)) dlsym(handle, name);

    if ( !table[index] ) {

        printf("invalid program pointer %s", dlerror());
        exit(-1);

    }

}