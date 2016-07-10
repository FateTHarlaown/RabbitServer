cmake_minimum_required(VERSION 3.2)
#project name

PROJECT(test_rabbit)

#head file path

INCLUDE_DIRECTORIES(

net

)

#source directory

AUX_SOURCE_DIRECTORY(net DIR_SRCS)

#set environment variable

SET(TEST_RABBIT

${DIR_SRCS}

)

#set DEBUG

SET(CMAKE_BUILD_TYPE Debug)

#set extern libraries

SET(LIBRARIES

boost

)

#add executable file

ADD_EXECUTABLE(test_rabbit ${TEST_RABBIT})

#add link library

#TARGET_LINK_LIBRARIES(../bin/rabbit_run ${LIBRARIES})
