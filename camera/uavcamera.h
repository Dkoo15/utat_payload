#pragma once

class Uavcam {

	public: 
		long bufsize; 

		Uavcam(){}
		virtual ~Uavcam(){}
		
		virtual int initCamSetting() = 0;
		virtual void startCam() = 0;
		virtual void sendTrigger() = 0;
		virtual unsigned char* getBuffer() = 0;
		virtual void endCam() = 0;

	protected:
		unsigned char* buffer;
};
