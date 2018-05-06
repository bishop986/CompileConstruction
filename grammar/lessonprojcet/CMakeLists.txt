cmake_minimum_required( VERSION 2.8 )

project( LessonProject )

add_definitions(-std=c++11)

set( CMAKE_CXX_FLAGS_DEBUG "-D _DEBUG_")

set( SOURCE_DIR ./main.cc
	./scan.cc
	./token.cc
	./analysis.cc
	./syntax_tree.cc)

add_executable( ${PROJECT_NAME} ${SOURCE_DIR})
