import { Logger } from "./libs/logger";
import { numberToBytes } from "./libs/model";
import { Bucket } from "./pipeline/bucket";

let types = ['u64', 'u8', 'i64', 'i32', 'i16', 'i8', 'f32', 'f64'];
function produceTypes(name: string) {
	return types.map(t => `${t}${name}`)
}

//These opcodes are arranged based on it's index, so it's useful to find index
// as well. we need a mapping of values as well.
let opcodes = [
	...produceTypes('add'), ...produceTypes('sub'),
	...produceTypes('mul'), ...produceTypes('div'),

	...produceTypes('xadd'), ...produceTypes('xsub'),
	...produceTypes('xmul'), ...produceTypes('xdiv'),
	
	'rsizei8', 'rsizei16', 'rsizei32', //resize to x
	'itof32', 'itof64', 'ftoi32', 'ftoi64', //x to y
	
	'jeq', 'jmp', 'cll', 'scll', 'ret',
	
	...produceTypes('eq'), ...produceTypes('neq'),
	...produceTypes('lteq'), ...produceTypes('lt'),

	'mov', 'cls',
	'hcpy', 'fhcpy',
	'load', 'fload',
	'alloc', 'free',
	'store', 'fstore',
	'rload', 'frload',
	'end'
]

let opcode_map = new Map<string, number>()
opcodes.forEach((opcode, index) => opcode_map.set(opcode, index));

let rawData = `
	; this is a inline comment.
	@data
		str char* "Hello World"
	@func
		$main () ::main
		
	@code
		call ::main
		::main
			load RSYS [char]
			syscall 0
			end
	
`

//stage-1 -> process raw bytecode and produce simple bucket stuff here
let buckets = new Bucket(rawData.trim().split('\n'));
buckets.produceBucket()

//stage-2 -> process data section to create a traversable symbol table
// as well as produce header uint8 data here.


console.log(
	new Uint8Array([10, 10, 5])
)

console.log(
	numberToBytes(657925)
)