//Standard Libraries
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

#include <csignal>
#include <thread>
#include <mutex>
#include <condition_variable>

//Project header files
#include "araviscamera.h" 
#include "fakecamera.h"
#include "imgproc.h"
#include "imagepacket.h"
#include "gpsmod.h"

//Preprocessor flags
#define USE_CAMERA false
#define ACQUIRE_GPS 1
#define SAVE_IMAGE 2
#define STREAM_IMAGE 3

#define PREFIX "save/im"

volatile std::sig_atomic_t finish = 0;
std::mutex mtx;
std::condition_variable convar;
bool write_img = false;
bool stream_packet = false;;
bool request_gps = false;
bool stop_work = false;
double location[4];
int ip; 
uavimage impacket;

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

void writeLine(std::ofstream &logstream){
	logstream<<PREFIX;
	logstream<<std::setfill('0')<<std::setw(4)<<ip;
	logstream<< ".jpg" <<"\t\t";
	logstream<<std::setprecision(8);
	logstream<< location[0]<<"\t";
	logstream<<std::setprecision(8);
	logstream<< location[1]<<"\t";
	logstream<< location[2]<<"\t\t";
	logstream<< location[3]<<"\t";
	logstream << std::endl;
}

void exit_signal(int param){
	finish  = 1;
	std::cout << "Exiting on next frame completion.." << std::endl;
}

void imageWriter(){
	std::unique_lock<std::mutex> lock(mtx);
	std::stringstream ss;
	while(1){ //repeat
		while( stop_work == false && write_img == false) convar.wait(lock);//Wait for work

		std::cout<<"Image Thread Awoken"<<std::endl;
		if(stop_work) break;	//If finish signal is given, skip and leave;
	
		ss<<PREFIX;
		ss<<std::setfill('0')<<std::setw(4)<<ip;
		ss<<".jpg";
		uavision::saveFullImage(ss.str());
		ss.str("");
	   	write_img = false;	
	}
	std::cout<<"Image Writing Thread Done"<<std::endl;
}

void gpsPoller(){
	std::unique_lock<std::mutex> lock(mtx);
	bool gps_ok;

	while(1){ //repeat
		while( stop_work == false && request_gps == false) convar.wait(lock);//Wait for work

		std::cout<<"GPS Thread Awoken"<<std::endl;
		if(stop_work) break;	//If finish signal is given, skip and leave;
	
		gps_ok = gps::getGPS(location);

	   	if(gps_ok == false)  std::cout<<"GPS data not good"<<std::endl;
		else	std::cout<<"Lat" << location[0] << ", Lon" <<location[1]<<std::endl;

		request_gps = false;	
	}
	std::cout<<"GPS Thread Done"<<std::endl;
}

void wakeThread(int id){
	std::unique_lock<std::mutex> lock(mtx);
	switch (id){
		case ACQUIRE_GPS:
			request_gps = true;
			break;
		case SAVE_IMAGE:
			write_img = true;
			break;
		case STREAM_IMAGE:
			stream_packet = true;
			break;
		default:
			break;
	}
	convar.notify_all();
}

int main(){
	Uavcam *camera; 
	std::vector<unsigned char> rawbuf;
	std::vector<unsigned char> jpgbuffer;
	char start;
 	bool camera_ok, buffer_ok, gps_ok;
	int num_saved;
	std::ofstream gpstream;
	
	//Begin Threads
	std::thread image_save_thrd(imageWriter);
	std::thread gps_poll_thrd(gpsPoller);

	//Set Signals
	std::signal(SIGINT,exit_signal); 	//Set up ctrl+c signal

	//Construct Classes
	if (!USE_CAMERA)	
		camera = new Imgfromfile();
	else
		camera = new AravisCam();

	//Check log and start numbering
	num_saved = checkLogInit();
	gpstream.open("save/uav_gps.log",std::ofstream::app);
	if (num_saved == -1){
		gpstream <<"Image File Name \tLatitude \tLongitude \tHeading \tAltitude" << std::endl;
		num_saved++;
	}
	ip = num_saved;	

	//Initialize GPS
 	if((gps_ok = gps::startGPS())) 	std::cout<<"GPS is working! Check Lock"<<std::endl;
	else		std::cout<<"GPS Error, no georeferencing"<<std::endl;
		
	//Initialize Camera Settings
	camera_ok = camera->initCamSetting();
	rawbuf.resize(camera->payload);
	uavision::initialize(camera->dim);

	//Start Camera!
	if (camera_ok) {
		std::cout << "Start camera acquisition? (y/n)" << std::endl;
		std::cin >> start;
		
		if (start == 'y'){ 	//---Start Acquisition

			camera->startCam();

			while(!finish){  //--Main Acquisition Loop
				camera->sendTrigger();
				buffer_ok = camera->getBuffer(rawbuf);
				wakeThread(ACQUIRE_GPS);
				if(buffer_ok){ //Acquired Image
					ip++;
					uavision::processRaw(rawbuf);
					wakeThread(SAVE_IMAGE);
					uavision::createPreview();
					//uavision::compressPreview(jpgbuffer);
					writeLine(gpstream);
				}
			}
			camera->endCam();
		}
	}

	stop_work = true;
	wakeThread(-1);
	image_save_thrd.join();
	gps_poll_thrd.join();
	gpstream.close();

	delete camera;
	return 0;
}
