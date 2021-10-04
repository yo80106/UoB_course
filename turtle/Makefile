CC = gcc
COMMON = -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -Werror
SDLCFLAGS = `sdl2-config --cflags`
SDLLIBS = `sdl2-config --libs`
DEBUG = -g3
SANITIZE = $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND = $(COMMON) $(DEBUG)
PRODUCTION = $(COMMON) -O3
LDLIBS = -lm

all: parse interp extension

# For making parser
parse: parser.c parser_test.c parser.h
	$(CC) parser.c parser_test.c -o parser $(COMMON) $(SDLCFLAGS) $(SDLLIBS) $(LDLIBS)

# For making interpreter
interp: interpreter.c interpreter_test.c interpreter.h
	$(CC) interpreter.c neillsdl2.c interpreter_test.c -o interpreter $(COMMON) $(SDLCFLAGS) $(SDLLIBS) $(LDLIBS)

# For making extension
extension: extension.c extension_test.c extension.h
	$(CC) extension.c neillsdl2.c extension_test.c -o extension $(COMMON) $(SDLCFLAGS) $(SDLLIBS) $(LDLIBS)