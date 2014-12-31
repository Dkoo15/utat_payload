#include <stdio.h>
#include <signal.h>

extern "C"{
#include <arv.h>
}

//Libraries
#include "arvteledyne.h" 
#include "fakecamera.h"
#include "uavcv.h"
#include "uavimage.h"

#define TESTING true

sig_atomic_t finish = 0;

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

int main(){
	Uavcam *camera; 
	unsigned char* imgbuf;
	compVision* ip;
	vector<unsigned char> jpgbuffer;
	char start;
 	int camera_ok;
	int num_saved;
	FILE* gpsfile;

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
		num_saved ++;
	}

	//Initialize Camera Settings
	camera_ok = camera->initCamSetting();
	(void) signal(SIGINT,exit_signal);
	
	//Start Camera!
	if (camera_ok) {
		printf("Start camera loop? [y/n] \n");
		start = getchar();
		getchar();

		if (start == 'y'){ 	//---Start Acquisition

			camera->startCam();

			while(!finish){  //--Main Acquisition Loop
				camera->sendTrigger();
				imgbuf = camera->getBuffer();
				if(imgbuf){
					ip->processRaw(imgbuf);
					ip->showImage();
					ip->saveFullImage(0);
					jpgbuffer = ip->compressPreview();
					writeLine(gpsfile);

				}
			}

			camera->endCam();
			jpgbuffer.clear();
		}
	}
	fclose(gpsfile);
	delete camera;
	delete ip;
	return 0;
}
