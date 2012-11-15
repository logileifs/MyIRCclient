//MyIRCclient is a simple IRC client
//Author: Benedikt Logi Leifsson Sörensen, Halldór Vilhjálmsson, Heimir Már Helgason
//Date: 10.11.2012

/*//Library headers
#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <ctime>
#include <string>
#include <fstream>*/

//Project headers
#include "Session.h"
//#include "Misc.h"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main(int argc, char*argv[])
{
	
	Session session;

	if(argc != 3) session.helpme();

	

	session.setserver(argc, argv);
	session.setport(argc, argv);

	session.opensocket(session.server, session.portnr);

	session.openconnection();

	if(session.connected())
	{
		session.startsession(session.data);
	}

	session.disconnect();

	WSACleanup();

	return session.success();
}