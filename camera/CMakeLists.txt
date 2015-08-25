if(USE_ARAVIS)
	add_library(uavcam araviscamera.cpp webcamera.cpp)
else(USE_ARAVIS)
	add_library(uavcam webcamera.cpp)
endif(USE_ARAVIS)

