//----------------------------------------------------------------------
// Standard/ASIO/Boost Headers and Namespaces

#include <ctime>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <asio.hpp>
#include <chat_message.hpp>
#include <gui.hpp>

using asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

//----------------------------------------------------------------------
// Utility Functions


//make time stamp to print
const std::string make_timestamp()
{
  time_t current_time = time(NULL);
  std::tm* now = std::localtime( &current_time );
  char buf[80];

  //string format time, return actual date time
  strftime( buf, sizeof(buf), "[%Y-%m-%d at %X]", now );
  return buf;
}

//----------------------------------------------------------------------
// Class Definition

class chat_client
{
  public:
    chat_client(  asio::io_context& io_context, 
                  const tcp::resolver::results_type& endpoints ) 
                  : io_context_( io_context ), socket_( io_context )
    {
      do_connect( endpoints );
    }

    void write( const chat_message& msg )
    {
      asio::post( io_context_,
                  [this, msg]()
                  {
                    //C++ Lambda
                    bool write_in_progress = !write_msgs_.empty();
                    write_msgs_.push_back(msg);
                    if ( !write_in_progress )
                    {
                      do_write();
                    }
                  }
                );
    }

    void close()
    {
      asio::post( io_context_, [this]() { socket_.close(); } );
    }

  private:
    void do_connect( const tcp::resolver::results_type& endpoints )
    {
      asio::async_connect(  socket_, 
                            endpoints,
                            [this]( std::error_code ec, tcp::endpoint )
                            {
                              if ( !ec )
                              {
                                do_read_header();
                              }
                            }
                          );
    }

    void do_read_header()
    {
      asio::async_read( socket_,
                        asio::buffer( read_msg_.data(), chat_message::header_length ),
                        [this]( std::error_code ec, std::size_t /*length*/ )
                        {
                          if ( !ec && read_msg_.decode_header() )
                          {
                            do_read_body();
                          }
                          else
                          {
                            socket_.close();
                          }
                        }
                      );
    }

    void do_read_body()
    {
      asio::async_read( socket_,
                        asio::buffer( read_msg_.body(), read_msg_.body_length() ),
                        [this]( std::error_code ec, std::size_t /*length*/ )
                        {
                          if (!ec)
                          {
                            // This is where message gets printed receiving from server
                            // Handle with ncurses
                            std::cout.write( read_msg_.body(), read_msg_.body_length() );
                            std::cout << "\n";
                            // mvwprintw( display_box, current_line, 1, read_msg_.body() );
                            // current_line++;
                            // wrefresh( display_box );
                            do_read_header();
                          }
                          else
                          {
                            socket_.close();
                          }
                        }
                      );
    }

    void do_write()
    {
      asio::async_write(  socket_,
                          asio::buffer( write_msgs_.front().data(), write_msgs_.front().length() ),
                          [this]( std::error_code ec, std::size_t /*length*/ )
                          {
                            if ( !ec )
                            {
                              write_msgs_.pop_front();
                              if ( !write_msgs_.empty() )
                              {
                                do_write();
                              }
                            }
                            else
                            {
                              socket_.close();
                            }
                          }
                        );
    }

  //private variables
  private:
    asio::io_context& io_context_;
    tcp::socket socket_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
};

//----------------------------------------------------------------------
// Program Main

int main( int argc, char* argv[] )
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: chat_client <host> <port>\n";
      return 1;
    }

    std::cout << "Enter username: ";
    std::string timestamp, username, content;
    std::getline( std::cin, username );
    username += ": ";

    //gui_init();

    asio::io_context io_context;

    tcp::resolver resolver( io_context );
    auto endpoints = resolver.resolve( argv[1], argv[2] );
    chat_client c( io_context, endpoints );

    std::thread t( [&io_context](){ io_context.run(); } );

    char line[chat_message::max_body_length + 1];
    
    //while ( std::cin.getline(line, chat_message::max_body_length + 1) )
    while ( std::getline( std::cin, content ) )
    {
      std::string timestamp = make_timestamp();
      strcpy( line, timestamp.c_str() );
      strcat( line, username.c_str() );
      strcat( line, content.c_str() );

      chat_message msg;

      //set length of the body 
      msg.body_length(std::strlen(line));

      //memcopy to display the line displayed 
      std::memcpy(msg.body(), line, msg.body_length());
      msg.encode_header();
      c.write(msg);
    }


  //   while ( true )
  //   {
  //     content.clear();
  //     content = get_string_from_window( message_box, maxx-2 );

  //     char line[ chat_message::max_body_length + 1 ];
  //     std::memset( line, '\0', chat_message::max_body_length + 1 );

  //     timestamp = make_timestamp();

  //     int provided = timestamp.length() + username.length() + content.length() + 1;

  //     strcpy( line, timestamp.c_str() );
  //     strcat( line, username.c_str() );
  //     strcat( line, content.c_str() );
  //     std::memset( line + provided, '\0', chat_message::max_body_length - provided); 

  //     chat_message msg;

  //     msg.body_length( std::strlen( line ) );
  //     std::memcpy( msg.body(), line, msg.body_length() );
  //     msg.encode_header();
  //     c.write( msg );

  //     safety_lock.lock();
  //     make_display_box();
  //     make_message_box();
  //     safety_lock.unlock();
  //   }

    c.close();
    t.join();

  }
  catch ( std::exception& e )
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

//----------------------------------------------------------------------