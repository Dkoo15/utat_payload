#include <stdio.h>
#include <signal.h>
#include <thread>
#include <mutex>
#include <condition_variable>
extern "C"{
#include <arv.h>
}

//Libraries
#include "arvteledyne.h" 
#include "fakecamera.h"
#include "uavcv.h"
#include "imagepacket.h"

//Preprocessor flags
#define TESTING true

volatile sig_atomic_t finish = 0;
std::mutex mtx;
std::condition_variable convar;
volatile bool write_img = false;
volatile bool stream_packet = false;;
volatile bool request_gps = false;
int ipic; 

int checkLogInit(){
	int saved = 0;
	FILE* tryFile = fopen("save/uav_gps.log", "r");

	if(tryFile == NULL){
		printf("Starting from scratch\n");
		return -1;
		}
	else {
		char linebuf[150];
		if (fgets(linebuf, 150, tryFile)!=NULL){
			while(fgets(linebuf,150,tryFile) != NULL){
			saved++;
			}
		}
		fclose(tryFile);
		printf("Starting after %d images\n", saved);
	    }
	return saved;
}

void writeLine(FILE* pFile){
//use stdio to printf the line
//ie. printf(/%s, %f %f... ., filename, lat, long, altitude)
//ALWAYS FLUSH
	fprintf(pFile,"go go go!\n");
	fflush(pFile);
}

void exit_signal(int param){
	finish  = 1;
	printf("Exiting on next frame completion...\n");
}

void imageWriter(){
	std::unique_lock<std::mutex> lock(mtx);
	while(1){ //repeat
		
		while(finish == 0 && write_img == false) convar.wait(lock);//Wait for work

		if(finish == 1) break;	//If finish signal is given, skip and leave;
		
		compVision::saveFullImage(ipic++);
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
	compVision* ip;
	vector<unsigned char> jpgbuffer;
	char start;
 	bool camera_ok;
	int num_saved;
	FILE* gpsfile;
	//	std::thread image_save_thrd(imageWriter);
	
	//---Construct Classes
	if (TESTING)	
		camera = new Imgfromfile();
	else
		camera = new Teledyne();

	ip = new compVision();

	//Open and Check log
	num_saved = checkLogInit();
	gpsfile = fopen("save/uav_gps.log","a");
	if (num_saved == -1){
		fprintf(gpsfile,"FILE \t LATITUDE \t LONGITUDE \t ALTITUDE \t HEADING \n");
		fflush(gpsfile);
		num_saved++;
	}

	//Initialize Camera Settings
	camera_ok = camera->initCamSetting();
	(void) signal(SIGINT,exit_signal); 	//Set up ctrl+c signal
   	ipic = num_saved;	

	//Start Camera!
	if (camera_ok) {
		printf("Start camera loop? [y/n] \n");
		start = getchar();
		getchar();

		if (start == 'y'){ 	//---Start Acquisition

			camera->startCam();

			while(!finish){  //--Main Acquisition Loop
				camera->sendTrigger();
				rawbuf = camera->getBuffer();
				if(rawbuf){ //Acquired Image
					ip->processRaw(rawbuf);
					ip->showImage();
					ipic++;
					//wakeThrd(2);
					compVision::saveFullImage(ipic++);
					jpgbuffer = ip->compressPreview();
					writeLine(gpsfile);
				}
			}

			camera->endCam();
			jpgbuffer.clear();
		}
	}
	//image_save_thrd.join();
	fclose(gpsfile);
	delete camera;
	delete ip;
	return 0;
}
