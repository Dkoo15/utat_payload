#include "imgproc.h"

namespace uavision
{
        cv::Mat raw;
        cv::Mat rgb;
        cv::Mat preview;
        std::vector<int> jpg_params;
	cv::Size size;
	cv::Mat channels[3];
	int total;

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
		total = dim[0]*dim[1];
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
		whiteBalance();
	}

	void compressPreview(std::vector<unsigned char> &jpgbufr){
		cv::imencode(".jpg",preview,jpgbufr,jpg_params);
	}

	void whiteBalance(){
		float r[] = {0,256};
		int hsize = 256;
		int j;
		float n, min, max;
		float qmin = total*0.05f;
		float qmax = total*0.95f;

		const float* range = {r};
		cv::Mat hist;

		cv::split(rgb,channels);
		for (int i = 0; i<3;i++){
			cv::calcHist(&channels[i], 1,0, cv::Mat(), hist,1,&hsize,&range );
			n = 0.0f;
			j = 0;
			min = 0.0f;
			while(n < qmin){
				n += hist.at<float>(j++);
				min += 1.0f;
			}

			n = (float)total;
			j = hsize;
			max = 255.0f;
			while(n > qmax){
				n -= hist.at<float>(j--);
				max -= 1.0f;	
			}
			channels[i] = (channels[i]-min)*255.0f/(max-min); 
		}
		cv::merge(channels,3,rgb);
	}
}
