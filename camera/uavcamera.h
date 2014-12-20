#pragma once

class Uavcam {

	public: 
		Uavcam(){}
		virtual ~Uavcam(){}
		
		virtual int initCamSetting() = 0;
		virtual void startCam() = 0;
		virtual void sendTrigger() = 0;
		virtual unsigned char* getBuffer() = 0;
		virtual void endCam() = 0;

	protected:
		int payload;
		unsigned char* buffer;
};
