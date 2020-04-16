/*****************************************************************************
 File name:   command.c
 Author:      Julian Bunch
 Date:        03/29/2020
 Class:       CS460
 Assignment:  Shell
 ****************************************************************************/

#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include <fcntl.h>
#include <sys/wait.h>

#include"../include/command.h"

/****************************************************************************
 Function:    commandClear
 Description: zeros out all data in Command
 Parameters:  psCommand - ptr to Command struct
 Returned:    None
 ****************************************************************************/
extern void commandClear (Command *psCommand)
{
	memset(psCommand->szArguments, '\0', MAX_COMMAND_SIZE);
	memset(psCommand->szCommand, '\0', MAX_COMMAND_SIZE);
	memset(psCommand->szOutput, '\0', MAX_COMMAND_SIZE);

	psCommand->bRunInBackground = false;
}

/****************************************************************************
 Function:    commandParse
 Description: Fills Command data structure
 Parameters:  psCommand - ptr to Command struct
 Returned:    None
 ****************************************************************************/
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

/****************************************************************************
 Function:    commandExecute
 Description: Executes the command
 Parameters:  psCommand - ptr to Command struct
 Returned:    None
 ****************************************************************************/
extern void commandExecute(Command *psCommand)
{
	int file;
	char szPath[MAX_COMMAND_SIZE];
	char *pToken;
	char *pSavePtr;
	char szArgs[MAX_COMMAND_SIZE];
	char *argv[MAX_COMMAND_SIZE];
	int pid, i;

	// Set up the path
	memcpy(szPath, psCommand->szCommand, strlen(psCommand->szCommand) + 1);

	// Set up the arguments
	memcpy(szArgs, psCommand->szArguments, strlen(psCommand->szArguments) + 1);
	argv[0] = malloc(strlen(psCommand->szCommand));
	memcpy(argv[0], psCommand->szCommand, strlen(psCommand->szCommand) + 1);
	pToken = strtok_r(szArgs, " \n", &pSavePtr);
	for (i = 1; pToken; i++)
	{
		argv[i] = malloc(strlen(pToken) + 1);
		memcpy(argv[i], pToken, strlen(pToken) + 1);
		pToken = strtok_r(NULL, " \n", &pSavePtr);
	}
	argv[i] = NULL;


	pid = fork();

	// Child process
	if (pid == 0)
	{
		if(*psCommand->szOutput)
		{
			file = open(psCommand->szOutput, O_WRONLY | O_TRUNC | O_CREAT, 0644);
			dup2 ((int)file, STDOUT_FILENO);
			execvp(szPath, argv);
			close(file);
		}
		else
		{
			execvp(szPath, argv);
		}

	  exit(0);
	}
	// Parent process
	else
	{
		if (!psCommand->bRunInBackground)
		{
			waitpid(pid, NULL, NULL);
		}
	}

	for (int i = 0; argv[i]; i++)
	{
		memset(argv[i], '\0', strlen(argv[i]) + 1);
		free(argv[i]);
	}
}

/****************************************************************************
 Function:    commandPrintDebug
 Description: Prints the command and arguments for debugging option
 Parameters:  psCommand - ptr to Command struct
 Returned:    None
 ****************************************************************************/
extern void commandPrintDebug(Command *psCommand)
{
	printf("command: %s\n", psCommand->szCommand);
	if (*psCommand->szArguments)
	{
		printf("\targuments: %s\n", psCommand->szArguments);
	}
	else
	{
		printf("\targuments: none\n");
	}
	printf("\tredirection:\n");
	printf("\t\tstdin: none\n");
	if (*psCommand->szOutput)
	{
		printf("\t\tstdout: %s\n", psCommand->szOutput);
	}
	else
	{
		printf("\t\tstdout: none\n");
	}
	printf("\tpipe: none\n");
	printf("background: ");
	if (psCommand->bRunInBackground)
	{
		printf("YES\n");
	}
	else
	{
		printf("no\n");
	}
}






