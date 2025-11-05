#include "help.h"
#include <stdio.h>

typedef struct {
    const char *cmd;
    const char *args;
    const char *desc;
} ToolEntry;

const char *logo =
" \033[90m____  _ _         ____ _____ \n"
"| __ )| (_)___ ___|  _ \\_   _|\n"
"|  _ \\| | / __/ __| |_) || |  \n"
"| |_) | | \\__ \\__ \\  _ < | |  \n"
"|____/|_|_|___/___/_| \\_\\|_|  \n\033[0";

const ToolEntry tools[] = {
	//make sure the space exist
	//because the loop formats this way.
    { "asm ",       "app.bsm -o app.bxe", "assembles Bliss assembly into bytecode" },
    { "run ",       "app.bxe",            "runs a Bliss bytecode executable" },
    { "doctor ",    "",                   "checks the current installation to identify and recover errors"},
    { "",           "",                   "shows this information screen" }
};

void print_logo() {
    printf("%s\n", logo);
}

void print_tool(const ToolEntry *t) {
    printf(
        "\033[90m$\033[0m "
       	"\033[1;33mbrt\033[0m "
        "\033[1;32m%s\033[0m", t->cmd
    );

    if (t->args && *t->args)
        printf("\033[36m%s\033[0m ", t->args);

    printf(": %s\n", t->desc);
}

void show_help() {
    print_logo();

    printf("\033[1;34mVersion 0x00, Prototypical stage\033[0m\n\n");
    printf("\033[1;4;37mAvailable Commands:\033[0m\n\n");

    size_t count = sizeof(tools) / sizeof(tools[0]);
    for (size_t i = 0; i < count; ++i)
        print_tool(&tools[i]);

    printf("\nDeveloped by Mayukh Chakraborty\n\n");
}