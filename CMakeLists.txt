cmake_minimum_required(VERSION 3.25)
project(QtMainLoopWDT)

add_subdirectory(mainLoopWDT)

option(Test "build usage example application" OFF)

if(Test)
	add_subdirectory(testApp)
endif()
