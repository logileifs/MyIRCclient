//MyIRCclient is a simple IRC client
//Author: Benedikt Logi Leifsson Sörensen
//Date: 10.11.2012

//Standard library headers
#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>

//Project headers
#include "StartSession.hpp"
#include "Misc.hpp"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main(int argc, char*argv[])
{
	cout << "Number of arguments: " << argc << endl;
	if(argc != 3) helpme();

	char * server = new char[];
	server = argv[1];

	cout << "Line 26" << endl;

	char * optport = new char[];
	optport = argv[2];

	cout << "Line 31" << endl;

	unsigned short portnr = (unsigned short) strtoul(optport, NULL, 0);	//Cast the port number from char to short int

	cout << "Line 34" << endl;

	MySocket my;

	my.opensock(server, portnr);

	SOCKET sock = my.getsocket();
	sockaddr_in serverAddr = my.getserveraddr();

	connect(sock, (sockaddr *)&serverAddr, sizeof(serverAddr));

	WSACleanup();

	cout << "Line 85" << endl;

	return success();
}