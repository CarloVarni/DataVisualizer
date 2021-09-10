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
include_directories( header/EventDataModel
		     header/Core
		     header/Algorithms )

SET( HEADER_FILES header/EventDataModel/Data.hpp 
     		  header/EventDataModel/MultiData.hpp
		  header/EventDataModel/Histogram_1D.hpp 
		  header/EventDataModel/Histogram_2D.hpp 
		  header/Core/IHolder.hpp
		  header/Core/EventContext.hpp
		  header/Core/BaseAlgorithm.hpp
		  header/Core/Scheduler.hpp )
		  
SET( SRC_FILES src/Core/BaseAlgorithm.cpp
     	       src/Core/Scheduler.cpp )

add_library( DataVisualizerLib SHARED ${SRC_FILES} ${HEADER_FILES} )

target_link_libraries( DataVisualizerLib ${ROOT_LIBRARIES} )
set_target_properties(DataVisualizerLib PROPERTIES LINKER_LANGUAGE CXX)

# Executables
add_executable( mainCode macros/maintmp.cxx )
target_link_libraries( mainCode DataVisualizerLib )
