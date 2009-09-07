#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <string>
#include <vector>
#include <fstream>
#include <cstring>

using namespace std;

struct levelInfo
{
	int level;
	int levelHighScore;
	int leastAmtCommands;
	int leastAmtInstructs;
};

class playerInfo
{
private:
	string playerName;
	int playerHighestLevel;
	int playerCurrentLevel;
	int maxLevel;
	vector<levelInfo*> playerLevelInfo;

public:
	playerInfo(int newMaxLevel)
	{
		maxLevel = newMaxLevel;
		playerName = "";
		playerHighestLevel = 1;
		playerCurrentLevel = 1;
		
		levelInfo* tempPlayerLevelInfo;
		for(int i = 0; i < maxLevel; i++)
		{
			tempPlayerLevelInfo = new levelInfo;
			tempPlayerLevelInfo->leastAmtCommands = -1;
			tempPlayerLevelInfo->leastAmtInstructs = -1;
			tempPlayerLevelInfo->level = i;
			tempPlayerLevelInfo->levelHighScore = -1;
			playerLevelInfo.push_back(tempPlayerLevelInfo);
		}
	};
	string			getPlayerName(){return playerName;};
	int				getPlayerHighestLevel(){return playerHighestLevel;};
	int				getPlayerCurrentLevel(){return playerCurrentLevel;};
	int				getPlayerLevel(int level){return playerLevelInfo[level]->level;};
	int				getPlayerLevelScore(int level){return playerLevelInfo[level]->levelHighScore;};
	int				getPlayerLeastCmd(int level){return playerLevelInfo[level]->leastAmtCommands;};
	int				getPlayerLeastInst(int level){return playerLevelInfo[level]->leastAmtInstructs;};
	void			setPlayerName(string name){playerName = name;};
	void			setPlayerHighestLevel(int level){playerHighestLevel = level;};
	void			setPlayerCurrentLevel(int level){playerCurrentLevel = level;};
	void			setPlayerLevelInfo(int newLevel, int score, int commands, int instructs)
	{	
		playerLevelInfo[newLevel]->level = newLevel;
		playerLevelInfo[newLevel]->levelHighScore = score;
		playerLevelInfo[newLevel]->leastAmtCommands = commands;
		playerLevelInfo[newLevel]->leastAmtInstructs = instructs;
	};
	void			setPlayerLevelScore(int level, int score)    {playerLevelInfo[level]->levelHighScore = score;};
	void			setPlayerLeastCmd  (int level, int commands) {playerLevelInfo[level]->leastAmtCommands = commands;};
	void			setPlayerLeastInst (int level, int instructs){playerLevelInfo[level]->leastAmtInstructs = instructs;};


};

class profileManager
{	
private:
	vector<playerInfo*> allPlayerInfo;
	playerInfo* tempPlayerInfo;
	int	currentRecord;
	int maxLevel;

public:
	profileManager(int newMaxLevel)
	{
		//allPlayerInfo.size(); = 0;
		currentRecord = -1;
		maxLevel = newMaxLevel;
		tempPlayerInfo = new playerInfo(maxLevel);
		loadAllProfiles();
	};
	void		saveProfile();
	bool		selectProfile(string);
	bool		deleteProfile(string);
	bool		createProfile(string);
	void		loadAllProfiles();


	void		setCurrentRecord(int record){currentRecord = record;};
	string		getPlayerName(){return allPlayerInfo[currentRecord]->getPlayerName();};
	int			getPlayerHighestLevel(){return allPlayerInfo[currentRecord]->getPlayerHighestLevel();};
	int			getPlayerCurrentLevel(){return allPlayerInfo[currentRecord]->getPlayerCurrentLevel();};
	int			getPlayerLevelScore(int level){return allPlayerInfo[currentRecord]->getPlayerLevelScore(level);};
	int			getPlayerLeastCmd(int level){return allPlayerInfo[currentRecord]->getPlayerLeastCmd(level);};
	int			getPlayerLeastInst(int level){return allPlayerInfo[currentRecord]->getPlayerLeastInst(level);};
	void		setPlayerName(string name){allPlayerInfo[currentRecord]->setPlayerName(name);};
	void		setPlayerHighestLevel(int level){allPlayerInfo[currentRecord]->setPlayerHighestLevel(level);};
	void		setPlayerCurrentLevel(int level){allPlayerInfo[currentRecord]->setPlayerCurrentLevel(level);};
	void		setPlayerLevelInfo(int level, int score, int commands, int instructs)
	{
		allPlayerInfo[currentRecord]->setPlayerLevelInfo(level, score, commands, instructs);
	};
	void		setPlayerLevelScore(int level, int score){allPlayerInfo[currentRecord]->setPlayerLevelScore(level, score);};
	void		setPlayerLeastCmd(int level, int command){allPlayerInfo[currentRecord]->setPlayerLeastCmd(level, command);};
	void		setPlayerLeastInst(int level, int instructs){allPlayerInfo[currentRecord]->setPlayerLeastInst(level, instructs);};
	int			getMaxRecords(){return allPlayerInfo.size();};
	int			getPlayerTotalScore();
};


#endif