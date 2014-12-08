#include <stdio.h>

//Libraries
#include "arvteledyne.h" 
#include "fakecamera.h"
#include "uavcv.h"

int main(){
	Uavcam *camera  = new Imgfromfile();
	char* imgbuf;
	compVision* ip = new compVision;
	char input;
	
	
	camera->initCamSetting();
	printf("Start camera loop?\n");
	input = getchar();
	while(input != 'x'){
		
		camera->sendTrigger();
		imgbuf = camera->getBuffer();
		ip->processRaw(imgbuf);
		ip->showImage();
		printf("Press x to close\n");
		input = getchar();
	}

	delete camera;
	delete ip;
	return 0;
}
