//****************************************************
// File name:  CS460_Shell.c
// Author:     Julian Bunch
// Date:       03/03/2020
// Class:      CS460
// Assignment: Shell
// Purpose:    Create our own Linux shell
//****************************************************

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct Command
{
	char* szCommand;
	char* szArguments;
	char* szInput;
	char* szOutput;
	bool bPipe;
	bool bBackground;

} Command;

int main(int argc, char *argv[])
{
	const size_t MAX_COMMAND_SIZE = 2048;
	const char* EXIT = "exit";
	const char* PIPE = "|";
	const char* END_COMMAND = ";";
	const char* BACKGROUND = "&\n";
	const char* OUTPUT = ">";
	const char* INPUT = "<";

	Command** psCommands = malloc(sizeof(Command*));
	int pid = getpid();
	int commandIndex = 0;
	int argIndex = 0;
	char *szArgs = malloc(MAX_COMMAND_SIZE);
	char *szArgsBackup = malloc(MAX_COMMAND_SIZE);
	char *pToken;
	char *pSavePtr;

	do
	{
		// Empty strings
		memset(szArgs, '\0', MAX_COMMAND_SIZE);
		memset(szArgsBackup, '\0', MAX_COMMAND_SIZE);

		psCommands[commandIndex] = NULL;

		// Input command
		printf("%d>", pid);
		getline(&szArgs, (size_t*)&MAX_COMMAND_SIZE, stdin);
		memcpy(szArgsBackup, szArgs, strlen(szArgs));
		printf("%s", szArgs);

		// Fill data structure(s)
		do
		{
			// If starting a new command, malloc a new Command
			if (NULL == psCommands[commandIndex])
			{
				psCommands[commandIndex] = (Command*)malloc(sizeof(Command));

				// NULL member variables
				psCommands[commandIndex]->szCommand = NULL;
				psCommands[commandIndex]->szArguments = NULL;
				psCommands[commandIndex]->szInput = NULL;
				psCommands[commandIndex]->szOutput = NULL;

				psCommands[commandIndex + 1] = NULL;
				argIndex = 0;

				// If the token was a pipe, set input to pipe
				if (strcmp(pToken, PIPE) == 0)
				{
					psCommands[commandIndex]->szInput = "PIPE";
				}
			}

			// First token of a new command must be the command
			if (argIndex == 0)
			{
				if (0 == commandIndex)
				{
					pToken = strtok_r(szArgs, " ", &pSavePtr);
				}
				else
				{
					pToken = strtok_r(NULL, " ", &pSavePtr);
				}
				argIndex++;
				psCommands[commandIndex]->szCommand = pToken;
			}
			// Tokens following the first
			else
			{
				pToken = strtok_r(NULL, " ", &pSavePtr);
				argIndex++;

				// If token is a pipe
				if (strcmp(pToken, PIPE) == 0)
				{
					psCommands[commandIndex]->szOutput = "PIPE";
					commandIndex++;
				}
				// If token is a >
				else if (strcmp(pToken, OUTPUT) == 0)
				{
					pToken = strtok_r(NULL, " ", &pSavePtr);
					argIndex++;
					psCommands[commandIndex]->szOutput = pToken;
					pToken = strtok_r(NULL, " ", &pSavePtr);
					argIndex++;
				}
				// If token is a <
				else if (strcmp(pToken, INPUT) == 0)
				{
					pToken = strtok_r(NULL, " ", &pSavePtr);
					argIndex++;
					psCommands[commandIndex]->szInput = pToken;
					pToken = strtok_r(NULL, " ", &pSavePtr);
					argIndex++;
				}

				// If token is an argument for the command
				if (strcmp(pToken, END_COMMAND) != 0 && strcmp(pToken, PIPE) != 0 && strcmp(pToken, BACKGROUND) != 0)
				{
					if (NULL == psCommands[commandIndex]->szArguments)
					{
						psCommands[commandIndex]->szArguments = pToken;
					}
					else
					{
						strcat(psCommands[commandIndex]->szArguments, pToken);
						strcat(psCommands[commandIndex]->szArguments, " ");
					}
				}
				// If token is an &
				else if (strcmp(pToken, BACKGROUND) == 0)
				{
					psCommands[commandIndex]->bBackground = true;
					commandIndex++;
					pToken = strtok_r(NULL, " ", &pSavePtr);
					argIndex++;
				}
			}
		}while (pToken != NULL);

		// Print Commands
		for (int i = 0; i < commandIndex; i++)
		{
			// command
			printf("command: %s\n", psCommands[i]->szCommand);

			// arguments
			if (psCommands[i]->szArguments != NULL)
			{
				printf("\targuments: %s\n", psCommands[i]->szArguments);
			}
			else
			{
				printf("\targuments: none\n");
			}

			// redirection
			printf("\tredirection:\n");

			// input
			if (psCommands[i]->szInput != NULL)
			{
				printf("\t\tstdin: %s\n", psCommands[i]->szInput);
			}
			else
			{
				printf("\t\tstdin: none\n");
			}

			// output
			if (psCommands[i]->szOutput != NULL)
			{
				printf("\t\tstdout: %s\n", psCommands[i]->szOutput);
			}
			else
			{
				printf("\t\tstdout: none\n");
			}

			// pipe
			if (psCommands[i]->bPipe)
			{
				printf("\tpipe: YES\n");
			}
			else
			{
				printf("\tpipe: none\n");
			}

			// background
			if (psCommands[i]->bBackground)
			{
				printf("background: YES\n");
			}
			else
			{
				printf("background: no\n");
			}
		}

		// Free Commands
		for (int i = 0; i < commandIndex; i++)
		{
			free(psCommands[i]);
		}
		pToken = NULL;
		argIndex = 0;
		commandIndex = 0;

	}while (strcmp(szArgs, EXIT) != 0);

	free(szArgs);
	free(szArgsBackup);
	free(psCommands);

	return 0;
}
