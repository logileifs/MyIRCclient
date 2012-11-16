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

	//Bool
		bool isConnected;

	//Time variables
		time_t rawTime;
		struct tm * currentTime;
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

	//	bool prefix;
	//	bool command;

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
		void charParse(char parse[]);
		void stringParse(string str, bool &startChat);

	//Log functions
		void openLog();
		void clientLog(char message[]);
		void serverLog(char data[]);
		void newLog(string str);
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
