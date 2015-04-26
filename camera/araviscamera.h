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
		~AravisCam();	
		
		bool initCamSetting(int &width, int &height);
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
