#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#define IMGHEIGHT 3072
#define IMGWIDTH 4096
#define IMGSIZE 12582912
#define DOWNSIZE 0.25
#define JPEG_QUAL 90
#define VIEW true
#define RECORD false

namespace uavision{
	
	void initialize();
	void freeMats();
	
	void processRaw(unsigned char* buffptr); 
	void showImage();
	std::vector<unsigned char> compressPreview();

	bool saveFullImage(std::string imagename);

}

