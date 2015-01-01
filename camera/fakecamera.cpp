#include "fakecamera.h"

Imgfromfile::Imgfromfile(){
	buffer = NULL;
}

Imgfromfile:: ~Imgfromfile(){
	if(buffer) delete buffer;
}

bool Imgfromfile::initCamSetting(){
	payload = 12582912;
	buffer = new unsigned char[payload];
	i = 0;
	n = 5;
	return true;
}

void Imgfromfile::startCam(){
	printf("Reading pictures from the saved buffers \n");
}

void Imgfromfile::sendTrigger(){
	printf("Fake trigger\n");
}


unsigned char* Imgfromfile::getBuffer(){
	FILE* pFile;
	char filename[150];
	size_t result;

	if(i > n)   i = 0;
	
	sprintf(filename,"scratch/img00%d",i);	
	i++;
	pFile = fopen(filename, "rb");

	if (pFile==NULL) {
		printf("No file named %s\n", filename);
		return 0;
	}
	else printf("Opened %s\n", filename);

	result = fread(buffer, 1, payload, pFile);
	
	fclose(pFile);
	if(result != payload) 	printf("Buffer size error\n");
	return buffer;
}

void Imgfromfile::endCam(){
	printf("Fake end \n");
}
