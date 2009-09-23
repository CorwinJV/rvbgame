#ifndef FUZZYGRAPHSTORAGE_H
#define FUZZYGRAPHSTORAGE_H

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include "fuzzyGraph.h"
#include "desirabilityMatrix.h"
#include "fuzzyInput.h"

class fuzzyGraphStorage
{
public:
	fuzzyGraphStorage();
	~fuzzyGraphStorage();
	void addType(string graphKey, double low, double medStart, double medEnd, double high);							// Registers a decision factor's graph, which is used to make fuzzy decisions!
	void addDesirabilityMatrix(string matrixKey, desirabilityMatrix mtx);											// Registers a desirabilityMatrix, which is used to qualify our data during defuzzification
	void tweakFuzzy(string graphKeym, double newLow, double newMedStart, double newMedEnd, double newHigh);			// Tweaks a registered fuzzy graph
	void getFuzzy(string graphKey, double input, double &outputLow, double &outputMed, double &outputHigh);			// Returns the fuzzy outputs for a given input on one registered fuzzy graph
	void brainify(vector<fuzzyInput*> inputs, desirability &outputType, double &outputValue, string matrixKey);		// Brainificates.
	void printGraphDataDEBUG(string graphKey);
	void printMapSizeDEBUG();

private:
	std::map<string, fuzzyGraph*, less<string>> availableGraphs;
	std::map<string, desirabilityMatrix, less<string>> availableDesirabilityMatrices;
};

#endif