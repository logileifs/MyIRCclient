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

	//Misc functions
	void helpme();

	int success();

	void logfileerror();

//This function is not being used
void quit();


#pragma endregion Class Functions


};

//#endif // Session_h__
