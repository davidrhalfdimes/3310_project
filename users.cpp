//users.cpp
//users class
#include <iostream>
#include "users.h"

//using namespace std;


users :: users(int user_id, std::string user_role, std::string user_nickname, std::string user_login)
{

}
//	: user_id(ui), user_role(ur), user_nickname(un), user_login(ul), {}//unique identifiers for members. probably don't need this

std::string users :: add_user (int user_id, std::string user_role, std::string user_nickname, std::string user_login)
{
	return std::string(); //temporary empty string
}

std::string users :: delete_user (int user_id, std::string user_role, std::string user_nickname, std::string user_login)
{
	return std::string();
}

std::string users :: search_user (int user_id) //probably just need (unique) user id to find user
{
	return std::string();
}

