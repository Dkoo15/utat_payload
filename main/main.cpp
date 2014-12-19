#include <stdio.h>

extern "C"{
#include <arv.h>
}

//Libraries
#include "arvteledyne.h" 
#include "fakecamera.h"
#include "uavcv.h"
#include "uavimage.h"

int main(){
//	Uavcam *camera  = new Imgfromfile();
	Teledyne *camera = new Teledyne(); 
	unsigned char* imgbuf;
	compVision* ip = new compVision;
	vector<unsigned char> jpgbuffer;
	char input;

	char *val, *set;
	char commands[25][100];
	int n;

	camera->parseInputs(commands,&n);

	for (int i = 0; i < n; i++){
		set = strtok(commands[i],"=");
		val = strtok(NULL,"=");
		printf("Feature = %s  |||| Value = %s", set,val);
	}	
	
/*	camera->initCamSetting();
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


	delete camera;a*/
	delete ip;
	return 0;
}
