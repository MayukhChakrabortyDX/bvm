#pragma once
//macro expansion is used here!
#define TYPES(NAME) OP_u64##NAME, OP_u8##NAME, OP_i64##NAME, OP_i32##NAME, OP_i16##NAME, OP_i8##NAME, OP_f32##NAME, OP_d64##NAME
//very simple type system without any unsigned stuff.