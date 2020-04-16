#****************************************************
# File name:  Makefile
# Author:     Julian Bunch
# Date:       03/01/2020
# Class:      CS460
# Assignment: Shell
#****************************************************

CC = gcc
CFLAGS = -g -Wall
OBJECTS = bin/CS460_Shell.o \
	bin/commandline.o \
	bin/command.o

.PHONY: all clean tarball

all: bin/CS460_Shell

bin:
	mkdir -p bin
	
bin/command.o: src/command.c include/command.h
	${CC} ${CFLAGS} -o bin/command.o -c src/command.c
	
bin/commandline.o: src/commandline.c include/commandline.h \
						include/command.h
	${CC} ${CFLAGS} -o bin/commandline.o -c src/commandline.c
	
bin/CS460_Shell: ${OBJECTS}
	${CC} ${CFLAGS} -o bin/CS460_Shell ${OBJECTS}

bin/CS460_Shell.o:	src/CS460_Shell.c \
						include/commandline.h \
						include/command.h
	${CC} ${CFLAGS} -o bin/CS460_Shell.o -c src/CS460_Shell.c
	
valgrind:	bin/CS460_Shell
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
