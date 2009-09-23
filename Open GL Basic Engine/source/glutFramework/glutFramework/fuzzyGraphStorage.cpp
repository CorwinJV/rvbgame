#include "fuzzyGraphStorage.h"

fuzzyGraphStorage::fuzzyGraphStorage()
{

}

fuzzyGraphStorage::~fuzzyGraphStorage()
{

}

void fuzzyGraphStorage::addType(string graphKey, double low, double medStart, double medEnd, double high)
{
	// Try to find the value that matches the given key
	// if the return of availableGraphs.find(graphKey) == availableGraphs.end()
	// then the element doesn't exist in the map.
	// If it exists, we need to delete it and replace it with a new graph.
	map<string, fuzzyGraph*, less<string>>::iterator itr;
	itr = availableGraphs.find(graphKey);
	if(itr != availableGraphs.end())
	{
		delete (*itr).second;
	}
	
	availableGraphs[graphKey] = new fuzzyGraph(low, medStart, medEnd, high);
}

void fuzzyGraphStorage::addDesirabilityMatrix(string matrixKey, desirabilityMatrix mtx)
{	
	availableDesirabilityMatrices[matrixKey] = mtx;
}


void fuzzyGraphStorage::tweakFuzzy(string graphKey, double newLow, double newMedStart, double newMedEnd, double newHigh)
{
	// Try to find the value that matches the given key
	// if the return of availableGraphs.find(graphKey) == availableGraphs.end()
	// then the element doesn't exist in the map.
	// If it exists, we tweak the values.
	map<string, fuzzyGraph*, less<string>>::iterator itr;
	itr = availableGraphs.find(graphKey);
	if(itr != availableGraphs.end())
	{
		(*itr).second->tweakFuzzyGraph(newLow, newMedStart, newMedEnd, newHigh);
	}
}

void fuzzyGraphStorage::getFuzzy(string graphKey, double input, double &outputLow, double &outputMed, double &outputHigh)
{
	// Try to find the value that matches the given key
	// if the return of availableGraphs.find(graphKey) == availableGraphs.end()
	// then the element doesn't exist in the map.
	// If it exists, we return the values.
	map<string, fuzzyGraph*, less<string>>::iterator itr;
	itr = availableGraphs.find(graphKey);
	if(itr != availableGraphs.end())
	{
		(*itr).second->getFuzzyData(input, outputLow, outputMed, outputHigh);
	}
}

