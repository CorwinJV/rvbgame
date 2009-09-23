#ifndef DESIRABILITYPAIR_H
#define DESIRABILITYPAIR_H

#include <string>
#include "fuzzy.h"

using namespace std;

class desirabilityPair
{
public:
	desirabilityPair(string graphIdentifier, desirability dRating);
	~desirabilityPair();
	
	string graphID;
	desirability rating;
};

#endif // DESIRABILITYPAIR_H