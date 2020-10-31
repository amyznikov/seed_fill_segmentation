CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)

project(seedfill_segmentation CXX)

if( NOT CMAKE_CXX_STANDARD )
	set (CMAKE_CXX_STANDARD 14)
endif()

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE OFF)
set(CMAKE_LINK_WHAT_YOU_USE OFF)


find_package(OpenCV REQUIRED)


set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE SOURCE_FILES
	${SRC}/*.c
	${SRC}/*.cc
	${SRC}/*.cpp)

file(GLOB_RECURSE HEADER_FILES
	${SRC}/*.h
	${SRC}/*.hpp)

set(APP_INCLUDES
	${SRC}
	${OpenCV_INCLUDE_DIRS})

set(APP_LIBRARIES
	${OpenCV_LIBS})

include_directories(
    ${APP_INCLUDES})

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
    ${HEADER_FILES})

target_link_libraries(${PROJECT_NAME}
	${APP_LIBRARIES})

add_custom_target(uninstall-${PROJECT_NAME}
	xargs rm -vf < ${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt)

