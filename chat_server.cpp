//
// chat_server.cpp
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
#include <list>
#include <memory>
#include <set>
#include <utility>
#include "asio.hpp"
#include "chat_message.hpp"
//#include "ncurses.hpp"

/*
#include<string.h>
#include<fctn1.h>
#include<uninstd.h>
#include<netinet/in.h>
#include<sys/socket.h>
*/
using asio::ip::tcp;

//----------------------------------------------------------------------

typedef std::deque<chat_message> chat_message_queue;

//----------------------------------------------------------------------

class chat_participant{ 
  public:
    std::string name, address;

    std::string getname()
    {
      return name;
    }

    std::string getaddress()
    {
      return address;
    }

    virtual ~chat_participant() {}
    virtual void deliver(const chat_message& msg) = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

//----------------------------------------------------------------------

class chat_room
{
  public:
    void join(chat_participant_ptr participant)
    {
      std::cout << "\nUser " << participant->getname() << " at address " << participant->getaddress() << " has joined." << " \nWaiting for new port: ";
      std::cout.flush();
//      std::cout << "\nWaiting for new port:"; 
      participants_.insert(participant);
      for (auto msg: recent_msgs_)
        participant->deliver(msg);
    }

    void leave(chat_participant_ptr participant)
    {
      std::cout << "\nUser " << participant->getname() << " at address " << participant->getaddress() << " has left" << " \nWaiting for new port: ";
      std::cout.flush();
      participants_.erase(participant);
    }

    void deliver(const chat_message& msg)
    {
      recent_msgs_.push_back(msg);
      while (recent_msgs_.size() > max_recent_msgs)
        recent_msgs_.pop_front();

      for (auto participant: participants_)
        participant->deliver(msg);

    }

  private:
    std::set<chat_participant_ptr> participants_; //user class will use this. list of participants in a group chat.
    enum { max_recent_msgs = 100 };
    chat_message_queue recent_msgs_; //need queue for chat queue. chat_queue
};

//----------------------------------------------------------------------

class chat_session
  : public chat_participant,
    public std::enable_shared_from_this<chat_session>
{
public:
  chat_session(tcp::socket socket, chat_room& room)
    : socket_(std::move(socket)),
      room_(room)
  {
    name = "Test"; //need to obtain username from user
    address = socket_.remote_endpoint().address().to_string();
  }

  void start()
  {
    room_.join(shared_from_this());
    do_read_header();
  }

  void deliver(const chat_message& msg)
  {
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress)
    {
      do_write();
    }
  }

private:
  void do_read_header()
  {
    auto self(shared_from_this());
    asio::async_read(socket_,
        asio::buffer(read_msg_.data(), chat_message::header_length),
        [this, self](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec && read_msg_.decode_header())
          {
            do_read_body();
          }
          else
          {
            room_.leave(shared_from_this());
          }
        });
  }

  void do_read_body()
  {
    auto self(shared_from_this());
    asio::async_read(socket_,
        asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this, self](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            // std::string message_content( read_msg_.body() );
            // if ( message_content.find( "ANNOUNCEMENT:" ) != std::string::npos )
            // {
            //   std::cout << message_content << std::endl;
            // }
            // else
            // {
            //   room_.deliver(read_msg_);
            // }
            room_.deliver( read_msg_ );
            do_read_header();
          }
          else
          {
            room_.leave(shared_from_this());
          }
        });
  }

  void do_write()
  {
    auto self(shared_from_this());
    asio::async_write(socket_,
        asio::buffer(write_msgs_.front().data(),
          write_msgs_.front().length()),
        [this, self](std::error_code ec, std::size_t /*length*/)
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
            room_.leave(shared_from_this());
          }
        });
  }

  tcp::socket socket_;
  chat_room& room_;
  chat_message read_msg_;
  chat_message_queue write_msgs_;
};

//----------------------------------------------------------

