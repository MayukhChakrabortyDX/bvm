#ifndef PARSER
#define PARSER

#include <stdint.h>
#include "../start/runner.h"

VMArgs *parse_bytecode_v0(uint8_t* file_data, uint64_t total_bytes);

#endif