void fuzzyGraphStorage::brainify(vector<fuzzyInput*> inputs, desirability &outputType, double &outputValue, string matrixKey)
{
	//=================================
	// Find the highest vals for 
	// undesirable, desirable, and 
	// very desirable.
	vector<vector<desirabilityPair*>>* currentDesirabilityList = NULL;
	vector<vector<desirabilityPair*>>::iterator dListItr;
	vector<desirabilityPair*>::iterator innerDListItr;

	// Find the highest "undesirable"
	currentDesirabilityList = availableDesirabilityMatrices[matrixKey].getDesirabilityCombos(LOW);
	dListItr = currentDesirabilityList->begin();
	double highestLow = 0.0;
	for(; dListItr != currentDesirabilityList->end(); dListItr++)
	{
		// Iterate through each "combination" of pairs
		innerDListItr = (*dListItr).begin();
		double runningTotal = 1.0;
		for(; innerDListItr != (*dListItr).end(); innerDListItr++)
		{
			// All of the elements we find in here are considered desirability pairs
			string graphName = (*innerDListItr)->graphID;
			double graphsFuzzyValue = availableGraphs[graphName]->getFuzzyDataPortion((*innerDListItr)->rating);
			runningTotal = FuzzyAND(runningTotal, graphsFuzzyValue);
		}
		// We've reached the end of a "combination" of pairs
		// Store the highest value for each combination as highestLow
		highestLow = FuzzyOR(highestLow, runningTotal);
	}

	// Find the highest "desirable"
	currentDesirabilityList = availableDesirabilityMatrices[matrixKey].getDesirabilityCombos(MEDIUM);
	

	// Find the highest "very desirable"
	currentDesirabilityList = availableDesirabilityMatrices[matrixKey].getDesirabilityCombos(HIGH);


	//// fuzzy data to be stored
	//desirability closeLow = ok;
	//desirability closeMedium = very;
	//desirability closeLots = very;
	//desirability mediumLow = bad;
	//desirability mediumMedium = ok;
	//desirability mediumLots = ok;
	//desirability farLow = bad;
	//desirability farMedium = bad;
	//desirability farLots = ok;
	//
	//availableDesirabilityMatrices[matrixKey] == what's above

	// The combined values for given decision factors
	//double f_closeLow = FuzzyAND(r_fuzzyClose, a_fuzzyLow);
	//double f_closeMedium = FuzzyAND(r_fuzzyClose, a_fuzzyMedium);
	//double f_closeLots = FuzzyAND(r_fuzzyClose, a_fuzzyLots);
	//double f_mediumLow = FuzzyAND(r_fuzzyMedium, a_fuzzyLow);
	//double f_mediumMedium = FuzzyAND(r_fuzzyMedium, a_fuzzyMedium);
	//double f_mediumLots = FuzzyAND(r_fuzzyMedium, a_fuzzyLots);
	//double f_farLow = FuzzyAND(r_fuzzyFar, a_fuzzyLow);
	//double f_farMedium = FuzzyAND(r_fuzzyFar, a_fuzzyMedium);
	//double f_farLots = FuzzyAND(r_fuzzyFar, a_fuzzyLots);

	//// Corwin's magicification
	//std::vector<desirability> desirabilityList;
	//std::vector<double> matchedFuzzifiedOutput;

	//desirabilityList.push_back(closeLow);
	//desirabilityList.push_back(closeMedium);
	//desirabilityList.push_back(closeLots);
	//desirabilityList.push_back(mediumLow);
	//desirabilityList.push_back(mediumMedium);
	//desirabilityList.push_back(mediumLots);
	//desirabilityList.push_back(farLow);
	//desirabilityList.push_back(farMedium);
	//desirabilityList.push_back(farLots);

	//matchedFuzzifiedOutput.push_back(f_closeLow);
	//matchedFuzzifiedOutput.push_back(f_closeMedium);
	//matchedFuzzifiedOutput.push_back(f_closeLots);
	//matchedFuzzifiedOutput.push_back(f_mediumLow);
	//matchedFuzzifiedOutput.push_back(f_mediumMedium);
	//matchedFuzzifiedOutput.push_back(f_mediumLots);
	//matchedFuzzifiedOutput.push_back(f_farLow);
	//matchedFuzzifiedOutput.push_back(f_farMedium);
	//matchedFuzzifiedOutput.push_back(f_farLots);

	//
	//// Corwin's determination of defuzification
	//double highestUndesirable = 0.0;
	//double highestDesirable = 0.0;
	//double highestVDesirable = 0.0;

	//for(int j = 0; j < 8; j++)
	//{
	//	switch(desirabilityList[j])
	//	{
	//	case 0: // Undesirable
	//		highestUndesirable = FuzzyOR(highestUndesirable, matchedFuzzifiedOutput[j]);
	//		break;
	//	case 1: // Desirable
	//		highestDesirable = FuzzyOR(highestDesirable, matchedFuzzifiedOutput[j]);
	//		break;
	//	case 2:
	//		highestVDesirable = FuzzyOR(highestVDesirable, matchedFuzzifiedOutput[j]);
	//		break;
	//	}
	//	/*cout << "J = " << j << endl;
	//	cout << "Highest Undesireable = " << highestUndesirable << endl;
	//	cout << "Highest Desireable = " << highestDesirable << endl;
	//	cout << "Highest Very Desireable = " << highestVDesirable << endl << endl;
	//	cout << "matchedFuzzifiedOutput[" << j << "] = " << matchedFuzzifiedOutput[j] << endl << endl;;*/
	//}


	//// defuzzy variables
	//double badStart = 33;
	//double badEnd = 50;
	//double okStart = 33;
	//double okMiddle = 50;
	//double okEnd = 66;
	//double veryStart = 50;
	//double veryEnd = 66;

	////FuzzyGrade(distance, veryStart, veryEnd);
	////FuzzyReverseGrade(distance, badStart, badEnd);
	////FuzzyTriangle(distance, okStart, okMiddle, okEnd);

	//double defuzzified[4][101];
	//double best = 0.0;

	///*cout << "Current Ammo = " << currentAmmo << " | Distance = " << distance << endl;
	//cout << "Highest Undesireable = " << highestUndesirable << endl;
	//cout << "Highest Desireable = " << highestDesirable << endl;
	//cout << "Highest Very Desireable = " << highestVDesirable << endl << endl;
	//system("pause");*/

	//for(int i = 0; i < 100; i++)
	//{
	//	defuzzified[0][i] = FuzzyReverseGrade(i+1, badStart, badEnd) ;
	//	defuzzified[1][i] = FuzzyTriangle(i+1, okStart, okMiddle, okEnd) ;
	//	defuzzified[2][i] = FuzzyGrade(i+1, veryStart, veryEnd) ;
	//	defuzzified[3][i] = ((defuzzified[0][i])* highestUndesirable) + 
	//						((defuzzified[1][i])* highestDesirable) + 
	//						((defuzzified[2][i])* highestVDesirable);
	//	/*cout << "Distance " << i << " : " << 
	//			" Undesireable = " << defuzzified[0][i] << 
	//			" Desireable = " << defuzzified[1][i]<< 
	//			" Very Desireable = " << defuzzified[2][i]<< 
	//			" Defuzzified = " << defuzzified[3][i] << endl;*/
	//	best = FuzzyOR(best, defuzzified[3][i]);

	//	//* highestVDesirable
	//	//* highestDesirable
	//	//* highestUndesirable
	//}

	////cout << best << endl;
	//
	////defuzzify some stuff that we just fuzzied up
	//return best;
}

void fuzzyGraphStorage::printGraphDataDEBUG(string graphKey)
{
	// Try to find the value that matches the given key
	// if the return of availableGraphs.find(graphKey) == availableGraphs.end()
	// then the element doesn't exist in the map.
	// If it exists, we cout the data.
	map<string, fuzzyGraph*, less<string>>::iterator itr;
	itr = availableGraphs.find(graphKey);
	if(itr != availableGraphs.end())
	{
		cout << "Graph Name: " << (*itr).first << endl;
		(*itr).second->printDEBUG();
		cout << endl << endl;
	}
}

void fuzzyGraphStorage::printMapSizeDEBUG()
{
	cout << "myFuzzyGraphStore element count: " << availableGraphs.size();
}