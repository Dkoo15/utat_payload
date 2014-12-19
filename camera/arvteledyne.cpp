#include "arvteledyne.h"

Teledyne::Teledyne(){
	buffer = NULL;
}

Teledyne:: ~Teledyne(){
	if(buffer) delete buffer;
}

int Teledyne::initCamSetting(){
	ArvGcNode *feature;
 	int i, n;
	char commands[25][100];
	char *setting, *value;
	GType value_type;
	gint payload;

//  Initial Setup - Find the Camera	
	printf("Looking for the Camera...\n");
	device = arv_open_device(NULL);

	if(device == NULL) {
		printf("No camera found!\n");
		return 0;	
	}
	printf("Found %s \n", arv_get_device_id(0));
	genicam = arv_device_get_genicam(device);

// Parse inputs from a configuration file
	parseInputs(commands,&n);
	if (n == 0) {
		printf("No features found, can't take proper pictures\n");
		return 0;
	}

//Apply setting and display to confirm
	for (i = 2; i < n; i++) {
		setting = strtok(commands[i],"=");
		value = strtok(NULL,"=");
		feature = arv_gc_get_node(genicam,setting);

		if (ARV_IS_GC_FEATURE_NODE (feature)) {
			if (ARV_IS_GC_COMMAND (feature)) printf ("%s is a command\n", setting);
			else {
				if (value != NULL)
					arv_gc_feature_node_set_value_from_string (ARV_GC_FEATURE_NODE (feature), value, NULL);
				
				value_type = arv_gc_feature_node_get_value_type (ARV_GC_FEATURE_NODE (feature));
				switch (value_type) { 
				 case G_TYPE_INT64:
				 	printf("%s = %" G_GINT64_FORMAT "\n", setting, arv_gc_integer_get_value (ARV_GC_INTEGER (feature), NULL));;
			 	 	break;
			 	 case G_TYPE_DOUBLE:
			 		printf("%s = %g\n", setting, arv_gc_float_get_value(ARV_GC_FLOAT(feature),NULL));
			 		break;
				 case G_TYPE_STRING:
					printf("%s = %s\n", setting, arv_gc_string_get_value(ARV_GC_STRING(feature),NULL));
					break;
				 case G_TYPE_BOOLEAN:
					printf("%s = %s\n", setting, arv_gc_integer_get_value(ARV_GC_INTEGER(feature),NULL)!=0 ?  "true" : "false");
					break;
				 default:
					printf("%s = %s\n", setting, arv_gc_feature_node_get_value_as_string(ARV_GC_FEATURE_NODE(feature), NULL));
				}
			}
		}
	}

	feature = arv_gc_get_node (genicam, "PayloadSize");
	payload = arv_gc_integer_get_value (ARV_GC_INTEGER (feature), NULL);
	printf("PayloadSize  = %d \n", payload);

	//Create Stream and fill buffer queue
	stream = arv_device_create_stream (device, NULL, NULL);

        for (i = 0; i < BUFFER_Q_SIZE; i++)
		arv_stream_push_buffer (stream, arv_buffer_new (payload, NULL));
	
	//Get and save the node that is the software trigger
	trigger = arv_gc_get_node(genicam,"TriggerSoftware");

	return 1;
}

void Teledyne::startCam(){
	ArvGcNode *start = arv_gc_get_node(genicam, "AcquisitionStart");
	arv_gc_command_execute( ARV_GC_COMMAND(start),NULL);
	printf("Beginning camera acquisition\n");
}

void Teledyne::sendTrigger(){
	arv_gc_command_execute(ARV_GC_COMMAND(trigger),NULL);
	printf("Sent Software Trigger to Teledyne\n"); 
}


unsigned char* Teledyne::getBuffer(){
	return NULL;
}

void Teledyne::endCam(){
	ArvGcNode *end;
	guint64 n_processed_buffers, n_failures, n_underruns;

	arv_stream_get_statistics (stream, &n_processed_buffers, &n_failures, &n_underruns);
	printf("Processed buffers = %d\n", (unsigned int) n_processed_buffers);
	printf("Failures          = %d\n", (unsigned int) n_failures);
	printf("Underruns         = %d\n", (unsigned int) n_underruns);

	end = arv_gc_get_node (genicam, "AcquisitionStop");
	arv_gc_command_execute (ARV_GC_COMMAND (end), NULL);
	printf("Ended camera acquisition\n");

	g_object_unref (stream);
	g_object_unref (device);
}

void Teledyne::parseInputs(char commands[][100], int *num){
	*num = 0;
	FILE* configFile;
	configFile = fopen("teledyne.cfg", "r");

	if(configFile == NULL) return;

	while(fgets(commands[(*num)], 100, configFile) != NULL){
		(*num)++;
	}
	fclose(configFile);
}
