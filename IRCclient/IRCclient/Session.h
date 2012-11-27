#pragma once

//#ifndef Session_h__
//#define Session_h__

//Library headers
#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <ctime>
#include <string>
#include <fstream>
#include <conio.h>
//#include <thread>		//Maybe test later

//Project headers
//#include "Misc.hpp"

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
		unsigned short int portNr;

	//Winsock variables
		SOCKET sock;			//Socket handle
		sockaddr_in serverAddr;	//Socket address information

	//Handles
//		HANDLE handles[2];
//		HANDLE hStdIn;

	//Bool
		bool isConnected;
		bool startChat;

	//Time variables
		time_t rawTime;
		struct tm * currentTime;

		struct timeval timeout;		//Timeval struct for select()
		timeval tv_sec; // = 3;		//Seconds variable for select()
		timeval tv_usec;//=0;		//Microseconds to maybe use with select()

		char clockTime[80];
		char dateTime[80];

	//Log variables
		ofstream out;
		char * logFile;

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
		void setServer(int argc, char*argv[]);
		void setPort(int argc, char*argv[]);

	//Initialize socket
		void openSocket(char server[], short portNr);

	//Connect and join functions
		void openConnection();
		void getUserInfo(char nick[]);
		void startSession(char data[]);

	//Send and chat
		int receive(char data[]);
		void sendMsg(char msg[]);
		void chat();

	//String parse
		void parseString(char parse[]);
		void parseString(string str, bool &startChat);

	//Log functions
		void openLog();
		void writeLog(string str);			//Put this in the same overloaded function
		void writeLog(string message, char nick[]);		//Put this in the same overloaded function
		void closeLog();

		
	//Time functions
		char* getTime();
		char* getDateTime();

	//Bool functions
		bool connected();

	//Disconnect function
		void disconnect();

	//Misc functions
		void helpMe();
		int success();
		void logFileError();

	//This function is not being used
		void quit();

	#pragma endregion Class Functions
};

//#endif // Session_h__
