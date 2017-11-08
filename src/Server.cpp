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


#include "Connection.cpp"


class Server
{
	public:
		Server();

		void getInformation();
		bool setListenPort(int port);
		bool startSocket();
		bool startListen();
		bool waitForConnection(Connection connection);

		bool setMaxTryNumber(int number);

		bool getInternalError();


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

	

	printf(" Server ready!\n");
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

bool Server::waitForConnection(Connection conexao)
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
			conexao = Connection(&clientAddr, clientSock);
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
}