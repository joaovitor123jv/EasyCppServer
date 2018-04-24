#include<cstdio>
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/ioctl.h>
#include<pthread.h>
#include<unistd.h>

class Connection
{
	public:
		Connection();
		Connection(struct sockaddr_in *address, int clientFileDescriptor);

		int getDescriptor();
		bool started();
		//struct sockaddr_in getAddress();

	private:
		struct sockaddr_in address;
		int fileDescriptor;
		bool initialized;

};

