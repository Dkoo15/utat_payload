#include "webcamera.h"

WebCam::WebCam(){}

WebCam::~WebCam(){}	
	
bool WebCam::initializeCam(){
	bool cam_ok = cam.open(0);
	if(cam_ok){
		std::cout<<"Webcam successfully opened"<<std::endl;
		this->width = (int) cam.get(CV_CAP_PROP_FRAME_WIDTH);
		this->height = (int) cam.get(CV_CAP_PROP_FRAME_HEIGHT);
	}

	return cam_ok;
}

void WebCam::trigger(){
	//Instead of clearing the sending a trigger, we're just going to clear the buffer
	bool grab;
	for (int i = 0; i < 10; i++)
		grab = cam.grab();

	if(grab)
		std::cout<<"Acquired Webcam picture"<<std::endl;
	else
		std::cout<<"Error acquiring webcam picutre" << std::endl;
}

bool WebCam::getImage(cv::Mat &frame){
	cam.retrieve(frame);
	return (!frame.empty());
}

