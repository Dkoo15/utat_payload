#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

#include <csignal>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <sys/time.h>

//Libraries
#include "araviscamera.h" 
#include "fakecamera.h"
#include "imgproc.h"
#include "imagepacket.h"

//Preprocessor flagsI
#define USE_CAMERA true
#define ACQUIRE_GPS 1
#define SAVE_IMAGE 2
#define STREAM_IMAGE 3

volatile std::sig_atomic_t finish = 0;
std::mutex mtx;
std::condition_variable convar;
bool write_img = false;
bool stream_packet = false;;
bool request_gps = false;
bool stop_work = false;
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
		
		while( stop_work == false && write_img == false) convar.wait(lock);//Wait for work

		std::cout<<"Image Thread Awoken"<<std::endl;
		if(stop_work) break;	//If finish signal is given, skip and leave;
	
		ss<<"save/im";
		ss<<std::setfill('0')<<std::setw(4)<<++ip;
		ss<<".jpg";
		uavision::saveFullImage(ss.str());
		ss.str("");
	   	write_img = false;	
	}
	std::cout<<"Image Writing Thread Done"<<std::endl;
}

void wakeThread(int id){
	std::unique_lock<std::mutex> lock(mtx);
	switch (id)
	{
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
 	bool camera_ok, buffer_ok;
	int num_saved;
	std::ofstream gpstream;
	std::stringstream ss;
	std::thread image_save_thrd(imageWriter);

	//Construct Classes
	if (!USE_CAMERA)	
		camera = new Imgfromfile();
	else
		camera = new AravisCam();

	//Open and Check log
	num_saved = checkLogInit();
	gpstream.open("save/uav_gps.log",std::ofstream::app);
	if (num_saved == -1){
		gpstream <<"Image File Name \t Latitude \t Longitude \t Altitude \t Heading" << std::endl;
		num_saved++;
	}

	//Initialize Camera Settings
	camera_ok = camera->initCamSetting();
	rawbuf.resize(camera->payload);
	uavision::initialize(camera->dim);

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
				buffer_ok = camera->getBuffer(rawbuf);
				if(buffer_ok){ //Acquired Image
					uavision::processRaw(rawbuf);
					ss<<"save/im";
					ss<<std::setfill('0')<<std::setw(4)<<++ip;
					ss<<".jpg";
					struct timeval start, end;
					gettimeofday(&start,NULL);
//					wakeThread(SAVE_IMAGE);
					uavision::saveFullImage(ss.str());
					uavision::createPreview();
					gettimeofday(&end,NULL);
					double delta =(end.tv_usec-start.tv_usec)/1000;
					std::cout<<delta<<"ms"<<std::endl;
					uavision::compressPreview(jpgbuffer);
					writeLine(gpstream,ss.str());
					ss.str(""); 
				}
			}
			

			camera->endCam();
			jpgbuffer.clear();
		}
	}
	stop_work = true;
	wakeThread(-1);
	image_save_thrd.join();
	gpstream.close();
	delete camera;
	return 0;
}
