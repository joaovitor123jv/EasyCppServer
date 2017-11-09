#include<cstdio>
#include "src/Server.cpp"

Server server;

void *funcao(void *arg)
{
	Connection *connection = static_cast<Connection *>(arg);
	std::string received;

	while(true)
	{
		received = server.receiveData(*connection);
		std::cout << "Data received: "<< received << std::endl;
		if(received == "exit")
		{
			server.sendData("OK, closing connection", *connection);
		}
		else
		{
			server.sendData("This is the data that will be sent to connection", *connection);
		}
	}
}

int main()
{
	server.open(8450);//Defines the port to be listenes, open the socket, start listen and bind a TCP Server

	while(true)
	{
		Connection connection;
		server.waitForConnection(&connection);
		server.runInBackground(funcao, &connection);
	}
	return 0;
}

