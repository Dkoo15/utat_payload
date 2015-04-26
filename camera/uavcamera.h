#pragma once
#include <vector>

class Uavcam {

	public: 
		Uavcam(){}
		virtual ~Uavcam(){}
		
		virtual bool initCamSetting(int &width, int &height) = 0;
		virtual void startCam() = 0;
		virtual void sendTrigger() = 0;
		virtual bool getBuffer(std::vector<unsigned char> &buffer) = 0;
		virtual void endCam() = 0;

		int payload;

};
