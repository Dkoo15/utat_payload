#include "webcamera.h"

#define BUFFSIZE 10

WebCam::WebCam(){}

WebCam::~WebCam(){}	
	
bool WebCam::initializeCam(){
	int w,h;

	cam.open(0);
	if(cam.isOpened()){
		w = (int) cam.get(CV_CAP_PROP_FRAME_WIDTH);
		h = (int) cam.get(CV_CAP_PROP_FRAME_HEIGHT);
		std::cout<<"Webcam opened at "<<w<<"x"<<h<<std::endl;
		return true;
	}
	else
		return false;
}

void WebCam::trigger(){
	//Instead of clearing the sending a trigger, we're just going to clear the buffer
	bool grab;
	for (int i = 0; i < BUFFSIZE; i++)
		grab = cam.grab();

	if(grab)
		std::cout<<"Took webcam picture"<<std::endl;
	else
		std::cout<<"Error taking webcam picutre" << std::endl;
}

bool WebCam::getImage(cv::Mat &frame){
	cam.retrieve(frame);
	return (!frame.empty());
}

