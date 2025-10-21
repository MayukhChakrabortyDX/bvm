General bytecode structure for byte-based decoding.

1. Limitations:

Encoding the instructions in series of 8 bits (1 byte) as per the universal format
of files.

2. Processes:

The VM decodes the byte based encoding to 64 bit (or 32 bit depending on system) that
is optimized for the virtual machine.

3. Assumptions:

To encode a number, depending upon the type, we will assume the following things:

1. i64, u64 & double will span 8 bytes
2. i32, u32 & float will span 4 bytes
3. i16, u16 will span 2 bytes
4. i8, u8 will span 1 byte

array sizes MUST be declared, and the size of an array is maximum exp(2, 32).

4. endianness:

Generally handled by the system hardware, but we still assume little endianness
for x86 systems for now.

5. Constants:

When programming in higher level languages, we often see 'constants' all across the
codebase.

It might feel wierd, but constants are generally any value that a programmer hard-codes.

For example:

```c
#include <stdio.h>

int main() {

    printf("Hello World");
    return 0;

}
```

we can identify two constants, `0` and `Hello World`. These constants are accounted
for beforehand, and they are constants because their lifetime is static.

So for our language, we need a way to encode constants too! Let's see how BVM's bytecode
encodes constants.

A. Identify the types first

When encoding the constants, the compiler must identify the type of the constant.
For example, a i64 is generally called `long`, short from in `l`.

so when the VM parser sees a `l`, it assumes the next 8 bytes is a `long`.

Here are the type annotations:

`l` -> long, assumes next 8 bytes
`i` -> int, assumes next 4 bytes
`s` -> short, assumes next 2 bytes
`b` -> byte, assume next byte
`f` -> float, assume next 4 bytes
`d` -> double, assumes next 8 bytes

B. Arrays

Although technically speaking, arrays are not really needed to be allocated as is,
but it is always better to NOT simulate things that can be much more performant,
for which we also have a dedicated array allocator.

When the parser sees `a`, it knows the value will be an array. But it must also
know the size of each element, and number of elements.

So it goes to the next byte to check the size, and ofc it's the type annotation,
because `l`, `f` etc tells a lot about element sizing.

Nice, now what left is the element count, and for that, we have allocated
4 bytes, representing an unsigned integer of 32 bit, which gives you the total
of 4,294,967,296 elements, and I don't think anyone is ever writing that much
in hand, but you never know.

so in ascii, it looks somewhat like this:

ai64 -> the parser assumes 32 bit values, 64 elements, and appropriately allocates
the blocks of it.

(NOTE: the 64 is numbered in 32 bits, it will look different bitwise.)

so roughly speaking, the bytecode for constant values look likes this:

i10
i20
ab11Hello World

although this does not represents the true nature, it roughly indicates how
it may look like.

i -> integer, and the constant is 10
simillarly the array of bytes of size 11 is Hello World, and we know strings
are just array of chars, and that's just unsigned bytes.

This assumption also helps us to avoid quotation parsing, so strings like

Hell"o Wor"ld is still an entirely valid string, including the quotes, even
with escape characters.

6. OPCODES and partition

Well, only constants are not going to help anything at all, we need to create
some space for OPCODES as well.

The character `e` tells the parser that the constant pool has ended. So that
means anything after the `e` byte will be OPCODE byte.

Fun fact is, I have designed the VM to not have OPCODEs more than 255, which
exactly fits 2^8 - 1, so each byte represent a single OPCODE.

generally some opcode assumes parameters of some size, and some do not.
based on this, the parser processes each OPCODE.

But if an unrecognized OPCODE happens, it raises fatal error.

for example, some inline OPCODEs and some load operators are preassumed
by the parser.

For example the OP_LOAD_8 assumes that the next 8 bytes will be the heap address,
next 8 bytes will be offset address, and next 1 byte is register address.

7. Metadata and information bytes

OPCODE sizes MUST be known during parsing for predictable results.
and same for the constant sizes.

so we allocate 8 bytes for the total number of opcodes. This is
used by the malloc in parser to allocate the buffer block beforehand.

In other words, the second 8 bytes always denotes the size of the
instruction space, calculated in 64 bit words, even if they are
encoded in byte.

And the first 2 bytes contains the version number of the bytecode
being executed.

The reason to store a version number is simple: Often time, the in-dev
virtual machines will come with dual parser, one with older version
and the other with the newer version.

The only constant thing about bytecode will ALWAYS be the first 2 bytes
that stores the version information.

This helps in transitioning to breaking changes without much issue.
Infact, if any changes do happen to the bytecode format itself, I will
make sure to provide transpilers that can upgrade the bytecode,
and also support a bytecode format for atleast 2 years at most, with
middle versions of VM coming with dual parser (selectable).

In essence, the bytecode format is turing complete from the start only,
so the only reason to upgrade the format is to optimize certain parsing
algorithm or when new opcodes gets introduced that reduces software emulation.