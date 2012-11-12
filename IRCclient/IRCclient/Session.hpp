#ifndef Session_h__
#define Session_h__

//Library headers
#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <ctime>
#include <string>
#include <fstream>

//Project headers
#include "Misc.hpp"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

public class Session
{
	public:
		char * server;
		char * port;
		unsigned short int portnr;
		SOCKET sock;			//Socket handle
		sockaddr_in serverAddr;	//Socket address information
		bool isconnected;

		time_t rawtime;
		struct tm * currenttime;
		string echo;
		char buffer[80];

		//Log variables
		ofstream out;
		char * logfile;

		void setserver(int argc, char*argv[]);
		void setport(int argc, char*argv[]);
//		SOCKET getsocket();
//		sockaddr_in getserveraddr();
		void opensocket(char server[], short portnr);
		void openconnection();

		void startsession();
		void join();
		void chat();
		void openlog();
		void writelog(string echo);
		void closelog();
		
		char* gettime();

		bool connected();
		void disconnect();
};

void Session::setserver(int argc, char*argv[])
{
	server = new char[];
	server = argv[1];
}

void Session::setport(int argc, char*argv[])
{
	port = new char[];
	port = argv[2];

	portnr = (unsigned short) strtoul(port, NULL, 0);	//Cast the port number from char to unsigned short int
}
/*
SOCKET Session::getsocket()
{
	return sock;
}
*/
/*
sockaddr_in Session::getserveraddr()
{
	return serverAddr;
}
*/
void Session::opensocket(char server[], short portnr)
{
	WSADATA wsaDATA;	//Start up Winsock
	int iResult;

	iResult = WSAStartup(MAKEWORD(2,2),&wsaDATA);
	if(iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		WSACleanup();
		exit(1);
	}

	hostent *host;

	host = (hostent *) gethostbyname((char *) server);		//Host address
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);		//Create socket

	if(sock == INVALID_SOCKET)
	{
		cerr << sock << endl;
		cout << "ERROR, can't open socket\n";
		exit(1);
	}

	serverAddr.sin_family = AF_INET;							//address family Internet
	serverAddr.sin_port = htons(portnr);						//Port to connect on
	serverAddr.sin_addr = *((struct in_addr *) host->h_addr);	//Target address
	memset(&(serverAddr.sin_zero), 0, 8);

}

void Session::openconnection()
{
	if(connect(sock, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)	isconnected = false;

	else
		{
			isconnected = true; //Success
		}
}

void Session::join()
{

}

void Session::chat()
{
	openlog();
	cout << "Standing by for input" << endl;

	while(connected())
	{
		cout << "Message: ";
		getline(cin, echo);

		cout << gettime() << echo << endl;
		writelog(echo);

		if(echo == "/quit") isconnected = false;
	}
}

void Session::openlog()
{
	logfile = "irc.log";

	out.open(logfile);

	if(out.fail())
	{
		logfileerror();
	}
}

void Session::writelog(string echo)
{
	out << gettime() << echo << endl;
}

void Session::closelog()
{
	out.close();
}

char* Session::gettime()
{
	time (&rawtime);
	currenttime = localtime(&rawtime);

	strftime(buffer, 80, "%I:%M:%S ", currenttime);

	return buffer;
}

bool Session::connected()
{
	return isconnected;
}

void Session::disconnect()
{
	//CLOSECONNECTION - shuts down the socket and closes any connection on it

	//Close the socket if it exists
	if (sock)
		closesocket(sock);

	cout << "The disconnect function has been called and the connection has been closed" << endl;

	closelog();

	WSACleanup(); //Clean up Winsock
}

#endif // Session_h__
