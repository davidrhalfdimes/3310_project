//
// chat_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//


#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include "asio.hpp"
#include "chat_message.hpp"
#include <ncurses.h>
#include "ncurses.h"
#include "ncurses.hpp"
#include <fstream> 


/*
#include<string.h>
#include<fctn1.h>
#include<uninstd.h>
#include<netinet/in.h>
#include<sys/socket.h>
*/

using asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;//queue for chat messages. also need to create queue for group chats(chat rooms) chat_room_queue for example. implement that in users class(?)

int yMax,xMax; //obtain maximum values 
int wmh_height, wmh_width;

const std::string make_timestamp()
{
	time_t current_time = time(NULL);
	std::tm* now = std::localtime(&current_time);
	char buf[80];
	strftime(buf,sizeof(buf),"[%Y-%m-%d at %X]",now);
	return buf;
}

void display_user_list()
{
	
}



class chat_client
{
public:
  chat_client(asio::io_context& io_context,
      const tcp::resolver::results_type& endpoints)
    : io_context_(io_context),
      socket_(io_context)
  {
    do_connect(endpoints);
  }

  void write(const chat_message& msg)
  {
    asio::post(io_context_,
        [this, msg]()
        {
          bool write_in_progress = !write_msgs_.empty();
          write_msgs_.push_back(msg);
          if (!write_in_progress)
          {
            do_write();
          }
        });
  }

  void close()
  {
    asio::post(io_context_, [this]() { socket_.close(); });
  }

  void changeport(const tcp::resolver::results_type& endpoints)
  {
    do_connect(endpoints);
  }

private:
  void do_connect(const tcp::resolver::results_type& endpoints)
  {
    asio::async_connect(socket_, endpoints,
        [this](std::error_code ec, tcp::endpoint)
        {
          if (!ec)
          {
            do_read_header();
          }
        });
  }

  void do_read_header()
  {
    asio::async_read(socket_,
        asio::buffer(read_msg_.data(), chat_message::header_length),
        [this](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec && read_msg_.decode_header())
          {
            do_read_body();
          }
          else
          {
            socket_.close();
          }
        });
  }

void do_read_body() //similar implementation to chat_client.cpp note differences
{
	asio::async_read(socket_,
        asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this](std::error_code ec, std::size_t /*length*/)
	{
        	if (!ec)
        	{
	/*		int i;
			for(i=current_line - 49*yMax/64; i<current_line ;i++)
			{
				mvwaddstr(win_message_history,i,1,chat_message_log[i]); //window, Y value, X value, char
			}
	*/
	    		char buff[read_msg_.body_length()+1];
			strncpy(buff,read_msg_.body(),read_msg_.body_length());
			buff[read_msg_.body_length()] = '\0';

			safety_lock.lock(); 

			current_line++; 
	
			if(current_line>=wmh_height-1) //current_line exceed win_message_history height
			{				
				wscrl(win_message_history,1);
				refresh();
				wrefresh(win_message_history);
				current_line--;
			}

			mvwaddstr(win_message_history,current_line,0,buff); //window, Y, X, char

			if(current_line>=wmh_height-1) //current_line exceed win_message_history height
			{				
				if((int)strlen(buff) > (wmh_width)) // message length is 2 lines
				{
					current_line--;	
				}

				if((int)strlen(buff) > 2*(wmh_width)) // message length is 3 lines
				{
					current_line--;	
				}
			}

			if((int)strlen(buff) > (wmh_width)) // message length is 2 lines
			{
				current_line++;
			}

			if((int)strlen(buff) > 2*(wmh_width)) // message length is 3 lines
			{
				current_line++;
			}

			//message limit is 135 characters, so additional cases not needed

			wrefresh(win_message_history);

			safety_lock.unlock();  

			do_read_header();
          	}
          	else
          	{
            		socket_.close();
          	}
        });
  }

  void do_write() //similar implementation to chat_server.cpp note differences
  {
    asio::async_write(socket_,
        asio::buffer(write_msgs_.front().data(),
          write_msgs_.front().length()),
        [this](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            write_msgs_.pop_front();
            if (!write_msgs_.empty())
            {
              do_write();
            }
          }
          else
          {
            socket_.close();
          }
        });
  }

private:
  asio::io_context& io_context_;
  tcp::socket socket_;
  chat_message read_msg_;
  chat_message_queue write_msgs_;
};

