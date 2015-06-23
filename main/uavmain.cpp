//Standard Libraries
#include <csignal>
#include <thread>
#include <chrono>
#include <mutex>
#include <iomanip>
#include <opencv2/opencv.hpp>

//Project header files
#include "araviscamera.h" 
#include "webcamera.h"
#include "gps_mod.h"
#include "io_mod.h"

#define FOLDER "Pictures/"

volatile std::sig_atomic_t finish = 0; //Signal Variable
std::mutex mtx;
Gps *ublox;
int cameratype, usegps, saveimg, view, start_delay, strm; //Options
double sizefac;

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

void writeLine(std::ofstream &logstream, std::string image){
	mtx.lock();
	logstream<< image  <<",";
	logstream.precision(9);
	logstream<< ublox->current_loc.latitude <<",";
	logstream<< ublox->current_loc.longitude<<",";
	logstream.precision(2);
	logstream<< ublox->current_loc.altitude <<",";
	logstream<< ublox->current_loc.heading << ",";
	logstream<< ublox->current_loc.tbuf;
	logstream<< std::endl;
	mtx.unlock();
}

int main(){
	Uavcam *camera; 
 	bool camera_ok, frame_ok;
	int n_saved;

	std::ofstream gpstream;
	std::stringstream filename;
	std::stringstream directory;
	std::vector<int> jpg_params;

	cv::Mat frame, preview;

	parseConfig();
	jpg_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	jpg_params.push_back(90);

	//Set Signals
	std::signal(SIGINT,exit_signal); 	//Set up Ctrl+C signal

	//Construct Classes
	if (cameratype == 1)
		camera = new AravisCam();
	else //(cameratype ==2)
		camera = new WebCam();
	
	if(view)	
		cv::namedWindow("Camera Viewer", cv::WINDOW_AUTOSIZE);
	
	//Check log and start numbering
	n_saved = checkLogInit();
	gpstream.open("Pictures/uav_gps.log",std::ofstream::app);
	gpstream<< std::fixed;
		if (n_saved == -1){
		gpstream <<"Image,Latitude[deg],Longitude[deg],Altitude[m],Heading[deg]" << std::endl;
		n_saved++;
	}

	//Initialize GPS
	if(usegps)
		ublox = new Gps();	
	std::thread gps_thread(gpsUpdate);
	
	//Initialize Camera Settings
	camera_ok = camera->initializeCam();

	if (camera_ok) {

		std::cout<<"Width = " << camera->width << " Height = " << camera->height<<std::endl;
		std::cout << "Start camera acquisition in " << start_delay << " seconds" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(start_delay));	

		while(!finish){  //--Main Acquisition Loop

			filename.str(""); directory.str(""); //Update filenames
			filename<<"im"<<std::setfill('0')<<std::setw(4)<<++n_saved<<".jpg";
			directory<<FOLDER<<filename.str();
		
			camera->trigger(); //Send camera trigger

			writeLine(gpstream, filename.str()); //Record GPS

			frame_ok = camera->getImage(frame); //Acquire the image

			if(frame_ok){ //Acquired Image

				cv::resize(frame,preview,cv::Size(),sizefac,sizefac,cv::INTER_NEAREST);

				if(saveimg) {
					cv::imwrite(directory.str(), preview, jpg_params);
					std::cout<<"Saved to " << filename.str() <<std::endl;
				}

				if(view) {
					cv::imshow("Camera Viewer", preview); 
					cv::waitKey(500);	
				}	
			}
			
			if(ublox->data_is_good)	
				std::cout<<"GPS up to date"<<std::endl;
			else	
				std::cout<<"No GPS available" <<std::endl;

			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
	}

	delete camera;
	gps_thread.join();
	gpstream.close();

	return 0;
}
