cmake_minimum_required( VERSION 2.8 )

project(cminus)

add_definitions(-std=c++11)

set( CMAKE_CXX_FLAGS_DEBUG "-D _DEBUG_")

set( SOURCE_DIR 
	./main.cc
	./parser.cc
	./token.cc)

add_executable( ${PROJECT_NAME} ${SOURCE_DIR})
