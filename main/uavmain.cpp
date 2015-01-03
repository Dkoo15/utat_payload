#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

#include <csignal>
#include <thread>
#include <mutex>
#include <condition_variable>

//Libraries
#include "arvteledyne.h" 
#include "fakecamera.h"
#include "imgproc.h"
#include "imagepacket.h"

//Preprocessor flags
#define TESTING true

volatile sig_atomic_t finish = 0;
std::mutex mtx;
std::condition_variable convar;
volatile bool write_img = false;
volatile bool stream_packet = false;;
volatile bool request_gps = false;
int ip; 

int checkLogInit(){
	int saved = 0;
	std::ifstream logstream ("save/uav_gps.log", std::ifstream::in);
	
	if(!logstream){
		std::cout << "Starting from scratch" << std::endl;
		return -1;
		}
	else {
		std::string line;
		if (std::getline(logstream,line)){
			while(std::getline(logstream,line))  saved++;
		}
		logstream.close();
		std::cout << "Starting after " << saved << " images" << std::endl;
	    }
	return saved;
}

void writeLine(std::ofstream &logstream, std::string imgname){
	logstream << imgname << std::endl;
}

void exit_signal(int param){
	finish  = 1;
	std::cout << "Exiting on next frame completion.." << std::endl;
}

void imageWriter(){
	std::unique_lock<std::mutex> lock(mtx);
	std::stringstream ss;
	while(1){ //repeat
		
		while(finish == 0 && write_img == false) convar.wait(lock);//Wait for work

		if(finish == 1) break;	//If finish signal is given, skip and leave;
		//ss<<"save/img";
		//ss<<std::setfill('0')<<std::setw(4)<<++ip<<".jpg";
		uavision::saveFullImage(ss.str());
		ss.str("");
	   	write_img = true;	
	}
}
void wakeThrd(int id){
	std::unique_lock<std::mutex> lock(mtx);
	switch (id)
	{
		case 1:
			request_gps = true;
			break;
		case 2:
			write_img = true;
			break;
		case 3:
			stream_packet = true;
			break;
		default:
			break;
	}

	convar.notify_all();
}
int main(){
	Uavcam *camera; 
	unsigned char *rawbuf;
	std::vector<unsigned char> jpgbuffer;
	char start;
 	bool camera_ok;
	int num_saved;
	std::ofstream gpstream;
	std::stringstream ss;
	//	std::thread image_save_thrd(imageWriter);
	
	//Construct Classes
	if (TESTING)	
		camera = new Imgfromfile();
	else
		camera = new Teledyne();

	//Open and Check log
	num_saved = checkLogInit();
	gpstream.open("save/uav_gps.log",std::ofstream::app);
	if (num_saved == -1){
		gpstream <<"Image File Name \t Latitude \t Longitude \t Altitude \t Heading" << std::endl;
		num_saved++;
	}

	//Initialize Camera Settings
	camera_ok = camera->initCamSetting();
	uavision::initialize();
	std::signal(SIGINT,exit_signal); 	//Set up ctrl+c signal
   	ip = num_saved;	

	//Start Camera!
	if (camera_ok) {
		std::cout << "Start camera acquisition? (y/n)" << std::endl;
		std::cin >> start;
		
		if (start == 'y'){ 	//---Start Acquisition

			camera->startCam();

			while(!finish){  //--Main Acquisition Loop
				camera->sendTrigger();
				rawbuf = camera->getBuffer();
				if(rawbuf){ //Acquired Image
					uavision::processRaw(rawbuf);
					uavision::showImage();
					//wakeThrd(2);
					ss<<"save/im";
					ss<<std::setfill('0')<<std::setw(4)<<++ip;
					ss<<".jpg";
					uavision::saveFullImage(ss.str());
					jpgbuffer = uavision::compressPreview();
					writeLine(gpstream,ss.str());
					ss.str("");
				}
			}

			camera->endCam();
			jpgbuffer.clear();
		}
	}
	//image_save_thrd.join();
	gpstream.close();
	delete camera;
	uavision::freeMats();
	return 0;
}
