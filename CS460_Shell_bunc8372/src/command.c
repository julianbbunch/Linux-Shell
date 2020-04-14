/*****************************************************************************
 File name:   command.c
 Author:      Julian Bunch
 Date:        03/29/2020
 Class:       CS460
 Assignment:  Shell
 ****************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include"../include/command.h"

extern void commandCreate (Command *psCommand)
{
	/*
	psCommand->szCommand = malloc(2048);
	psCommand->szArguments = malloc(2048);
	psCommand->szOutput = malloc(2048);
*/
	memset(psCommand->szArguments, '\0', MAX_COMMAND_SIZE);
	memset(psCommand->szCommand, '\0', MAX_COMMAND_SIZE);
	memset(psCommand->szOutput, '\0', MAX_COMMAND_SIZE);

	psCommand->bRunInBackground = false;
}

extern void commandTerminate (Command *psCommand)
{

}

extern void commandParse (Command *psCommand, char *szString)
{
	char *pToken = NULL;
	char *pSavePtr;

	pToken = strtok_r(szString, " \n", &pSavePtr);
	memcpy(psCommand->szCommand, pToken, strlen(pToken));
	pToken = strtok_r(NULL, " \n", &pSavePtr);

	while (pToken)
	{
		if (*pToken != '&' && *pToken != '>')
		{
			strcat(psCommand->szArguments, pToken);
			strcat(psCommand->szArguments, " ");
		}
		else if (*pToken == '>')
		{
			pToken = strtok_r(NULL, " \n", &pSavePtr);
			strcat(psCommand->szOutput, pToken);
		}
		else if (*pToken == '&')
		{
			psCommand->bRunInBackground = true;
		}
		pToken = strtok_r(NULL, " \n", &pSavePtr);
	}
}

extern char *commandGetCommand(Command *psCommand)
{
	return psCommand->szCommand;
}

extern char *commandGetArguments(Command *psCommand)
{
	return psCommand->szArguments;
}

extern char *commandGetOutput(Command *psCommand)
{
	return psCommand->szOutput;
}

extern bool commandRunInBackground(Command *psCommand)
{
	return psCommand->bRunInBackground;
}

extern commandFullCommandString(Command *psCommand, char *pString)
{
	strcat(pString, psCommand->szCommand);
	strcat(pString, " ");
	strcat(pString, psCommand->szArguments);

	if (*psCommand->szOutput)
	{
		strcat(pString, "> ");
		strcat(pString, psCommand->szOutput);
	}

	if (psCommand->bRunInBackground)
	{
		strcat(pString, " &");
	}
}









