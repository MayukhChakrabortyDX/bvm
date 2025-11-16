// Windows-only engine (slow af) using switch cases instead of
// computed gotos because window sucks.
//
//  But we will introduce JIT so no worries about that.

#include "../logger/logger.h"
#include "engine.h"
#include "error.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define COMPOSE(name, T, fibre)                                                                                        \
    OP_i64##name : T(i64, fibre);                                                                                      \
    break;                                                                                                             \
    OP_u64##name : T(u64, fibre);                                                                                      \
    break;                                                                                                             \
    OP_i32##name : T(i32, fibre);                                                                                      \
    break;                                                                                                             \
    OP_i16##name : T(i16, fibre);                                                                                      \
    break;                                                                                                             \
    OP_i8##name : T(i8, fibre);                                                                                        \
    break;                                                                                                             \
    OP_u8##name : T(u8, fibre);                                                                                        \
    break;                                                                                                             \
    OP_f32##name : T(f32, fibre);                                                                                      \
    break;                                                                                                             \
    OP_d64##name : T(d64, fibre);                                                                                      \
    break;

void schedule_fibres(Fibre* fibre, uint64_t* instructions, uint8_t* heap, struct BlockUnit* heap_metadata, SystemMethodTable* table, BytecodeMethodTable** fx_table) {

    while (fibre) {
    
    	fibre = fibre->next;
     
        printf("iptr: %llu\n", _RPC);

        switch (instructions[_RPC]) {

            COMPOSE(ADD, _ADD, fibre)
            COMPOSE(MUL, _MUL, fibre)
            COMPOSE(SUB, _SUB, fibre)
            COMPOSE(DIV, _DIV, fibre)

            COMPOSE(XADD, _XADD, fibre)
            COMPOSE(XMUL, _XMUL, fibre)
            COMPOSE(XSUB, _XSUB, fibre)
            COMPOSE(XDIV, _XDIV, fibre)

        OP_INCREMENT:
            fibre->registers[instructions[_RPC + 1]].u64++;
            _RPC += 2;
            break;
        OP_DECREMENT:
            fibre->registers[instructions[_RPC + 1]].u64--;
            _RPC += 2;
            break;

        OP_iRESIZE_8:
            RESIZE(i8);
            break;
        OP_iRESIZE_16:
            RESIZE(i16);
            break;
        OP_iRESIZE_32:
            RESIZE(i32);
            break;

        OP_iCONVf:
            CONV(float, i32, f32) break;
        OP_lCONVd:
            CONV(double, i64, d64) break;
        OP_fCONVi:
            CONV(int32_t, f32, i32) break;
        OP_dCONVl:
            CONV(int64_t, d64, i64) break;

        OP_FREE:
            vmfree(_R1(u64), heap_metadata, &_RERR);
            _RPC++;
            break;

        OP_ALLOC:
            vmalloc(_R1(u64), &_ROUT(u64), heap_metadata, &_RERR);
            _RPC++;
            break;

        OP_LOAD:
            memcpy(
                // this looks a bit complicated so let me clarify
                // the destination is a register, so we need to pass pointe
                // to the destination register.
                // but the address to the register is stored in the instructions space, just next to the
                // current one.
                // locate the register itself -> the first operand is the address to the register
                &fibre->registers[instructions[fibre->registers[RPC + 1].u64]].u64,
                // locate the heap -> the second operand is the address to the heap
                &heap[instructions[_RPC + 2]],
                // size to copy -> copy this many bytes from heap to the register.
                instructions[_RPC + 3]);
            _RPC += 4;
            break;

        OP_fLOAD:
            memcpy(
                // this looks a bit complicated so let me clarify
                // the destination is a register, so we need to pass pointe
                // to the destination register.
                // but the address to the register is stored in the instructions space, just next to the
                // current one.
                // locate the register itself -> the first operand is the address to the register
                &fibre->registers[instructions[_RPC + 1]].u64,
                // locate the heap -> the second operand is the address to the heap
                &heap[instructions[_RPC + 2] + _RFX // this is offset instead of raw offset.
            ],
                // size to copy -> copy this many bytes from heap to the register.
                instructions[_RPC + 3]);
            _RPC += 4;
            break;

        OP_MEMSTORE:
            memcpy(&heap[_R1(u64) // address
            ],
                   &_R2(u64), instructions[_RPC + 1]);
            _RPC += 2;
            break;

        OP_fMEMSTORE:
            memcpy(&heap[_R1(u64) + _RFX // address + function offset
            ],
                   &_R2(u64), instructions[_RPC + 1]);
            _RPC += 2;
            break;

        OP_REGLOAD: // address to heap is stored in R1, offset in R2
            memcpy(&_ROUT(u64),
                   &heap[_R1(u64) // address
            ],
                   instructions[_RPC + 1]);
            _RPC += 2;
            break;

        OP_fREGLOAD:
            memcpy(&_ROUT(u64),
                   &heap[_R1(u64) + _RFX // address + function offset.
            ],
                   instructions[_RPC + 1]);
            _RPC += 2;
            break;

        OP_MOV:
            fibre->registers[instructions[_RPC + 1]].u64 = fibre->registers[instructions[_RPC + 2]].u64;
            _RPC += 3;
            break;

        OP_CLEAR:
            fibre->registers[instructions[_RPC + 1]].u64 = 0;
            _RPC += 2;
            break;

            // Heap Copy and Function Heap Copy
            //  heap copy operator copies between heap address
            //  Destination is stored in R1, source in R2 and the size in GR1
            //  The function heap copy on the other hand uses the function heap offset _RFX instead
            //  the destination is still R1, but R2 is now function offset instead of pure address
            //  GR1 still stores the size.

        OP_HEAP_COPY:
            memcpy(&heap[_R1(u64)], &heap[_R2(u64)], fibre->registers[GR1].u64);
            _RPC++;
            break;

        OP_fHEAP_COPY:
            memcpy(&heap[_R1(u64)], &heap[_RFX + _R2(u64)], fibre->registers[GR1].u64);
            _RPC++;
            break;

        OP_JUMP_IF:
            _RPC = _RFLAG == 1 ? instructions[_RPC + 1] : _RPC + 2;
            break;
        OP_JUMP:
            _RPC = instructions[_RPC + 1];
            break;

        OP_CALL:
            //? OP_CALL _id, and _argument is taken from R1, _callee_heap is taken from RFX
            {
                // simple step 1 -> allocate the memory
                uint64_t adrs = 0;
                vmalloc(fx_table[instructions[_RPC + 1]]->funtion_size, &adrs, heap_metadata, &_RERR);

                // step 2 -> verify the allocation, if fail then silent closure
                if (_RERR == HEAP_MEMORY_FULL) {
                    logger(ERROR, "Heap Memory Full", "Unable to allocate enough memory for function call.");
                    return;
                }

                // step 3 -> heap_address, return_address, callee_heap_address, arguments, data
                memcpy(&heap[adrs], &adrs, 8);                       // copy over the heap address itself.
                memcpy(&heap[adrs + 8], &instructions[_RPC + 2], 8); // copy over the return address (8 bytes)
                memcpy(&heap[adrs + 16], &_RFX, 8);                  // copy over the heap address of the callee.
                memcpy(&heap[adrs + 24], &heap[_R1(u64)],
                       fx_table[instructions[_RPC + 1]]->arg_size); // copy over the argument.

                _RFX = adrs;                                              // set the current function register to this
                _RPC = fx_table[instructions[_RPC + 1]]->instruction_ptr; // go here
                break;
            }

        OP_SYSCALL:
            fibre->status = WAITING;
            // a system call can ONLY manipulate registers and heap.
            table[instructions[_RPC + 1]](fibre, heap);
            _RPC += 2;
            break;

        OP_RETURN:
            // giving this so that zed formats it well
            {
                memcpy(&_RPC, &heap[_RFX + 8], 8); // again, 8th padding so here we are.
                uint64_t adrs = _RFX;
                //_RFX now should point to the callee heap address instead.
                memcpy(&_RFX, &heap[_RFX + 16], 8); // 16 because that's where the callee heap address is
                // free the memory
                vmfree(adrs, heap_metadata, &_RERR);
            }

            COMPOSE(EQ, EQUAL, fibre)
            COMPOSE(NEQ, NOT_EQUAL, fibre)
            COMPOSE(LT, LESS_THAN, fibre)
            COMPOSE(LTEQ, LESS_THAN_EQUAL, fibre)

        OP_PROGRAM_END:
            // again giving this for better formatting
            {
                fibre->status = TERMINATED;
                //?: the reference is itself, so that means no more fibre left to execute.
                if (fibre->next == fibre)
                    return;
                //? rewires the pool for next/previous pointers
                fibre->before->next = fibre->next;
                fibre->next->before = fibre->before;
                //? There MUST exist a next fibre, so switch there

                {
                    Fibre* this = fibre;
                    fibre = fibre->next;
                    __drop_fibre__(this);
                }

                //? No need for the scheduler to run again, jump directly to __MAIN.
                break;
            }
        }

    } // while
}
