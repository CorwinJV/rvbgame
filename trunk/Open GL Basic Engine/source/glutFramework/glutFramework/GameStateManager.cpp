#include "GameStateManager.h"
#include <iostream>

GameStateManager::GameStateManager()//(Game game)        : base(game)
{
	Initialize();
}

bool GameStateManager::Initialize()
{
    // TODO: Add your initialization code here
	stateCount = -1;
	numStates = 0;

	//why is this here? - so the gamestate knows who its parent gamestate manager is
	GameState* tempGS = new GameState(*this, stateCount);

	return true;
	// if something bad happens, return false
}

bool GameStateManager::removeGameStateAt(int index)
{
	if (index < 0) // make sure we have an index that's at least position 0
		return false;

	// erase the item at this index
	vector<GameState*>::iterator itr = stateList.begin();
	for(int i = 0; i < index; i++)
	{
		itr++;
		if(itr >= stateList.end()) // lets also make sure that we're working within the confines of the vector
		{
			return false;
		}
	}

	delete (*itr);
	stateList.erase(itr);

	numStates--;

	return true;
	// if something bad happens, return false
}

bool GameStateManager::removeGameStateID(int nukeID)
{
	if (nukeID < 0) // make sure we have an index that's at least position 0
		return false;

	// erase the item at this index
	vector<GameState*>::iterator itr = stateList.begin();
	for (; itr != stateList.end(); itr++)
    {
		if((*itr)->getID() == nukeID)
		{
			delete (*itr);
			stateList.erase(itr);
			numStates--;
			return true; // item is found and erased
		}
    }

	return false; // item wasn't found
}

bool GameStateManager::removeTopGameState()
{
	stateList.pop_back();
	return true;
	// if something bad happens, return false
}


bool GameStateManager::Update()
{
	//=======================================
	// Update Game States
	//=======================================
	vector<GameState*>::iterator itr = stateList.begin();
	if(!stateList.empty())
		if((stateList.back()->getStatus() != Active) && (stateList.back()->getStatus() != DeleteMe))
		{
			//std::cout << "top state was not active, setting it to active" << endl;
			stateList.back()->setStatus(Active);
		}

	// update statelist
	for (; itr != stateList.end(); itr++)
    {
#ifdef GSMStatus
		std::cout << "GSM - State ID : " << (*itr)->getID() << " status is ";
		switch((*itr)->getStatus())
		{
			case 0:			std::cout << " Active ";			break;	
			case 1:			std::cout << " Passive ";			break;	
			case 2:			std::cout << " Hidden ";			break;
			case 3:			std::cout << " DeleteMe ";			break;
			default:		std::cout << " INVALID STATE ";		break;
		}			
		std::cout << endl;
#endif

		if((*itr)->getStatus() == Active)
		{
			(*itr)->Update();
#ifdef GSMStatus
			std::cout << "GSM - State ID : " << (*itr)->getID() << " has been updated" << endl;
#endif

		}
    }

	//=======================================
	// Delete Pending Game States
	//=======================================
	// Instead of using the same iterator to delete, let's use a reverse_iterator
	vector<GameState*>::reverse_iterator rItr = stateList.rbegin();
	for(; rItr != stateList.rend(); rItr++)
	{
		if((*rItr)->getStatus() == DeleteMe)
		{
#ifdef GSMWork
			std::cout << "GSM - State ID : " << (*rItr)->getID() << " is about to be deleted" << endl;
#endif
			delete(*rItr);
			stateList.erase(rItr.base() - 1);
			numStates--;
			break;
		}
	}

	//itr = stateList.end();

	//if(stateList.empty() == false)
	//	itr--;

	//// delete any states flagged for deletion
	//for (; itr != stateList.begin(); itr--)
 //   {
	//	if((*itr)->getStatus() == DeleteMe)
	//	{
	//		std::cout << "GSM - State ID : " << (*itr)->getID() << " is about to be deleted" << endl;
	//		delete (*itr);
	//		stateList.erase(itr);
	//		numStates--;
	//		break;
	//	}
 //   }

	//=======================================
	// Add Pending Game States
	//=======================================
	itr = statesToAdd.begin();
	for(; itr != statesToAdd.end(); itr++)
	{
		stateList.push_back((*itr));
		numStates++;
	}
	statesToAdd.clear();

	return true;
	// if something bad happens, return false
}

bool GameStateManager::Draw()//(GameTime gameTime)
{
    // TODO: Add your update code here
	vector<GameState*>::iterator itr = stateList.begin();

	for (; itr != stateList.end(); itr++)
    {
		if((*itr)->getStatus() <= Passive)
		{
			(*itr)->Draw();
#ifdef GSMStatus
			std::cout << "GSM - State ID : " << (*itr)->getID() << " has been drawn" << endl;
#endif
		}
    }

	return true;
	// if something bad happens, return false
}

bool GameStateManager::setState(int stateID, State newStatus = Active)
{
	if (stateID < 0) // make sure we have an index that's at least position 0
		return false;

	vector<GameState*>::iterator itr = stateList.begin();
	for (; itr != stateList.end(); itr++)
    {
		if((*itr)->getID() == stateID)
		{
			(*itr)->setStatus(newStatus);
			return true;
		}
    }
	return false; // item wasn't found
}

int GameStateManager::getStateCount()
{
	return stateCount;
}

int GameStateManager::getNumStates()
{
	return numStates;
}

bool GameStateManager::setList(vector<GameState*> newlist)
{
	// delete stateList; // ?? not sure if this is needed or not
	stateList = newlist;
	return true;
}

void GameStateManager::processMouse(int x, int y)
{
	//=======================================
	// Update Mouse States
	//=======================================
	vector<GameState*>::iterator itr = stateList.begin();

	// update statelist
	for (; itr != stateList.end(); itr++)
    {
		if((*itr)->getStatus() == Active)
			(*itr)->processMouse(x, y);
	}			
}

void GameStateManager::processMouseClick(int button, int state, int x, int y)
{
	//=======================================
	// Update Mouse States
	//=======================================
	vector<GameState*>::iterator itr = stateList.begin();

	// update statelist
	for (; itr != stateList.end(); itr++)
    {
		if((*itr)->getStatus() == Active)
			(*itr)->processMouseClick(button, state, x, y);
	}	
}

void GameStateManager::keyboardInput(unsigned char c, int x, int y)
{
	vector<GameState*>::iterator itr = stateList.begin();

	// update statelist
	for (; itr != stateList.end(); itr++)
    {
		if((*itr)->getStatus() == Active)
			(*itr)->keyboardInput( c,  x,  y);
	}	
}

GameStateManager::~GameStateManager()
{
	vector<GameState*>::iterator itr = stateList.begin();

	// delete buttonlist contents
	for (; itr != stateList.end(); itr++)
    {
		(*itr)->~GameState();
    }
	stateList.clear();
}

void GameStateManager::setAllButTopPassive()
{
	vector<GameState*>::iterator itr = stateList.begin();

	// update statelist
	for (; itr != stateList.end()-1; itr++)
    {
		(*itr)->setStatus(Passive);
	}
}