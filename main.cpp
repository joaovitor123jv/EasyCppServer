#include<cstdio>
#include "src/Server.cpp"

Server server;


void *funcao(void *arg)
{
	Connection *connection = static_cast<Connection *>(arg);
	//connection = (Connection *)arg;
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
	server.setListenPort(7420);
	server.startSocket();
	server.startListen();

	while(true)
	{
		Connection connection;
		server.waitForConnection(&connection);
		//server.runInBackground(funcao, (void *)&connection);
		server.runInBackground(funcao, &connection);

		//std::cout<< server.receiveData(connection)<<std::endl;
		//server.sendData("Servidor inicializado e funcionando", connection);
		if(server.getInternalError())
		{
			printf(" Erro detectado !\n");
			server.getInformation();
			break;
		}
	}
	return 0;
}

