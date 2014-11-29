
include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(glib_PKGCONF glib-2.0)

# Include dir
find_path(glib_INCLUDE_DIR
	NAMES 
	glib.h
	glib-object.h
	gmodule.h`
	PATHS 
	${glib_PKGCONF_INCLUDE_DIRS} 
	/usr/local/include
	/usr/local/include/glib2.0
	/usr/local/include/gio-unix-2.0
)

# Finally the library itself
find_library(glib_LIBRARY
	NAMES 
	glib-2.0
	libglib-2.0
	libgobject-2.0
	libgio-2.0
	libgobject-2.0
	libgthread-2.0
	PATHS 
	${glib_PKGCONF_LIBRARY_DIRS}
	/usr/local/lib
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
libfind_process(glib)

