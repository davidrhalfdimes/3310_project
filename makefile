CXX=g++

CPPFLAGS=-I/usr/local/asio-1.12.2/include -I./

CXXFLAGS=-Wall -O0 -g -std=c++11

all: chat_client chat_server

COMMON_HEADER = chat_message.hpp

chat_client.o: ${COMMON_HEADER} chat_client.cpp DBMS.cpp #added here bc client class

chat_client:chat_client.o DBMS.o
	${CXX} -o chat_client chat_client.o DBMS.o -lpthread -lncurses

chat_server.o: ${COMMON_HEADER} chat_message.hpp chat_server.cpp

chat_server:chat_server.o DBMS.o
	${CXX} -o chat_server chat_server.o DBMS.o -lpthread

DBMS.cpp:${COMMON_HEADER} DBMS.h #some error here

clean:
	-rm -f chat_client chat_server chat_client.o chat_server.o DBMS.o

