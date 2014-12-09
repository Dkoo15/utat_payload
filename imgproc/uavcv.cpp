#include "uavcv.h"

//Temp debug stuff
#include <stdio.h>
#include <time.h>

compVision:: compVision(){
	jpg_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	jpg_params.push_back(JPEG_QUAL);

}

compVision:: ~compVision(){
	preview.release();
	raw.release();
}

void compVision::showImage(){
	namedWindow("Image Preview", WINDOW_AUTOSIZE);
	imshow("Image Preview",preview);
	waitKey(0);
}

void compVision::processRaw(unsigned char* buffptr){	

	clock_t strt;
	double elapsed;
	char a;
	
	raw = Mat(IMGHEIGHT,IMGWIDTH,CV_8UC1,buffptr);
	
	a = getchar();
	strt = clock();		
	
	cvtColor(raw,rgb,CV_BayerGB2RGB);
	
	elapsed = (clock()-strt)/(double)(CLOCKS_PER_SEC/1000);
	printf("Time for bayer to rgb conversion %.5f ms \n", elapsed);
	a = getchar();
	
	//Average Time = ~15 ms once data has allocated. 30 ms fo first time 
	strt=clock();
	
	resize(rgb,preview,Size(),DOWNSIZE,DOWNSIZE,INTER_NEAREST);
//	
//	Average time = ~2 ms
	elapsed = (clock()-strt)/(double)(CLOCKS_PER_SEC/1000);
	printf("Time for image resize  %.5f ms \n", elapsed);
	a = getchar();
}

int compVision::saveFullImage(int n){
	char filename[150];
	const char* dir = DIRECTORY;
	
	if (!rgb.data) return -1;

	clock_t strt;
	double elapsed;
	char a;
	sprintf(filename, "%simg%04d.jpg",dir,n);	
	printf("Saving to file %s \n", filename);
	a = getchar();
	strt = clock();

	imwrite(filename, rgb, jpg_params);
	
	elapsed = (clock()-strt)/(double)(CLOCKS_PER_SEC/1000);
	printf("Time for image jpg compression and write  %.5f ms \n", elapsed);

	return 1;
}

vector <unsigned char> compVision::compressPreview(){
	
	clock_t strt;
	double elapsed;
	char a;

	vector<unsigned char> jpgbuff; 

	printf("Compressing preview into jpeg buffer");
	a = getchar();
	strt = clock();

	imencode(".jpg",preview,jpgbuff,jpg_params);

	elapsed = (clock()-strt)/(double)(CLOCKS_PER_SEC/1000);
	printf("Time for preview jpg compression  %.5f ms \n", elapsed);

	return jpgbuff;
}
