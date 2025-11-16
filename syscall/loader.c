#include "loader.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#else
	#include <dlfcn.h>
#endif

#include "../logger/logger.h"

void load_system_call( uint8_t index, char* name, SystemMethodTable *table, char *syscall_registry) {

	#ifdef _WIN32
		
	#else
	    void *handle = dlopen( syscall_registry, RTLD_NOW );
	
	    if (!handle) {
	   		logger(ERROR, "Unable to load system library", "The requested system library is undefined. Please run brt doctor to detect errors.");
	    }
	
	    table[index] = (void (*)(struct Fibre*, uint8_t* heap)) dlsym(handle, name);
	
	    if ( !table[index] ) {
	
	        printf("invalid program pointer %s", dlerror());
	        exit(-1);
	
	    }
    #endif

}