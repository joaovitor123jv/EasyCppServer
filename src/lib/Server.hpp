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

#include"Connection.hpp"

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

		void disableLogs();//Turn showing logs OFF

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