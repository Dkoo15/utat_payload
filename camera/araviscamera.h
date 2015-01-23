extern "C"{
#include <arv.h>
}

#include "uavcamera.h"
#include <iostream>
#include <fstream>
#include <string>

#define BUFFER_Q_SIZE 5
#define WAIT_CYCLES 25
#define CONFIG_FILE "teledyne.cfg"

class AravisCam: public Uavcam {

	public: 
		AravisCam();
		~AravisCam();	
		
		bool initCamSetting();
		void startCam();
		void sendTrigger();
		bool getBuffer(std::vector<unsigned char> &buffer);
		void endCam();
		bool parseInputs(std::vector<std::string> &commands);;

	private:
		ArvGc *genicam;
		ArvDevice *device;
		ArvStream *stream;
		ArvGcNode *trigger;
};
