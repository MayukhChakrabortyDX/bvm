#pragma once

typedef struct {
	char *name;
	char *value;
} Flag;

typedef struct {
	Flag *flags; //just use a buffer and a NULL terminated datapoint.
	char** values;
	char* command;
} Argument;
//produces a well-formed argument here
Argument process_args(int argc, char** argv);
//no need to 