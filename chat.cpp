#include "chat.h"

void Chat::edit_chat(std::string message)
{
	return;
}

void Chat::delete_chat(std::string message)
{
	return;
}

std::string Chat::search_chat(std::string message)
{
	return;
}

void Chat::add_users(std::string user_nickname, std::string chat_id)
{
	return;
}

void Chat::create_chat(std::string user_nickname, std::string chat_id)
{
	return;
}

void Chat::invite_user(std::string user_nickname, std::string chat_id)
{
	return;
}

void Chat::block_user(std::string user_nickname, std::string chat_id)
{
	return;
}

int Chat::unread_counter(bool message_read)
{
	if (message_read == true)
	{
		return _unread_counter += 1;
	}
}