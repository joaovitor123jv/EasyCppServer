#include<cstdio>
#include "src/Server.cpp"

void *funcao(void *nothing)
{
	while(true)
	{
		printf("asdhjskahdjksahdjksadas\n");
	}
}

int main()
{
	Connection connection;
	Server server;
	server.startSocket();
	server.startListen();

	while(true)
	{
		server.waitForConnection(connection);
		server.runInBackground(funcao);
		if(server.getInternalError())
		{
			printf(" Erro detectado !\n");
			server.getInformation();
			break;
		}
	}
	return 0;
}

