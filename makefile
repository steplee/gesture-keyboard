.DEFAULT_GOAL := all
FLAGS := -std=c++14 -g
LIBS := -lboost_system -lpthread
INCL := -I ./external/websocketpp/ -I ./external/json/single_include

#OTHER_SRCS := src/trie.cc src/keyboard.cc
#MAIN_SRCS := src/main.cc 
#SRCS := src/main.cc src/trie.cc src/keyboard.cc src/search.cc

%.o: src/%.cc src/%.h
	g++ -c $< $(FLAGS) $(INCL) -o $@

# Needs it's own target since it doesn't have a header (and the above rule checks for one)
main.o: src/main.cc
	g++ -c $< $(FLAGS) $(INCL) -o $@

all: main.o keyboard.o trie.o search.o
	g++ $^ -o app $(LIBS) $(INCL) $(FLAGS)


clean:
	rm -rf *.o app
