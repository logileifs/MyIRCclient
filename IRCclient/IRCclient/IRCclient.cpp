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
	if(argc != 3) helpme();

	Session my;
	cout << "Number of arguments: " << argc << endl;

	my.setserver(argc, argv);
	my.setport(argc, argv);

	my.opensocket(my.server, my.portnr);

	connect(my.sock, (sockaddr *)&my.serverAddr, sizeof(my.serverAddr));

	WSACleanup();

	return success();
}