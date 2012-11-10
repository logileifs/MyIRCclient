#ifndef OpenSocket_h__
#define OpenSocket_h__

#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>


#pragma comment(lib, "Ws2_32.lib")

using namespace std;

public class MySocket
{
	public:
		SOCKET sock;
		sockaddr_in serverAddr;

		SOCKET getsocket();
		sockaddr_in getserveraddr();
		void opensock(char server[], short portnr);
};

SOCKET MySocket::getsocket()
{
	return sock;
}

sockaddr_in MySocket::getserveraddr()
{
	return serverAddr;
}

void makeSYN(char SYN[])
{
	//char syn[16];

	SYN[0] = 0x53;	//SMID

	SYN[1] = 0;		//FLAGS

	SYN[2] = 0;		//SID
	SYN[3] = 0;		//SID

	SYN[4] = 0;		//LENGTH
	SYN[5] = 0;		//LENGTH
	SYN[6] = 0;		//LENGTH
	SYN[7] = 0;		//LENGTH

	SYN[8] = 0;		//SEQNUM
	SYN[9] = 0;		//SEQNUM
	SYN[10] = 0;		//SEQNUM
	SYN[11] = 0;		//SEQNUM

	SYN[12]	= 0;	//WINDW
	SYN[13] = 0;	//WINDW
	SYN[14] = 0;	//WINDW
	SYN[15] = 0;	//WINDW
}

void MySocket::opensock(char server[], short portnr)
{
	WSADATA wsaDATA;
	int iResult;

	iResult = WSAStartup(MAKEWORD(2,2),&wsaDATA);
	if(iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		exit(1);
	}

	hostent *host;

	host = (hostent *) gethostbyname((char *) server);
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

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
}

#endif // OpenSocket_h__
