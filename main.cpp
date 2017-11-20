#include<cstdio>

#include <Server.cpp>
#include <MysqlIntegration.cpp>

Server server;

void *funcao(void *arg)
{
	std::string received;
	Connection connection = Connection(arg);

	while(true)
	{
		received = server.receiveData(connection);
		std::cout << "Data received: "<< received << std::endl;
		if(received == "exit")
		{
			server.sendData("OK, closing connection", connection);
		}
		else
		{
			server.sendData("This is the data that will be sent to connection", connection);
		}
	}
}


int main()
{
	server.open(8450);//Defines the port to be listeners, open the socket, start listen and bind a TCP Server

	while(true)
	{
		Connection connection;
		server.waitForConnection(&connection);
		server.runInBackground(funcao, &connection);
	}
	return 0;
}

