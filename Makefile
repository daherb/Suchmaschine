CC=g++
CFLAGS=-std=c++11
OUTFILE=main
ifdef CLANG
CC=clang
OUTFILE=main_clang
endif
# for debugging
ifdef DEBUG
CFLAGS+=-ggdb -DDEBUG -Wall
endif

all: main
main: Main.cpp *.hpp
	$(CC) $(CFLAGS) -o $(OUTFILE) Main.cpp
