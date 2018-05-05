cmake_minimum_required( VERSION 2.8 )

project( analysis )

set( CMAKE_CXX_FLAG_DEBUG "-D _DEBUG_ -g")

set( CMAKE_CXX_FLAG_RELEASE "")

add_definitions( -std=c++11 )

set( SOURCE_DIR ./main.cc
	./analysis.cc 
	./token.cc 
	./scan.cc)

add_executable(${PROJECT_NAME} ${SOURCE_DIR})
