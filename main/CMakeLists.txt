add_executable(uavPayload main.cpp)
install (TARGETS uavPayload DESTINATION /Users/davidkoo/uav/)

include_directories( ../camera/)
include_directories(../imgproc/)

#Linked Libraries
target_link_libraries(uavPayload uavcam)
target_link_libraries(uavPayload imageproc)
target_link_libraries(uavPayload ${OpenCV_LIBS})
target_link_libraries(uavPayload ${glib_LIBRARY})
target_link_libraries(uavPayload ${aravis_LIBRARY})

