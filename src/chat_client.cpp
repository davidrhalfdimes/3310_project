#include "common_header.hpp"

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cerr << "Usage: chat_client <host> <port>\n";
    return 1;
  }

  try
  {
    //ASIO primary communicator
    asio::io_context io_context;
    tcp::resolver resolver(io_context);
    std::thread t([&io_context](){ io_context.run(); });

    //resolve endpoint address and port
    auto endpoints = resolver.resolve(argv[1], argv[2]);

    //init chat client
    chat_client c(io_context, endpoints);

    // ORIGINAL CODE - loop infinitely sending user input
    // char line[chat_message::max_body_length + 1];
    // while (std::cin.getline(line, chat_message::max_body_length + 1))
    // {
    //   chat_message msg;
    //   msg.body_length(std::strlen(line));
    //   std::memcpy(msg.body(), line, msg.body_length());
    //   msg.encode_header();
    //   c.write(msg);
    // }

    //exiting?
    c.close();
    t.join();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
