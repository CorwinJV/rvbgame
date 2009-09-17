#include "RPSEntity.h"

RPSEntity::RPSEntity()
{
	generateBasicHandTables();
	shortTermMemory.lastHand = RPSUNKNOWN;
	shortTermMemory.twoAgo = RPSUNKNOWN;
	shortTermMemory.predictedHand = RPSUNKNOWN;
	handsPlayed = 0;
	handsCorrect = 0;
}

RPSEntity::~RPSEntity()
{
}


void RPSEntity::generateBasicHandTables()
{
	RPSHistory* tempHistory;
	// two hands ago...
	for(int x = 0; x < 3; x++)
	{
		// last hand
		for(int y = 0; y < 3; y++)
		{
			// predicted hand
			for(int z = 0; z < 3; z++)
			{		
				tempHistory = new RPSHistory();
				tempHistory->twoAgo			= (RPSHand)x;
				tempHistory->lastHand		= (RPSHand)y;
				tempHistory->predictedHand	= (RPSHand)z;
				tempHistory->weight			= 0.5;
				myHandKnowledge.push_back(tempHistory);
			}
		}
	}
}

void RPSEntity::opponentPlayedHand(RPSHand newHand)
{
	handsPlayed++;
	// time to update weights based on the last two hands and what was actually played
	vector<RPSHistory*>::iterator iHistory = myHandKnowledge.begin();
	if(newHand == shortTermMemory.predictedHand)
	{
		handsCorrect++;
		//cout << "Woo! I predicted it right! That's " << handsCorrect << " out of " << handsPlayed << " correct!" << endl;
		//cout << "Total Percent Correct = " << (double)((double)handsCorrect/(double)handsPlayed) << endl;
	}
	for(;iHistory < myHandKnowledge.end(); iHistory++)
	{
		if( (*iHistory)->twoAgo == shortTermMemory.twoAgo)
		{
			if( (*iHistory)->lastHand == shortTermMemory.lastHand)
			{
				if( (*iHistory)->predictedHand == newHand)
				{
					(*iHistory)->weight += 0.05;
				}
				else
				{
					(*iHistory)->weight -= 0.1;
					if((*iHistory)->weight < 0)
					{
						(*iHistory)->weight = 0;
					}
				}
			}
		}
	}
	
	// now lets roll stuff around
	shortTermMemory.twoAgo = shortTermMemory.lastHand;
	shortTermMemory.lastHand = newHand;

	// and lets make a prediction!
	double rockWeight = 0.0;
	double paperWeight = 0.0;
	double scissorsWeight = 0.0;

	iHistory = myHandKnowledge.begin();
	for(;iHistory < myHandKnowledge.end(); iHistory++)
	{
		if( (*iHistory)->twoAgo == shortTermMemory.twoAgo)
		{
			if( (*iHistory)->lastHand == shortTermMemory.lastHand)
			{
				if( (*iHistory)->predictedHand == RPSROCK)
				{
					rockWeight = (*iHistory)->weight;
				}
				else if ( (*iHistory)->predictedHand == RPSPAPER)
				{
					paperWeight = (*iHistory)->weight;
				}
				else if ( (*iHistory)->predictedHand == RPSSCISSORS)
				{
					scissorsWeight = (*iHistory)->weight;
				}
				
			}
		}
	}

	double bestWeight = 0.0;
	RPSHand bestHand = RPSUNKNOWN;

	if(rockWeight > bestWeight)
	{
		bestWeight = rockWeight;
		bestHand = RPSROCK;
	}
	if(paperWeight > bestWeight)
	{
		bestWeight = paperWeight;
		bestHand = RPSPAPER;
	}
	if(scissorsWeight > bestWeight)
	{
		bestWeight = scissorsWeight;
		bestHand = RPSSCISSORS;
	}
	
	//cout << "-------------------------------------------------------" << endl;
	//cout << "Rock = " << rockWeight << " | Paper = " << paperWeight << " | Scissors = " << scissorsWeight << endl;
	//cout << "Best was " << bestWeight << " and " << bestHand << endl;
	shortTermMemory.predictedHand = bestHand;
	shortTermMemory.weight = bestWeight;

}

RPSHand RPSEntity::getNextHand()
{
	RPSHand ourPrediction;
	// if we have a prediction, send it out
	if(shortTermMemory.predictedHand != RPSUNKNOWN)
	{
		ourPrediction = shortTermMemory.predictedHand;
	}
	// otherwise lets just guess blindly heh
	else
	{
		ourPrediction = (RPSHand)(rand()%3);
	}

	// and now since we want to win..
	// if we predict they will play scissors, we need to play rock, otherwise we will play whatever they are playing +1;
	if(ourPrediction == RPSSCISSORS)
	{
		return RPSROCK;
	}
	else
	{
		return ((RPSHand)((int)ourPrediction + 1));
	}
}


