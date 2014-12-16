#include <stdio.h>

//Libraries
#include "arvteledyne.h" 
#include "fakecamera.h"
#include "uavcv.h"
#include "uavimage.h"

int main(){
	Uavcam *camera  = new Imgfromfile();
	unsigned char* imgbuf;
	compVision* ip = new compVision;
	vector<unsigned char> jpgbuffer;
	char input;
	
	
	camera->initCamSetting();
	printf("Start camera loop?\n");
	camera->startCam();
	input = getchar();
//	while(input != 'x'){
		
		camera->sendTrigger();
		imgbuf = camera->getBuffer();
		ip->processRaw(imgbuf);
		ip->showImage();
		ip->saveFullImage(0);
		jpgbuffer = ip->compressPreview();
		printf("Size of compress preview jpg %lu", jpgbuffer.size());
//		printf("Press x to close\n");
//		input = getchar();
//	}
	camera->endCam();
	jpgbuffer.clear();
	delete camera;
	delete ip;
	return 0;
}
