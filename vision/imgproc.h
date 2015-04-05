#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#define DOWNSIZE 0.25
#define FRAME_MS 800
#define JPEG_QUAL 90
#define USE_WHITE_BALANCE false
#define VIEW true

namespace uavision{
	
	void initialize(int dim[2]);
	
	void processRaw(std::vector<unsigned char> &rawbuffer); 
	void createPreview();
	void compressPreview(std::vector<unsigned char> &jpgbufr);

	void whiteBalance();

	void saveFullImage(std::string imagename);

}

