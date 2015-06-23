#include "webcamera.h"

#define BUFFSIZE 10

WebCam::WebCam(){}

WebCam::~WebCam(){}	
	
bool WebCam::initializeCam(){
	cam.open(0);
	if(cam.isOpened()){
		std::cout<<"Webcam successfully opened"<<std::endl;
		this->width = (int) cam.get(CV_CAP_PROP_FRAME_WIDTH);
		this->height = (int) cam.get(CV_CAP_PROP_FRAME_HEIGHT);
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

