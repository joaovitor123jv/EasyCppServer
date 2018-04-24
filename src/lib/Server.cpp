#include<iostream>
#include"Server.hpp"


//*********************** CONFIGURAÇÕES *********************
bool Server::startSocket()
{
	bool deuCerto = false;
	int contador = 0;
	while(!deuCerto)
	{
		if((sockfd = socket(domain, type, protocol))< 0)
		{
			if(this->logEnabled)
			{
				std::cout<<" ERROR: Failed to create socket"<<std::endl;
			}
			if( contador < this->maxTryNumber)
			{
				contador = contador + 1;
				if( logEnabled)
				{
					std::cout<<"\tTrying again (%d)"<< contador<<std::endl;
				}
				sleep(this->trySleepTime);
			}
			else
			{
				if( this->logEnabled )
				{
					std::cout<<" ERROR: Action failed: startSocket on Server"<<std::endl;
					this->getInformation();
				}
				return false;
			}
		}
		else
		{
			if(this->logEnabled)
			{
				std::cout<<" LOG: Socket created"<<std::endl;
				std::cout<<" LOG: Socket File Descriptor = %d"<< sockfd<< std::endl;
			}
			
			deuCerto = true;
		}
	}
	
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);//Aceita qualquer endereco IP como entrada
	serverAddr.sin_port = htons(port);
	deuCerto = false;

	while(!deuCerto)
	{
		
		std::cout<<" Trying to start bind----->"<<std::endl;

		if(bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
		{
			if( logEnabled )
			{
				std::cout<<" Failed to bind"<<std::endl;
			}
			if( contador < maxTryNumber)
			{
				contador = contador + 1;
				if( logEnabled)
				{
					std::cout<<"\tTrying again (%d)"<< contador << std::endl;
				}
				sleep(trySleepTime);
			}
			else
			{
				if(logEnabled)
				{
					std::cout<<" Max number os tries reached, stopping"<< std::endl;
				}
				return false;
			}
			
		}
		else
		{
			if( logEnabled )
			{
				std::cout<<"Started!"<<std::endl;
				std::cout<<" Socket initialized in port "<< port<<std::endl;
			}
			canStartListen = true;
			deuCerto = true;
			return true;
		}
	}
	return true;
}

/** 
 * @brief  Defines the port to be listened by Server->startListen()
 * @note   
 * @param  port: the port Number to be listened
 * @retval true if no port is being listened, false otherwise
 */
bool Server::setListenPort(int port)
{
	if( listening )
	{
		if( logEnabled )
		{
			std::cout<<" You can\'t edit the listening port in runtime"<<std::endl;
			std::cout<<" Please, setup the server correctly before start to listen on any port"<<std::endl;
		}
		return false;
	}
	else
	{
		this->port = port;
		return true;
	}
}

/** 
 * @brief  Configures the maximum of tries something can have. This way you can specify the max number the server will try to bind the server, as example.
 * @note   
 * @param  number: the number to be the maximum of tries
 * @retval true, forever
 */
bool Server::setMaxTryNumber(int number)
{
	this->maxTryNumber = number;
	return true;
}

/** 
 * @brief  Set the time in seconds to wait till a new try (connection try, socket creation try... these things)
 * @note   
 * @param  number: number of seconds the server will wait till try something again
 * @retval true, forever
 */
bool Server::setTimeBetweenTries(int number)
{
	this->trySleepTime = number;
	return true;
}

/** 
 * @brief  This function will disable the logs, great for production environments
 * @note   
 * @retval None
 */
void Server::disableLogs()
{
	this->logEnabled = false;
}





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




//************** PARTE PRATICA *************************





/** 
 * @brief  Starts the server with the default Values
 * @note   
 * @retval CONSTRUCTOR
 */
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
}

/** 
 * @brief  Start listening to a previously specified port (or default port)
 * @note   
 * @retval returns true if it start listening, false otherwise
 */
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

/** 
 * @brief  Wait till a connection happens, then create and store it in Connection *
 * @note   
 * @param  *conexao: 
 * @retval return true if some connection occurs, false if some error occurs
 */
bool Server::waitForConnection(Connection *connection)
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
			*connection = Connection(&clientAddr, clientSock);
			return true;
		}
	}
}

/** 
 * @brief  Check for errors during server configuration, or stopping listening
 * @note   
 * @retval true if some error occurs, false otherwise
 */
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

/** 
 * @brief  Takes a function as argument, then start it in background (thread)
 * @note   passes no argument to the mencioned function
 * @param  *(*function: the function to be executed
 * @retval true if succeed, false otherwise
 */
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

/** 
 * @brief  Takes a function as argument, then start it in background (thread)
 * @note   passes connection as argument to the mencioned function
 * @param  *(*function: the function to be executed
 * @param  Connection *: the connection to be sent as argument to the function
 * @retval true if succeed, false otherwise
 */
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

/** 
 * @brief  Sends the data to the client
 * @note   Send data as "string"
 * @param  data: string that will be sent to the client
 * @param  connection: The connection that the Client is using
 * @retval return "true" if succeed, "false" if some failure occurs
 */
bool Server::sendData(std::string data, Connection connection)
{
	if(connection.started())
	{
		if( write(connection.getDescriptor(), data.c_str(), data.length()+1) != (unsigned int)data.length()+1 )
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

/** 
 * @brief  Wait till receive a stream of Server->bufferSize bytes of Data
 * @note   Return the data as "std::string"
 * @param  connection: The connection that will receive the data
 * @retval the string containing all the data (on tha max bufferSize amount)
 */
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

/** 
 * @brief  Starts the server on the previously specified port
 * @note   (default port = 7440)
 * @retval return true if server is opened and ready to listen for data, false otherwise
 */
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

/** 
 * @brief  Configures the port to be listened, the open a server (start to listen, configure...) o it
 * @note   
 * @param  port: the port to be listened
 * @retval true if the server can be opened, false otherwise
 */
bool Server::open(int port)
{
	this->setListenPort(port);
	return this->open();
}