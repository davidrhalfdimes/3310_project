CXX=g++

CPPFLAGS=-I/usr/local/asio-1.12.2/include 

CXXFLAGS=-Wall -O0 -g -std=c++11

all: chat_client chat_server

COMMON_HEADER = chat_message.hpp

#~~Adding client class: put .cpp in next line
chat_client.o: ${COMMON_HEADER} chat_client.cpp DBMS.cpp role.cpp users.cpp ncurses.cpp

#~~Adding client class: put .o in next two lines
chat_client:chat_client.o DBMS.o role.o users.o ncurses.o
	${CXX} -o chat_client ncurses.o chat_client.o DBMS.o role.o users.o -lpthread -lncurses

chat_server.o: ${COMMON_HEADER} chat_server.cpp

#~~Adding server class: put .o in next two lines
chat_server:chat_server.o DBMS.o users.o
	${CXX} -o chat_server chat_server.o DBMS.o users.o -lpthread

#~~Adding any class: follow this formula below
DBMS.cpp:${COMMON_HEADER} DBMS.h 

role.cpp:${COMMON_HEADER} role.h

users.cpp:${COMMON_HEADER} users.h

ncurses.cpp:${COMMON_HEADER} ncurses.h

#~~Adding any class: so that the 'make clean' command works properly, put .o in next line
clean:
	-rm -f chat_client chat_server chat_client.o chat_server.o DBMS.o role.o users.o ncurses.o


