let types = ['u64', 'u8', 'i64', 'i32', 'i16', 'i8', 'f', 'd'];
function produceTypes(name: string) {
	return types.map(t => `${t}${name}`)
}

let opcodes = [
	...produceTypes('add'), ...produceTypes('sub'),
	...produceTypes('mul'), ...produceTypes('div'),

	...produceTypes('xadd'), ...produceTypes('xsub'),
	...produceTypes('xmul'), ...produceTypes('xdiv'),
	
	'rsizei8', 'rsizei16', 'rsizei32',
	'i32tof', 'i64tod', 'ftoi32', 'dtoi64',
	
	'jeq', 'jmp', 'call', 'scall', 'ret',
	
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

//process the bytecode and produce the bytes as-is on the fly, because
// that's a lot better than waiting to produce bytes itself.

function processDataSection(statement: string) {
	
}

//stage-1 -> process raw bytecode and produce simple bucket stuff here
let buckets = new Bucket(rawData.trim().split('\n'));
buckets.produceBucket()
Logger("INFO", 'Stage 1 Completed', body)

	let mode: null | 'data' | 'code' | 'func' = null
	
	for ( let line of lines ) {
	}
	
}