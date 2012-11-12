//MyIRCclient is a simple IRC client
//Author: Benedikt Logi Leifsson Sörensen
//Date: 10.11.2012

//Library headers
#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <string>

//Project headers
#include "Session.hpp"
#include "Misc.hpp"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main(int argc, char*argv[])
{
	if(argc != 3) helpme();

	Session session;
	cout << "Number of arguments: " << argc << endl;

	session.setserver(argc, argv);
	session.setport(argc, argv);

	session.opensocket(session.server, session.portnr);

	session.openconnection();
//	int IRC::start(char* server, int port, char* nick, char* user, char* name, char* pass);		//Connects to an IRC server.

	if(session.connected())
	{
		session.chat();
	}

	session.disconnect();

	WSACleanup();

	return success();
}