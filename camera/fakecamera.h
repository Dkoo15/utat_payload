#include "uavcamera.h"
#include <stdio.h>

class Imgfromfile: public Uavcam {

	public: 
		Imgfromfile();
		~Imgfromfile();	
		
		int initCamSetting();
		void startCam();
		void sendTrigger();
	  	unsigned char* getBuffer();
		void endCam();
	private:
		int i, n;
};
