#include "fibre.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
    As always, since this factory can be reused for virtual thread,
    we cannot assume that heap to be allocated.

    Since heaps can be shared, it's best left to the main function
    to manage all threads and execution engine itself.
*/

//NOTE: Moves Ownership! MUST be used to create new stuff.
Fibre *__new_fibre__() {

    Fibre* fibre = (Fibre *) malloc( sizeof(Fibre) );
    fibre->registers = ( RegisterStorage* ) malloc( sizeof( RegisterStorage ) * 13 );
    fibre->next = fibre;
    fibre->before = fibre;
    return fibre;

}

void __drop_fibre__(Fibre *fibre) {

    free(fibre->registers);
    free(fibre);

}