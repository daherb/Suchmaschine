CC=g++
# For GCC 4.7
#CFLAGS=-std=c++11 -Wall
# For GCC 4.6
CFLAGS=-std=c++0x -Wall
ifdef CLANG
CC=clang
OUTFILE=main_clang
endif
# for debugging
ifdef DEBUG
CFLAGS+=-ggdb -DDEBUG
endif

all: trainer searcher
searcher: Searcher.cpp *.hpp
	$(CC) $(CFLAGS) -o Searcher Searcher.cpp
trainer: LanguageTrainer.cpp LanguageRecognizer.hpp
	$(CC) $(CFLAGS) -o Trainer LanguageTrainer.cpp
