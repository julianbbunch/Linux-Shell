/*****************************************************************************
 File name:   commandline.h
 Author:      Julian Bunch
 Date:        03/29/2020
 Class:       CS460
 Assignment:  Shell
 ****************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include"../include/commandline.h"
#include"../include/command.h"

extern void clCreate (CommandLine *psCommandLine)
{
	psCommandLine->psCommands = malloc(sizeof(Command*));
	psCommandLine->psCommands[0] = NULL;
	psCommandLine->bRunInBackground = false;
}

extern void clTerminate (CommandLine *psCommandLine)
{

}

extern void clParse (CommandLine *psCommandLine, char *szString)
{

}
