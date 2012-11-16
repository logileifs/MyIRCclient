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

void Session::setServer(int argc, char*argv[])
{
	server = new char[];
	server = argv[1];
}

void Session::setPort(int argc, char*argv[])
{
	port = new char[];
	port = argv[2];

	portNr = (unsigned short) strtoul(port, NULL, 0);	//Cast the port number from char to unsigned short int
}

void Session::openSocket(char server[], short portnr)
{
	WSADATA wsaDATA;	//Start up Winsock
	int result;

	result = WSAStartup(MAKEWORD(2,2),&wsaDATA);
	if(result != 0)
	{
		printf("WSAStartup failed: %d\n", result);
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
	serverAddr.sin_port = htons(portNr);						//Port to connect on
	serverAddr.sin_addr = *((struct in_addr *) host->h_addr);	//Target address
	memset(&(serverAddr.sin_zero), 0, 8);

}

void Session::openConnection()
{
	if(connect(sock, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)	
		isConnected = false;
	else
		isConnected = true; //Success
}

void Session::getUserInfo(char nick[])
{
	cout << "Enter your nickname: ";
	cin.get(nick, 9);
	cin.ignore(strlen(nick), '\n');		//Clear input buffer

	cout << "Your nickname is: " << nick << endl;
}

void Session::startSession(char data[])
{
	openLog();		//Open log file for the session

	getUserInfo(nick);

	parseString(NICK);
	parseString(USER);

	sendMsg(CAP);		//Send CAP command
	sendMsg(NICK);		//Send NICK command
	sendMsg(USER);		//Send USER command
	sendMsg(CAPREQ);	//Send CAPREQ command
	sendMsg(CAPEND);	//Send CAPEND command

	//Receive data from server

	out << getDateTime() << " server: ";
	do
	{
		receive(data);

		serverLog(data);

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

void Session::sendMsg(char msg[])
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
		cout << getTime() << message << endl;
		sendMsg(message);
		clientLog(message);

		parseString(message);

		cin.ignore(strlen(message), '\n');		//Clear input buffer
		
//		receive(data);
//		cout << data;
	}
}

void Session::parseString(char parse[])
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
		if(strcmp(message, "/quit")==0)
			disconnect();

		if(strcmp(message, "ragequit")==0)
			disconnect();

		strcat_s(message, CRLF);
	}
}

void Session::openLog()
{
	logFile = "irc.log";

	out.open(logFile);

	if(out.fail())
		logFileError();
}

void Session::clientLog(char message[])
{
	out << endl << getDateTime() << " "  << nick << ": " << message;
}

void Session::serverLog(char data[])
{
	if(strncmp(data, "\r\n", 2)==0)
		out << endl << getDateTime() << " server: ";
	else
		out << data;		//Need to parse input first
}

void Session::closeLog()
{
	out.close();
}

char* Session::getTime()
{
	time (&rawTime);
	currentTime = localtime(&rawTime);
//	currenttime = localtime_s(currenttime, &rawtime);		//Try this later

	strftime(clockTime, 80, "%I:%M:%S ", currentTime);

	return clockTime;
}

char* Session::getDateTime()
{
	time (&rawTime);
	currentTime = localtime(&rawTime);

	strftime(dateTime, 80, "%d %b %Y %I:%M:%S ", currentTime);

	return dateTime;
}

bool Session::connected()
{
	return isConnected;
}

void Session::disconnect()
{
	//CLOSECONNECTION - shuts down the socket and closes any connection on it

	//Close the socket if it exists
	if (sock)
		closesocket(sock);

	cout << "The disconnect function has been called and the connection has been closed" << endl;
	cout << "Here, the user should be asked if he wants to reconnect or quit" << endl;

	closeLog();		//Close the log file

	WSACleanup();	//Clean up Winsock
	exit(1);		//Quit the program
}

void Session::helpMe()
{
	cout << "This is the help function, if you are reading this, that means something went wrong";
	exit(1);
}

int Session::success()
{
	cout << "If you are reading this, that means the program exited correctly";

	return 0;
}

void Session::logFileError()
{
	cout << "Failed to open log file" << endl;
}

//This function is not being used
void Session::quit()
{
	cout << "Press any key to quit. If no key will be pressed the program will automatically stop after 10 seconds";
}

