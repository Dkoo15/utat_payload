#include "arvteledyne.h"

Teledyne::Teledyne(){
	buffer = NULL;
}

Teledyne:: ~Teledyne(){
	if(buffer) delete buffer;
}

int Teledyne::initCamSetting(){
	return 0;
}

int Teledyne::sendTrigger(){
	return 0;
}


char* Teledyne::getBuffer(){
	return 0;
}



