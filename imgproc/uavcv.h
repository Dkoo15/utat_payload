#include <opencv2/opencv.hpp>

#define IMGHEIGHT 3072
#define IMGWIDTH 4096

using namespace cv;

class compVision {

	public: 
		compVision();
		~compVision();

		void showImage();

		Mat rawHolder;
		Mat display;
		Mat currentFrame;
};

