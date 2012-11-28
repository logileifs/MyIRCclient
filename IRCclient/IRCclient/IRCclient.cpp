//MyIRCclient is a simple IRC client
//To start the program open it from command prompt with two arguments
//the first argument is the server to connect to and the second is the port number
//
//Author: Benedikt Logi Leifsson Sörensen, Halldór Vilhjálmsson, Heimir Már Helgason
//Date: 28.11.2012

//Project headers
#include "Session.h"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main(int argc, char*argv[])
{
	Session session;					//Create a new session instance

	if(argc != 3) session.helpMe();		//If the program is not started with two arguments the helpMe function is called

	session.setServer(argc, argv);		//Set the server as first argument
	session.setPort(argc, argv);		//Set the port as second argument

	session.openSocket(session.server, session.portNr);	//Open socket connection

	session.openConnection();		//Open server connection

	if(session.connected())						//Check if server connection is open
		session.startSession(session.data);		//Start the IRC session

	session.disconnect();		//Disconnect from server

	WSACleanup();				//Clean up Winsock

	return session.success();	//Exit the program
}