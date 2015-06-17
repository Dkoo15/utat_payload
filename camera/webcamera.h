
#include "uavcamera.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <iostream>
#include <fstream>
#include <string>

class WebCam: public Uavcam {

	public: 
		WebCam();
		~WebCam();	
		
		bool initCamSetting(int &width, int &height);
		void startCam();
		void sendTrigger();
		bool getBuffer(std::vector<unsigned char> &buffer);
		void endCam();

		bool openWebcam();
		void saveFrame(std::string filename);

	private:
		cv::VideoCapture cam;
		cv::Mat frame;
		std::vector<int> jpg_params;
};
