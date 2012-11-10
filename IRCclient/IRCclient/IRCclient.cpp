//MyIRCclient is a simple IRC client
//Author: Benedikt Logi Leifsson Sörensen
//Date: 10.11.2012

#include <iostream>
#include <winsock.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main(int argc, char*argv[])
{
	WSADATA wsaDATA;
	int iResult;

	iResult = WSAStartup(MAKEWORD(2,2),&wsaDATA);
	if(iResult != 0)
	{
		printf("WSAStartup failed: %dn", iResult);
		exit(1);
	}
}