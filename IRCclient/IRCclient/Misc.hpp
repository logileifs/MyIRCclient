#ifndef Misc_h__
#define Misc_h__

#include <iostream>
#include <time.h>
#include <ctime>

using namespace std;

void helpme()
{
	cout << "This is the help function, if you are reading this, that means something went wrong";
	exit(1);
}

int success()
{
	cout << "If you are reading this, that means the program exited correctly";

	return 0;
}

void logfileerror()
{
	cout << "Failed to open log file" << endl;
}

//This function is not being used
void quit()
{
	cout << "Press any key to quit. If no key will be pressed the program will automatically stop after 10 seconds";
}
#endif // Misc_h__