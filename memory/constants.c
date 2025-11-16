#include <stdlib.h>
#include <stdint.h>
#include "constants.h"

ConstantBlock* __new_constant_block__( uint8_t* value, uint64_t size, ConstantBlock *next_block ) {

    ConstantBlock *factory = (ConstantBlock*)malloc(sizeof( ConstantBlock ));
    factory->size = size;
    factory->value = value;
    factory->next_block = next_block;

    return factory;

}

ConstantBlock* __new_constant_block__( uint8_t* value, uint64_t size, ConstantBlock *next_block );
void __drop_constant_block__(ConstantBlock* ptr) {
	
	ConstantBlock* next = ptr->next_block;
	ConstantBlock *this = ptr;
	
	while (this) {
		free(this);
		this = next;
		next = next->next_block;
	}
	
}