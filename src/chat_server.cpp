#include "common_header.hpp"

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "Usage: chat_server <port> [<port> ...]\n";
    return 1;
  }

  try
  {
    asio::io_context io_context;
    std::list<chat_server> servers;

    // ORIGINAL CODE - spawn server for all port and set on message bouncing
    // for (int i = 1; i < argc; ++i)
    // {
    //   tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
    //   servers.emplace_back(io_context, endpoint);
    // }

    // io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
