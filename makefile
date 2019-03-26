CXX=g++

CPPFLAGS=-I/usr/local/asio-1.12.2/include -I./

CXXFLAGS=-Wall -O0 -g -std=c++11

all: chat_client chat_server

COMMON_HEADER = chat_message.hpp

chat_client.o: ${COMMON_HEADER} chat_client.cpp DBMS.cpp #client .cpp files go here

chat_client:chat_client.o DBMS.o #client .o files go here
	${CXX} -o chat_client chat_client.o DBMS.o -lpthread -lncurses

chat_server.o: ${COMMON_HEADER} chat_message.hpp chat_server.cpp

chat_server:chat_server.o DBMS.o #server .o files go here
	${CXX} -o chat_server chat_server.o DBMS.o -lpthread

#to add more stubs (skeletons) of classes from class diagram, follow this format:
DBMS.cpp:${COMMON_HEADER} DBMS.h 

#also add the .o file here so that the 'make clean' command cleans out all necessary files
clean:
	-rm -f chat_client chat_server chat_client.o chat_server.o DBMS.o


