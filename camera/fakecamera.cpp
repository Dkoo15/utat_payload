#include "fakecamera.h"

Imgfromfile::Imgfromfile(){}

Imgfromfile:: ~Imgfromfile(){}

bool Imgfromfile::initCamSetting(int &width, int &height){
	width = 4096;
	height = 3072;
	payload = 12582912;
	i = 1;
	n = 8;
	return true;
}

void Imgfromfile::startCam(){
	std::cout << "Reading pictures from saved buffers/" << std::endl;
}

void Imgfromfile::sendTrigger(){
	std::cout << "Fake software trigger" << std::endl;
}


bool Imgfromfile::getBuffer(std::vector<unsigned char> &buffer){
	std::ifstream bufferfile;
	std::stringstream ss;
	std::string filename;

	if(i > n)   i = 1;
	
	ss << "uav/utat_payload/camera/stockbuffers/im000" <<i;
	filename = ss.str();
	i++;
	bufferfile.open(filename,std::ifstream::binary);

	if (!bufferfile) {
		std::cout << "No file named "<< filename << std::endl;
		return false;
	}
	else std::cout << "Opened " << filename << std::endl;

	bufferfile.read((char*)&buffer[0],payload);
	
	if(bufferfile) std::cout<<"Read successful"<<std::endl;
	else std::cout<<"Read unsuccessful"<<std::endl;

	bufferfile.close();	

	return true;
}

void Imgfromfile::endCam(){
	std::cout << "Fake camera finish" << std::endl;
}
