#include "webcamera.h"

WebCam::WebCam(){}

WebCam::~WebCam(){}	
	
bool WebCam::initCamSetting(int &width, int &height){
	bool cam_ok = openWebcam();
	if(cam_ok){
		width = (int) cam.get(CV_CAP_PROP_FRAME_WIDTH);
		height = (int) cam.get(CV_CAP_PROP_FRAME_HEIGHT);
		payload = width*height*3;
	}

	return cam_ok;
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

bool WebCam::openWebcam(){
	jpg_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	jpg_params.push_back(90);
	bool status = cam.open(0);

	if(cam.isOpened()){
		std::cout<<"Webcam initialized"<<std::endl;
		cam.set(CV_CAP_PROP_FRAME_WIDTH,1280);
		cam.set(CV_CAP_PROP_FRAME_HEIGHT,960);
	}

	if(status)
		std::cout<<"Webcam initialized"<<std::endl;
	else 
		std::cout<<"Error initializing webcam" <<std::endl;
	
	return status;
}

void WebCam::saveFrame(std::string filename){
	for (int i = 0; i < 15; i++)
		cam.grab();

	cam.retrieve(frame);
	if(!frame.empty()){
		cv::imwrite(filename,frame,jpg_params);
	}
}
