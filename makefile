
FLAGS := -std=c++14 -g
LIBS := -lboost_system -lpthread
INCL := -I ./external/websocketpp/ -I ./external/json/single_include

OTHER_SRCS := src/trie.cc src/keyboard.cc
MAIN_SRCS := src/main.cc 
SRCS := src/main.cc src/trie.cc src/keyboard.cc src/search.cc

%.o: src/%.cc src/%.h
	g++ -c $< $(FLAGS) $(INCL) -o $@

app: main.o keyboard.o trie.o search.o
	g++ *.o -o app $(LIBS) $(INCL) $(FLAGS)


clean:
	rm -rf *.o app
