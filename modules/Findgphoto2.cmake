
include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(gphoto2_PKGCONF gphoto2)

# Include dir
find_path(gphoto2_INCLUDE_DIR
	NAMES 
	gphoto2.h
	PATHS 
	${gphoto2_PKGCONF_INCLUDE_DIRS} 
	/usr/local/include
	/usr/local/include/gphoto2
)

# Finally the library itself
find_library(gphoto2_LIBRARY
	NAMES 
	libgphoto2
	gphoto2
	libgphoto2.6
	PATHS 
	${gphoto2_PKGCONF_LIBRARY_DIRS}
	/usr/local/lib
	/usr/local/bin
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
libfind_process(gphoto2)

