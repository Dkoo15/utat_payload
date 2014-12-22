#include <stdio.h>

extern "C"{
#include <arv.h>
}

//Libraries
#include "arvteledyne.h" 
#include "fakecamera.h"
#include "uavcv.h"
#include "uavimage.h"

#define TESTING true

int main(){
	Uavcam *camera; 
	unsigned char* imgbuf;
	compVision* ip;
	vector<unsigned char> jpgbuffer;
	char input;
 	int camera_ok;

	//---Construct Classes
	if (TESTING)	
		camera = new Imgfromfile();
	else
		camera = new Teledyne();

	ip = new compVision();

	//---Initialize Camera Settings
	camera_ok = camera->initCamSetting();

	if (camera_ok) {
		printf("Start camera loop? [y/n] \n");
		input = getchar();
		getchar();

		if (input == 'y'){ 	//---Start Acquisition

			camera->startCam();

			while(input != 'x'){  //--Main Acquisition Loop
				camera->sendTrigger();
				imgbuf = camera->getBuffer();
				if(imgbuf){
					ip->processRaw(imgbuf);
					ip->showImage();
					ip->saveFullImage(0);
					jpgbuffer = ip->compressPreview();
					printf("Press x to close\n");
					input = getchar();
				}
			}

			camera->endCam();
			jpgbuffer.clear();
		}
	}
	delete camera;
	delete ip;
	return 0;
}

int checkLogInit(FILE *pfile){
//Open the gps.log file and check if it has been written.
//If exists, read the log and find out the line, then add new photos to that line
//If it doesnt exist, create the log
// Later write a cleanup.sh script to make a folder and mv *.jpg *.log to that folder/
}

void writeLine(){
//use stdio to printf the line
//ie. printf(/%s, %f %f... ., filename, lat, long, altitude)
//fflush() - to print the the file!
}
