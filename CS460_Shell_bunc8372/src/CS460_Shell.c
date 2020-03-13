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
	char** szArguments;
	char* szInput;
	char* szOutput;
	bool bPipe;
	bool bBackground;

} Command;

/****************************************************************************
 Function:    main
 Description: Runs a shell
 Parameters:  argc - number of command line arguments
              argv - argument values
 Returned:    0
****************************************************************************/
int main(int argc, char *argv[])
{
	const size_t MAX_COMMAND_SIZE = 2048;
	const char* EXIT = "exit\n";
	const char* PIPE = "|";
	const char* END_COMMAND = ";";
	const char* BACKGROUND = "&";
	const char* OUTPUT = ">";
	const char* INPUT = "<";

	Command** psCommands = malloc(sizeof(Command*));
	int pid = getpid();
	int commandIndex = 0;
	int argIndex = 0;
	char *szArgs = malloc(MAX_COMMAND_SIZE);
	char *szArgsBackup = malloc(MAX_COMMAND_SIZE);
	char *pToken = NULL;
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

		// Fill data structure(s)
		do
		{
			// If starting a new command, malloc a new Command
			if (NULL == psCommands[commandIndex])
			{
				psCommands[commandIndex] = (Command*)malloc(sizeof(Command));

				// NULL member variables
				psCommands[commandIndex]->szCommand = NULL;
				psCommands[commandIndex]->szArguments = malloc(sizeof(char*));
				psCommands[commandIndex]->szArguments[0] = NULL;
				psCommands[commandIndex]->szInput = NULL;
				psCommands[commandIndex]->szOutput = NULL;
				psCommands[commandIndex]->bPipe = false;
				psCommands[commandIndex]->bBackground = false;

				//psCommands[commandIndex + 1] = NULL;
				argIndex = 0;

				// If the last token was a pipe, set input to pipe
				if (pToken && !strcmp(pToken, PIPE))
				{
					psCommands[commandIndex]->szInput = "PIPE";
				}
			}



			// First token of a new command must be the command
			if (argIndex == 0)
			{
				if (0 == commandIndex)
				{
					pToken = strtok_r(szArgs, " \n", &pSavePtr);
				}
				else
				{
					pToken = strtok_r(NULL, " \n", &pSavePtr);
				}
				argIndex++;
				psCommands[commandIndex]->szCommand = pToken;
			}
			// Tokens following the first
			else
			{
				pToken = strtok_r(NULL, " \n", &pSavePtr);
				argIndex++;

				if (pToken)
				{
					// If token is a pipe
					if (!strcmp(pToken, PIPE))
					{
						psCommands[commandIndex]->szOutput = "PIPE";
						commandIndex++;
					}
					// If token is a >
					else if (!strcmp(pToken, OUTPUT))
					{
						pToken = strtok_r(NULL, " \n", &pSavePtr);
						argIndex++;
						psCommands[commandIndex]->szOutput = pToken;
						pToken = strtok_r(NULL, " \n", &pSavePtr);
						argIndex++;
					}
					// If token is a <
					else if (!strcmp(pToken, INPUT))
					{
						pToken = strtok_r(NULL, " \n", &pSavePtr);
						argIndex++;
						psCommands[commandIndex]->szInput = pToken;
						pToken = strtok_r(NULL, " \n", &pSavePtr);
						argIndex++;
					}

					// If token is an argument for the command
					if (pToken && strcmp(pToken, END_COMMAND) &&
							strcmp(pToken, PIPE) && strcmp(pToken, BACKGROUND))
					{
						if (!psCommands[commandIndex]->szArguments[0])
						{
							psCommands[commandIndex]->szArguments[0] = pToken;
						}
						else
						{
							psCommands[commandIndex]->szArguments[argIndex - 2] = pToken;
						}
					}
					// If token is an &
					else if (pToken && !strcmp(pToken, BACKGROUND))
					{
						psCommands[commandIndex]->bBackground = true;
						//commandIndex++;
						pToken = strtok_r(NULL, " \n", &pSavePtr);
						argIndex++;
					}
					// If token was a ;
					else if (pToken && !strcmp(pToken, END_COMMAND))
					{
						commandIndex++;
					}
				}
			}
		}while (pToken);

		// Print Commands
		if (strcmp(szArgsBackup, EXIT))
		{
			for (int i = 0; i < commandIndex + 1; i++)
			{
				// command
				printf("command: %s\n", psCommands[i]->szCommand);

				// arguments
				if (psCommands[i]->szArguments[0])
				{
					printf("\targuments:");
					for (int j = 0; psCommands[i]->szArguments[j]; j++)
					{
						printf(" %s", psCommands[i]->szArguments[j]);
					}
					printf("\n");
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
				if ( !(i < commandIndex && psCommands[i]->szOutput &&
						!strcmp(psCommands[i]->szOutput, psCommands[i + 1]->szInput))
						|| i == commandIndex)
				{
					if (psCommands[i]->bBackground)
					{
						printf("background: YES\n");
					}
					else
					{
						printf("background: no\n");
					}
				}
			}
		}

		// Free Commands
		for (int i = 0; i < commandIndex + 1; i++)
		{
			free(psCommands[i]->szArguments);
			memset(psCommands[i], '\0', sizeof(Command));
			free(psCommands[i]);
			psCommands[i] = NULL;
		}
		pToken = NULL;
		argIndex = 0;
		commandIndex = 0;

	}while (strcmp(szArgsBackup, EXIT));

	free(szArgs);
	free(szArgsBackup);
	free(psCommands);

	return 0;
}
