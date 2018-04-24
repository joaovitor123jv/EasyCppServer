#include "Connection.hpp"

Connection::Connection(struct sockaddr_in *address, int clientFileDescriptor)
{
	this->address = *address;
	this->fileDescriptor = clientFileDescriptor;
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