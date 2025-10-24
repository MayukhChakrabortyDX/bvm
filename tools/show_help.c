#include "show_help.h"

const char *logo =
" ____  _ _         ____ _____ \n"
"| __ )| (_)___ ___|  _ \\_   _|\n"
"|  _ \\| | / __/ __| |_) || |  \n"
"| |_) | | \\__ \\__ \\  _ < | |  \n"
"|____/|_|_|___/___/_| \\_\\|_|  \n";

const char *data = 
"\033[1;34mVersion Alpha, Prototypical stage\033[0m\n"
"                                  \n"

"\033[1;4;37mAvailable Commands:\033[0m \n\n"

"\033[90m$\033[0m "      // $ in gray
"\033[1;32mbliss\033[0m " // command in bold green
" : shows this screen (info screen)\n\n"

"\033[90m$\033[0m "      // $ in gray
"\033[1;32mbliss\033[0m " // command in bold green
"\033[36mapp.bin\033[0m "
" : runs a bliss bytecode app\n\n"

"\033[90m$\033[0m "      // $ in gray
"\033[1;32mbliss\033[0m " // command in bold green
"\033[1;35mdump\033[0m "
"\033[36mapp.bin\033[0m "
" : produces dump-file for bliss binary\n\n"

"visit \033[1;4;34mwww.omegaone.in\033[0m for more information\n"
"Made by Mayukh Chakraborty\n\n";

void show_help() {
    printf(
        "%s%s", logo,data
    );
}