int main(int argc, char* argv[])
{ 	
	try
  	{
    		if (argc != 3)
    		{
      			std::cerr << "Usage: chat_client <host> <port>\n";
      			return 1;
   		}
		initscr();
		getmaxyx(stdscr,yMax,xMax);
		wmh_height = 49*yMax/64; //win_message_history height
		wmh_width = 5*xMax/8; //win_message_history width	
		
		std::string timestamp,username,content; //content: actual message that user types

		welcome_draw();
		username = login_screen(); 
		lobby_draw();
		//group_screen_draw();

		asio::io_context io_context;

		//this one is current client that will receive message
		tcp::resolver resolver(io_context); //instead of io_context, need class that will contain endpoints of each user and pass that member as an argument into this function as many times as necessary
		//need as many io_context's as we have # of groups
		//maybe create loop that keeps passing in endpoint until i>endpoint
		    
		//how many groups there are: create that many threads
		//if that thread is already created, don't create another. this will be managed by the static queue_groups and vector bool<> members in userse class
		    
		//this one is for server IP and port: 127.0.0.1 9000, for example
		auto endpoints = resolver.resolve(argv[1], argv[2]);

		chat_client c(io_context, endpoints);

		//call upon user class function: if user is part of 0 group, use that thread to create new thread. dequeue the group when the group needs to be deleted.
		//how are we storing the data?? - 
		std::thread t([&io_context](){ io_context.run(); });
	
/*		while (std::cin.getline(line, chat_message::max_body_length + 1)) //this entire function different
  	 	{
		chat_message msg;
		msg.body_length(std::strlen(line));
		std::memcpy(msg.body(), line, msg.body_length());
		msg.encode_header();
		c.write(msg);
    		}
*/

		chat_message announcement;
		char ann_content[chat_message::max_body_length + 1];
		std::memset(ann_content,'\0', chat_message::max_body_length+1);
		strcpy( ann_content, "ANNOUNCEMENT: User " );
		strcat( ann_content, username.substr(0, username.find(':')).c_str() );
		std::string u_name = username.substr(0, username.find(':')).c_str();


		
		strcat( ann_content, " has joined." );
		announcement.body_length( std::strlen( ann_content ) );
		std::memcpy( announcement.body(), ann_content, announcement.body_length() );
		announcement.encode_header();
		c.write( announcement );


		std::fstream file;
		file.open("debug.txt",std::ios::app);
		file << u_name<<"\n";
		file.close();


		while(true)
		{
			char line[chat_message::max_body_length + 1];
			std::memset(line,'\0', chat_message::max_body_length+1);

			content.clear();
			content = getInput(win_message); 

			if(content.find("/quit") != std::string::npos)
			{
				endwin();
				std::exit(0);
			}
		
			if(content.find("/switch") != std::string::npos)
			{
				c.close();
				std::string portnumber;
				mvwaddstr(win_message,2,1,"Enter port number:\n|"); //window, Y, X, char
				portnumber = getInput(win_message);
				endpoints = resolver.resolve(argv[1], portnumber.c_str() );
				io_context.stop();
				io_context.reset();
				c.changeport(endpoints);
				io_context.run();
			}
	
			else
			{
				timestamp = make_timestamp(); //this function at top of program

				strcpy(line,timestamp.c_str());
				strcat(line,username.c_str()); //return value of login_screen
				strcat(line,content.c_str());

				chat_message msg;

				msg.body_length(std::strlen(line));
				std::memcpy(msg.body(),line,msg.body_length());
				msg.encode_header();
				c.write(msg);

				safety_lock.lock();
				//lobby_draw();
				refresh_win_message_history();
				refresh_win_message();
				safety_lock.unlock();
   			}
		}

		endwin(); //ncurses cleanup

		c.close();
		t.join();
  	}
  
	catch (std::exception& e)
  	{
    		std::cerr << "Exception: " << e.what() << "\n";
  	}

  	/*

  //implementation of the file transfer
  //code for ftp in client side

  //creating variable as bool for file handler
  int file_handler;

  struct sockaddr_in server;
  struct hostent *host_party; //hp = host_party

  //variable for counter and file array
  int counter; //cnct == counter
  char file_array[300]= {' '};

  server.sin_family=AF_INET;
  server.sin_port=htons(PORT);
  server.sin_addr.s_addr=INADDR_ANY;

  //using the socket command to create socket for file
  file_handler=socket(AF_INET,SOCK_STREAM,0);
  if(file_handler<0){
    cout<<"Socket Creation Error: Connection unable to be established.\n";
    cout<<"Please try again\n";
    return 0;
  }

 cout<<"Successful creation of SOCKET!\n";

  host_party=gethostbyname(argv[1]);
  bcopy((char *)host_party->h_addr,(char *)&server.sin_addr.s_addr,host_party->h_length);

  //storing the value of the file handler when connection is attempted to the server
  counter=connect(file_handler,(struct sockaddr*)&server,sizeof(server));

  //error message
  if(counter<0){
    cout<<"ERROR\n";
    return 0;
  }
  
  //variables created as booleans for the storing position in the file
  //and the destination itself
  int record;
  int destination;

  destination=create("sample_client.txt",0777);
  if(destination<0){
    cout<<"Error creating file\n";
    return 0;
  }

  //attempting to receive the file passed in
  int write;
  while(record=recv(file_handler,file_array,sizeof(file_array),0)){
    if(record<0){
      cout<<"ERROR: File not received\n";
      return 0;
    }
    //if the program doesn't run into errors
    write=write(destination,file_array,record);
  }

  //closing and shutting down the file handler
  close(file_handler);
  shutdown(file_handler,0);

  */
	return 0;
}
