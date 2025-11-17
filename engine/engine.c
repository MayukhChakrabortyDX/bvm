//#define DEBUG
#ifdef DEBUG
	#include <stdio.h>
#endif

//these are for a superior platform, i.e linux
#include "engine.h"
#include "error.h"
#include "opcode.h"
#include <stdint.h>
#include <string.h>
#include "../logger/logger.h"

//Okay so this macro simply helps me avoid repetitive code in COMPILE TIME!!
#define DISPATCH_TABLE_TYPES(NAME) \
    [OP_i64##NAME] = &&OP_i64##NAME, \
    [OP_u64##NAME] = &&OP_u64##NAME, \
    [OP_i32##NAME] = &&OP_i32##NAME, \
    [OP_i16##NAME] = &&OP_i16##NAME, \
    [OP_i8##NAME] = &&OP_i8##NAME, \
    [OP_u8##NAME] = &&OP_u8##NAME, \
    [OP_f32##NAME] = &&OP_f32##NAME, \
    [OP_d64##NAME] = &&OP_d64##NAME

#define COMPOSE(name, T, fibre, L) \
    OP_i64##name: T(i64, fibre); goto L; \
    OP_u64##name: T(u64, fibre); goto L; \
    OP_i32##name: T(i32, fibre); goto L; \
    OP_i16##name: T(i16, fibre); goto L ;\
    OP_i8##name: T(i8, fibre); goto L; \
    OP_u8##name: T(u8, fibre); goto L; \
    OP_f32##name: T(f32, fibre); goto L; \
    OP_d64##name: T(d64, fibre); goto L;

void schedule_fibres(Fibre *fibre, uint64_t *instructions, uint8_t *heap, struct BlockUnit *heap_metadata, SystemMethodTable *table, BytecodeMethodTable **fx_table) {

    //use a doubly-linked list for faster dispatch algorithm.

    static void *dispatch_fetch_table[2] = { &&FETCH, &&__NEXT };
    static void *dispatch_table[OP_PROGRAM_END + 1] = {
        DISPATCH_TABLE_TYPES(ADD),
        DISPATCH_TABLE_TYPES(SUB),
        DISPATCH_TABLE_TYPES(MUL),
        DISPATCH_TABLE_TYPES(DIV),

        DISPATCH_TABLE_TYPES(XADD),
        DISPATCH_TABLE_TYPES(XSUB),
        DISPATCH_TABLE_TYPES(XMUL),
        DISPATCH_TABLE_TYPES(XDIV),

        [OP_iRESIZE_8] = &&OP_iRESIZE_8,
        [OP_iRESIZE_16] = &&OP_iRESIZE_16,
        [OP_iRESIZE_32] = &&OP_iRESIZE_32,

        [OP_iCONVf] = &&OP_iCONVf, 
        [OP_lCONVd] = &&OP_lCONVd, 
        [OP_fCONVi] = &&OP_fCONVi, 
        [OP_dCONVl] = &&OP_dCONVl,
        
        [OP_JUMP_IF] = &&OP_JUMP_IF,
        [OP_JUMP] = &&OP_JUMP,
        [OP_CALL] = &&OP_CALL,
        [OP_SYSCALL] = &&OP_SYSCALL,
        [OP_RETURN] = &&OP_RETURN,

        DISPATCH_TABLE_TYPES(EQ),
        DISPATCH_TABLE_TYPES(NEQ),
        DISPATCH_TABLE_TYPES(LTEQ),
        DISPATCH_TABLE_TYPES(LT),

        [OP_MOV] = &&OP_MOV,
        [OP_CLEAR] = &&OP_CLEAR,

        [OP_HEAP_COPY] = &&OP_HEAP_COPY,
        [OP_fHEAP_COPY] = &&OP_fHEAP_COPY,

        [OP_LOAD] = &&OP_LOAD,
        [OP_fLOAD] = &&OP_fLOAD,

        [OP_ALLOC] = &&OP_ALLOC,
        [OP_FREE] = &&OP_FREE,

        [OP_MEMSTORE] = &&OP_MEMSTORE,
        [OP_fMEMSTORE] = &&OP_fMEMSTORE,

        [OP_REGLOAD] = &&OP_REGLOAD,
        [OP_fREGLOAD] = &&OP_fREGLOAD,

        [OP_PROGRAM_END] = &&OP_PROGRAM_END
    };

    goto __MAIN;

    FETCH:
        //this part is for the scheduler's logic of moving
        //through the fibre pointers.
        //?: Using doubly linked list, so that's that.
        fibre = fibre->next;

    __MAIN:
        
        //TODO: Minimize these waiting stuff, maybe minimize it someway.
        //?: Todo is done
        //in our enum, OP_PROGRAM_END denotes the last opcode, so there can't be something more than that
        goto *dispatch_fetch_table[fibre->status == WAITING ? 0 : 1];
        //TODO: Verify the bytecode beforehand.
        // if ( fibre->instructions[ fibre->registers[RPC].u64 ] > OP_PROGRAM_END ) {
        //     goto INVALID;
        // }

    __NEXT:
        //MUCH FASTER than switch or if statements.
        goto *dispatch_table[instructions[ fibre->registers[RPC].u64 ]];

    COMPOSE(ADD, _ADD, fibre, FETCH)
    COMPOSE(MUL, _MUL, fibre, FETCH)
    COMPOSE(SUB, _SUB, fibre, FETCH)
    COMPOSE(DIV, _DIV, fibre, FETCH)
    
    COMPOSE(XADD, _XADD, fibre, FETCH)
    COMPOSE(XMUL, _XMUL, fibre, FETCH)
    COMPOSE(XSUB, _XSUB, fibre, FETCH)
    COMPOSE(XDIV, _XDIV, fibre, FETCH)

    OP_iRESIZE_8:  RESIZE(i8);  goto FETCH;
    OP_iRESIZE_16: RESIZE(i16); goto FETCH;
    OP_iRESIZE_32: RESIZE(i32); goto FETCH;

    OP_iCONVf: CONV(float, i32, f32) goto FETCH;
    OP_lCONVd: CONV(double, i64, d64) goto FETCH;
    OP_fCONVi: CONV(int32_t, f32, i32) goto FETCH;
    OP_dCONVl: CONV(int64_t, d64, i64) goto FETCH;

    OP_FREE:       
        vmfree(_R1(u64), heap_metadata, &_RERR);
        _RPC++; goto FETCH;
    
    OP_ALLOC:  
        vmalloc(_R1(u64), &_ROUT(u64),heap_metadata, &_RERR);
        _RPC++; goto FETCH;

    OP_LOAD:  
        memcpy(
            //this looks a bit complicated so let me clarify
            //the destination is a register, so we need to pass pointe
            //to the destination register.
            //but the address to the register is stored in the instructions space, just next to the 
            //current one.
            //locate the register itself -> the first operand is the address to the register
            &fibre->registers[
                instructions[
                    fibre->registers[RPC + 1].u64
                ]
            ].u64, 
            //locate the heap -> the second operand is the address to the heap
            &heap[
                instructions[_RPC + 2]
            ], 
            //size to copy -> copy this many bytes from heap to the register.
            instructions[ _RPC + 3 ]
        ); _RPC += 4; goto FETCH;

    OP_fLOAD:
        memcpy(
            //this looks a bit complicated so let me clarify
            //the destination is a register, so we need to pass pointe
            //to the destination register.
            //but the address to the register is stored in the instructions space, just next to the 
            //current one.
            //locate the register itself -> the first operand is the address to the register
            &fibre->registers[
                instructions[
                    _RPC + 1
                ]
            ].u64, 
            //locate the heap -> the second operand is the address to the heap
            &heap[
                instructions[_RPC + 2] + _RFX //this is offset instead of raw offset.
            ], 
            //size to copy -> copy this many bytes from heap to the register.
            instructions[ _RPC + 3 ]
        ); _RPC += 4; goto FETCH;

    OP_MEMSTORE:      
        memcpy(
            &heap[
                _R1(u64) //address
            ], 
            &_R2(u64), 
            instructions[ _RPC + 1 ]
        ); _RPC+=2; goto FETCH;

    OP_fMEMSTORE:
        memcpy(
            &heap[
                _R1(u64) + _RFX //address + function offset
            ], 
            &_R2(u64), 
            instructions[ _RPC + 1 ]
        ); _RPC+=2; goto FETCH;

    OP_REGLOAD:      //address to heap is stored in R1, offset in R2
        memcpy(
            &_ROUT(u64), 
            &heap[
                _R1(u64)//address
            ], 
            instructions[_RPC + 1]
        ); _RPC += 2; goto FETCH;
    
    OP_fREGLOAD:
        memcpy(
            &_ROUT(u64), 
            &heap[
                _R1(u64) + _RFX//address + function offset.
            ], 
            instructions[_RPC + 1]
        ); _RPC += 2; goto FETCH;

    OP_MOV:       
        fibre->registers[ instructions[_RPC + 1] ].u64 = fibre->registers[ instructions[_RPC + 2] ].u64;
        _RPC+=3; goto FETCH;

    OP_CLEAR:     
        fibre->registers[ instructions[_RPC + 1] ].u64 = 0;
        _RPC += 2; goto FETCH;

    //Heap Copy and Function Heap Copy
    // heap copy operator copies between heap address
    // Destination is stored in R1, source in R2 and the size in GR1
    // The function heap copy on the other hand uses the function heap offset _RFX instead
    // the destination is still R1, but R2 is now function offset instead of pure address
    // GR1 still stores the size.
        
    OP_HEAP_COPY:
        memcpy(&heap[ _R1(u64) ], &heap[ _R2(u64) ], fibre->registers[ GR1 ].u64);
        _RPC++; goto FETCH;

    OP_fHEAP_COPY:
        memcpy(&heap[ _R1(u64)], &heap[ _RFX + _R2(u64) ], fibre->registers[ GR1 ].u64 );
        _RPC++; goto FETCH;
        
    OP_JUMP_IF: _RPC = _RFLAG == 1 ? instructions[_RPC + 1] : _RPC + 2; goto FETCH;
    OP_JUMP: _RPC = instructions[ _RPC + 1 ]; goto FETCH;

    OP_CALL:
        //? OP_CALL _id, and _argument is taken from R1, _callee_heap is taken from RFX
        {
            //simple step 1 -> allocate the memory
            uint64_t adrs = 0; 
            vmalloc(fx_table[ instructions[_RPC + 1] ]->funtion_size, &adrs, heap_metadata, &_RERR);
            
            //step 2 -> verify the allocation, if fail then silent closure
            if ( _RERR == HEAP_MEMORY_FULL ) {
                logger(ERROR, "Heap Memory Full", "Unable to allocate enough memory for function call."); 
                return;
            }

            //step 3 -> heap_address, return_address, callee_heap_address, arguments, data
            memcpy(&heap[adrs], &adrs, 8); //copy over the heap address itself.
            memcpy(&heap[adrs + 8], &instructions[_RPC + 2], 8); //copy over the return address (8 bytes)
            memcpy(&heap[adrs + 16], &_RFX, 8); //copy over the heap address of the callee.
            memcpy(&heap[adrs + 24], &heap[_R1(u64)], fx_table[instructions[_RPC + 1]]->arg_size); //copy over the argument.
            
            _RFX = adrs; //set the current function register to this
            _RPC = fx_table[ instructions[_RPC + 1] ]->instruction_ptr; //go here
            
        }

        goto *dispatch_table[instructions[ _RPC ]];

    OP_SYSCALL:
    
    	#ifdef DEBUG
     		printf("Debug: SYSCALL reached");
       	#endif
        fibre->status = WAITING;
        //a system call can ONLY manipulate registers and heap.
        table[instructions[_RPC + 1]](fibre, heap); _RPC+=2;
        goto FETCH;

    OP_RETURN:
        {   
            memcpy(&_RPC, &heap[ _RFX + 8 ], 8); //again, 8th padding so here we are.
            uint64_t adrs = _RFX;
            //_RFX now should point to the callee heap address instead.
            memcpy(&_RFX, &heap[_RFX + 16], 8); //16 because that's where the callee heap address is
            //free the memory
            vmfree(adrs, heap_metadata, &_RERR);
        }
        
        goto *dispatch_table[instructions[ _RPC ]]; //basically return.

    COMPOSE(EQ, EQUAL, fibre, FETCH)
    COMPOSE(NEQ, NOT_EQUAL, fibre, FETCH)
    COMPOSE(LT, LESS_THAN, fibre, FETCH)
    COMPOSE(LTEQ, LESS_THAN_EQUAL, fibre, FETCH)

    // naturally ends VM
    OP_PROGRAM_END: 
        fibre->status = TERMINATED;
        //?: the reference is itself, so that means no more fibre left to execute.
        if ( fibre->next == fibre ) return;
        //? rewires the pool for next/previous pointers
        fibre->before->next = fibre->next;
        fibre->next->before = fibre->before;
        //? There MUST exist a next fibre, so switch there
        
        {
            Fibre *this = fibre;
            fibre = fibre->next;
            __drop_fibre__(this);
        }

        //? No need for the scheduler to run again, jump directly to __MAIN.
        goto __MAIN;
    return;

}