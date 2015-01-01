#include <opencv2/opencv.hpp>
#include <vector>

#define IMGHEIGHT 3072
#define IMGWIDTH 4096
#define IMGSIZE 12582912
#define DOWNSIZE 0.25
#define JPEG_QUAL 90
#define DIRECTORY "save/"
#define SHOWIMAGE true
#define STEAMIMAGE true
#define SAVEIMAGE false

namespace uavision{
	cv::Mat raw;
	cv::Mat rgb;
	cv::Mat preview;

	void initialize();
	void freemats();
	
	void processRaw(); 
	void showImage();
	vector<unsigned char> compressPreview();

	bool saveFullImage(int n);

}

