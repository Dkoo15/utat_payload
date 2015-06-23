add_executable(utxPayload io_mod.cpp uavmain.cpp)
install (TARGETS utxPayload DESTINATION /usr/local/bin/)

include_directories( ../camera/)
include_directories(../gps/)

#Linked Libraries
target_link_libraries(utxPayload uavcam)
target_link_libraries(utxPayload gpsmodule)
target_link_libraries(utxPayload ${OpenCV_LIBS})
target_link_libraries(utxPayload ${GLIB_LIBRARIES})
target_link_libraries(utxPayload ${GOBJECT_LIBRARIES})
target_link_libraries(utxPayload ${aravis_LIBRARY})
target_link_libraries(utxPayload ${LIBGPS_LIBRARIES})
