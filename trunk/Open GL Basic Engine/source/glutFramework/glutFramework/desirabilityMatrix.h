#ifndef DESIRABILITYMATRIX_H
#define DESIRABILITYMATRIX_H

#include <vector>
#include "desirabilityPair.h"

using namespace std;

class desirabilityMatrix
{
public:
	desirabilityMatrix() {};
	~desirabilityMatrix() {};

	void addDesirabilityCombo(vector<desirabilityPair*>, desirability rating);		// Takes the combination provided and adds it to the vector which corrosponds to rating (low, med, high)
	const vector<vector<desirabilityPair*>> & getDesirabilityCombos(desirability rating);	// Returns a const reference to the list which corrosponds to the given rating

private:
	vector<vector<desirabilityPair*>> low;								// Low list, each element is a list of decision factor combinations that are rated low for this graph
	vector<vector<desirabilityPair*>> medium;							// Medium list, each element is a list of decision factor combinations that are rated medium for this graph
	vector<vector<desirabilityPair*>> high;								// High list, each element is a list of decision factor combinations that are rated high for this graph
	vector<vector<desirabilityPair*>> defaultReturn;					// Used for returning an empty vector in our get function.
};

#endif // DESIRABILITYMATRIX_H