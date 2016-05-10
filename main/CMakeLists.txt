cmake_minimum_required(VERSION 3.0)
add_executable(utxPayload io_utils.cpp uavmain.cpp)
install (TARGETS utxPayload DESTINATION /usr/local/bin/)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories( ../camera/)
include_directories(../gps/)

#Linked Libraries
target_link_libraries(utxPayload uavcam)
target_link_libraries(utxPayload gpsmodule)
target_link_libraries(utxPayload ${OpenCV_LIBS})
target_link_libraries(utxPayload ${LIBGPS_LIBRARIES})

if(USE_ARAVIS)
	target_link_libraries(utxPayload ${GLIB_LIBRARIES})
	target_link_libraries(utxPayload ${GOBJECT_LIBRARIES})
	target_link_libraries(utxPayload ${aravis_LIBRARY})
endif(USE_ARAVIS)

