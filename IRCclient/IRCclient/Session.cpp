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

	//Initialize time variables
	timeout.tv_sec = 1;								//1 second timeval variable, currently not used
	timeout.tv_usec = 0;
}

void Session::setServer(int argc, char*argv[])
{
	server = new char[];							//Create a new dynamic array
	server = argv[1];								//Set first argument as the server
}

void Session::setPort(int argc, char*argv[])
{
	port = new char[];								//Create a new dynamic array
	port = argv[2];									//Set the second argument as the port

	portNr = (unsigned short) strtoul(port, NULL, 0);	//Cast the port number from char to unsigned short int
}

void Session::openSocket(char server[], short portnr)
{
	WSADATA wsaDATA;								//Start up Winsock
	int result;										//Create an int variable for result

	result = WSAStartup(MAKEWORD(2,2),&wsaDATA);	//Check if winsock started up correctly
	if(result != 0)
	{
		printf("WSAStartup failed: %d\n", result);	//Print out an error message
		WSACleanup();								//Clean up winsock
		exit(1);									//Exit the program
	}

	hostent *host;									//Create a new hostent struct

	host = (hostent *) gethostbyname((char *) server);		//Host address
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);		//Create socket

	if(sock == INVALID_SOCKET)								//Check if a valid socket has been opened
	{
		cerr << sock << endl;								//Print out an error message
		cout << "ERROR, can't open socket\n";				//Print out another error message
		exit(1);											//Exit the program
	}

	serverAddr.sin_family = AF_INET;							//address family Internet
	serverAddr.sin_port = htons(portNr);						//Port to connect on
	serverAddr.sin_addr = *((struct in_addr *) host->h_addr);	//Target address
	memset(&(serverAddr.sin_zero), 0, 8);						//Set the first 8 items of serverAddr array as 0

}

void Session::openConnection()
{
	if(connect(sock, (sockaddr *)&serverAddr, sizeof(serverAddr)) == 0)		//Open a TCP connection
		isConnected = true;													//If it opened set isConnected as true
	else
		isConnected = false; 												//Else set isConnected as false
}

void Session::getUserInfo(char nick[])
{
	cout << "Enter your nickname: ";
	cin.get(nick, 9);					//Get nickname from input, no longer than 9 characters
	cin.ignore(strlen(nick), '\n');		//Clear input buffer

	cout << "Your nickname is: " << nick << endl;	//Print out user's nickname
}

void Session::startSession(char data[])
{
	string str;			//Create a new string variable for output

	openLog();			//Open log file for the session

	getUserInfo(nick);	//Get user's nickname

	parseString(NICK);	//Create the initial NICK command
	parseString(USER);	//Create the USER command

	sendMsg(CAP);		//Send CAP command
	sendMsg(NICK);		//Send NICK command
	sendMsg(USER);		//Send USER command
	sendMsg(CAPREQ);	//Send CAPREQ command
	sendMsg(CAPEND);	//Send CAPEND command

	do
	{
		do
		{
			receive(data);						//Receive data from server
			str.append(data);					//Add data to the output string
		}while(strncmp(data, "\r\n", 2)!=0);	//While the server is not sending \r\n the loop will go on forever
		cout << str;							//Print out the string received from server
		
		writeLog(str);							//Write the string to the log file

		parseString(str, startChat);			//Parse the string
		str.clear();							//Clear the string

		if(startChat == true)					//Check if connection has been established
			chat();								//Start the chat

	}while(bytes != 0);							//Add timer/timeout function
}

int Session::receive(char data[])
{
	bytes = recv(sock, data, strlen(data), 0);		//Receive data from open socket

	return bytes;
}

void Session::sendMsg(char msg[])
{
	send(sock, msg, strlen(msg), 0);				//Send message to open socket
}

void Session::chat()
{
	HANDLE handles[10];										//Create 10 handles
	HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);			//set hStdIn as an input handle

	handles[0] = WSACreateEvent();							//Set handle 0 as an event
	handles[1] = hStdIn;									//Set handle 1 as an input event
	WSAEventSelect(sock, handles[0], FD_READ | FD_CLOSE);	//Set handle 1 event as active or inactive socket

	DWORD result, bytesRead;								//DWORD variables for result and bytesRead

	string str;												//Create a string variable for output
	cout << endl;
	cout << "Standing by for input";

	while(connected())												//While the TCP connection is open the loop goes on
	{
		result = WaitForMultipleObjects(2, handles, false, 250);	//Wait for 250 msec and get a signal from handles

		if (result == WAIT_OBJECT_0+1)								//Check for input signal
		{
			cout << "\nMessage: ";
			cin.get(message, 510);									//Get message to send from keyboard
			cout << getTime() << message << endl;					//Print out time and message sent
			parseString(message);									//Parse the message
			sendMsg(message);										//Send last message
			writeLog(message, nick);								//Send last message to writeLog function

			cin.clear();											//Clear input buffer
			cin.ignore(512, '\n');									//Ignore all newline characters
		}
		
		else if(result == WAIT_OBJECT_0)							//Check for socket signal
		{
			memset(&data, 0, BUFF);									//Clear data array
			bytesRead = recv(sock, data, BUFF, 0);					//Receive data from server
			str.append(data);										//Add data to output string
			checkData(data);										//Parse incoming data from server
			cout << str;											//Print out string
			str.clear();											//Clear the string
			WSAResetEvent(handles[0]);								//Reset socket handle
		}
	}
}

