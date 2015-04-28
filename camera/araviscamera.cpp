#include "araviscamera.h"
#include <cstring>

AravisCam::AravisCam(){
	timeout = 20;
	bufferq = 5;
}

AravisCam:: ~AravisCam(){}

bool AravisCam::initCamSetting(int &width, int &height){
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

	std::cout<< "Found "<< arv_get_device_id(0) << std::endl;
	genicam = arv_device_get_genicam(device);

	feature = arv_gc_get_node(genicam,"Width");
	width = arv_gc_integer_get_value(ARV_GC_INTEGER (feature), NULL);
		
	feature = arv_gc_get_node(genicam,"Height");
	height = arv_gc_integer_get_value(ARV_GC_INTEGER (feature), NULL);

	feature = arv_gc_get_node (genicam, "PayloadSize");
	payload = arv_gc_integer_get_value (ARV_GC_INTEGER (feature), NULL);

	//Create Stream and fill buffer queue
	stream = arv_device_create_stream (device, NULL, NULL);

        for (int i = 0; i < bufferq; i++)
		arv_stream_push_buffer (stream, arv_buffer_new (payload, NULL));
	
	//Get and save the node that is the software trigger
	trigger = arv_gc_get_node(genicam,"TriggerSoftware");

	return true;
}

void  AravisCam::startCam(){
	ArvGcNode *start = arv_gc_get_node(genicam, "AcquisitionStart");
	arv_gc_command_execute( ARV_GC_COMMAND(start),NULL);
	std::cout<< "Beginning camera acquisition"<<std::endl;
}

void AravisCam::sendTrigger(){
	arv_gc_command_execute(ARV_GC_COMMAND(trigger),NULL);
	std::cout<< "Sent software trigger" << std::endl;
}


bool AravisCam::getBuffer(std::vector<unsigned char> &buffer){
	ArvBuffer * arvbufr;
	bool snapped = false;
	int cycles = 0;
	std::cout<<"Getting Buffer..."<<std::endl;
	do {
		g_usleep (100000);
		cycles++;
		//do  {
		for (int i = 0; i < bufferq; i++){
			arvbufr = arv_stream_try_pop_buffer (stream);
			if (arvbufr != NULL){					
				switch(arvbufr->status){
					case ARV_BUFFER_STATUS_SUCCESS: std::cout<<"Buffer Success"<<std::endl; break;
					case ARV_BUFFER_STATUS_TIMEOUT: std::cout<<"Buffer Timeout"<<std::endl; break;
					default: std::cout<<"error"<<std::endl;;
				}
				if (arvbufr->status == ARV_BUFFER_STATUS_SUCCESS){
					memcpy(&buffer[0],arvbufr->data,payload);
					snapped = true;			
				}	 
				arv_stream_push_buffer (stream, arvbufr);
			}		 
		}
		//} while (arvbufr != NULL);// && !snapped);
	}while(cycles < timeout && !snapped);

	return snapped;
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

	g_object_unref (stream);
	g_object_unref (device);
}

