#ifndef Misc_h__
#define Misc_h__

#include <iostream>

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
#endif // Misc_h__