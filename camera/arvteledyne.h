extern "C"{
#include <arv.h>
}

class Teledyne {

	public: 
		int imgheight, imgwidth;
		
		ArvGc *genicam;

		Teledyne();
		~Teledyne();	
		
		int initCamSetting();
		char* getBuffer();

	private:
		char* buffer;
	/*
	 * 	 ArvDevice *device;
	 *       ArvStream *stream;
	 *       ArvGc *genicam;	
	 *       			               	*/

};
