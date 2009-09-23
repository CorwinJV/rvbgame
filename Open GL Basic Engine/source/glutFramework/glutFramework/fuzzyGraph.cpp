#include "fuzzyGraph.h"

fuzzyGraph::fuzzyGraph(double low, double medStart, double medEnd, double high)
{
	lowStart = low;
	lowEnd = medStart;
	mediumStart = low;
	mediumMidStart = medStart;
	mediumMidEnd = medEnd;
	mediumEnd = high;
	highStart = medEnd;
	highEnd = high;
}

void fuzzyGraph::printDEBUG()
{
	cout << "lowStart: " << lowStart << endl;
	cout << "lowEnd: " << lowEnd << endl;
	cout << "mediumStart: " << mediumStart << endl;
	cout << "mediumMidStart: " << mediumMidStart << endl;
	cout << "mediumMidEnd: " << mediumMidEnd << endl;
	cout << "mediumEnd: " << mediumEnd << endl;
	cout << "highStart: " << highStart << endl;
	cout << "highEnd: " << highEnd << endl;
}

void fuzzyGraph::getFuzzyData(double input, double &outputLow, double &outputMed, double &outputHigh)
{
	outputLow = FuzzyReverseGrade	(input, lowStart, lowEnd);
	outputMed = FuzzyTrapezoid		(input, mediumStart, mediumMidStart, mediumMidEnd, mediumEnd);
	outputHigh = FuzzyGrade			(input, highStart, highEnd);
}

void fuzzyGraph::tweakFuzzyGraph(double newLow, double newMedStart, double newMedEnd, double newHigh)
{
	lowStart = newLow;
	lowEnd = newMedStart;
	mediumStart = newLow;
	mediumMidStart = newMedStart;
	mediumMidEnd = newMedEnd;
	mediumEnd = newHigh;
	highStart = newMedEnd;
	highEnd = newHigh;
}

