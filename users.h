#ifndef users_H
#define users_H

class users
{
	private:

	public:
	
	users(int user_id, string user_role, string user_nickname, string user_login);
	string add_user(int user_id, string user_role, string user_nickname, string user_login);
	string delete_user(int user_id, string user_role, string user_nickname, string user_login);
	string search_user(int user_id);	
	

	protected:

};

#endif
