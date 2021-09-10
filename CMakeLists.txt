cmake_minimum_required( VERSION 3.10 FATAL_ERROR )
project ( DataVisualizer )

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ROOT
list( APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS} )
find_package(ROOT REQUIRED COMPONENTS RIO Net ASImage Minuit)
include( ${ROOT_USE_FILE} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )

# My Code
include_directories( header )

SET( HEADER_FILES header/Data.hpp 
     		  header/MultiData.hpp
		  header/Histogram_1D.hpp 
		  header/Histogram_2D.hpp )
SET( SRC_FILES src/Data.cpp )

add_library( DataVisualizerLib SHARED ${SRC_FILES} ${HEADER_FILES} )
target_link_libraries( DataVisualizerLib ${ROOT_LIBRARIES} )

# Executables
add_executable( mainCode macros/maintmp.cxx )
target_link_libraries( mainCode DataVisualizerLib )
