cmake_minimum_required( VERSION 2.8 )

project( tiny )

add_definitions(-std=c++14)

set( CMAKE_CXX_FLAGS_DEBUG "-D _DEBUG_" )

set( SOURCE_DIR 
	./main.cc 
	./token.cc
	./scan.cc
	./analysis.cc
	./syntax_treenode.cc)

add_executable(${PROJECT_NAME} ${SOURCE_DIR})