class chat_server
{
	
public:
  chat_server(asio::io_context& io_context,
      const tcp::endpoint& endpoint)
    : acceptor_(io_context, endpoint)
  {
    do_accept();
  }

private:
  void do_accept()
  {
    acceptor_.async_accept(
        [this](std::error_code ec, tcp::socket socket)
        {
          if (!ec)
          {
            std::make_shared<chat_session>(std::move(socket), room_)->start();
          }

          do_accept();
        });
  }

  tcp::acceptor acceptor_;
  chat_room room_;
};

//----------------------------------------------------------------------

asio::io_context io_context;
std::list<chat_server> servers;

void server_runner( int argc, char*argv[])
{
	try
	{
		if(argc < 2)
		{
			std::cerr << "Usage: chat_server <port> [<port> ...]\n";
			std::exit(1);
		}

		for(int i = 1; i < argc; ++i)
		{
			tcp::endpoint endpoint( tcp::v4(), std::atoi(argv[i]));
			servers.emplace_back(io_context,endpoint);
		}

		io_context.run();
	}

	catch(std::exception& e)
	{
		std::cerr << "exception: " << e.what() << "\n";
	}
}

void server_spawner()
{
	while(true)
	{
		std::string newport;
		std::cout << "Waiting for new port: ";
		std::getline(std::cin,newport);
		
		try
		{
			tcp::endpoint endpoint(tcp::v4(),std::atoi(newport.c_str() ) );
			servers.emplace_back(io_context,endpoint);
		}
		
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
			continue;
		}
	}
}


int main(int argc, char* argv[])
{
	std::thread asio_runner(server_runner, argc, argv);
	std::thread asio_spawner(server_spawner);
	
	asio_runner.join();
	asio_spawner.detach();
	
	
/*  try
  {
    if (argc < 2)
    {
      std::cerr << "Usage: chat_server <port> [<port> ...]\n";
      return 1;
    }

    asio::io_context io_context;

    std::list<chat_server> servers;
    for (int i = 1; i < argc; ++i)
    {
      tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
      servers.emplace_back(io_context, endpoint);
    }

    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  */


/*
  //implementation of the file transfer
  //code for ftp in server side
  int file_handler1;
  int file_handler2;

  int binder;

  //hear variable to listen to socket
  int hear;
  int stream;

  //array created for storing the text file's content
  char file_array[300]={' '};  

  
  struct sockaddr_in server;
  struct sockaddr_in client;
  
  //using the socket command to create socket for file1
  file_handler1 =socket(AF_INET,SOCK_STREAM,0);
  if(file_handler1<0){
    cout<<"Socket Creation Error: Connection unable to be established.\n";
    cout<<"Please try again\n";
    return 0;
  }

  cout<<"Successful creation of SOCKET!\n";

  server.sin_family=AF_INET;
  server.sin_port=htons(PORT);
  server.sin_addr.s_addr=INADDR_ANY;
  
  
  binder = bind(file_hander1 ,(struct sockaddr *)&server,sizeof(server));
  if(binder <0){
    cout<<"ERROR\n";
    return 0;
  }
  
  hear  =listen(file_hander1,BACKLOG);
  if(hear<0){
    cout<<"Error listening\n";
    return 0;
  }
  
  socklen_t file_socket_length =sizeof(client);

  file_handler2=accept(file_handler1,(struct sockaddr*)&client,&file_socket_length);
  if(file_handler2<0){
    cout<<"Unable to establish connection with second file handler\n";
    return 0;
  }

  int source; //from  == source
  source =open("sample_server.txt",O_RDONLY);
  if(source<0){
	cout<<"Text file ran into errors when attempting to open\n";
    return 0;
  }

  int counter =1 ;
  while((counter = read(source,file_array,sizeof(file_array)))!=0){
    //s=send(fd2,buf,sizeof(buf),0);
    stream =write(file_handler2,file_array,counter);
    if(stream<0)
    {
    	cout<<"error sending\n";
    	return 0;
    }
  }

  //closing both file handlers
  close(file_handler1);
  close(file_handler2);

  //shutting both down
  shutdown(file_handler1,0);
  shutdown(file_handler2,0);


  */
   return 0;
}
