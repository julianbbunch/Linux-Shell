#****************************************************
# File name:  Makefile
# Author:     Julian Bunch
# Date:       03/01/2020
# Class:      CS460
# Assignment: Shell
#****************************************************

CC = gcc
CFLAGS = -g -Wall
SHELL_OBJECTS = bin/CS460_Shell.o
ALL_OBJECTS = ${SHELL_OBJECTS}

.PHONY: all clean tarball

all: bin bin/CS460_Shell

bin:
	mkdir -p bin
	
bin/CS460_Shell: ${SHELL_OBJECTS}
	${CC} ${CFLAGS} -o bin/CS460_Shell ${SHELL_OBJECTS}

bin/CS460_Shell.o:	src/CS460_Shell.c
	${CC} ${CFLAGS} -o bin/CS460_Shell.o -c src/CS460_Shell.c
	
valgrind:
	valgrind -v --leak-check=yes --track-origins=yes --leak-check=full --show-leak-kinds=all bin/CS460_Shell
	
debug:
	bin/CS460_Shell -d

valgrind_debug:
	valgrind -v --leak-check=yes --track-origins=yes --leak-check=full --show-leak-kinds=all bin/CS460_Shell -d

runTest:
	bin/CS460_Shell

clean:
	rm -f bin/CS460_Shell ${ALL_OBJECTS}

tarball: clean
	tar czf ../CS460_Shell.tar.gz CS460_Shell_bunc8372
	