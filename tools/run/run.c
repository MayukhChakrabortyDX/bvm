#include "run.h"
#include <stdio.h>
#include "../../logger/logger.h"

//lifetime: spans the runtime of the entire Runtime; also gives the status code
int create_runtime(char *source) {
	
	//Stages of creating the runtime
	//1. Start with loading the source code (bytecode) first
	FILE *src = fopen(source, "r");
	if ( !src ) {
		logger(ERROR, "Unable to open file", "The source file provided cannot be opened");
		return -1;
	}
	
	return 0;
	
}