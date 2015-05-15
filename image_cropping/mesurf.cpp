#include <opencv2/features2d/features2d.hpp>  
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <string>
#include <iostream>

#include "dbscan.h"

// Log:
//
// 1.jpg works with threshold 15000, doesn't work with 2 or 3
// fastnlmeans -> no improvement, still 13 keypoints.
// EUREKA! Everything works with AC1 -> filter by hue


#define ACTIVE_CHANNEL 1

float eps = 200;
int minPts = 2;
int roiProportional = 10;

cv::Point2i getMean(std::vector<cv::KeyPoint>& subsetKeys)
{
	if (subsetKeys.empty() == true)
		return cv::Point2i(0, 0);
	cv::Point2i meanpt(0, 0);
	int greatest_dist = 0;
	int maxsize = 0;

	// Calculate mean
	for (int i = 0; i < subsetKeys.size(); ++i) 
	{ 
		meanpt.x += subsetKeys[i].pt.x; 
		meanpt.y += subsetKeys[i].pt.y; 

		std::cout << "~~ x: ";
		std::cout << subsetKeys[i].pt.x;
		std::cout << " y: ";
		std::cout << subsetKeys[i].pt.y << std::endl;
	}
	meanpt.x /= subsetKeys.size();
	meanpt.y /= subsetKeys.size();

	std::cout << ">>> key pt. avg: " << meanpt.x << " , ";
	std::cout << meanpt.y << std::endl;

	return meanpt;
}

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cout << 
			"./image_proc <file> <thresh> (-1 for default)" << std::endl;
    return -1;
  }

  int thresh;
	std::vector<std::vector<cv::KeyPoint> > clusters;

	// This usually works
  if (strcmp(argv[2], "-1") == true)
	  thresh = 4000;
  else
	  thresh = atoi(argv[2]);

  std::cout << thresh << std::endl;

  cv::Mat test_im = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
  cv::Mat hsvim, outim, channels[3], roiIm, roiChan[3];
  std::vector<cv::KeyPoint> keypoints, roiPoints;

  // Convert to HSV
  std::cout << "Converting to HSV ..." << std::endl;
  cv::cvtColor(test_im, hsvim, CV_RGB2HSV); 
  cv::split(hsvim, channels);

  cv::Mat hsvchannel = channels[ACTIVE_CHANNEL];


  // Invert
  //std::cout << "Inverting image ..." << std::endl;
  //cv::bitwise_not(hsvchannel, hsvchannel);

  
  // Threshold
  //adaptiveThreshold(hsvchannel, hsvchannel, 185, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY_INV, 7, 0);

  // Denoise
	std::cout << "Blurring ..." << std::endl;
  //cv::fastNlMeansDenoising(hsvchannel, hsvchannel, 10);
  //cv::medianBlur(hsvchannel, hsvchannel, 27);
 	cv::blur(hsvchannel, hsvchannel, cv::Size(10,10));

  /*
  cv::SimpleBlobDetector::Params params;
  params.filterByInertia = false;
  params.filterByConvexity = false;
  params.filterByColor = false;
  params.filterByCircularity = false;
  params.filterByArea = true;
  //params.blobColor = 0;
  params.minArea = 800.0f;
  params.maxArea = 1600.0f;

  // Trying to use blob detector
  cv::SimpleBlobDetector blobme(params);

  blobme.detect(hsvchannel, keypoints);

  // Print out
  //std::cout << keypoints << std::endl;
  */

  // Using surf
  std::cout << "Running SURF ..." << std::endl;
  cv::SurfFeatureDetector medetect(thresh);
  medetect.detect(hsvchannel, keypoints);
	
	  // Print keypoints
  std::cout << "Drawing keypoints ..." << std::endl;
  cv::drawKeypoints(hsvchannel, keypoints, outim, (0, 255, 0), 4);
  std::cout << "Number of keypoints: " << keypoints.size() << std::endl;

	// DBSCAN actual
	std::cout << "Running DBSCAN ..." << std::endl;
	clusters = DBSCAN_keypoints(&keypoints, eps, minPts);

	std::cout << "Cluster size: " << clusters.size() << std::endl;

	for (int i = 0; i < clusters.size(); ++i)
	{
		std::ostringstream oss;
		std::cout << "::Processing cluster " << i << std::endl;

		// Calculate mean
		cv::Point2i mean;
		cv::Point2i roi;
		std::cout << ":::Getting cluster mean" << std::endl;
		mean = getMean(clusters[i]);

		if (mean.x == 0 && mean.y == 0)
			continue;

		// Get ROI size as function of image size
		roi.x = hsvchannel.cols / roiProportional;
		roi.y = hsvchannel.rows / roiProportional;

		mean.x -= roi.x / 2;
		mean.y -= roi.y / 2;

		if (mean.x < 0)
			mean.x = 0;
		else if (mean.x + roi.x > hsvchannel.cols) 
			roi.x = hsvchannel.cols - mean.x;

		if (mean.y < 0)
			mean.y = 0;
		else if (mean.y + roi.y > hsvchannel.cols) 
			roi.y = hsvchannel.cols - mean.y;

		// Name image
		std::string raw_name(argv[1]); 
		int firstindex = raw_name.find_last_of("/");
		int lastindex = raw_name.find_last_of(".");
		std::string proc_name = raw_name.substr(firstindex + 1, lastindex);
		std::cout << "ROI_" << proc_name << i << ".jpg" << std::endl;
		oss << "ROI_" << proc_name << "_" << i << ".jpg";

		roiIm = test_im(cv::Rect(mean.x, mean.y, roi.x, roi.y));
		cv::split(roiIm, roiChan);	

		// Using FAST
		std::cout << "Edge detection ..." << std::endl;
		cv::FAST(roiChan[ACTIVE_CHANNEL], roiPoints, 100);	

		std::cout << "Drawing keypoints ..." << std::endl;
		cv::drawKeypoints(roiChan[ACTIVE_CHANNEL], roiPoints, roiIm);

		// Display ROI
		std::cout << "Saving roi image ..." << std::endl;
		cv::imwrite(oss.str(), roiChan[ACTIVE_CHANNEL]);
	}

  std::cout << "Saving image ..." << std::endl;
  cv::imwrite("imout.jpg", outim);

  //cv::waitKey(0);
  std::cout << "Done." << std::endl;
  return 0;
}
