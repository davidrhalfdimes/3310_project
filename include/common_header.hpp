#ifndef COMMON_HEADER
#define COMMON_HEADER

//standard headers
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <iostream>
#include <thread>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <vector>

//ASIO header
#include "asio.hpp"

//UUID generation headers
//this is meant to identify who sent and who receive 
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

//programmer created headers
#include "chat_message.hpp"
#include "chat_client.hpp"
#include "chat_server.hpp"

#endif
