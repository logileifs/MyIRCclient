#ifndef OpenSocket_h__
#define OpenSocket_h__

//Standard library headers
#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>

//Project headers
#include "Misc.hpp"


#pragma comment(lib, "Ws2_32.lib")

using namespace std;

public class Session
{
	public:
		char * server;// = new char[];
		char * port;// = new char[];
		unsigned short int portnr;
		SOCKET sock;			//Socket handle
		sockaddr_in serverAddr;	//Socket address information

		void setserver(int argc, char*argv[]);
		void setport(int argc, char*argv[]);
		SOCKET getsocket();
		sockaddr_in getserveraddr();
		void opensocket(char server[], short portnr);
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

SOCKET Session::getsocket()
{
	return sock;
}

sockaddr_in Session::getserveraddr()
{
	return serverAddr;
}

void Session::opensocket(char server[], short portnr)
{
	WSADATA wsaDATA;	//Start up Winsock
	int iResult;

	iResult = WSAStartup(MAKEWORD(2,2),&wsaDATA);
	if(iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		exit(1);
	}

	hostent *host;

	host = (hostent *) gethostbyname((char *) server);
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

#endif // OpenSocket_h__