void Session::checkData(char msg[])
{

	char PONG[1024];	// Initializes PONG array to reply PING server request
	PONG[0] = 'P';
	PONG[1] = 'O';
	PONG[2] = 'N';
	PONG[3] = 'G';
	PONG[4] = '\r';
	PONG[5] = '\n';
	PONG[6] = '\0';


	for(int i = 0; i < strlen(msg); i++)
		if (msg[i] == 'P' && msg[i+1] == 'I' && msg[i+2] == 'N' && msg[i+3] == 'G'){  // if ping is found , PONG is replied
				sendMsg(PONG);
				memset(&PONG, 0, 1024); // clears the array to stop resend of PONG's
			}
			
	char myVersion[1024];
	char nickV[20];
	string ircVersion = "VERSION MyIRCclient alpha 0.01 ";
	int nickLength = 0;
	int countMyV = 8;

	for(int i = 0; i < strlen(msg); i++)
		if(msg[i] == '\001' && msg[i+1] == 'V' && msg[i+2] == 'E' && msg[i+3] == 'R' && msg[i+4] == 'S' && msg[i+5] == 'I' && msg[i+6] == 'O' && msg[i+7] == 'N' && msg[i+8] == '\001' )
	{
		//:ingvar NOTICE yournick :\001VERSION emacsIRC:2.1b:GNU Emacs IRC v2.1b on Linux:FTP.SIP.IS:/pub/emacs/emacsIRC-2.1b.tgz README\001   PRIVMSG
		
		for(int i = 0; msg[i] != '!'; i++, nickLength++)
		{
			nickV[i] = msg[i+1];
		}

		myVersion [0] = 'P';
		myVersion [1] = 'R';
		myVersion [2] = 'I';
		myVersion [3] = 'V';
		myVersion [4] = 'M';
		myVersion [5] = 'S';
		myVersion [6] = 'G';
		myVersion [7] = ' ';
		for(int i=0; i<nickLength-1; i++, countMyV++)
			myVersion[countMyV] = nickV[i];
		myVersion [countMyV] = ' ';
		myVersion [countMyV+1] = ':';
		myVersion [countMyV+2] = '\001';
		countMyV += 3;
		for(int i=0; i<ircVersion.length(); i++, countMyV++)
			myVersion[countMyV] = ircVersion[i];
		myVersion [countMyV] = '\001';
		myVersion [countMyV+1] = '\r';
		myVersion [countMyV+2] = '\n';
		myVersion [countMyV+3] = '\0';
		
		sendMsg(myVersion);
		memset(&myVersion, 0, 1024); // clears the array
	}

}

void Session::parseString(char parse[])
{
	if(parse == NICK)
	{
		strcat_s(NICK, nick);	//Place the nickname behind the NICK command
		strcat_s(NICK, CRLF);	//Place the \r\n characters behind the nick
	}

	else if(parse == USER)
	{
		strcat_s(USER, nick);		//Place the nickname behind the USER command
		strcat_s(USER, USERTRAIL);	//Place the USER trail behind the nickname
	}

	else if(parse == message)
	{
		if(strcmp(message, "/quit")==0)		//Check if user has typed in /quit
			disconnect();					//Call the disconnect function

		if(strcmp(message, "ragequit")==0)	//Check if user has typed in ragequit
			disconnect();					//Call the disconnect function

		strcat_s(message, CRLF);			//Add \r\n to end of string
	}
}

void Session::parseString(string str, bool &startChat)
{
	if(str.find("/MOTD command.")!=string::npos)			//Check if /MOTD command. is found in the string
	{
		startChat = true;									//If it's found startChat becomes true
		cout << "Server connection established" << endl;
	}
}

void Session::openLog()
{
	logFile = "irc.log";	//Log file to open is irc.log

	out.open(logFile);		//Open the log file

	if(out.fail())			//Check if the file was opened successfully
		logFileError();		//Call the logFileError function
}

void Session::writeLog(string str)				
{
	out << getDateTime() << str;	//Write to log file date, time and the string received from server
}

void Session::writeLog(string message, char nick[])			
{
	out << endl << getDateTime() << " "  << nick << ": " << message;	//Write to log file date, time, nick and the message
}

void Session::closeLog()
{
	out.close();		//Close the log file
}

char* Session::getTime()
{
	time (&rawTime);									//Get raw time
	currentTime = localtime(&rawTime);					//Convert raw time to local time

	strftime(clockTime, 80, "%I:%M:%S ", currentTime);	//Format the current time string and save it as clockTime

	return clockTime;
}

char* Session::getDateTime()
{
	time (&rawTime);											//Get raw time
	currentTime = localtime(&rawTime);							//Convert raw time to local time

	strftime(dateTime, 80, "%d %b %Y %I:%M:%S ", currentTime);	//Format the current time string and save it as dateTime

	return dateTime;
}

bool Session::connected()
{
	return isConnected;		//Return whether the TCP connection is open or not
}

void Session::disconnect()
{
	if (sock)				//Check if socket exists
		closesocket(sock);	//Close the socket

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
	exit(1);
}

//This function is not being used
void Session::quit()
{
	cout << "Press any key to quit. If no key will be pressed the program will automatically stop after 10 seconds";
}

