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

extern void commandCreate (Command *psCommand)
{
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

extern void commandFullCommandString(Command *psCommand, char *pString)
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

extern void commandExecute(Command *psCommand)
{
	int file;
	char szPath[MAX_COMMAND_SIZE];
	char *pToken;
	char *pSavePtr;
	char szArgs[MAX_COMMAND_SIZE];
	char *argv[MAX_COMMAND_SIZE];
	int pid;

	// Set up the path
	memcpy(szPath, psCommand->szCommand, strlen(psCommand->szCommand));

	// Set up the arguments
	memcpy(szArgs, psCommand->szArguments, strlen(psCommand->szArguments));
	argv[0] = malloc(strlen(psCommand->szCommand));
	memcpy(argv[0], psCommand->szCommand, strlen(psCommand->szCommand));
	pToken = strtok_r(szArgs, " \n", &pSavePtr);
	for (int i = 1; pToken; i++)
	{
		argv[i] = malloc(strlen(pToken));
		memcpy(argv[i], pToken, strlen(pToken));
		pToken = strtok_r(NULL, " \n", &pSavePtr);
	}


	pid = fork();

	// Child process
	if (pid == 0)
	{
		if(*commandGetOutput(psCommand))
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
		memset(argv[i], '\0', strlen(argv[i]));
		free(argv[i]);
	}
}

extern void commandPrintDebug(Command *psCommand)
{
	printf("command: %s\n", commandGetCommand(psCommand));
	if (*commandGetArguments(psCommand))
	{
		printf("\targuments: %s\n", commandGetArguments(psCommand));
	}
	else
	{
		printf("\targuments: none\n");
	}
	printf("\tredirection:\n");
	printf("\t\tstdin: none\n");
	if (*commandGetArguments(psCommand))
	{
		printf("\t\tstdout: %s\n", commandGetOutput(psCommand));
	}
	else
	{
		printf("\t\tstdout: none\n");
	}
	printf("\tpipe: none\n");
	printf("background: ");
	if (commandRunInBackground(psCommand))
	{
		printf("YES\n");
	}
	else
	{
		printf("no\n");
	}
}






