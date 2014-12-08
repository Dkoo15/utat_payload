#include "uavcamera.h"
#include <stdio.h>

class Imgfromfile: public Uavcam {

	public: 
		Imgfromfile();
		~Imgfromfile();	
		
		int initCamSetting();
		int sendTrigger();
		char* getBuffer();

	private:
		int i, n;
};
