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
	  	bool getBuffer(std::vector<unsigned char> &buffer);
		void endCam();
	private:
		int i, n;
};
