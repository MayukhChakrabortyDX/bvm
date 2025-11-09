import colors from "colors"

export function Logger(type: "ERROR" | "WARNING" | "INFO", title: string, body: string) {
	
	let msg = `[${type}]`
	
	switch (type) {
		case "ERROR":
			msg = colors.bold(msg.red);
			break;
		case "WARNING":
			msg = colors.bold(msg.yellow);
			break;
		case "INFO":
			msg = colors.bold(msg.blue);
	}

	console.log(`${msg} ${colors.bold(title)}\n${body}`)
}