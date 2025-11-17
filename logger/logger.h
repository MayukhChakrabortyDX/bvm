#ifndef __LOGGER__
#define __LOGGER__

enum logType {

    ERROR,
    WARNING,
    INFORMATION

}; typedef enum logType logType ;

void logger( logType name, char* title, char* message );

#endif