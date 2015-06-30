cmake_minimum_required(VERSION 3.0.2)
project("UTAT UAV")

option(USE_ARAVIS "Use Aravis Library for GenieCam" OFF)
option(USE_GPHOTO "Use Gphoto Library for DSLR" ON)

set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")
FIND_PACKAGE(OpenCV REQUIRED)
FIND_PACKAGE(Gpsd REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${LIBGPS_INCLUDE_DIR})

configure_file (
	"${PROJECT_SOURCE_DIR}/config.h.in"
	"${PROJECT_BINARY_DIR}/config.h"
)
include_directories("${PROJECT_BINARY_DIR}")

if (USE_ARAVIS)
	message ("Using ARAVIS Library")
	FIND_PACKAGE(GObject REQUIRED)
	FIND_PACKAGE(aravis REQUIRED)
	include_directories(${GOBJECT_INCLUDE_DIRS})
	include_directories(${aravis_INCLUDE_DIR})
endif(USE_ARAVIS)

if (USE_GPHOTO)
	message ("Using gphoto2")
	FIND_PACKAGE(gphoto2 REQUIRED)
	include_directories(${gphoto2_INCLUDE_DIR})
endif(USE_GPHOTO)

add_subdirectory(main)
add_subdirectory(camera)
add_subdirectory(gps)
