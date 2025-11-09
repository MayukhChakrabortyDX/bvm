import { Logger } from "./logger";

export const baseTypes = [
	'i8', 'i16', 'i32', 'i64',
	'u64', 'char', 'f32', 'f64'
] as const;

const sizeof = new Map<typeof baseTypes[number], number>()
sizeof.set('char', 1)
sizeof.set('f32', 4)
sizeof.set('f64', 8)
sizeof.set('i16', 2)
sizeof.set('i32', 4)
sizeof.set('i64', 8)
sizeof.set('i8', 1)
sizeof.set('u64', 8)

export type kind = 'scalar' | 'vector-fill' | 'vector-defined'

export function numberToBytes(number: number) {
	//conver a number to uint8 bytes
	if ( number == 0 ) {
		return [0]
	}
	
	let bytes = []
	let buffer: number = number;
	
	while ( buffer != 0 ) {
		bytes.push( buffer % 256 )
		buffer = Math.floor(buffer / 256)
	}
	
	return bytes.reverse() //the byte ordering is big endiann for our use case.
}

export function make8bytes(bytes: number[]) {
	
}

export class DataEntry {

	constructor(
		public kind: kind,
		public value: any, //we will use the kind to think about it
		public baseType: typeof baseTypes[number],
		public itemCount: number, //number of items present.
		public fillValue: number //this is also infered based on kind
	) {}
	
	emitBytes() {
		//emit bytes for this line of data point.
		//format: <bytes to copy> -> < bytes of data >
		//@ts-ignore	
    let bytesToCopy = numberToBytes(this.itemCount * sizeof.get(this.baseType));
    let rawBytes = []
    if ( this.kind == 'scalar' ) {
   		rawBytes.push( ...numberToBytes( this.value as number ) ) //because all primitives are numbers
    } else if ( this.kind == 'vector-defined' ) {
   		//assume flattened array alread.
     	(this.value as number[]).forEach(item => rawBytes.push( ...numberToBytes(item) ))
    
    } else if ( this.kind ==	'vector-fill' ) {
   		rawBytes.push( ...numberToBytes( this.fillValue ) )
    } else {
   		Logger("ERROR", "Invalid Data Kind", `The data kind ${ this.kind } is unrecognized`)
     	process.exit(0)
    }
    return new Uint8Array( [ ...bytesToCopy, ...rawBytes ] ) 
	}	
}

//test it first!
// 
const simpleData = new DataEntry( "vector-fill", [], "char", 2585754, 8643416541654)
console.log( simpleData.emitBytes() )