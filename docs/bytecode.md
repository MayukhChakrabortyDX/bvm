# Bytecode Format

The bytecode file is divided into two broad sections:

## Header section

Contains the version information, the constant pool, system
call/invokation registry and instruction size.

[ version number ( 2 bytes ) ]
[ constant pool region (variable bytes) ]
[ invoke registry (variable bytes) ]
[ header end ]

1. Version Number

This is an unsigned integer type value taking up 2 bytes of
space. This tell the VM the version of the bytecode, often
helping in telling compatibility.

2. Constant pool

This is a variable byte region because the constants used
depends on the program itself.

By definition, constants are those values that are never
cleared from the memory (heap). They have static lifetimes
and are loaded to the heap one by one.

### Memory Layout of constant pool

[ size of pool (8 bytes) ]
[ bytes to copy ( 8 bytes, unsigned long ) ]
[ actual raw binary data, of the given size. (variable bytes) ]

In other words, the algorithm goes somewhat like this:

Step 1: The parser looks at the first 8 bytes of the constant pool region
Step 2: The number stored there tells us exactly how many bytes to expect (copy to heap) next.
Step 3: Once the copying is done, the parser again checks next 8 bytes for size information.
Step 4: Do this exactly number of pool size.

Since the size of the pool, i.e number of constant is predefined, the algorithm runs
that many times, copying that many bytes to heap.

The compiler also do not assume any type, it just memory copies as it is.
The type must be verified by the compiler. The VM verifies the integrity of the bytecode
beforehand either way.

3. Invoke registry

This is basically an array of string that contains the address to the dll/so files
for system calls/invokable.

These functions are loaded ahead-of-time because they are known to be used.
On the top level, they look something like this:

[ "math", "io" ] etc.

### Caveats on using FFI/System libraries

The BVM comes with it's own set of standard libraries and set of dll/so files,
each exposing exactly one function (i.e exactly same as name of the file).

If you open the path where the BVM is stored, you will find a folder called `/libs`
This folder contains all the library files.

So if you ever make a system library, it MUST be stored in the `/libs` folder
because that's where the VM searches for dynamically linked libraries.

There is also a registry for the same reason: registy.json and registry.bin.
The JSON stores the registry as follows:

```jsonc
{
    "libraries": [
        "math", "io", //etc
    ]
}
```

The compiler and the VM, DOES NOT reads this file. It MUST be compiled to the
binary equivalent first to even work.

The reason why it's in JSON is to make it easier for package managers to
easily make modifications.

### Memory layout of the invoke registry

[ size of registry ]
[ squence of indexes, ( 4 bytes each ) ]

The algorithm to read this is simple, it just:

Step 1: Read the size of the registry
Step 2: Open the registry.bin from path
Step 3: load the corresponding library to the index mentioned
Step 4: Do this to match the size of the registry.

Once we exhaust this list, we are sure that the registry has been loaded
succesfully. Any error that comes during this phase is promtply reported and
execution is terminated.

What follows next is the instructions itself.

## Instruction space

The instructions are also sized before they are loaded.

### Memory Layout

[ total size of instruction space ( 8 bytes ) ]
[ instructions themselves (variable) ]

The algorithm pre-allocates an instruction space to accomodate the
bytecode itself.

Once this stage is done, some defaults are used that are NOT present
in the bytecode but are provided via the terminal such as:

1. Minimum heap size
2. Multiplication factor

This is the entirety of the bytecode format.