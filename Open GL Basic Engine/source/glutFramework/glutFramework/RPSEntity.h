#ifndef RPSENTITY_H
#define RPSENTITY_H
#include <iostream>
#include "oglGameVars.h"
#include <vector>

enum RPSHand 
{
	RPSUNKNOWN = -1,
	RPSROCK,
	RPSPAPER,
	RPSSCISSORS
};

struct RPSHistory
{
public:
	RPSHand lastHand;
	RPSHand twoAgo;
	RPSHand predictedHand;
	double weight;
};

class RPSEntity
{
private:
	void generateBasicHandTables();
	int handsPlayed;
	int handsCorrect;

	vector<RPSHistory*> myHandKnowledge;

	// this will be used for matching
	RPSHistory	shortTermMemory;	
	
public:
	RPSEntity();
	~RPSEntity();

	void opponentPlayedHand(RPSHand);
	RPSHand getNextHand();

};
#endif