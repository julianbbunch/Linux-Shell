//****************************************************
// File name:  CS460_SysCalls.c
// Author:     Julian Bunch
// Date:       02/12/2020
// Class:      CS460
// Assignment: SysCalls
// Purpose:    Practice using system calls
//****************************************************

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct Command
{
	char* command;
	char* arguments;
	char* input;
	char* outout;
	bool bPipe;
	bool bBackground;

} Command;

int main(int argc, char *argv[])
{
	const size_t MAX_COMMAND_SIZE = 2048;
	const char* EXIT = "exit";

	Command* psCommands;
	int pid = getpid();
	char *szArgs = malloc(MAX_COMMAND_SIZE);

	do
	{
		memset(szArgs, '\0', MAX_COMMAND_SIZE);

		printf("%d>", pid);
		getline(&szArgs, (size_t*)&MAX_COMMAND_SIZE, stdin);
		printf("%s", szArgs);


	}while (strcmp(szArgs, EXIT) != 0);

	free(szArgs);

	return 0;
}
