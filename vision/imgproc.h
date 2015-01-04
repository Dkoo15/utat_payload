#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#define DOWNSIZE 0.25
#define JPEG_QUAL 90
#define VIEW true
#define RECORD false

namespace uavision{
	
	void initialize(int h, int w);
	void freeMats();
	
	void processRaw(unsigned char* buffptr); 
	void createPreview();
	void compressPreview(std::vector<unsigned char> &jpgbufr);

	bool saveFullImage(std::string imagename);

}

