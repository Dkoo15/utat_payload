#include "uavcamera.h"
#include <iostream>
#include <fstream>
#include <sstream>

class Imgfromfile: public Uavcam {

	public: 
		Imgfromfile();
		~Imgfromfile();	
		
		bool initCamSetting();
		void startCam();
		void sendTrigger();
	  	unsigned char* getBuffer();
		void endCam();
	private:
		int i, n;
};
