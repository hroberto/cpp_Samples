cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project( SharedMemory VERSION  1.0.0   LANGUAGES C CXX )
message("|-- Project name ${CMAKE_PROJECT_NAME}")

set ( TARGET  sm_client )


# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Library)


file(GLOB  MAIN_SOURCE_CPP_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(${TARGET}  ${MAIN_SOURCE_CPP_FILES} )
target_link_libraries( ${TARGET} ${LIBCPP_PRJ_LIBRARIES} )


set(_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}/)

set_target_properties( ${TARGET}  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG  "${CMAKE_SOURCE_DIR}/bin/Debug/${PROJECT_NAME}/"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/Release/${PROJECT_NAME}/"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/bin/Release/${PROJECT_NAME}/"
)


set(BUILD_SHARED_LIBS  ON)
include( ${CMAKE_MODULE_PATH}/dependencies.cmake )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


##############
## CONFIG
execute_process( COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/config/${PROJECT_NAME}/ )
execute_process( COMMAND mkdir -p ${_OUTPUT_DIRECTORY} )

file(GLOB CONFIG_FILES  ${CMAKE_SOURCE_DIR}/config/${PROJECT_NAME}/*)
foreach(config_file IN ITEMS  ${CONFIG_FILES} )
    execute_process( COMMAND ln -sf ${config_file}  ${_OUTPUT_DIRECTORY} )
endforeach()


#include(${CMAKE_HOME_DIRECTORY}/cmake/PrintEnv.cmake)
#print_env_cmake()