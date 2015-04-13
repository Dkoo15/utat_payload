#include "araviscamera.h"
#include <cstring>

AravisCam::AravisCam(){
dim[0] = 4096;
dim[1] = 3072;
}

AravisCam:: ~AravisCam(){}

bool AravisCam::initCamSetting(){
	ArvGcNode *feature;
	std::vector<std::string> settings; 
	GType value_type;
	bool config_ok;
/*  Initial Setup - Read config and find the Camera	
	config_ok = parseInputs(settings);
	if(!config_ok) return false; */
	
	std::cout<<"Looking for the Camera...\n";
	device = arv_open_device(NULL);
	
	if(device == NULL) {
		std::cout<< "No camera found!" << std::endl;
		return false;	
	}

	std::cout<< "Found "<< arv_get_device_id(0) << std::endl;
	genicam = arv_device_get_genicam(device);

/*Apply setting and display to confirm
	for  (std::vector<std::string>::size_type i = 0; i != settings.size(); i++){
		feature = arv_gc_get_node(genicam,settings[i].c_str());

		if (ARV_IS_GC_FEATURE_NODE (feature)) {
			if (ARV_IS_GC_COMMAND (feature)) std::cout<< settings[i] << " is a command" <<std::endl;
			else {
				//I can't set the feature properly. Use initteledyne
				//arv_gc_feature_node_set_value_from_string (ARV_GC_FEATURE_NODE (feature), value, NULL);
				value_type = arv_gc_feature_node_get_value_type (ARV_GC_FEATURE_NODE (feature));
				std::cout << settings[i] << " = ";

				switch (value_type) { 
				 case G_TYPE_INT64:
					std::cout << arv_gc_integer_get_value(ARV_GC_INTEGER(feature),NULL) << std::endl;
			 	 	break;
			 	 case G_TYPE_DOUBLE:
					std::cout << arv_gc_float_get_value(ARV_GC_FLOAT(feature),NULL) << std::endl;
			 		break;
				 case G_TYPE_STRING:
					std::cout << arv_gc_string_get_value(ARV_GC_STRING(feature),NULL) << std::endl;
					break;
				 case G_TYPE_BOOLEAN:
					std::cout << arv_gc_integer_get_value(ARV_GC_INTEGER(feature),NULL) << std::endl;
					break;
				 default:
					std::cout << arv_gc_feature_node_get_value_as_string(ARV_GC_FEATURE_NODE(feature),NULL) << std::endl; 
				}
			}
		}
		else
			std::cout<<settings[i]<<" is not a node!"<<std::endl;
	}
*/
	feature = arv_gc_get_node (genicam, "PayloadSize");
	payload = arv_gc_integer_get_value (ARV_GC_INTEGER (feature), NULL);
	std::cout<< "PayloadSize = " << payload << std::endl;

	//Create Stream and fill buffer queue
	stream = arv_device_create_stream (device, NULL, NULL);

        for (int i = 0; i < BUFFER_Q_SIZE; i++)
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
		do  {
			arvbufr = arv_stream_try_pop_buffer (stream);
			if (arvbufr != NULL){
				std::cout<<"Buffer: ";
				switch(arvbufr->status){
					case ARV_BUFFER_STATUS_SUCCESS: std::cout<<"buffer success"<<std::endl; break;
					case ARV_BUFFER_STATUS_TIMEOUT: std::cout<<"timeout"<<std::endl; break;
					default: std::cout<<"error"<<std::endl;;
				}
				if (arvbufr->status == ARV_BUFFER_STATUS_SUCCESS){
					memcpy(&buffer[0],arvbufr->data,payload);
					
					snapped = true;			
				}	 
				arv_stream_push_buffer (stream, arvbufr);
			}		 
		} while (arvbufr != NULL && !snapped);
	}while(cycles < WAIT_CYCLES && !snapped);

	if (cycles >= WAIT_CYCLES)	
		return false;
	else 	
		return true;
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

bool AravisCam::parseInputs(std::vector<std::string> &commands){
	std::ifstream cfgstream(CONFIG_FILE, std::ifstream::in);
	std::string word;
	bool cfg_ok = false;

	if(!cfgstream) return cfg_ok;

	if(cfgstream >> dim[0] && cfgstream >> dim[1]){
		std::getline(cfgstream,word);
		while(std::getline(cfgstream,word)) 
			commands.push_back(word);	
		cfg_ok = true;
	}

	cfgstream.close();
	return cfg_ok;
}
