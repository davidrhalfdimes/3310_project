CXX = g++
CXXFLAGS = -Wall -O0 -g -std=c++11 \
					 -I./include
					 
all: chat_client chat_server

chat_client:
	${CXX} ${CXXFLAGS} -o chat_client src/chat_client.cpp -lpthread -lncurses

chat_server:
	${CXX} ${CXXFLAGS} -o chat_server src/chat_server.cpp -lpthread

clean:
	rm -f chat_client chat_server chat_client.o chat_server.o

