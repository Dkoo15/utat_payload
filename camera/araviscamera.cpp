#include "araviscamera.h"
#include <cstring>

#define TIMEOUT 3
#define BUFFERQ 5

AravisCam::AravisCam(){}

AravisCam:: ~AravisCam(){
	endCam();
	delete rawbuffer;
}

bool AravisCam::initializeCam(){
	ArvGcNode *feature;
	std::vector<std::string> settings; 
	GType value_type;
	bool config_ok;

//  Initial Setup - Find the Camera	
	std::cout<<"Looking for the Camera...\n";
	device = arv_open_device(NULL);
	
	if(device == NULL) {
		std::cout<< "No camera found!" << std::endl;
		return false;	
	}
	
	acquisition = false;

	std::cout<< "Found "<< arv_get_device_id(0) << std::endl;
	genicam = arv_device_get_genicam(device);

	feature = arv_gc_get_node(genicam,"Width");
	this->width = arv_gc_integer_get_value(ARV_GC_INTEGER (feature), NULL);
		
	feature = arv_gc_get_node(genicam,"Height");
	this->height = arv_gc_integer_get_value(ARV_GC_INTEGER (feature), NULL);

	feature = arv_gc_get_node (genicam, "PayloadSize");
	payload = arv_gc_integer_get_value (ARV_GC_INTEGER (feature), NULL);

	//Create Stream and fill buffer queue
	stream = arv_device_create_stream (device, NULL, NULL);

        for (int i = 0; i < BUFFERQ; i++)
		arv_stream_push_buffer (stream, arv_buffer_new (payload, NULL));
	
	//Allocate buffer and size
	rawbuffer = new unsigned char[payload];	
	size = cv::Size(width, height);

	//Get and save the node that is the software trigger
	triggernode = arv_gc_get_node(genicam,"TriggerSoftware");

	return true;
}

void AravisCam::trigger(){
	if(acquisition == false)
		startCam();

	arv_gc_command_execute(ARV_GC_COMMAND(triggernode),NULL);
	std::cout<< "Sent software trigger" << std::endl;
}

bool AravisCam::getBuffer(){
	ArvBuffer * arvbufr;
	bool gotbuf = false;
	int cycles = 0;

	std::cout<<"Getting Buffer...";
	do {
		g_usleep (50000);
		cycles++;
		do  {
			arvbufr = arv_stream_try_pop_buffer (stream);
			if (arvbufr != NULL){
				switch(arvbufr->status){
					case ARV_BUFFER_STATUS_SUCCESS: std::cout<<"Success"<<std::endl; break;
					case ARV_BUFFER_STATUS_TIMEOUT: std::cout<<"Timeout"<<std::endl; break;
					default: std::cout<<"Error"<<std::endl;;
				}
				if (arvbufr->status == ARV_BUFFER_STATUS_SUCCESS){
					memcpy(rawbuffer,arvbufr->data,payload);
					gotbuf = true;			
				}	 
				arv_stream_push_buffer (stream, arvbufr);
			}		 
		} while (arvbufr != NULL && !gotbuf);
	}while(cycles < TIMEOUT && !gotbuf);

	return (cycles<TIMEOUT);
}

void  AravisCam::startCam(){
	ArvGcNode *start = arv_gc_get_node(genicam, "AcquisitionStart");
	arv_gc_command_execute( ARV_GC_COMMAND(start),NULL);
	std::cout<< "Beginning camera acquisition"<<std::endl;
	acquisition = true;
	g_usleep(50000);
}

void AravisCam::endCam(){
	ArvGcNode *end;
	guint64 n_processed_buffers, n_failures, n_underruns;

	arv_stream_get_statistics (stream, &n_processed_buffers, &n_failures, &n_underruns);
	std::cout << "Processed\t = " << (unsigned int) n_processed_buffers << "\n";
	std::cout << "Failures\t  = " << (unsigned int) n_failures << "\n";
	std::cout << "Underruns\t = " << (unsigned int) n_underruns << "\n";

	end = arv_gc_get_node (genicam, "AcquisitionStop");
	arv_gc_command_execute (ARV_GC_COMMAND (end), NULL);
	std::cout << "Ended Camera Acquisition" << std::endl;
	acquisition = false;

	g_object_unref (stream);
	g_object_unref (device);
}

bool AravisCam::getImage(cv::Mat &frame){	

	bool image_ok = getBuffer();
	
/*	unsigned char* pix = rawbuffer;
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
	}	*/	

	if(image_ok){
		rawmat = cv::Mat(size,CV_8UC1,rawbuffer);
		cv::cvtColor(rawmat,frame,CV_BayerGB2RGB);	//Average Time = ~15 ms 
	}
	
	return image_ok;
}

