#pragma once
#include <opencv2/opencv.hpp>

std::vector<std::vector<cv::KeyPoint> > DBSCAN_keypoints(std::vector<cv::KeyPoint> *keypoints, float eps, int minPts);

std::vector<int> regionQuery(std::vector<cv::KeyPoint> *keypoints, cv::KeyPoint *keypoint, float eps);
