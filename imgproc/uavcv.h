#include <opencv2/opencv.hpp>

#define IMGHEIGHT 3072
#define IMGWIDTH 4096
#define IMGSIZE 12582912
#define DOWNSIZE 0.125
#define SHOWIMAGE true
#define STEAMIMAGE true
#define SAVEIMAGE true

using namespace cv;

class compVision {

	public: 
		compVision();
		~compVision();
		
		void processRaw(char* buffptr); 
		void showImage();

		Mat raw;
		Mat rgb;
		Mat preview;
};

