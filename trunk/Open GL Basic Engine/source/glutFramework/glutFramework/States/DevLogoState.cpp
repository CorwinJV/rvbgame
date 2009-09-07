#include "DevLogoState.h"
#include ".\..\GameStateManager.h"

#include "MainMenuState.h"
#include "..\tempstate.h"
#include "..\oglGameVars.h"


bool DevLogoState::Update()
{
	// start/end times are given as seconds duration based on start of the state
	

	// per tick shall be
	// start time * 1000 = start
	// end time * 1000 = end
	// end - start = total # of ticks we will need
	// 1 / total ticks = fade amount per tick

	double fadeinamt =  (1/((devlogofadeinend*1000)  - (devlogofadeinstart * 1000)));
	double fadeoutamt = (1/((devlogofadeoutend*1000) - (devlogofadeoutstart* 1000)));

	// logo 1
	logo = 0;

	// at any given point... our fade in amount is...
	// current time - start time * fadeinamt

	timer = clock() - startTime;

	if(timer < (devlogofadeinend*1000))									 
	{		
		logo = 1;
		opacity = (timer - (devlogofadeinstart*1000)) / ((devlogofadeinend*1000) - (devlogofadeinstart * 1000));
	}


	if((timer > (devlogofadeinend*1000)) && (timer < (devlogofadeoutstart*1000)))	 {	logo = 1;	opacity = 1.0;	        }


	// since we're going backwards, we do 1 - our old formula
	if( (timer > (devlogofadeoutstart*1000) && (timer < (devlogofadeoutend*1000))))
	{		
		logo = 1;
		opacity = 1 - (timer - (devlogofadeoutstart*1000)) / ((devlogofadeinend*1000) - (devlogofadeinstart * 1000));;
	}

	//// load logo 2
	if((timer > logo2fadeinstart*1000) && (logostate == 0))
	{
		logo = 2;
		opacity = 0;
		logostate = 1;
		fadeinamt =  1/((logo2fadeinend*1000)  - (logo2fadeinstart * 1000));
		fadeoutamt = 1/((logo2fadeoutend*1000) - (logo2fadeoutstart* 1000));
	}

	if((timer < (logo2fadeinend*1000)) && (timer > (logo2fadeinstart * 1000)))
	{		
		logo = 2;
		opacity = (timer - (logo2fadeinstart*1000)) / ((logo2fadeinend*1000) - (logo2fadeinstart * 1000));
	}
	if((timer > (logo2fadeinend*1000)) && (timer < (logo2fadeoutstart*1000)))	 {	logo = 2;	opacity = 1.0;	        }


	// since we're going backwards, we do 1 - our old formula
	if( (timer > (logo2fadeoutstart*1000) && (timer < (logo2fadeoutend*1000))))
	{		
		logo = 2;
		//opacity = 1 - (clock() - (logo2fadeoutstart*1000)) * fadeoutamt;
		opacity = 1 - (timer - (logo2fadeoutstart*1000)) / ((logo2fadeinend*1000) - (logo2fadeinstart * 1000));
	}

	//// load logo 3
	if((timer > logo3fadeinstart*1000) && (logostate == 1))
	{
		logo = 3;
		opacity = 0;
		logostate = 2;
		fadeinamt =  1/((logo3fadeinend*1000)  - (logo3fadeinstart * 1000));
		fadeoutamt = 1/((logo3fadeoutend*1000) - (logo3fadeoutstart* 1000));
	}

	if((timer < (logo3fadeinend*1000)) && (timer > (logo3fadeinstart * 1000)))
	{	
		logo = 3;
		opacity = (timer - (logo3fadeinstart*1000)) / ((logo3fadeinend*1000) - (logo3fadeinstart * 1000));
	}
	if((timer > (logo3fadeinend*1000)) && (timer < (logo3fadeoutstart*1000)))	 {	logo = 3;	opacity = 1.0;	        }


	// since we're going backwards, we do 1 - our old formula
	if( (timer > (logo3fadeoutstart*1000) && (timer < (logo3fadeoutend*1000))))
	{		
		logo = 3;
		opacity = 1 - (timer - (logo3fadeoutstart*1000)) / ((logo3fadeinend*1000) - (logo3fadeinstart * 1000));
	}

	if((timer > logo3fadeoutend*1000) || theyWantOut)
	{
		GSM->addGameState<MainMenuState>();
		this->setStatus(DeleteMe);
	}

	//std::cout << "Opacity is " << opacity << endl;
	//std::cout << "Logo is " << logo << endl;
	return true;
}

bool DevLogoState::Draw()
{
	//logo->drawImage();
	switch(logo)
	{
		case 1:
			logo1->drawImageFaded(opacity);
			break;
		case 2:
			logo2->drawImageFaded(opacity);
			break;
		case 3:
			logo3->drawImageFaded(opacity);
			break;
		default:
			break;
	}
	//std::cout << "opacity = " << opacity << endl;

	//std::cout<< "I drew something at " << timer << endl;
	return false;
}

DevLogoState::~DevLogoState()
{
	delete logo1;
	delete logo2;
	delete logo3;
}

void DevLogoState::keyboardInput(unsigned char c, int x, int y)
{
	switch(c)
	{
	case 27:
		theyWantOut = true;
		break;
	}
}