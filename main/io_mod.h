#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define CONFIG_FILE "uav/utat_payload/options.cfg"

extern int cameratype, usegps, saveimg, view, sizefac, jpgq, bufferq, timeout, start_delay, imgstrm;

bool parseConfig();
int checkLogInit();
