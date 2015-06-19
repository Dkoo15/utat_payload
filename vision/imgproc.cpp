#include "imgproc.h"

namespace uavision
{
        cv::Mat raw;
        cv::Mat rgb;
        cv::Mat preview;
	float gain[3];
        std::vector<int> jpg_params;
	cv::Size size;

	void saveFullImage(std::string imagename){ 	//This function will be called form a separate thread
		bool iswritten;
		iswritten = cv::imwrite(imagename, preview, jpg_params); 
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

	void openViewer(){
		cv::imshow("Camera Viewer",preview);
		cv::waitKey(1000);
	}

	void processRaw(std::vector<unsigned char> &rawbuffer){	
		//Apply Gain Matrix	
		/*
		unsigned char* pix = &rawbuffer[0];
		float tmp;
		int jmax = (int) (size.height/2);
		int imax = (int) (size.width/2);
		for (int j = 0; j<jmax; j++){
			for (int i = 0; i<imax; i++){
				tmp = (float)(*pix);
				(*pix) = tmp*gain[1];
				pix++;
				tmp = (float)(*pix);
				(*pix) = tmp*gain[2];
				pix++;
			}		
			for (int i = 0; i<imax; i++){
				tmp = (float)(*pix);
				(*pix) = tmp*gain[0];
				pix++;
				tmp = (float)(*pix);
				(*pix) = tmp*gain[1];
				pix++;
			}
		}	
		*/
		raw = cv::Mat(size,CV_8UC1,&rawbuffer[0]);
		cv::cvtColor(raw,rgb,CV_BayerGB2RGB);	//Average Time = ~15 ms 
	}
	void assignData(std::vector<unsigned char> &rawbuffer){
		rgb = cv::Mat(size,CV_8UC3, &rawbuffer[0]);	
	}

	void compressPreview(std::vector<unsigned char> &jpgbufr){
		cv::imencode(".jpg",preview,jpgbufr,jpg_params);
	}
}
