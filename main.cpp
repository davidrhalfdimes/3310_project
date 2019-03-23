#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;


int main()
{
	//file descriptor for socket
    int intial_socket; 

    //stores the value of the port number
    int port_num;

    //array created for reading from socket
    char text_buffer[256];

    //socket creation
    //AF_INET is the IPv4 protocol.
    // SOCK_STREAM is the communication type. 
    //Last parameter is always 0
    intial_socket = socket(AF_INET, SOCK_STREAM, 0);

    //socket error given if not connected
    if(intial_socket<0)
    {
    	cout<<"ERROR: Socket connection not established";
    }



    
    
    return 0;
}
