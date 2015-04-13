#include "imgproc.h"

namespace uavision
{
        cv::Mat raw;
        cv::Mat rgb;
        cv::Mat preview;
        std::vector<int> jpg_params;
	cv::Size size;

	void saveFullImage(std::string imagename){ 	//This function will be called form a separate thread
		bool iswritten;
		if (!rgb.data) return;

		iswritten = cv::imwrite(imagename, rgb, jpg_params); //Average Time ~300 ms
		std::cout <<"Saved image to file " << imagename << std::endl;

	}


	void initialize(int w, int h, bool view, int qual){
		jpg_params.push_back(CV_IMWRITE_JPEG_QUALITY);
		jpg_params.push_back(qual);
		size = cv::Size(w, h);
		if (view) cv::namedWindow("Camera Viewer", cv::WINDOW_AUTOSIZE);
	}

	void createPreview(int fac){
		double downsize = double(1/double(fac));
		cv::resize(rgb,preview,cv::Size(),downsize,downsize,cv::INTER_NEAREST);
	}

	void openViewer(int delay){
		cv::imshow("Camera Viewer",preview);
		cv::waitKey(delay*1000);
	}

	void processRaw(std::vector<unsigned char> &rawbuffer){	
		raw = cv::Mat(size,CV_8UC1,&rawbuffer[0]);
		cv::cvtColor(raw,rgb,CV_BayerGB2RGB);	//Average Time = ~15 ms 
		//whiteBalance();
	}

	void compressPreview(std::vector<unsigned char> &jpgbufr){
		cv::imencode(".jpg",preview,jpgbufr,jpg_params);
	}
}
