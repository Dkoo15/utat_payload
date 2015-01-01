#include "uavcv.h"
#include <stdio.h>

//Temp debug stuff
//#include <time.h>
//
unsigned char* compVision::rgbptr = NULL;

bool compVision::saveFullImage(int n){
	char filename[150];
	const char* dir = DIRECTORY;
	
	vector<int> jpg_params;
	jpg_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	jpg_params.push_back(JPEG_QUAL);

	if (!rgbptr) return false;
	if (!SAVEIMAGE) return false;

	sprintf(filename, "%simg%04d.jpg",dir,n);	
	printf("Saving to file %s \n", filename);

	Mat fulljpeg = Mat(IMGHEIGHT, IMGWIDTH, CV_8UC3, rgbptr);
	imwrite(filename, fulljpeg, jpg_params);

	//Average Time ~300 ms
	//This function will be called form a separate thread
	fulljpeg.release();	
	jpg_params.clear();
	return true;
}


compVision::compVision(){
	namedWindow("Image Preview", WINDOW_AUTOSIZE);
}

compVision::~compVision(){
	preview.release();
	raw.release();
	rgb.release();
}

void compVision::showImage(){
	if (!SHOWIMAGE) return;

	imshow("Image Preview",preview);
	waitKey(1000);
}

void compVision::processRaw(unsigned char* buffptr){	
	raw = Mat(IMGHEIGHT,IMGWIDTH,CV_8UC1,buffptr);
	cvtColor(raw,rgb,CV_BayerGB2RGB);
	
	printf("Bayer to RGB conversion \n");
	//Time = ~30 ms for the first time since data must be allocated
	//Average Time = ~15 ms afterwards since OpenCV resuses the old buffer
	
	resize(rgb,preview,Size(),DOWNSIZE,DOWNSIZE,INTER_NEAREST);
	//Average time = ~2 ms
	
	rgbptr = rgb.data;
}

vector <unsigned char> compVision::compressPreview(){
	vector<unsigned char> jpgbuff; 
	vector<int> jpg_params;
	jpg_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	jpg_params.push_back(JPEG_QUAL);

	printf("Compressing preview into jpeg buffer\n");
	imencode(".jpg",preview,jpgbuff,jpg_params);

	//Average time ~10 ms
	//This could go into the main execution loop

	return jpgbuff;
}
