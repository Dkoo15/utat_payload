extern "C"{
#include <arv.h>
}

#include "uavcamera.h"
#include <stdio.h>
#include <string.h>
#define BUFFER_Q_SIZE 20

class Teledyne: public Uavcam {

	public: 
		Teledyne();
		~Teledyne();	
		
		int initCamSetting();
		void startCam();
		void sendTrigger();
		unsigned char* getBuffer();
		void endCam();
		void parseInputs(char commands[][100], int *num);

	private:
		ArvGc *genicam;
		ArvDevice *device;
		ArvStream *stream;
		ArvGcNode *trigger;
};
