#include<cstdio>
#include<iostream>
#include<stdlib.h>
//#include<string.h>
#include<string>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/ioctl.h>
#include<pthread.h>
#include<unistd.h>

/*
	*Defaults:
		*Logs enabled 
		*Start listening on port 7440 
		*TCP Server set (in future, there will be a way to change to another server types (like UDP))
		*Number os max tries: 100
		*Interval between tries: 1 second (minimum)
*/


#include "Connection.cpp"


class Server
{
	public:
		Server();

		bool startSocket();
		bool startListen();
		bool open();//showtcut to startSocket() and startListen() in order to run properly.
		bool open(int port);//showtcut to startSocket() and startListen(), defining the "port" number in order to run properly.
		bool waitForConnection(Connection *connection);

		//All "runInBackground" methods returns true if succeed to create a thread with "function", and false otherwise.
		bool runInBackground(void *(*function)(void *), void *parameters);// Runs "function" in background, passes "*parameters" as argument (function may return "void *" and receive "void *")
		bool runInBackground(void *(*function)(Connection *), Connection *connection);// Runs a function that returns "void *" and receives "Connection *", passing *connection it as argument.
		bool runInBackground(void *(*function)(void *));// Runs "function" in background, no parameters (function may return "void *" and receive "void *")

		bool disableLogs();//Turn showing logs OFF

		bool setMaxTryNumber(int number);//Define the number os max tries (when setting up server, trying to do bind...)
		bool setTimeBetweenTries(int timeInSeconds);//Change the interval in seconds, between tries
		bool setListenPort(int port);//Define the port to listen

		bool getInternalError();//Check for some errors (almost useless by now)
		void getInformation();//Returns Data of the server created

		bool sendData(std::string data, Connection connection);//Send a string of data to client, identified by "connection" (received from waitForConnection())
		std::string receiveData(Connection connection);//Receive data from client connected (connection), and convert it in string

	private:
		
		bool logEnabled;
		bool listening;
		bool canStartListen;

		int port;
		int sockfd;
		int domain;
		int type;
		int protocol;
		int simultaneousListeners;

		int bufferSize;

		int maxTryNumber;
		int trySleepTime;

		struct sockaddr_in serverAddr;
};

#include "Server_Configuration.h"

void Server::getInformation()
{
		std::cout<< "Server information: " <<std::endl;
		std::cout<< "\tLOG: " << logEnabled <<std::endl;
		std::cout<< "\tListening ?: " << listening <<std::endl;
		std::cout<< "\tCan start Listening ?: " << canStartListen <<std::endl;
		std::cout<< "\tListening port: " << port <<std::endl;
		std::cout<< "\tSocket data:" <<std::endl;
		std::cout<< "\t\tDomain:" << domain <<std::endl;
		std::cout<< "\t\tType:" << type <<std::endl;
		std::cout<< "\t\tProtocol:"<< protocol << std::endl;
		std::cout<< "\t\tSocket File Descriptor:"<< sockfd << std::endl;
		std::cout<< "\tMax number of tries:"<< maxTryNumber <<std::endl;
		std::cout<< "\tTime between tries:"<< trySleepTime <<std::endl;
		std::cout<< "\tMax simultaneous listeners:"<< simultaneousListeners <<std::endl;
		std::cout<< "\tBuffer Size:"<< bufferSize <<std::endl;
}

Server::Server()
{
	logEnabled = true;
	listening = false;
	canStartListen = false;
	port = 7440;

	domain = PF_INET;
	type = SOCK_STREAM;
	protocol = IPPROTO_TCP;

	sockfd = 0;
	
	maxTryNumber = 100;
	trySleepTime = 1;

	simultaneousListeners = 5;
	
	bufferSize = 1024;//1024 Bytes by default

	//printf(" Server ready!\n");
}

