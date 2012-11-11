//MyIRCclient is a simple IRC client
//Author: Benedikt Logi Leifsson Sörensen
//Date: 10.11.2012

//Library headers
#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <time.h>

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
		char q;
		int timer = clock();	//Start timer
//		while ((timer/1000) < 10)	//While timer is less than 10 seconds
		while(session.connected())
		{
			timer = clock();		//Update the timer
//			cout << "The connection is open and you can do something" << endl;
//			cout << "Enter q to quit, else the timer will just keep on running" << endl;
			printf("The connection is open and you can do something\r");
//			cin >> q;
			if(q == 'q')
				session.isconnected = false;
			if((timer/1000) % 5 == 0)
			{
				//cout << "The connection has been open for " << (timer/1000) << " seconds" << endl;
//				cout << "Enter q to quit, else the timer will just keep on running" << endl;
				printf("Enter q to quit, else the timer will just keep on running\r");
				cin >> q;
//			Sleep(100);//wait for 100 milliseconds
			}
		}
	}

	session.disconnect();

	//Do some stuff

	WSACleanup();

	return success();
}