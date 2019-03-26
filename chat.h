#ifndef __CHAT_H
#define __CHAT_H
#include <string>
#include <ostream>
#include <istream>

class Chat {
  public:
	  std::string chat_id;
	  std::string chat_history;
	  std::string chat_owner;
	  std::string chat_time;
  private:
	  Chat();
	  void edit_chat(std::string message);
	  void delete_chat(std::string message);
	  std::string search_chat(std::string message);
	  void add_users(std::string user_nickname, std::string chat_id);
	  void create_chat(std::string user_nickname, std::string chat_id);
	  void invite_user(std::string user_nickname, std::string chat_id);
	  void block_user(std::string user_nickname, std::string chat_id);
	  bool empty_room;
	  int _unread_counter;
	  int unread_counter(bool message_read);
};
#endif
