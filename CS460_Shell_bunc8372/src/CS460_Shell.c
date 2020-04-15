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
void executeCommand(Command *psCommand);

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
		commandCreate(psCommand);

		// Input command
		printf("%d>", pid);
		getline(&szArgs, (size_t*) &sSize, stdin);
		memcpy(szArgsBackup, szArgs, strlen(szArgs));

		if (strcmp(szArgsBackup, EXIT))
		{
			// Fill data structure
			commandParse(psCommand, szArgs);

			// Print debug info
			if (argc > 1)
			{
				printCommand(psCommand);
			}

			// Execute Commands
			else
			{
				executeCommand(psCommand);
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

void executeCommand(Command *psCommand)
{
	char szOutput[MAX_COMMAND_SIZE];
	char szPath[MAX_COMMAND_SIZE] = "/bin/";
	char *pToken;
	char *pSavePtr;
	char szArgs[MAX_COMMAND_SIZE];
	char *argv[MAX_COMMAND_SIZE];
	int i;

	memcpy(szPath + strlen(szPath), commandGetCommand(psCommand), strlen(
			commandGetCommand(psCommand)));
	memcpy(szArgs, commandGetArguments(psCommand), strlen(commandGetArguments
			(psCommand)));
	argv[0] = malloc(strlen(commandGetCommand(psCommand)));
	memcpy(argv[0], commandGetCommand(psCommand), strlen(commandGetCommand
			(psCommand)));


	pToken = strtok_r(szArgs, " \n", &pSavePtr);
	for (i = 1; pToken; i++)
	{
		argv[i] = malloc(strlen(pToken));
		memcpy(argv[i], pToken, strlen(pToken));
		pToken = strtok_r(NULL, " \n", &pSavePtr);
	}

	int link[2];

	pipe(link);
	int pid = fork();

	if(pid == 0)
	{
		dup2 (link[1], STDOUT_FILENO);
		close(link[0]);
		close(link[1]);
		execv(szPath, argv);
	  exit(0);
	}
	else
	{
		waitpid(pid);
		close(link[1]);
		read(link[0], szOutput, sizeof(szOutput));
		while(*szOutput)
		{
			printf("%s\n", szOutput);
			read(link[0], szOutput, sizeof(szOutput));
			memset(szOutput, '\0', strlen(szOutput));
		}
		wait(NULL);
	}

	for (i = 0; argv[i]; i++)
	{
		memset(argv[i], '\0', strlen(argv[i]));
		free(argv[i]);
	}


}









