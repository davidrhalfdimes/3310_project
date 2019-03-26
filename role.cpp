//role.cpp
//role class
//role tide

#include <iostream>
#include <string>
#include "role.h"

//using namespace std;

role::role (std::string role_title, std::string role_description) //deleted role_ID: string. not necessary since classification of users is covered by role_title
{

}

std::string assign_role (std::string role_title, std::string user_id) //role_title: what role being assigned. user_ID: which user is being assigned a role. member comes from users.cpp (will be accessible when we merge class holder files like this one into the main cpp files (server, client, or both)
{
	return std::string();
}

void role::edit_role(std::string role_title, std::string user_id) //return type tentative, may need to change
{

}
