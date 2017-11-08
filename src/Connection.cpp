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

	private:
		struct sockaddr_in address;
		int fileDescriptor;
		bool initialized;

};

Connection::Connection(struct sockaddr_in *address, int clientFileDescriptor)
{
	this->address = *address;
	this->fileDescriptor = clientFileDescriptor;
	this->initialized = true;
	printf ("Pode parecer loucura, mas funcionou kkkk\n");
}

Connection::Connection()
{
	this->initialized = false;
}