cmake_minimum_required(VERSION 3.2)
#project name

PROJECT(test_rabbit)

#head file path

INCLUDE_DIRECTORIES(

net
base

)

#source directory

AUX_SOURCE_DIRECTORY(net DIR_SRCS)
AUX_SOURCE_DIRECTORY(base DIR_SRCS)
AUX_SOURCE_DIRECTORY(HTTP DIR_SRCS)

#set environment variable

SET(TEST_RABBIT

${DIR_SRCS}

)

#set DEBUG

SET(CMAKE_BUILD_TYPE Debug)

#set extern libraries

SET(LIBRARIES pthread)

#add executable file
ADD_EXECUTABLE(test_rabbit ${TEST_RABBIT})

#add link library
TARGET_LINK_LIBRARIES(test_rabbit ${LIBRARIES})



