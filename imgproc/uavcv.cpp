#include "uavcv.h"

//Temp debug stuff
//#include <stdio.h>
//#include <time.h>

compVision:: compVision(){}
compVision:: ~compVision(){
	preview.release();
	raw.release();
}

void compVision::showImage(){
	namedWindow("Image Preview", WINDOW_AUTOSIZE);
	imshow("Image Preview",preview);
	waitKey(0);
}

void compVision::processRaw(char* buffptr){	

//	clock_t strt;
//	double elapsed;
//	char a;
	
	raw = Mat(IMGHEIGHT,IMGWIDTH,CV_8UC1,buffptr);

//	strt = clock();	
	cvtColor(raw,rgb,CV_BayerGB2RGB);
//	elapsed = (clock()-strt)/(double)(CLOCKS_PER_SEC/1000);
//	printf("Time for bayer to rgb conversion %.5f ms \n", elapsed);
//	a = getchar();
	//Average Time = ~15 ms once data has allocated. 30 ms fo first time 
//	strt=clock();
	resize(rgb,preview,Size(),DOWNSIZE,DOWNSIZE,INTER_NEAREST);
//	Average time = ~2 ms
//	elapsed = (clock()-strt)/(double)(CLOCKS_PER_SEC/1000);
//	printf("Time for image resize  %.5f ms \n", elapsed);
	//a = getchar();
}
