# # - Find Apache Portable Runtime
# #
# # Once done this will define
# #  APR_FOUND - System has APR
# #  APR_INCLUDE_DIRS - The APR include directories
# #  APR_LIBRARIES - The libraries needed to use APR
# #  APR_DEFINITIONS - Compiler switches required for using APR

# include(Genial/LibFindMacros)

# libfind_pkg_check_modules(APR_PKGCONF apr-1)

# find_path(APR_INCLUDE_DIR apr.h
#           DOC "APR include file"
#           HINTS ${APR_PKGCONF_INCLUDE_DIRS}
#           PATH_SUFFIXES apr-1)

# find_library(APR_LIBRARY NAMES apr apr-1 libapr-1.0 libapr-1
#              DOC "APR library file"
#              HINTS ${APR_PKGCONF_LIBRARY_DIRS} )

# libfind_process(APR)


# find_path( QPID_PROTON_INCLUDE_DIR
#            NAMES cproton.i
#            PATH_SUFFIXES proton
#            DOC "Qpid Proton C++ include files" )

find_library( QPID_PROTON_LIBRARY 
            NAMES qpid-proton-cpp
            DOC "Qpid Proton C++ library" )
