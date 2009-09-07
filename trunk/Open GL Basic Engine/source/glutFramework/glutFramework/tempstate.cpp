#include "States\DevLogoState.h"
#include "GameStateManager.h"

#include "tempstate.h"


bool tempstate::Update()
{
	/*timer++;
	if(timer > 20)
	{
		GSM->addGameState<tempstate>();
		this->setStatus(DeleteMe);
	}*/
	return true;
}

bool tempstate::Draw()
{
	logo->drawImage();

	std::cout << "attempting to draw tempstate" << std::endl;
	return false;
}