bool Server::startListen()
{
	if(canStartListen)
	{
		if(listen(sockfd, simultaneousListeners) < 0)
		{
			if( logEnabled )
			{
				printf(" Warning: Failed to start listening\n");
				getInformation();
			}
			listening = false;
			return false;
		}
		else
		{
			if( logEnabled )
			{
				printf(" LOG: Listening on port %d\n", port);
			}
			listening = true;
			return true;
		}
	}
	else
	{
		if( logEnabled )
		{
			printf(" You can\'t start listen yet !, check your code!\n");
			getInformation();
		}
		listening = false;
		return false;
	}
}

bool Server::waitForConnection(Connection *conexao)
{
	if( !listening )
	{
		if( logEnabled )
		{
			printf(" ERROR: Hey, you can\'t wait for client. Start listening first !\n");
		}
		return false;
	}
	else
	{
		struct sockaddr_in clientAddr;
		unsigned int clientLen = sizeof(clientAddr);
		int clientSock = accept(sockfd, (struct sockaddr *)&clientAddr, &clientLen);

		if(clientSock < 0)
		{
			if( logEnabled )
			{
				printf(" ERROR: Failed to get any connection\n");
			}
			return false;
		}
		else
		{
			if( logEnabled )
			{
				printf(" LOG: Client accepted successfully\n");
			}
			*conexao = Connection(&clientAddr, clientSock);
			return true;
		}
	}
}

bool Server::getInternalError()
{
	if(!listening)
	{
		return true;
	}
	if(sockfd<0)
	{
		return true;
	}
	return false;
}

bool Server::runInBackground( void *(*function)(void *), void *parameters )
{
	pthread_t thread;
	if(pthread_create(&thread, NULL, function, parameters) != 0)
	{
		printf("ERROR: Failed to start running function in background\n");
		return false;
	}
	else
	{
		pthread_detach(thread);
		return true;
	}
}

bool Server::runInBackground( void *(*function)(void *))
{
	pthread_t thread;
	if(pthread_create(&thread, NULL, function, NULL) != 0)
	{
		printf("ERROR: Failed to start running function in background\n");
		return false;
	}
	else
	{
		pthread_detach(thread);
		return true;
	}
}

bool Server::runInBackground( void *(*function)(Connection *), Connection *connection)
{
	pthread_t thread;

	if(pthread_create(&thread, NULL, (void *(*)(void*))function, (void *)connection) != 0)
	{
		printf("ERROR: Failed to start running function in background\n");
		return false;
	}
	else
	{
		pthread_detach(thread);
		return true;
	}
}

bool Server::sendData(std::string data, Connection connection)
{
	if(connection.started())
	{
		if(write(connection.getDescriptor(), data.c_str(), data.length()+1) != data.length()+1)
		{
			if(logEnabled)
			{
				printf(" Failed to write all data to specific connection!\n");
			}
			return false;
		}
		else
		{
			if(logEnabled)
			{
				printf(" Data send to specific connection\n");
			}
			return true;
		}
	}
	else
	{
		if(logEnabled)
		{
			printf("This connection wasn't started yet!\n");
		}
		return false;
	}
}

std::string Server::receiveData(Connection connection)
{
	std::string response = "";
	char *cstr = (char *)malloc(sizeof(char) * bufferSize);
	if(cstr == NULL)
	{
		if(logEnabled)
		{
			printf("Failed to alloc memory to temporary pointer\n");
		}
		return response;
	}

	read(connection.getDescriptor(), cstr, bufferSize);
	response.assign(cstr);
	free(cstr);
	cstr = NULL;

	return response;
}

bool Server::open()
{
	if(this->startSocket())
	{
		if(this->startListen())
		{
			if(logEnabled)
			{
				printf(" Listening on port %d\n", port);
			}
			return true;
		}
		else
		{
			if(logEnabled)
			{
				printf(" Failed to start listening on %d port\n", port);
				printf(" \tPlease check if %d port is currently being used in your system\n", port);
			}
			return false;
		}
	}
	else
	{
		if(logEnabled)
		{
			printf(" Failed to start socket, can't start Listening\n");
		}
		return false;
	}
}

bool Server::open(int port)
{
	this->setListenPort(port);
	return this->open();
}