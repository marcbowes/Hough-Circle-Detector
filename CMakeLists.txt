# project name
PROJECT(HOUGH_CIRCLE_DETECTOR)

# 2.4.3 is actual minimum, but you have to add additional hacks
# uncomment this if you need 2.4.3 compatibility
# if(COMMAND cmake_policy)
#	 cmake_policy(SET CMP0003 NEW)
# endif(COMMAND cmake_policy)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

# find and setup Qt4 for this project
FIND_PACKAGE(Qt4 REQUIRED)

# tell cmake to process CMakeLists.txt in these subdirs
SUBDIRS(src)

