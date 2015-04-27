//Standard Libraries
#include <csignal>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

//Project header files
#include "araviscamera.h" 
#include "fakecamera.h"
#include "webcamera.h"
#include "imgproc.h"
#include "gpsmod.h"
#include "io_mod.h"

//Constants
#define ACQUIRE_GPS 1
#define SAVE_IMAGE 2
#define STREAM_IMAGE 3
#define PREFIX "uav/save/im"

volatile std::sig_atomic_t finish = 0;
std::mutex mtx;
std::condition_variable convar;
bool write_img = false;
bool packet = false;;
bool request_gps = false;
bool stop_work = false;
bool bayer = false;
int cameratype, usegps, saveimg, view;
int sizefac, jpgq;
int bufferq, timeout;
int start_delay, delay;
double location[4];
int ip;

void exit_signal(int param){
	finish  = 1;
	std::cout << "Exiting on next frame completion.." << std::endl;
}

void imageWriter(){
	std::unique_lock<std::mutex> lock(mtx);
	std::stringstream ss;
	while(1){ //repeat
		while( stop_work == false && write_img == false) convar.wait(lock);//Wait for work

		if(stop_work) break;	//If finish signal is given, skip and leave;
	
		ss<<PREFIX;
		ss<<std::setfill('0')<<std::setw(4)<<ip;
		ss<<".jpg";

		if(saveimg) 
		       	uavision::saveFullImage(ss.str());

		ss.str("");
	   	write_img = false;	
	}
	std::cout<<"Image Writing Thread Done"<<std::endl;
}

void gpsPoller(){
	std::unique_lock<std::mutex> lock(mtx);
	bool gps_ok = false;

	while(1){ //repeat
		while( stop_work == false && request_gps == false) convar.wait(lock);//Wait for work

		if(stop_work) break;	//If finish signal is given, skip and leave;

		if(usegps){
			gps_ok = gps::getGPS(location);

	   		if(gps_ok == false)  std::cout<<"GPS data not good"<<std::endl;
			else	std::cout<<"Lat" << location[0] << ", Lon" <<location[1]<<std::endl;
		}
		
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
			packet = true;
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
 	bool camera_ok, buffer_ok, gps_ok;
	int num_saved;
	int width, height;
	std::ofstream gpstream;
	
	parseConfig();
	//Begin Threads
	std::thread image_save_thrd(imageWriter);
	std::thread gps_poll_thrd(gpsPoller);

	//Set Signals
	std::signal(SIGINT,exit_signal); 	//Set up ctrl+c signal

	//Construct Classes
	if (cameratype == 0)	
		camera = new Imgfromfile();
	else if (cameratype == 1)
		camera = new AravisCam();
	else if (cameratype == 2)
		camera = new WebCam();

	//Check log and start numbering
	num_saved = checkLogInit();
	gpstream.open("uav/save/uav_gps.log",std::ofstream::app);
	if (num_saved == -1){
		gpstream <<"Image,Latitude[deg],Longitude[deg],Altitude[m],Heading[deg]" << std::endl;
		num_saved++;
	}
	ip = num_saved;	

	//Initialize GPS
 	if((gps_ok = gps::startGPS())) 	std::cout<<"GPS is working! Check Lock"<<std::endl;
	else		std::cout<<"GPS Error, no georeferencing"<<std::endl;
		
	//Initialize Camera Settings
	camera_ok = camera->initCamSetting(width, height);
	std::cout<<"Width = " << width << " Height = " << height << " Payload = "<< camera->payload<<std::endl;

	//Start Camera!
	if (camera_ok) {
		rawbuf.resize(camera->payload);
		uavision::initialize(width, height, view, jpgq);

		std::cout << "Start camera acquisition in " << start_delay << " seconds" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(start_delay));	
		camera->startCam();

		while(!finish){  //--Main Acquisition Loop
			camera->sendTrigger();
			buffer_ok = camera->getBuffer(rawbuf);
			wakeThread(ACQUIRE_GPS);
			if(buffer_ok){ //Acquired Image
				ip++;

				if(cameratype <= 1)
					uavision::processRaw(rawbuf);
				else 
					uavision::assignData(rawbuf);

				wakeThread(SAVE_IMAGE);
				
				if(view){
					uavision::createPreview(sizefac);
					uavision::openViewer(delay);
					//uavision::compressPreview(jpgbuffer)
				}
				else
					std::this_thread::sleep_for(std::chrono::seconds(delay));

				writeLine(gpstream);
			}
			else{std::this_thread::sleep_for(std::chrono::seconds(1));	}

		}
		camera->endCam();
	}

	stop_work = true;
	wakeThread(-1);
	image_save_thrd.join();
	gps_poll_thrd.join();
	gpstream.close();

	delete camera;
	return 0;
}
