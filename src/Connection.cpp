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
		Connection(void *arg);
		Connection(struct sockaddr_in *address, int clientFileDescriptor);

		int getDescriptor();
		bool started();
		sockaddr_in getAddress();

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
}

Connection::Connection(void *arg)
{
	Connection *connection = static_cast<Connection *>(arg);	
	this->address = connection->getAddress();
	this->fileDescriptor = connection->getDescriptor();
	this->initialized = true;
}

Connection::Connection()
{
	this->initialized = false;
}

int Connection::getDescriptor()
{
	if(this->initialized)
	{
		return this->fileDescriptor;
	}
	return -1;
}

bool Connection::started()
{
	return this->initialized;
}

sockaddr_in Connection::getAddress()
{
	return this->address;
}