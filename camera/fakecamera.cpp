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
	std::cout << "Reading pictures from scratch/" << std::endl;
}

void Imgfromfile::sendTrigger(){
	std::cout << "Fake software trigger" << std::endl;
}


unsigned char* Imgfromfile::getBuffer(){
	std::ifstream bufferfile;
	std::stringstream ss;
	std::string filename;

	if(i > n)   i = 0;
	
	ss << "scratch/img00" <<i;
	filename = ss.str();
	i++;
	bufferfile.open(filename,std::ifstream::binary);

	if (!bufferfile) {
		std::cout << "No file named "<< filename << std::endl;
		return 0;
	}
	else std::cout << "Opened " << filename << std::endl;

	bufferfile.read((char*)buffer,payload);
	
	if(bufferfile) std::cout<<"Read successful"<<std::endl;
	else std::cout<<"Read unsuccessful"<<std::endl;

	bufferfile.close();	

	return buffer;
}

void Imgfromfile::endCam(){
	std::cout << "Fake camera finish" << std::endl;
}
