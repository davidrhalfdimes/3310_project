CXX=g++
CXXFLAGS=-I./include -Wall -O0 -g -std=c++11

COMMON_HEADER = include/chat_message.hpp \
								include/chat_client.hpp \
								include/chat_server.hpp \
								include/ncursesgui.hpp 

all: chat_server chat_client

chat_client: src/chat_client.cpp
	${CXX} -o $@ ${CXXFLAGS} $^ -lpthread -lncurses

chat_server: src/chat_server.cpp
	${CXX} -o $@ ${CXXFLAGS} $^ -lpthread -lncurses

clean:
	-rm -rf chat_client chat_server chat_client.o chat_server.o chat_client.dSYm chat_server.dSYM

