#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

namespace uavision{
	
	void initialize(int, int, bool, int);
	
	void processRaw(std::vector<unsigned char> &rawbuffer); 
	void createPreview(int fac);
	void openViewer(int delay);
	void compressPreview(std::vector<unsigned char> &jpgbufr);

	void saveFullImage(std::string imagename);

}
