
bool Server::startSocket()
{
	bool deuCerto = false;
	int contador = 0;
	while(!deuCerto)
	{
		if((sockfd = socket(domain, type, protocol))< 0)
		{
			if(logEnabled)
			{
				printf(" ERROR: Failed to create socket\n");
			}
			
			if( contador < maxTryNumber)
			{
				contador = contador + 1;
				if( logEnabled)
				{
					printf("\tTrying again (%d)\n", contador);
				}
				sleep(trySleepTime);
			}
			else
			{
				if( logEnabled )
				{
					printf(" ERROR: Action failed: startSocket on Server\n");
					this->getInformation();
				}
				return false;
			}
		}
		else
		{
			if(logEnabled)
			{
				printf(" LOG: Socket created\n");
				printf(" LOG: Socket File Descriptor = %d\n", sockfd);
			}
			
			deuCerto = true;
		}
	}


	
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);//Aceita qualquer coisa como entrada
	serverAddr.sin_port = htons(port);
	deuCerto = false;



	while(!deuCerto)
	{
		
		printf(" Trying to start bind----->");

		if(bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
		{
			if( logEnabled )
			{
				printf(" Failed to bind\n");
			}
			if( contador < maxTryNumber)
			{
				contador = contador + 1;
				if( logEnabled)
				{
					printf("\tTrying again (%d)\n", contador);
				}
				sleep(trySleepTime);
			}
			else
			{
				if(logEnabled)
				{
					printf(" Max number os tries reached, stopping\n");
				}
				return false;
			}
			
		}
		else
		{
			if( logEnabled )
			{
				printf("Started!\n");
				printf(" Socket initialized in port %d\n", port);
			}
			canStartListen = true;
			deuCerto = true;
			return true;
		}
	}
}


bool Server::setListenPort(int port)
{
	if( listening )
	{
		if( logEnabled )
		{
			printf(" You can\'t edit the listening por in runtime\n");
		}
		return false;
	}
	else
	{
		this->port = port;
		return true;
	}
}

bool Server::setMaxTryNumber(int number)
{
	this->maxTryNumber = number;
	return true;
}