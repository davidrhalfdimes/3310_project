#ifndef users_H
#define users_H

class users
{
	private:

	public:
	
	users(int user_id, std::string user_role, std::string user_nickname, std::string user_login);
	std::string add_user(int user_id, std::string user_role, std::string user_nickname, std::string user_login);
	std::string delete_user(int user_id, std::string user_role, std::string user_nickname, std::string user_login);
	std::string search_user(int user_id);	
	

	protected:

};


#endif
