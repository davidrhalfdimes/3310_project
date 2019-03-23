#include <iostream>
#include <lobby.h>

using namespace std;
 

lobby :: lobby(string user_role, string user_id, string chat_id,  
	int user_count, int chat_count, bool receieve_invite, string role_id) 
// : user_role(ur), user_id (ui), chat_id(ci), chat_history (ch), chat_time{ct}
  

 
string lobby :: chat_history(string [] messages) //
{


}		

//return type is set as int for representing the time in milliseconds  
int lobby :: chat_time() 
{
  
}

//unsure about return type
void lobby :: create_new_chat(string chat_id) 
{
	//creating the file descriptor 
	fd_set master;

	//clearing the file descriptor before use
	FD_ZERO(&master);

	//adding the first socket
	fd_set(initial_socket, &master);

	while(1)
	{
		//creating a duplicate version of master socket
		fd_set duplicate = master;

		//keeping count of sockets
		int sock_count = select(0, &copy, NULL, NULL, NULL);

		fd_set(initial_socket, &master);


		for(int i=0;i<sock_count;i++)
		{
			SOCKET sock = duplicate.fd_array[i];
			if(sock == initial_socket)
			{
				//allow new connection to be made
				SOCKET client = accept(initial_socket, NULL, NULL);

				//adding the connection with rest of clients
				fd_set(client, &master);

				//intial greeting message seen by all clients
				string greeting = "Welcome to the Group Chat";
				send(client, greeting, 0);
			}
		}
	}
	



}		

//return type set as 'bool' as a check 
bool lobby :: delete_users(string user_id) 
{
  
}




