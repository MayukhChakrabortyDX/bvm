#include "../fibre/fibre.h"
#include <stdint.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_REGISTRY_SIZE 1

struct Fibre;
//used by dlopen.

//used by dlopen.

//method table is global, has a global lifetime that lives beyond main.
//okay so it basically loads the system call item here.
//although we are using globals that could cause issue I feel.
__attribute__((always_inline)) inline void load_system_call( uint8_t index, char* name, struct MethodTable *table, char *syscall_registry) {

    void *handle = dlopen( syscall_registry, RTLD_NOW );

    if (!handle) {
        printf("invalid program handle");
        exit(-1);
    }

    table[index].ptr = (void (*)(struct Fibre*)) dlsym(handle, name);

    if ( !table[index].ptr ) {

        printf("invalid program pointer %s", dlerror());
        exit(-1);

    }

    table[index].is_loaded = 1;

}

__attribute__((always_inline)) inline void call_method(struct Fibre* fibre, uint8_t index, struct MethodTable *table) {

    if ( index > MAX_REGISTRY_SIZE ) {
        printf("unrecognized access to syscall %i\n", index);
        exit(-1);
    }

    //else call it.
    if ( table[index].is_loaded == 1 ) {
        table[index].ptr( fibre );
    } else {
        printf("fatal error: invalid library\n");
    }

}