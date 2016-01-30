//Standard Libraries
#include <csignal>
#include <thread>
#include <chrono>
#include <mutex>
#include <sstream>
#include <iomanip>

//#include <sys/time.h>

//Opencv
#include <opencv2/opencv.hpp>

//Project header files
#include "webcamera.h"
#include "gps_mod.h"
#include "io_utils.h"
#include "config.h"
#ifdef USE_ARAVIS
#include "araviscamera.h" 
#endif

#define FOLDER "Pictures/"

volatile std::sig_atomic_t finish = 0; //Signal Variable
std::mutex mtx;
Gps *ublox;
int cameratype, usegps, saveimg, view, start_delay, strm; //Options
double sizefac;

struct timespec start, end;

void exit_signal(int param){
	mtx.lock();
	finish  = 1;
	std::cout << "Exiting on next frame completion.." << std::endl;
	mtx.unlock();
}

void gpsUpdate(){
	while(!finish){
		if (usegps)
			ublox->readGPS();

		std::this_thread::sleep_for(std::chrono::milliseconds(150));
	}
	std::cout<<"GPS Thread joining..."<<std::endl;
}

void printTimeDiff(struct &start, struct &end){
	seconds = end
}

int main(){
	Uavcam *camera; 
	bool camera_ok, frame_ok;
	int n_saved;

	std::stringstream filename;
	std::stringstream directory;

	std::vector<int> jpg_params;
	cv::Mat frame, preview;

	parseConfig();
	jpg_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	jpg_params.push_back(90);

	//Set Signals
	std::signal(SIGINT,exit_signal); 	//Set up Ctrl+C signal

	//Construct Cameras
	if (cameratype == 1){
#ifdef USE_ARAVIS
		camera = new AravisCam();
		std::cout<<"Using Aravis camera"<<std::endl;
#else
		camera = new WebCam();
#endif
	}

	if(view)	
		cv::namedWindow("Camera Viewer", cv::WINDOW_AUTOSIZE);

	n_saved = checkLog(); 			//Check the log and open it
	openLogtoWrite(n_saved);

	ublox = new Gps();			//Initialize the GPS
	std::thread gps_thread(gpsUpdate);

	//clock_gettime(CLOCK_MONOTONIC, &start);
	camera_ok = camera->initializeCam();  	//Initialize the camera
	//clock_gettime(CLOCK_MONOTONIC, &end);

	if (camera_ok) {

		std::cout << "Start camera acquisition in " << start_delay << " seconds" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(start_delay));	

		while(!finish){  //--Main Acquisition Loop

			filename.str(""); directory.str(""); //Update filenames
			filename<<"im"<<std::setfill('0')<<std::setw(4)<<++n_saved<<".jpg";
			directory<<FOLDER<<filename.str();

			frame_ok = camera->trigger(frame); //Send camera trigger

			if (usegps){
				if(ublox->data_is_good)	
					std::cout<<"GPS up to date"<<std::endl;
				else	
					std::cout<<"No GPS available" <<std::endl;
			}

			mtx.lock();
			writeImageInfo(ublox->current_loc, filename.str()); //Record GPS 
			mtx.unlock();

			//frame_ok = camera->getImage(frame); //Acquire the image

			if(frame_ok){ //Acquired Image

				cv::resize(frame,preview,cv::Size(),sizefac,sizefac,cv::INTER_NEAREST);

				if(saveimg) {
					cv::imwrite(directory.str(), preview, jpg_params);
					std::cout<<"Saved to " << filename.str() <<std::endl;
				}

				if(view) {
					cv::imshow("Camera Viewer", preview); 
					cv::waitKey(50);
				}	
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(250));
		} 
		//Finished photographing
		delete camera;
	}

	gps_thread.join(); 
	closeLog();

	return 0;
}
