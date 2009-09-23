#ifndef RVB_FUZZYGRAPH_H
#define RVB_FUZZYGRAPH_H

#include <string>
#include <iostream>
#include "fuzzy.h"

using namespace std;

class fuzzyGraph
{
private:
	double lowStart;
	double lowEnd;
	double mediumStart;
	double mediumMidStart;
	double mediumMidEnd;
	double mediumEnd;
	double highStart;
	double highEnd;


public:
	fuzzyGraph(double low, double medStart, double medEnd, double high);
	~fuzzyGraph() {};
	void tweakFuzzyGraph(double newLow, double newMedStart, double newMedEnd, double newHigh);
	void getFuzzyData(double input, double& outputLow, double& outputMed, double& outputHigh);
	void printDEBUG();
};

#endif // RVB_FUZZYGRAPH_H