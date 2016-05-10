#pragma once
#include <opencv2/opencv.hpp>

#include <vector>
#include <iostream>

class Uavcam {

	public: 
		Uavcam(){}
		virtual ~Uavcam(){}
		
		virtual bool initializeCam() = 0;
		virtual bool trigger(cv::Mat &frame) = 0;
		virtual bool getImage(cv::Mat &frame) = 0;

};
