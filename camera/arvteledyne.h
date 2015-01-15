extern "C"{
#include <arv.h>
}

#include "uavcamera.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define BUFFER_Q_SIZE 10
#define WAIT_CYCLES 25

class Teledyne: public Uavcam {

	public: 
		Teledyne();
		~Teledyne();	
		
		bool initCamSetting();
		void startCam();
		void sendTrigger();
		unsigned char* getBuffer();
		void endCam();
		void parseInputs(std::vector<std::string> &commands);;

	private:
		ArvGc *genicam;
		ArvDevice *device;
		ArvStream *stream;
		ArvGcNode *trigger;
};
