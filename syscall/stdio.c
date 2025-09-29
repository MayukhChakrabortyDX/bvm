#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "../fibre/fibre.h"

void println(struct Fibre *fibre) {

    //get the memory (heap) address from the syscall register
    //and get the length of the string first.

    uint64_t strlen = 0;
    uint8_t *accumulator = &fibre->heap[fibre->registers[ RSYS ].i64]; //addressing using 64 bits ofcourse

    //calculate the length first because we are storing strings as C-style strings.
    while (accumulator) {
        
        if ( accumulator == 0 ) {
            strlen++;
            break;
        }
        
        strlen++;
        accumulator = &fibre->heap[fibre->registers[ RSYS ].i64 + strlen];

    }

    accumulator = &fibre->heap[fibre->registers[ RSYS ].i64];
    uint8_t *buffer = (uint8_t *)malloc(strlen);

    memcpy(buffer, &fibre->heap[ fibre->registers[RSYS].i64 ], strlen );

    printf("%s\n", buffer);
    free(buffer);

}