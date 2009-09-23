#ifndef FUZZYGRAPHSTORAGE_H
#define FUZZYGRAPHSTORAGE_H

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include "fuzzyGraph.h"
#include "fuzzyInput.h"


class fuzzyGraphStorage
{
public:
	fuzzyGraphStorage();
	~fuzzyGraphStorage();
	void addType(string graphKey, double low, double medStart, double medEnd, double high);
	void tweakFuzzy(string graphKeym, double newLow, double newMedStart, double newMedEnd, double newHigh);
	void getFuzzy(string graphKey, double input, double &outputLow, double &outputMed, double &outputHigh);
	void brainify(vector<fuzzyInput*> inputs, desirability &outputType, double &outputValue);
	void printGraphDataDEBUG(string graphKey);
	void printMapSizeDEBUG();

private:
	std::map<string, fuzzyGraph*, less<string>> availableGraphs;

};

#endif