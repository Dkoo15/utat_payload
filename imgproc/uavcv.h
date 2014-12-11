#include <opencv2/opencv.hpp>
#include <vector>

#define IMGHEIGHT 3072
#define IMGWIDTH 4096
#define IMGSIZE 12582912
#define DOWNSIZE 0.125
#define JPEG_QUAL 90
#define DIRECTORY "save/"
#define SHOWIMAGE true
#define STEAMIMAGE true
#define SAVEIMAGE true

using namespace cv;

class compVision {

	public: 
		compVision();
		~compVision();
		
		void processRaw(unsigned char* buffptr); 
		void showImage();
		int saveFullImage(int n);
		vector<unsigned char> compressPreview();

	private:
		Mat raw;
		Mat rgb;
		Mat preview;
		vector<int> jpg_params;
};

