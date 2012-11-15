#include "Session.h"

Session::Session(void)
{
	//Initialize commands

	strcpy_s(CAP, "CAP LS\r\n");					//CAP LS command

	strcpy_s(NICK, "NICK ");						//NICK command

	strcpy_s(CRLF, "\r\n");							//Carriage return, line feed string

	strcpy_s(USER, "USER ");						//USER command
	strcpy_s(USERTRAIL, " 0 * :...\r\n");			//USER trail

	strcpy_s(CAPREQ, "CAP REQ :multi-prefix\r\n");	//CAPREQ command

	strcpy_s(CAPEND, "CAP END\r\n");				//CAP END command

	strcpy_s(LIST, "LIST \r\n");					//LIST command
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

		parsestring(message);

		cin.ignore(strlen(message), '\n');		//Clear input buffer
		
//		receive(data);
//		cout << data;
	}
}

void Session::parsestring(char parse[])
{
	//Use switch statement here instead of if
	if(parse == NICK)
	{
		strcat_s(NICK, nick);	//Place the nickname behind the NICK command
		strcat_s(NICK, CRLF);	//Place the \r\n characters behind the nick
	}

	if(parse == USER)
	{
		strcat_s(USER, nick);		//Place the nickname behind the USER command
		strcat_s(USER, USERTRAIL);	//Place the USER trail behind the nickname
	}

	if(parse == message)	//This is working but needs more testing
	{
		if(strcmp(message, "quit")==0)
			disconnect();

		if(strcmp(message, "ragequit")==0)
			disconnect();

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
	out << getdatetime() << " "  << nick << ": " << message << endl;
}

void Session::serverlog(char data[])
{
	out << getdatetime() << " server: " << data;		//Need to parse input first
}

void Session::closelog()
{
	out.close();
}

char* Session::gettime()
{
	time (&rawtime);
	currenttime = localtime(&rawtime);
//	currenttime = localtime_s(currenttime, &rawtime);		//Try this later

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

void Session::helpme()
{
	cout << "This is the help function, if you are reading this, that means something went wrong";
	exit(1);
}

int Session::success()
{
	cout << "If you are reading this, that means the program exited correctly";

	return 0;
}

void Session::logfileerror()
{
	cout << "Failed to open log file" << endl;
}

//This function is not being used
void Session::quit()
{
	cout << "Press any key to quit. If no key will be pressed the program will automatically stop after 10 seconds";
}

