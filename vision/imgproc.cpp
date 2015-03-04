#include "imgproc.h"

namespace uavision
{
        cv::Mat raw;
        cv::Mat rgb;
        cv::Mat preview;
	cv::Mat wb;
        std::vector<int> jpg_params;
	cv::Size size;

	void saveFullImage(std::string imagename){ 	//This function will be called form a separate thread
		bool iswritten;
		if (!rgb.data) return;

		iswritten = cv::imwrite(imagename, rgb, jpg_params); //Average Time ~300 ms
		std::cout <<"Saved image to file " << imagename << std::endl;

	}


	void initialize(int dim[2]){
		jpg_params.push_back(CV_IMWRITE_JPEG_QUALITY);
		jpg_params.push_back(JPEG_QUAL);
		size = cv::Size(dim[0], dim[1]);

		if (VIEW) cv::namedWindow("Camera Viewer", cv::WINDOW_AUTOSIZE);
	}

	void createPreview(){
		cv::resize(rgb,preview,cv::Size(),DOWNSIZE,DOWNSIZE,cv::INTER_NEAREST);
		if (VIEW){
			cv::imshow("Camera Viewer",preview);
			cv::waitKey(FRAME_MS);
		}
	}

	void processRaw(std::vector<unsigned char> &rawbuffer){	
		raw = cv::Mat(size,CV_8UC1,&rawbuffer[0]);
		cv::cvtColor(raw,rgb,CV_BayerGB2RGB);	//Average Time = ~15 ms 
	}

	void compressPreview(std::vector<unsigned char> &jpgbufr){
		cv::imencode(".jpg",preview,jpgbufr,jpg_params);
	}

	void whiteBalance(){
		cv::Scalar m = cv::mean(preview);
		double a, B;
		a = m.val[1]/m.val[0];
		B = m.val[1]/m.val[2];
		cv::Scalar gain = cv::Scalar(a,1.0,B);
		wb = preview.mul(gain);
	}
}
