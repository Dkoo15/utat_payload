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

#define PREFIX "Pictures/"

volatile std::sig_atomic_t finish = 0; //Signal Variable
std::mutex mtx;
//std::condition_variable convar;
bool stop_work = false;
int cameratype, usegps, saveimg, view, sizefac, jpgq, bufferq, timeout, start_delay, delay, imgstrm; //Options
double location[4]; //GPS Information

void exit_signal(int param){
	finish  = 1;
	std::cout << "Exiting on next frame completion.." << std::endl;
}

void gpsPoll(){
	std::unique_lock<std::mutex> lock(mtx);
/*	
 	while(1){ //repeat
		while( stop_work == false && request_gps == false) convar.wait(lock);//Wait for work

		if(stop_work) break;	//If finish signal is given, skip and leave;

		if(usegps) gps::getGPS(location);
		
		request_gps = false;	
	}
	std::cout<<"GPS Thread Done"<<std::endl;	
*/
}

/*
void wakeThread(int id){
	std::unique_lock<std::mutex> lock(mtx);
	convar.notify_all();
}
*/

int main(){
	Uavcam *camera; 
	std::vector<unsigned char> rawbuf;
	std::vector<unsigned char> jpgbuffer;
 	bool camera_ok, buffer_ok, gps_ok;
	int n_saved;
	int width, height;
	std::ofstream gpstream;
	std::stringstream filename;
	std::stringstream fulldirectory;

	parseConfig();

	//Begin Threads
	std::thread gps_poll_thread(gpsPoll);

	//Set Signals
	std::signal(SIGINT,exit_signal); 	//Set up Ctrl+C signal

	//Construct Classes
	if (cameratype == 0)	
		camera = new Imgfromfile();
	else if (cameratype == 1)
		camera = new AravisCam();
	else if (cameratype == 2)
		camera = new WebCam();

	//Check log and start numbering
	n_saved = checkLogInit();
	gpstream.open("Pictures/uav_gps.log",std::ofstream::app);
	if (n_saved == -1){
		gpstream <<"Image,Latitude[deg],Longitude[deg],Altitude[m],Heading[deg]" << std::endl;
		n_saved++;
	}

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

			if(buffer_ok){ //Acquired Image

				if(cameratype <= 1)
					uavision::processRaw(rawbuf);
				else 
					uavision::assignData(rawbuf);

				filename.str("");
				fulldirectory.str("");
				filename<<"im"<<std::setfill('0')<<std::setw(4)<<++n_saved;
				filename<<".jpg";
				fulldirectory<<PREFIX<<filename.str();

				writeLine(gpstream, filename.str());

				if(imgstrm)  uavision::compressPreview(jpgbuffer);
					
				if(saveimg)  uavision::saveFullImage(fulldirectory.str());

				if(view){
					uavision::createPreview(sizefac);
					uavision::openViewer(delay);
				}
				else	std::this_thread::sleep_for(std::chrono::seconds(delay));

			}
		}
		camera->endCam();
	}

	stop_work = true;

	gps_poll_thread.join();
	gpstream.close();

	delete camera;
	return 0;
}
