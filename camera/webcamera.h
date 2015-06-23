#include "uavcamera.h"

class WebCam: public Uavcam {

	public: 
		WebCam();
		~WebCam();	
		
		bool initializeCam();
		void trigger();
		bool getImage(cv::Mat &frame);

	private:
		cv::VideoCapture cam;
};
