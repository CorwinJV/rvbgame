#include "RPSGame.h"
#include <string>
#include <sstream>

// init
void RPSGame::init()
{
	CPUPlayer = new RPSEntity();
	lastPlayerHand = RPSUNKNOWN;
	lastCPUHand = RPSUNKNOWN;
	playerScore = 0;
	cpuScore = 0;
	victor = -1;
}

bool RPSGame::Draw()
{
	// make the background white
	glClearColor(255, 255, 255, 0);

	// draw the bg image
	GameVars->RPSBG->drawImage(1024, 768, 0, 0);

	// draw the player buttons at top
	GameVars->RPSRock->drawImage(128, 128, 100, 100);
	GameVars->RPSPaper->drawImage(128, 128, 300, 100);
	GameVars->RPSScissors->drawImage(128, 128, 500, 100);

	oglTexture2D*	tempImage = NULL;

	// draw the player's hand
	switch(lastPlayerHand)
	{
	case RPSROCK:
		tempImage = GameVars->RPSRock;
		break;
	case RPSPAPER:
		tempImage = GameVars->RPSPaper;
		break;
	case RPSSCISSORS:
		tempImage = GameVars->RPSScissors;
		break;
	default:
		tempImage = NULL;
		break;
	}
	if(tempImage != NULL)
	{
		tempImage->drawImage(128, 128, 200, 250);
	}

	// draw the cpu's hand
	switch(lastCPUHand)
	{
	case RPSROCK:
		tempImage = GameVars->RPSRock;
		break;
	case RPSPAPER:
		tempImage = GameVars->RPSPaper;
		break;
	case RPSSCISSORS:
		tempImage = GameVars->RPSScissors;
		break;
	default:
		tempImage = NULL;
		break;
	}
	if(tempImage != NULL)
	{
		tempImage->drawImage(128, 128, 400, 250);
	}


	// draw results of last hand
	switch(victor)
	{
	case 0:
		tempImage = GameVars->cpuWins;
		break;
	case 1:
		tempImage = GameVars->playerWins;
		break;
	case 2:
		tempImage = GameVars->tieHand;
		break;
	default:
		tempImage = NULL;
		break;
	}

	if(tempImage != NULL)
	{
		tempImage->drawImage(128, 128, 300, 500);
	}

	// draw scores
	//cout << "Player Score == " << playerScore << endl;
	//cout << "Computer Score == " << cpuScore << endl;
	stringstream pScore;
	stringstream cScore;
	pScore << "Player: " << playerScore;
	cScore << "CPU: " << cpuScore;

	string pScoreS = pScore.str();
	string cScoreS = cScore.str();

	GameVars->fontArial24.drawText(700, 200, pScoreS);
	GameVars->fontArial24.drawText(700, 250, cScoreS);

	
	return true;
}

void RPSGame::processMouseClick(int button, int state, int x, int y)
{
	
	// we need to process mouse clicking etc
	// if mouse is clicked, 
	if((button == GLUT_LEFT) && (state == GLUT_DOWN))
	{	
		if((y >= 100) && (y <= 228))
		{
			// if you click on rock
			if((x > 100) && (x < 228))
			{
				lastPlayerHand = RPSROCK;
			}
			// if you click on paper
			if((x > 300) && (x < 428))
			{
				lastPlayerHand = RPSPAPER;
			}

			// if you click on scissors
			if((x > 500) && (x < 628))
			{
				lastPlayerHand = RPSSCISSORS;
				
			}
			lastCPUHand = CPUPlayer->getNextHand();
			CPUPlayer->opponentPlayedHand(lastPlayerHand);
		
			bool keepChecking = true;
			// lets figure out who won the hand
			// is player hand scissors
			if(lastPlayerHand == RPSSCISSORS)
			{
			  // is cpu hand rock? 
				if(lastCPUHand == RPSROCK)
				{
				// if so cpu wins
					victor = 0;
					cpuScore++;
					keepChecking = false;
				}
			}
			
			// is cpu hand scissors?
			else if(lastCPUHand == RPSSCISSORS)
			{
			  // is player hand rock?
				if(lastPlayerHand == RPSROCK)
				// if so player wins
				{
					victor = 1;
					playerScore++;
					keepChecking = false;
				}
			}

			// failing both of those, highest wins
			// rock = 0, paper = 1, scissors = 2
			if(keepChecking)
				{
				if(lastCPUHand > lastPlayerHand)
				{
					victor = 0;
					cpuScore++;
				}
				else if(lastPlayerHand > lastCPUHand)
				{
					victor = 1;
					playerScore++;
				}
				else if(lastPlayerHand == lastCPUHand)
				{
					victor = 2;
				}
				else
				{
					cout << "RPSGame: we somehow have a non winner wtf?" << endl;
				}
			}
			//cout << "Player Score = " << playerScore << endl;
			//cout << "CPU Score = " << cpuScore << endl;
		}
	}
}

void RPSGame::processMouse(int x, int y)
{

}

void RPSGame::keyboardInput(unsigned char c, int x, int y)
{
}

bool RPSGame::Update()
{
	return true;
}

