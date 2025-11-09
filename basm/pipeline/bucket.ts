//This process will produce buckets of the source code - `@data`, `@func`, `@code` buckets
// 
import { Logger } from "../libs/logger";
//these are raw sections btw. Because sub-tokenization will happen again, based on the context
//as provided by the section itself.
// 
// Also this is all line-based delimiters, so newlines are important here.
export class Bucket {
	
	constructor(
		public rawCode: string[],
		public dataSection: string[] = [],
		public funcSection: string[] = [],
		public codeSection: string[] = [],
	) {}
	
	produceBucket() {
		//produces the actual stuff to bucket
		let mode: "DATA" | "FUNCTION" | "CODE" | "START" = "START";
		
		for ( let line of this.rawCode ) {
			
			line = line.trim()
		
			if (line.startsWith(';')) continue;
			
			if ( line == '@data' ) {
				mode = "DATA"
				continue 
			} else if ( line == '@code' ) {
				mode = "CODE"; continue;
			} else if ( line == '@func' ) {
				mode = "FUNCTION"; continue;
			} else if ( line.startsWith('@') ) {
				Logger('ERROR', "Invalid Section", `The section @${line} is not recognized.`);
				process.exit();
			} else if ( mode == "START" ) {
				Logger('WARNING', 'Statement Before Section', `You have defined a section ${ line.underline } outside of valid sections. \n This will be ignored.`)
				continue
			}
			
			switch(mode) {
				case "DATA":
					this.dataSection.push(line); break;
				case "CODE":
					this.codeSection.push(line); break;
				case "FUNCTION":
					this.funcSection.push(line); break;
			}
		}
	}
}