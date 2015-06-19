//Standard Libraries
#include <csignal>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <iomanip>

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
int cameratype, usegps, saveimg, view, sizefac, jpgq, bufferq, timeout, start_delay, delay, imgstrm; //Options

void exit_signal(int param){
	mtx.lock();
	finish  = 1;
	std::cout << "Exiting on next frame completion.." << std::endl;
	mtx.unlock();
}

void gpsPoll(){
	while(!finish){
		gps::readGPS();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	std::cout<<"GPS Thread joining..."<<std::endl;
}

void writeLine(std::ofstream &logstream, std::string image){
	mtx.lock();
	logstream<< image  <<",";
	logstream.precision(9);
	logstream<< gps::current_loc.latitude <<",";
	logstream<< gps::current_loc.longitude<<",";
	logstream.precision(2);
	logstream<< gps::current_loc.altitude <<",";
	logstream<< gps::current_loc.heading << ",";
	logstream<< gps::current_loc.tbuf;
	logstream<< std::endl;
	mtx.unlock();
}

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
	gpstream<< std::fixed;
		if (n_saved == -1){
		gpstream <<"Image,Latitude[deg],Longitude[deg],Altitude[m],Heading[deg]" << std::endl;
		n_saved++;
	}

	//Initialize GPS
 	if((gps_ok = gps::startGPS())) 	
		std::cout<<"GPS is working! Check Lock"<<std::endl;
	else		
		std::cout<<"GPS Error, no georeferencing"<<std::endl;

	std::thread gps_poll_thread(gpsPoll);
	
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
				
				if(gps::data_is_good)
					std::cout<<"GPS up to date"<<std::endl;
				else
					std::cout<<"No GPS available" <<std::endl;

				writeLine(gpstream, filename.str());
				
				uavision::createPreview(sizefac);
				if(saveimg)  
					uavision::saveFullImage(fulldirectory.str());

				if(view)
					uavision::openViewer();
				else	
					std::this_thread::sleep_for(std::chrono::milliseconds(50));

			}
			else	
				std::this_thread::sleep_for(std::chrono::milliseconds(250));

		}
		camera->endCam();
	}

	gps_poll_thread.join();
	gpstream.close();

	delete camera;
	return 0;
}
