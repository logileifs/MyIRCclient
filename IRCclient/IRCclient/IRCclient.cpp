//MyIRCclient is a simple IRC client
//Author: Benedikt Logi Leifsson Sörensen, Halldór Vilhjálmsson, Heimir Már Helgason
//Date: 10.11.2012

//Project headers
#include "Session.h"
//#include "Misc.h"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main(int argc, char*argv[])
{
	Session session;

	if(argc != 3) session.helpMe();

	session.setServer(argc, argv);
	session.setPort(argc, argv);

	session.openSocket(session.server, session.portNr);

	session.openConnection();

	if(session.connected())
		session.startSession(session.data);

	session.disconnect();

	WSACleanup();

	return session.success();
}