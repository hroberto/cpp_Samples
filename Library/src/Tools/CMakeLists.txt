cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project( Library )
set ( TARGET  LibTRTools )

project(Library  VERSION  1.0.0   LANGUAGES C CXX )
message("|-- Project name ${CMAKE_PROJECT_NAME}")

set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBCPP_PRJ_BINARY_DIR} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIBCPP_PRJ_BINARY_DIR} )


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Library)


file(GLOB  SOURCE_CPP_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)


add_library( ${TARGET} STATIC ${SOURCE_CPP_FILES} )

set(BUILD_SHARED_LIBS  ON)
include( ${CMAKE_MODULE_PATH}/dependencies.cmake )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
