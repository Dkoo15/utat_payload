#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define CONFIG_FILE "uav/utat_payload/options.cfg"

extern int cameratype, usegps, saveimg, view, start_delay, strm;
extern double sizefac;

bool parseConfig();
int checkLogInit();
