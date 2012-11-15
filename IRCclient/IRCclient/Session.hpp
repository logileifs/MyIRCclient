#ifndef Session_h__
#define Session_h__

//Library headers
#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <ctime>
#include <string>
#include <fstream>
//#include <thread>		//Maybe test later

//Project headers
#include "Misc.hpp"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

const int BUFF = 512;

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
		char clocktime[80];
		char datetime[80];

	//Log variables
		ofstream out;
		char * logfile;

	//User info
		char nick[BUFF];

	//Commands
		char CAP[BUFF];
		char NICK[BUFF];
		char CRLF[BUFF];
		char USER[BUFF];
		char USERTRAIL[BUFF];
		char CAPREQ[BUFF];
		char CAPEND[BUFF];
		char LIST[BUFF];

	//Send/receive
		int bytes;
		char data[BUFF];
		char message[BUFF];

#pragma endregion Class Variables

#pragma region Class Functions
	//Constructor
		Session::Session(void);

	//Handle input / set functions
		void setserver(int argc, char*argv[]);
		void setport(int argc, char*argv[]);

	//Initialize socket
		void opensocket(char server[], short portnr);

	//Connect and join functions
		void openconnection();
		void getuserinfo(char nick[]);
		void startsession(char data[]);

	//Send and chat
		int receive(char data[]);
		void sendmsg(char msg[]);
		void chat();

	//String parse
		void parsestring(char parse[]);

	//Log functions
		void openlog();
		void clientlog(char message[]);
		void serverlog(char data[]);
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

Session::Session(void)
{
	//Initialize commands

	strcpy_s(CAP, "CAP LS\r\n");

	strcpy_s(NICK, "NICK ");

	strcpy_s(CRLF, "\r\n");

	strcpy_s(USER, "USER ");		//USER = "USER [nick] 0 * :...\r\n"
	strcpy_s(USERTRAIL, " 0 * :...\r\n");

	strcpy_s(CAPREQ, "CAP REQ :multi-prefix\r\n");

	strcpy_s(CAPEND, "CAP END\r\n");

	strcpy_s(LIST, "LIST \r\n");
}

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

void Session::getuserinfo(char nick[])
{
	cout << "Enter your nickname: ";
	cin.get(nick, 9);
	cin.ignore(strlen(nick), '\n');		//Clear input buffer

	cout << "Your nickname is: " << nick << endl;
}

void Session::startsession(char data[])
{
	openlog();		//Open log file for the session

	getuserinfo(nick);

	parsestring(NICK);
	parsestring(USER);

	sendmsg(CAP);		//Send CAP command
	sendmsg(NICK);		//Send NICK command
	sendmsg(USER);		//Send USER command
	sendmsg(CAPREQ);	//Send CAPREQ command
	sendmsg(CAPEND);	//Send CAPEND command

	//Receive data from server

	do
	{
		receive(data);
		serverlog(data);

		if(strchr(data,'0')!=nullptr)
			if(strchr(data, '0')!=nullptr)
				if(strchr(data, '1')!=nullptr)
				{
					cout << "Server connection established" << endl;
					chat();
				}

		cout << data;

	}while(bytes != 0);		//Add timer/timeout function
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
	cout << endl;
	cout << "Standing by for input" << endl;

	while(connected())
	{
		cout << "Message: ";
		cin.get(message, 510);
//		parsestring(message);		//Test this function later
		cout << gettime() << message << endl;
		sendmsg(message);
		clientlog(message);

		if(message == "quit" || message == "ragequit") disconnect();	//Quit commands not working :(

		cin.ignore(strlen(message), '\n');		//Clear input buffer
		
//		receive(data);
//		cout << data;
	}
}

void Session::parsestring(char parse[])
{
	if(parse == NICK)
	{
		cout << "This should parse the NICK command" << endl;
		strcat_s(NICK, nick);	//Place the nickname behind the NICK command
		strcat_s(NICK, CRLF);	//Place the \r\n characters behind the nick
	}
	if(parse == USER)
	{
		cout << "This should parse the USER command" << endl;
		strcat_s(USER, nick);
		strcat_s(USER, USERTRAIL);
	}
	if(parse == message)	//This needs to be tested
	{
		cout << "This should parse the message" << endl;
		strcat_s(message, CRLF);
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

void Session::clientlog(char message[])
{
	out << getdatetime() << " client: " << message << endl;
}

void Session::serverlog(char data[])
{
	out << getdatetime() << " server: " << data;
}

void Session::closelog()
{
	out.close();
}

char* Session::gettime()
{
	time (&rawtime);
	currenttime = localtime(&rawtime);
//	currenttime = localtime_s(currenttime);		//Try this later

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
	cout << "Here, the user should be asked if he wants to reconnect or quit" << endl;

	closelog();		//Close the log file

	WSACleanup();	//Clean up Winsock
	exit(1);		//Quit the program
}

#endif // Session_h__
