#ifndef FUZZYINPUT_H
#define FUZZYINPUT_H

#include <iostream>

using namespace std;

struct fuzzyInput
{
	fuzzyInput(string id, double val)
	{
		identifier = id;
		value = val;
	}

	string identifier;
	double value;
};

#endif // FUZZYINPUT_H