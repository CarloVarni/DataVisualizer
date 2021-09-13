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
		  header/Core/Scheduler.hpp
		  header/Core/Messager.hpp
		  header/Algorithms/RootFileReaderAlgorithm.hpp
		  header/Algorithms/SeedingPerformanceRootReaderAlgorithm.hpp
		  header/Algorithms/DataHistogramMakerAlgorithm.hpp
		  header/Algorithms/MultiDataHistogramMakerAlgorithm.hpp	
		  header/Algorithms/FilteringAlgorithm.hpp
		  header/Algorithms/PlotComparisonAlgorithm.hpp
		  header/Algorithms/PlotterAlgorithm.hpp )
		  
SET( SRC_FILES src/Core/BaseAlgorithm.cpp
     	       src/Core/Scheduler.cpp
	       src/Core/Messager.cpp
	       src/Algorithms/RootFileReaderAlgorithm.cpp
	       src/Algorithms/SeedingPerformanceRootReaderAlgorithm.cpp
	       src/Algorithms/DataHistogramMakerAlgorithm.cpp
	       src/Algorithms/MultiDataHistogramMakerAlgorithm.cpp
	       src/Algorithms/PlotComparisonAlgorithm.cpp
	       src/Algorithms/PlotterAlgorithm.cpp )

add_library( DataVisualizerLib SHARED ${SRC_FILES} ${HEADER_FILES} )

target_link_libraries( DataVisualizerLib ${ROOT_LIBRARIES} )
set_target_properties(DataVisualizerLib PROPERTIES LINKER_LANGUAGE CXX)

# Executables
add_executable( testRootReader macros/testRootReader.cpp)
target_link_libraries( testRootReader DataVisualizerLib )
