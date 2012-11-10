//MyIRCclient is a simple IRC client
//Author: Benedikt Logi Leifsson Sörensen
//Date: 10.11.2012

#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main(int argc, char*argv[])
{
	char server [] = "irc.freenode.net";
	short portnr = 6667;


#pragma region Socket and port stuff
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
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(sock == INVALID_SOCKET)
	{
		cerr << sock << endl;
		cout << "ERROR, can't open socket\n";
		return EXIT_FAILURE;
	}

	serverAddr.sin_family = AF_INET;							
	serverAddr.sin_port = htons(portnr);						//portNr is passed into the htons function and assigned to sin_port of serverAddr struct
	serverAddr.sin_addr = *((struct in_addr *) host->h_addr);	//The host address is assigned to serverAddr struct
	memset(&(serverAddr.sin_zero), 0, 8);
#pragma endregion Socket and port stuff


	//Put code here


	WSACleanup();
	return EXIT_SUCCESS;
}