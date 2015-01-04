#include "imgproc.h"
//#include <time.h>

namespace uavision
{
        cv::Mat raw;
        cv::Mat rgb;
        cv::Mat preview;
        std::vector<int> jpg_params;
	int width, height;

	bool saveFullImage(std::string imagename){
		if (!rgb.data) return false;
		if (!RECORD) return false;

		std::cout <<"Saving to file " << imagename << std::endl;
		cv::imwrite(imagename, rgb, jpg_params); //Average Time ~300 ms

		//This function will be called form a separate thread
		return true;
	}


	void initialize(int h, int w){
		width = w;
		height = h;
		jpg_params.push_back(CV_IMWRITE_JPEG_QUALITY);
		jpg_params.push_back(JPEG_QUAL);

		if (VIEW) cv::namedWindow("Camera Viewer", cv::WINDOW_AUTOSIZE);
	}

	void freeMats(){
		preview.release();
		raw.release();
		rgb.release();
		jpg_params.clear();
	}

	void createPreview(){
		cv::resize(rgb,preview,cv::Size(),DOWNSIZE,DOWNSIZE,cv::INTER_NEAREST);
		//Average time = ~2 ms

		if (VIEW){
			cv::imshow("Camera Viewer",preview);
			cv::waitKey(1000);
		}
	}

	void processRaw(unsigned char* buffptr){	
		raw = cv::Mat(height,width,CV_8UC1,buffptr);
		cv::cvtColor(raw,rgb,CV_BayerGB2RGB);
	
		std::cout << "Bayer to RGB conversion" << std::endl;	
		//Average Time = ~15 ms afterwards since OpenCV resuses the old buffer
	}

	void compressPreview(std::vector<unsigned char> &jpgbufr){
		std::cout << "Compressing preview into jpeg" << std::endl;		
		cv::imencode(".jpg",preview,jpgbufr,jpg_params);
	}
}
