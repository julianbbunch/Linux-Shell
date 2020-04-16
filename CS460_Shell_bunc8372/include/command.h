/*****************************************************************************
 File name:   command.h
 Author:      Julian Bunch
 Date:        03/29/2020
 Class:       CS460
 Assignment:  Shell
 ****************************************************************************/

#ifndef INCLUDE_COMMAND_H_
#define INCLUDE_COMMAND_H_

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
typedef struct Command
{
	char szCommand[MAX_COMMAND_SIZE];
	char szArguments[MAX_COMMAND_SIZE];
	char szOutput[MAX_COMMAND_SIZE];
	bool bRunInBackground;

}Command;

//****************************************************
// Functions
//****************************************************
extern void commandClear (Command *psCommand);
extern void commandParse (Command *psCommand, char *szString);
extern void commandExecute(Command *psCommand);
extern void commandPrintDebug(Command *psCommand);


#endif /* INCLUDE_COMMAND_H_ */

