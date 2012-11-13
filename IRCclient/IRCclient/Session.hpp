#ifndef Session_h__
#define Session_h__

//Library headers
#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <ctime>
#include <string>
#include <fstream>

//Project headers
#include "Misc.hpp"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

public class Session
{
	public:

#pragma region Class Variables
	//Input arguments
		char * server;
		char * port;
		unsigned short int portnr;

	//Winsock variables
		SOCKET sock;			//Socket handle
		sockaddr_in serverAddr;	//Socket address information

	//Bool
		bool isconnected;

	//Time variables
		time_t rawtime;
		struct tm * currenttime;
		string echo;
		char clocktime[80];
		char datetime[80];

	//Log variables
		ofstream out;
		char * logfile;

	//User info
		char nick[9];

	//Send/receive
		int bytes;
#pragma endregion Class Variables

#pragma region Class Functions
	//Handle input
		void setserver(int argc, char*argv[]);
		void setport(int argc, char*argv[]);

	//Initialize socket
		void opensocket(char server[], short portnr);

	//Connect and join functions
		void openconnection();
		void startsession();

	//Send and chat
		int receive(char data[]);
		void sendmsg(char msg[]);
		void chat();

	//Log functions
		void openlog();
		void writelog(string echo);
		void closelog();
		
	//Time functions
		char* gettime();
		char* getdatetime();

	//Bool functions
		bool connected();

	//Disconnect function
		void disconnect();
#pragma endregion Class Functions
};

void Session::setserver(int argc, char*argv[])
{
	server = new char[];
	server = argv[1];
}

void Session::setport(int argc, char*argv[])
{
	port = new char[];
	port = argv[2];

	portnr = (unsigned short) strtoul(port, NULL, 0);	//Cast the port number from char to unsigned short int
}

void Session::opensocket(char server[], short portnr)
{
	WSADATA wsaDATA;	//Start up Winsock
	int iResult;

	iResult = WSAStartup(MAKEWORD(2,2),&wsaDATA);
	if(iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		WSACleanup();
		exit(1);
	}

	hostent *host;

	host = (hostent *) gethostbyname((char *) server);		//Host address
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);		//Create socket

	if(sock == INVALID_SOCKET)
	{
		cerr << sock << endl;
		cout << "ERROR, can't open socket\n";
		exit(1);
	}

	serverAddr.sin_family = AF_INET;							//address family Internet
	serverAddr.sin_port = htons(portnr);						//Port to connect on
	serverAddr.sin_addr = *((struct in_addr *) host->h_addr);	//Target address
	memset(&(serverAddr.sin_zero), 0, 8);

}

void Session::openconnection()
{
	if(connect(sock, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)	isconnected = false;

	else
		{
			isconnected = true; //Success
		}
}

void Session::startsession()
{
	char data[512];
	//Receive data from server

	char cap[] = "CAP LS\r\n";
	sendmsg(cap);

	char nick[] = "NICK Theragon\r\n";
	sendmsg(nick);

	char user[] = "USER Theragon 0 * :...\r\n";
	sendmsg(user);

	char capreq[] = "CAP REQ :multi-prefix\r\n";
	sendmsg(capreq);

	char capend[] = "CAP END\r\n";
	sendmsg(capend);

	while(true)
	{
		receive(data);
		cout << data;
	}


/*
	//Get user info
	cout << "Enter nickname: ";
	cin.get(nick, 9);

	char user[] = "Theragon";
*/

	//Get channel list
//	char CAP[] = "LIST\r\n";
//	sendmsg(CAP);

//	sendmsg(channel);
}

int Session::receive(char data[])
{
	bytes = recv(sock, data, strlen(data), 0);

	return bytes;
}

void Session::sendmsg(char msg[])
{
	send(sock, msg, strlen(msg), 0);
}

void Session::chat()
{
	openlog();
	cout << "Standing by for input" << endl;

	while(connected())
	{
		cout << "Message: ";
		getline(cin, echo);

		cout << gettime() << echo << endl;		//This should be a send function
		writelog(echo);

		if(echo == "/quit" || echo == "ragequit") isconnected = false;
	}
}

void Session::openlog()
{
	logfile = "irc.log";

	out.open(logfile);

	if(out.fail())
	{
		logfileerror();
	}
}

void Session::writelog(string echo)
{
	out << getdatetime() << " client: " << echo << endl;
}

void Session::closelog()
{
	out.close();
}

char* Session::gettime()
{
	time (&rawtime);
	currenttime = localtime(&rawtime);

	strftime(clocktime, 80, "%I:%M:%S ", currenttime);

	return clocktime;
}

char* Session::getdatetime()
{
	time (&rawtime);
	currenttime = localtime(&rawtime);

	strftime(datetime, 80, "%d %b %Y %I:%M:%S ", currenttime);

	return datetime;
}

bool Session::connected()
{
	return isconnected;
}

void Session::disconnect()
{
	//CLOSECONNECTION - shuts down the socket and closes any connection on it

	//Close the socket if it exists
	if (sock)
		closesocket(sock);

	cout << "The disconnect function has been called and the connection has been closed" << endl;

	closelog();

	WSACleanup(); //Clean up Winsock
}

#endif // Session_h__
