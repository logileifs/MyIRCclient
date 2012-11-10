#ifndef OpenSocket_h__
#define OpenSocket_h__

#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

void opensock(char server[], short portnr)
{
	WSADATA wsaDATA;
	int iResult;

	iResult = WSAStartup(MAKEWORD(2,2),&wsaDATA);
	if(iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		exit(1);
	}

	SOCKET sock;
	sockaddr_in serverAddr;
	hostent *host;

	host = (hostent *) gethostbyname((char *) server);
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if(sock == INVALID_SOCKET)
	{
		cerr << sock << endl;
		cout << "ERROR, can't open socket\n";
		exit(1);
	}

	serverAddr.sin_family = AF_INET;							
	serverAddr.sin_port = htons(portnr);						//portNr is passed into the htons function and assigned to sin_port of serverAddr struct
	serverAddr.sin_addr = *((struct in_addr *) host->h_addr);	//The host address is assigned to serverAddr struct
	memset(&(serverAddr.sin_zero), 0, 8);

	WSACleanup();
}

#endif // OpenSocket_h__
