CC=g++
CFLAGS=-std=c++11 -Wall
OUTFILE=main
ifdef CLANG
CC=clang
OUTFILE=main_clang
endif
# for debugging
ifdef DEBUG
CFLAGS+=-ggdb -DDEBUG
endif

all: main
main: Main.cpp *.hpp
	$(CC) $(CFLAGS) -o $(OUTFILE) Main.cpp
