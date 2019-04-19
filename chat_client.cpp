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

using asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;//queue for chat messages. also need to create queue for group chats(chat rooms) chat_room_queue for example. implement that in users class(?)

int yMax,xMax; //obtain maximum values to be able to check if current_line exceeds height

const std::string make_timestamp()
{
	time_t current_time = time(NULL);
	std::tm* now = std::localtime(&current_time);
	char buf[80];
	strftime(buf,sizeof(buf),"[%Y-%m-%d at %X]",now);
	return buf;
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

	    char buff[read_msg_.body_length()+1];
	    strncpy(buff,read_msg_.body(),read_msg_.body_length());
	    buff[read_msg_.body_length()] = '\0';

	    safety_lock.lock(); 
	    current_line++; //check: does 'buff'(which has timestamp,username,content) length exceed width of win_message_history? if so, increment current_line appropriately
	    /*
	     *if(buff.length() % (49*yMax/64))
	     {
	     	increment current_line appropriately
	     }
	     */
	    mvwprintw(win_message_history,current_line,1,buff);
	    wrefresh(win_message_history);
	    safety_lock.unlock();  

           // std::cout.write(read_msg_.body(), read_msg_.body_length());
           // std::cout << "\n";
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

/*void chat_function(chat_client *c,Ncurses obj)
{
    //char line[chat_message::max_body_length + 1]; //this would be reading off the command line like in example

 
    while (std::cin.getline(line, chat_message::max_body_length + 1))
    {

//      refresh(); // or wrefresh?
      chat_message msg;
      msg.body_length(std::strlen(line));
      std::memcpy(msg.body(), line, msg.body_length());
      msg.encode_header();
      c->write(msg); //dereferencing c object
     
//      obj.init_draw(); //draws tryna DEBUG. probably don't need this
     // obj.lobby_draw();
      obj.group_screen_draw();
   //   obj.login_screen();
      //  getstr(str); //attempting to obtain user input
    }
}
*/

int main(int argc, char* argv[])
{ 	
	try
  	{
    		if (argc != 3)
    		{
      			std::cerr << "Usage: chat_client <host> <port>\n";
      			return 1;
   		}

		getmaxyx(stdscr,yMax,xMax);

		std::string timestamp,username,content; //content: actual message that user types

		welcome_draw();
		username = login_screen();
		//username = "Tom: ";
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
	
		//chat function(&c, NC);

		//char line[chat_message::max_body_length + 1];

		//char user_line{}; //need to determine syntax and return type of group_screen_draw() to link that function to this while loop by replacing "line" with "user_line." currently running into invalid conversion errors
		//user_line  = NC.group_screen_draw();
		
/*		while (std::cin.getline(line, chat_message::max_body_length + 1)) //this entire function different
  	 	{
		chat_message msg;
		msg.body_length(std::strlen(line));
		std::memcpy(msg.body(), line, msg.body_length());
		msg.encode_header();
		c.write(msg);
    		}
*/

		while(true)
		{
			char line[chat_message::max_body_length + 1];
			std::memset(line,'\0', chat_message::max_body_length+1);

			content.clear();
			content = getInput(win_message); 

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

		endwin(); //ncurses cleanup

		c.close();
		t.join();
  	}
  
	catch (std::exception& e)
  	{
    		std::cerr << "Exception: " << e.what() << "\n";
  	}


	return 0;
}
