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
