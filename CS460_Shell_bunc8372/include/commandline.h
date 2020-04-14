/*****************************************************************************
 File name:   commandline.h
 Author:      Julian Bunch
 Date:        03/29/2020
 Class:       CS460
 Assignment:  Shell
 ****************************************************************************/

#ifndef INCLUDE_COMMANDLINE_H_
#define INCLUDE_COMMANDLINE_H_

#include "command.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//****************************************************
// Constants
//****************************************************
#define MAX_COMMAND_SIZE 2048


//****************************************************
// User-defined types
//****************************************************
typedef struct CommandLine
{
	Command** psCommands;
	bool bRunInBackground;
}CommandLine;



//****************************************************
// Functions
//****************************************************
extern void clCreate (CommandLine *psCommandLine);
extern void clTerminate (CommandLine *psCommandLine);
extern void clParse (CommandLine *psCommandLine, char *szString);


#endif /* INCLUDE_COMMANDLINE_H_ */


