#include "loader.h"
#include <stdio.h>
#include <stdlib.h>
#include "../logger/logger.h"

#ifdef _WIN32
	#include <windows.h>
#else
	#include <dlfcn.h>
#endif


void load_system_call( uint8_t index, char* name, SystemMethodTable *table, char *syscall_registry) {

	#ifdef _WIN32
		HMODULE handle = LoadLibrary(syscall_registry);
		
		if (!handle) {
		 	logger(ERROR, "Unable to load system library", "The requested system library is undefined. Please run brt doctor to detect errors.");
		}
	
		table[index] = (void (*)(struct Fibre*, uint8_t* heap)) GetProcAddress(handle, name);
		
		if ( !table[index] ) {
		      printf("invalid program pointer %lu", GetLastError());
		      exit(-1);
		}
			
	#else
	    void *handle = dlopen( syscall_registry, RTLD_NOW );
	
	    if (!handle) {
	   		logger(ERROR, "Unable to load system library", "The requested system library is undefined. Please run brt doctor to detect errors.");
	    }
	
	    table[index] = (void (*)(struct Fibre*, uint8_t* heap)) dlsym(handle, name);
	
	    if ( !table[index] ) {
	
	        printf("invalid program pointer, error: %s", dlerror());
	        exit(-1);
	
	    }
    #endif

}