#include<cstdio>
#include "src/Server.cpp"


int main()
{
	Connection connection;
	Server server;
	server.startSocket();
	server.startListen();

	while(true)
	{
		server.waitForConnection(connection);
		if(server.getInternalError())
		{
			break;
		}
	}
	return 0;
}

