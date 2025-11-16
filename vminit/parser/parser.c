#include <stdint.h>
//#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "../../logger/logger.h"

/*

    Please refer to the docs/bytecode.md for structure and parsing algorithm
    for bytecode
    
*/

//bytecode parser version v0
VMArgs *parse_bytecode_v0(uint8_t* file_data, uint64_t total_bytes) {

	VMArgs *args = __new_vmargs__(0, 0, 0, 0, 0, 0);
    //parses the bytecode to produce valid instruction.
    //already assumed that the version number is 0.
    uint8_t* buffer = file_data;
    //stage 1 - prepare the header, i.e get the active version number and such
    uint16_t version_number = 0;
    
    if ( total_bytes >= 2 ) {
   		memcpy(&version_number, buffer, 2);
    } else {
   		logger(ERROR, "Malformed Bytecode Version", "The bytecode bytes are malformed, cannot proceed further");
     	exit(0);
    }
    
    if ( 1 < version_number || version_number < 0 ) {
   		logger(ERROR, "Bytecode version mismatch", "This runtime (version 0) do not support this bytecode version");
    	exit(0);
    }
    
    return args;

}