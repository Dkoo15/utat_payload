#include "uavcamera.h"

extern "C"{
#include <arv.h>
}

class AravisCam: public Uavcam {

	public: 
		AravisCam();
		~AravisCam();	
		
		bool initializeCam();
		bool trigger(cv::Mat &frame);
		bool getImage(cv::Mat &frame);

	private:
		void endCam();
		void startCam();
		bool getBuffer();

		ArvGc *genicam;
		ArvDevice *device;
		ArvStream *stream;
		ArvGcNode *triggernode;
		ArvGcNode *start;
		ArvGcNode *end;

		cv::Mat rawmat;
		cv::Size size;

		int payload;
		bool acquisition;
		unsigned char* rawbuffer;
};
