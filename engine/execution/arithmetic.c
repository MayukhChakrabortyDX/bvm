/*

    Contains all the inline functions that operates on arithmetic operations.

    To do arithmetic, store values to the R1 and R2 registers and then use the
    opcode.
    
    The result will be stored in ROUT, if any overflow occurs that will be indicated by
    RCARRY (1=overflow, 0=no overflow).

*/

#pragma once
#include "../../fibre/fibre.h"
#include <stdint.h>
#define XOR ^

static inline void i64_add(struct Fibre *fibre) {

    fibre->registers[ROUT].i64 = fibre->registers[R1].i64 + fibre->registers[R2].i64;

    if ( (int64_t)((fibre->registers[ROUT].u64 XOR fibre->registers[R1].u64) & (fibre->registers[ROUT].u64 XOR fibre->registers[R2].u64)) < 0) {

        // this may look confusing but let me explain:
        // for signed bits, the numbers always belong in range R = [-2^63, 2^63]
        // let x, y belong to R. if x * y < 0 -> that means their sign is different.
        // it is proven that their sum cannot overflow. -> which is our first condition, that the sign bits
        // of operators must be same for a potential overflow.

        // now if we add two numbers of same sign, we know the output MUST have the same sign too.
        // because if -x + -y = -(x+y) given x,y > 0.
        // or x + y = 1(x+y) given x,y > 0. -> both cases the sign is same.
        // if the sign bit somehow happen to flip, meaning that the overflow happened.

        // we know the signed numbers are stored as 2's complement, so MSB is sign bit.
        // we know the XOR of same bits (0, 0) and (1, 1) is 0. Which mean that if two number
        // has same MSB, their XOR will produce a positive number always.
        // if it does not, then no carry can happen (sign mismatch).

        // and we know adding two numbers with same MSB must also produce same MSB, as sign remains same.
        // If we can detect mismatch there, then a carry is possible.

        // it is proved, I am not writing a paper on proofs, so I am going to just give an example for easiness:
        //  1001 + 1010 = 10011 -> truncating (due to limitation) = 0011
        // MSB changed.
        //  1001 XOR 0011 = 1110
        //  1010 XOR 0011 = 1001
        //  1110 AND 1001 = 1000 -> is a negative signed number, (< 0) indicating a overflow.
        fibre->registers[RCARRY].u64 = 1;

    }

    //something that you should not forget.
    fibre->registers[RPC].u64++;
    return;

}