#pragma once

enum logType {

    ERROR,
    WARNING,
    INFORMATION

}; typedef enum logType logType ;

void logger( logType name, char* title, char* message );