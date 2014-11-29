#include "uavcv.h"

compVision:: compVision(){}
compVision:: ~compVision(){}

void compVision::showImage(){

	display = imread("scratch//uscarea.png");
	namedWindow("Display Image", WINDOW_AUTOSIZE);
	imshow("Display Image", display);
	waitKey(0);


}

