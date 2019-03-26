#ifndef role_H
#define role_H

class role
{
	private:


	public:
		
	role(std::string role_title, std::string role_description);
	std::string assign_role(std::string role_title, std::string user_id);
	void edit_role(std::string role_title, std::string user_id);	

	protected:

};

#endif
