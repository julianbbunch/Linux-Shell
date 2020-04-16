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
	char szArgs[MAX_COMMAND_SIZE];
	char szArgsBackup[MAX_COMMAND_SIZE];
	char *pArgs = szArgs;

	Command sCommand;
	Command *psCommand = &sCommand;
	commandCreate(psCommand);

	while (strcmp(szArgsBackup, EXIT))
	{
		// Clear data
		memset(szArgs, '\0', MAX_COMMAND_SIZE);
		memset(szArgsBackup, '\0', MAX_COMMAND_SIZE);
		commandCreate(psCommand);

		// Input command
		printf("%d>", pid);
		getline(&pArgs, (size_t*) &sSize, stdin);
		memcpy(szArgsBackup, szArgs, strlen(szArgs));

		if (strcmp(szArgsBackup, EXIT))
		{
			// Fill data structure
			commandParse(psCommand, szArgs);

			// Print debug info
			if (argc > 1)
			{
				commandPrintDebug(psCommand);
			}

			// Execute Command
			else
			{
				commandExecute(psCommand);
			}
		}
	}

	return 0;
}






