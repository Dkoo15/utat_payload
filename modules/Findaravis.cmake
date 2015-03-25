
include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(aravis_PKGCONF aravis-0.4)

# Include dir
find_path(aravis_INCLUDE_DIR
	NAMES 
	arv.h
	PATHS 
	${aravis_PKGCONF_INCLUDE_DIRS} 
	/usr/local/include
	/usr/local/include/aravis-0.4
)

# Finally the library itself
find_library(aravis_LIBRARY
	NAMES 
	libaravis-0.4
	aravis
	aravis-0.4
	libaravis
	PATHS 
	${aravis_PKGCONF_LIBRARY_DIRS}
	/usr/local/lib
	/usr/local/bin
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
libfind_process(aravis)

