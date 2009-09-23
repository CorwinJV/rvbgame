#include "desirabilityMatrix.h"

void desirabilityMatrix::addDesirabilityCombo(vector<desirabilityPair*> comboToAdd, desirability rating)
{
	vector<vector<desirabilityPair*>>* vectorToAppendTo = NULL;
	switch(rating)
	{
	case LOW:
		vectorToAppendTo = &low;
		break;
	case MEDIUM:
		vectorToAppendTo = &medium;
		break;
	case HIGH:
		vectorToAppendTo = &high;
		break;
	}
	vectorToAppendTo->push_back(comboToAdd);
}

const vector<vector<desirabilityPair*>> & desirabilityMatrix::getDesirabilityCombos(desirability rating)
{
	switch(rating)
	{
	case LOW:
		return low;
		break;
	case MEDIUM:
		return medium;
		break;
	case HIGH:
		return high;
		break;
	default:
		return defaultReturn;
		break;
	}
	//return NULL;
}
