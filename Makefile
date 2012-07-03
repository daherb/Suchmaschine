CC=g++
CFLAGS=-std=c++11 -Wall
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
trainer: LanguageTrainer.cpp Language.hpp
	$(CC) $(CFLAGS) -o Trainer LanguageTrainer.cpp
