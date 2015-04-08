extern "C"{
#include <arv.h>
}

#include "uavcamera.h"
#include <iostream>
#include <fstream>
#include <string>

class AravisCam: public Uavcam {

	public: 
		AravisCam();
		AravisCam(int, int);
		~AravisCam();	
		
		bool initCamSetting();
		void startCam();
		void sendTrigger();
		bool getBuffer(std::vector<unsigned char> &buffer);
		void endCam();

	private:
		ArvGc *genicam;
		ArvDevice *device;
		ArvStream *stream;
		ArvGcNode *trigger;
		int timeout, bufferq;
};
