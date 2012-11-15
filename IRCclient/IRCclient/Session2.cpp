#include <iostream>
#include "Session.h"

using namespace std;

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

	char NICK[] = "NICK ";
	char CRLF[] = "\r\n";
	strcat(nick, CRLF);
	strcat(NICK, nick);

	char CAP[] = "CAP LS\r\n";

	cout << "strlen(CAP): " << strlen(CAP) << endl;

	char USER[] = "USER Theragon 0 * :...\r\n";

	char CAPREQ[] = "CAP REQ :multi-prefix\r\n";

	char CAPEND[] = "CAP END\r\n";

	sendmsg(CAP);

	sendmsg(NICK);

	sendmsg(USER);

	sendmsg(CAPREQ);

	sendmsg(CAPEND);

	//Receive data from server

	do
	{
		receive(data);
		serverlog(data);

		if(strchr(data,'0')!=nullptr)
			if(strchr(data, '0')!=nullptr)
				if(strchr(data, '1')!=nullptr)		//Search for "[:prefix][001][nick][:trailer]"
				{
					cout << "Server connection established" << endl;
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
		cin.get(message, 512);
		cout << gettime() << message << endl;
		sendmsg(message);
		clientlog(message);
		cin.ignore(strlen(message), '\n');		//Clear input buffer

		//receive(data);

		if(message == "quit" || message == "ragequit") disconnect();	//Quit commands not working :(
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
	out << getdatetime() << " server: " << data << endl;
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
	cout << "Here, the user should be asked if he wants to reconnect or quit" << endl;

	closelog();		//Close the log file

	WSACleanup();	//Clean up Winsock
	exit(1);		//Quit the program
}