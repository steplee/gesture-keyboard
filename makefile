
FLAGS := -std=c++14 -g
LIBS := -lboost_system -lpthread
INCL := -I ./external/websocketpp/ -I ./external/json/single_include

APP_SRCS := src/main.cc src/trie.cc src/keyboard.cc


native_app:
	g++ $(APP_SRCS) -o app $(FLAGS) $(LIBS) $(INCL)
