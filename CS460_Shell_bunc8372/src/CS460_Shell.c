//****************************************************
// File name:  CS460_Shell.c
// Author:     Julian Bunch
// Date:       03/03/2020
// Class:      CS460
// Assignment: Shell
// Purpose:    Create our own Linux shell
//****************************************************

#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

#include"../include/commandline.h"
#include"../include/command.h"

void printCommand(Command *psCommand);

/****************************************************************************
 Function:    main
 Description: Runs a shell
 Parameters:  argc - number of command line arguments
 argv - argument values
 Returned:    0
 ****************************************************************************/
int main(int argc, char *argv[])
{
	const size_t sSize = MAX_COMMAND_SIZE;
	const char *EXIT = "exit\n";
	int pid = getpid();
	char *szArgs = malloc(MAX_COMMAND_SIZE);
	char *szArgsBackup = malloc(MAX_COMMAND_SIZE);

	Command sCommand;
	Command *psCommand = &sCommand;
	commandCreate(psCommand);

	while (strcmp(szArgsBackup, EXIT))
	{
		// Clear data
		memset(szArgs, '\0', MAX_COMMAND_SIZE);
		memset(szArgsBackup, '\0', MAX_COMMAND_SIZE);
		memset(psCommand, '\0', sizeof(Command));

		// Input command
		printf("%d>", pid);
		getline(&szArgs, (size_t*) &sSize, stdin);
		memcpy(szArgsBackup, szArgs, strlen(szArgs));

		if (strcmp(szArgsBackup, EXIT))
		{
			// Fill data structure
			commandParse(psCommand, szArgs);

			// Print Commands
			if (argc > 1)
			{
				printCommand(psCommand);
			}


			// Execute Commands
			else
			{

			}

		}
	}

	// Free data
	free(szArgs);
	free(szArgsBackup);

	return 0;
}

void printCommand(Command *psCommand)
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











