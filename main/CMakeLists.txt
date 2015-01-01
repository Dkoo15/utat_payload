add_executable(utxPayload uavmain.cpp)
install (TARGETS utxPayload DESTINATION /Users/davidkoo/uav/)

include_directories( ../camera/)
include_directories(../vision/)

#Linked Libraries
target_link_libraries(utxPayload uavcam)
target_link_libraries(utxPayload vision)
target_link_libraries(utxPayload ${OpenCV_LIBS})
target_link_libraries(utxPayload ${GLIB_LIBRARIES})
target_link_libraries(utxPayload ${GOBJECT_LIBRARIES})
target_link_libraries(utxPayload ${aravis_LIBRARY})

