#include "webcamera.h"

WebCam::WebCam(){}

WebCam::~WebCam(){}	
	
bool WebCam::initCamSetting(int &width, int &height){
	cam.open(0);	

	if(cam.isOpened()){
		std::cout<<"Webcam initialized"<<std::endl;
		cam.set(CV_CAP_PROP_FRAME_WIDTH,1280);
		cam.set(CV_CAP_PROP_FRAME_HEIGHT,960);
		width = (int) cam.get(CV_CAP_PROP_FRAME_WIDTH);
		height = (int) cam.get(CV_CAP_PROP_FRAME_HEIGHT);
		payload = width*height*3;
		return true;
	}
	else {
		std::cout<<"Error initializing webcam" <<std::endl;
		return false;
	}

}

void WebCam::startCam(){
	std::cout<<"Using first available webcam..."<<std::endl;
}

void WebCam::sendTrigger(){
	bool grab;
	for (int i = 0; i<15; i++)
		grab = cam.grab();
	if (grab)
		std::cout<<"Acquired Webcam picture"<<std::endl;
	else
		std::cout<<"Error acquiring webcam picture" <<std::endl;
}

bool WebCam::getBuffer(std::vector<unsigned char> &buffer){
	cam.retrieve(frame);

	if(frame.empty()){
		std::cout<<"Error getting frame " << std::endl;
		return false;
	}
	memcpy(&buffer[0],frame.data,payload);
	return true;
}

void WebCam::endCam(){}
