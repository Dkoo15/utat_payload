
include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(gstreamer_PKGCONF gstreamer)

# Include dir
find_path(gstreamer_INCLUDE_DIR
	NAMES 
	gst/gst.h
	PATHS 
	${gstreamer_PKGCONF_INCLUDE_DIRS} 
	/usr/include
	/usr/include/gstreamer-0.10
)

# Finally the library itself
find_library(gstreamer_LIBRARY
	NAMES 
	libgstcoreelements
	gstcoreelements
	PATHS 
	${gstreamer_PKGCONF_LIBRARY_DIRS}
	/usr/lib/
	/usr/lib/gstreamer-0.10/
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
libfind_process(gstreamer)

