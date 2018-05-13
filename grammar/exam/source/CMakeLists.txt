cmake_minimum_required( VERSION 2.8 )

project(thread)

add_definitions(-std=c++11)

set( CMAKE_CXX_FLAGS_DEBUG "-D _DEBUG_")

set( SOURCE_DIR 
	./token.cc
	./scan.cc
	./main.cc
	./syntax_treenode.cc
	./analysis.cc)

add_executable(${PROJECT_NAME} ${SOURCE_DIR})
